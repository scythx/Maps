/*
 * Copyright 2016-2017 Raefaldhi Amartya Junior <raefaldhiamartya@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "SearchForm.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#include "tinyxml/tinyxml.h"

#include <LayoutBuilder.h>
#include <TextView.h>
#include <Url.h>
#include <UrlRequest.h>
#include <UrlProtocolRoster.h>
#include <UrlProtocolListener.h>
#include <Window.h>

#include "MapsData.h"
#include "MapsView.h"

class SearchBar : public BTextView {
public:
	SearchBar(SearchResultList*);
	virtual ~SearchBar();
	
	virtual void KeyDown(const char*, int32);	
	virtual void MessageReceived(BMessage*);
private:
	regex_t* regex0;
	regex_t* regex1;
	regmatch_t matches[3];
	BString current;
	SearchResultList* searchResultList;
};

struct SearchResultList_Data {
	float longitude;
	float latitude;
};

class SearchResultList : public BListView, public BUrlProtocolListener {
public:
	SearchResultList();
	virtual ~SearchResultList();
	
	virtual void MakeEmpty();

	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage*);
	virtual void TargetedByScrollView(BScrollView*);
	
	virtual void DataReceived(BUrlRequest*, const char*, off_t, ssize_t);
	virtual void RequestCompleted(BUrlRequest*, bool);
private:
	static BString baseUrl;
	
	int current;
	BMallocIO* result;
	
	thread_id thread;
	BUrlRequest* request;
	
	BScrollView *scrollBar;
	std::map<int, SearchResultList_Data*> itemList;
};

SearchForm::SearchForm() {
	searchResultList = new SearchResultList();
	searchBar = new SearchBar(searchResultList);

	MapsData::AddHandler(searchBar);
	MapsData::AddHandler(searchResultList);
}

SearchForm::~SearchForm() {

}

BTextView* SearchForm::GetSearchBar() {
	return searchBar;
}

BListView* SearchForm::GetSearchResultList() {
	return searchResultList;
}

SearchBar::SearchBar(SearchResultList* view)
		  : BTextView("searchForm", B_WILL_DRAW | B_PULSE_NEEDED | B_SUPPORTS_LAYOUT) {
	searchResultList = view;
	
	BFont font;
	SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, font.Size() * 1.4));

	SetMaxBytes(32);
	DisallowChar(B_RETURN);
	SetAlignment(B_ALIGN_CENTER);

	regex0 = (regex_t*)malloc(sizeof(regex_t));;
	regex1 = (regex_t*)malloc(sizeof(regex_t));;

	regcomp(regex0, "^([0-9]+\\.[0-9]+),([0-9]+\\.[0-9]+)$", REG_EXTENDED);
	regcomp(regex1, "^([0-9]+),([0-9]+)$", REG_EXTENDED);
}

SearchBar::~SearchBar() {
	regfree(regex0);
	regfree(regex1);
}

void SearchBar::KeyDown(const char* bytes, int32 numBytes) {
	switch (bytes[0]) {
		case B_ENTER: {
			MakeFocus(false);

			BMessenger messenger(searchResultList);

			BMessage* message = new BMessage(M_SEARCHRESULTLIST_SHOW);
			
			BString temp;			
			BString regxstr(Text());
			regxstr.ReplaceAll(" ", "");						
			if (!regexec(regex0, regxstr.String(), 3, matches, 0)) {
				regxstr.CopyInto(temp, matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
				MapsData::SetLongitude(atof(temp.String()));
				regxstr.CopyInto(temp, matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);
				MapsData::SetLatitude(atof(temp.String()));
			
				MapsData::Retrieve();
			}
			else if (!regexec(regex1, regxstr.String(), 3, matches, 0)){
				regxstr.CopyInto(temp, matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
				MapsData::SetLongitude(atof(temp.String()));
				regxstr.CopyInto(temp, matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);
				MapsData::SetLatitude(atof(temp.String()));
			
				MapsData::Retrieve();
			}
			else {
				message->AddString("Text", BUrl::UrlEncode(Text(), false, false));
				messenger.SendMessage(message);
			}
		}break;
		default: {
			BTextView::KeyDown(bytes, numBytes);	
		}break;
	}
}

void SearchBar::MessageReceived(BMessage* msg) {
	switch (msg->what) {
		case MAPDATA_UPDATE: {
			current.SetTo("");
			MapsVector mapsVector = MapsData::GetVector();	

			current << mapsVector.longitude << ", ";
			current << mapsVector.latitude;
			
			SetText(current);
		}break;
		case M_MAPSVIEW_ON_FOCUS: {
			MakeFocus(false);
			SetText(current);
		}break;
		default: {
			BTextView::MessageReceived(msg);
		}break;
	}
}

BString SearchResultList::baseUrl("https://nominatim.openstreetmap.org/search?q=%s&format=xml");

SearchResultList::SearchResultList()
				 : BListView("resultList") {	
	request		= NULL;
	scrollBar	= NULL;
	Hide();
}

SearchResultList::~SearchResultList() {
	if (request != NULL) {
		request->Stop();
		wait_for_thread(thread, NULL);
				
		delete request;
		request = NULL;
	}
}

void SearchResultList::MakeEmpty() {
	itemList.clear();
	BListView::MakeEmpty();
}

void SearchResultList::AttachedToWindow() {
	SetSelectionMessage(new BMessage(M_SEARCHRESULTLIST_ON_SELECT));
	SetInvocationMessage(new BMessage(M_SEARCHRESULTLIST_ON_INVOKE));
	SetTarget(this);	
}

void SearchResultList::MessageReceived(BMessage* message) {
	switch (message->what) {
		case M_SEARCHRESULTLIST_SHOW: {
			MakeEmpty();
			
			BString url;
			url.SetToFormat(baseUrl.String(), message->GetString("Text"));

			if (request != NULL) {
				request->Stop();
				wait_for_thread(thread, NULL);
				
				delete result;
				delete request;
				request = NULL;
			}
			result = new BMallocIO();
			request = BUrlProtocolRoster::MakeRequest(BUrl(url.String()), this);
			thread = request->Run();
		}break;
		case M_SEARCHRESULTLIST_ON_SELECT: {
			current = CurrentSelection();
		}break;
		case M_SEARCHRESULTLIST_ON_INVOKE: {
			// We have to make sure that the list is not empty
			// when the user clicks it. Otherwise, it throws an error.
			if ((itemList.size() > current) && (current >= 0)) { // Revised
				MapsData::SetLongitude(itemList[current]->longitude);
				MapsData::SetLatitude(itemList[current]->latitude);
				MapsData::Retrieve();
			}
		}
		case M_MAPSVIEW_ON_FOCUS: {
			if (scrollBar != NULL) {
				scrollBar->Hide();
			}
			Hide();
		}break;
		case M_SEARCHRESULTLIST_ON_RESULT: {
			BString xmlData((const char*)result->Buffer());
			
			TiXmlDocument document;
			document.Parse(xmlData);

			int index = 0;
			TiXmlElement* searchresult = document.FirstChildElement("searchresults");
			// Also, we would want to tell the user when there are no matches.
			TiXmlElement* e = searchresult->FirstChildElement("place");
			if (e == NULL) {
				SearchResultList_Data* itemData = new SearchResultList_Data();
				AddItem(new BStringItem("No matches found!"), 0);
				itemData->longitude = 0;
				itemData->latitude = 0;
				itemList.insert(std::pair<int, SearchResultList_Data*>(0, itemData));
			}
			for (e; e != NULL; e = e->NextSiblingElement("place")) {
				AddItem(new BStringItem(e->Attribute("display_name")), index);
				
				SearchResultList_Data* itemData = new SearchResultList_Data();
				itemData->longitude = atof(e->Attribute("lon"));
				itemData->latitude = atof(e->Attribute("lat"));
				
				itemList.insert(std::pair<int, SearchResultList_Data*>(index, itemData));
				index++;
			}
			// The list will only show AFTER all the data is inserted into the list.
			if (scrollBar != NULL) {
				scrollBar->Show();
			}
			Show();
	
		}break;
		default: {
			BListView::MessageReceived(message);
		}break;
	}
}

void SearchResultList::TargetedByScrollView(BScrollView* view) {
	scrollBar = view;
	view->Hide();

	BListView::TargetedByScrollView(view);
}

void SearchResultList::DataReceived(BUrlRequest* caller, const char* data, off_t position, ssize_t size) {
	result->WriteAt(position, data, size);
}

void SearchResultList::RequestCompleted(BUrlRequest* caller, bool success) {
	if (success) {
		Invoke(new BMessage(M_SEARCHRESULTLIST_ON_RESULT));
	}
} 
