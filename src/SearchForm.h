#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <map>

#include <Message.h>
#include <String.h>
#include <ListView.h>
#include <ScrollView.h>
#include <TextView.h>

enum {
	M_SEARCHRESULTLIST_SHOW		 = 'srsa',
	M_SEARCHRESULTLIST_ON_SELECT = 'srlb',
	M_SEARCHRESULTLIST_ON_INVOKE = 'srlc',
	M_SEARCHRESULTLIST_ON_RESULT = 'srld'	
};

class SearchBar;
class SearchResultList;

class SearchForm {
public:
	SearchForm();
	~SearchForm();

	BTextView* GetSearchBar();
	BListView* GetSearchResultList();
private:
	SearchBar* searchBar;
	SearchResultList* searchResultList;
};

#endif // SEARCHFORM_H
