#ifndef WORDITEM_H
#define WORDITEM_H

# include <iostream>
# include <string>
# include <vector>
using namespace std;

struct DocumentItem
{
	string documentName;
	int count;

	bool operator==( DocumentItem & rhs)
	{
		if ((*this).documentName == rhs.documentName)
		{
			return true;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WordItem
{
public:
	//constructors
	WordItem();
	WordItem(string keyword, DocumentItem documentInfo)
	{
		vector <DocumentItem> temp;

		this->keyWord = keyword;

		temp.push_back(documentInfo);
		this->listOfDocuments = temp;
	}

	bool operator < ( const WordItem & r );
	bool operator < ( string & r )
	{
		bool check = ( this->keyWord < r );
		return check;
	}

	bool operator > ( const WordItem & r );
	bool operator > ( string & r )
	{
		bool check = ( this->keyWord > r );
		return check;
	}

	bool operator == ( const WordItem & r )const ;
	bool operator == ( string & r )
	{
		bool check = ( this->keyWord == r );
		return check;
	}

	bool operator <= (const WordItem & r) const;
	bool operator <= (string & r )
	{
		bool check = ( this->keyWord <= r );
		return check;
	}


	void addTo_listOfDocuments(DocumentItem item)
	{
		listOfDocuments.push_back(item);
	}


	void changekeyWord_as(string & s)
	{
		this->keyWord = s;
	}

	bool operator != ( const WordItem & r )
	{
		return !((*this) == r);
	}

	vector<DocumentItem> * access_listOfDocuments()
	{
		return &(*this).listOfDocuments;
	}

	vector<DocumentItem> getCopyof_listOfDocuments()
	{
		return (*this).listOfDocuments;
	}

	string access_keyWord()
	{
		return this->keyWord;
	}

private:
	string keyWord;
	vector<DocumentItem> listOfDocuments;

};

WordItem::WordItem()
{
	string emptyString =  "";
	vector <DocumentItem> myList; // initially empty
	this->keyWord = emptyString;
	this->listOfDocuments = myList;
}

bool WordItem::operator < (const WordItem & r )
{
	bool check = false;
	if ((*this).keyWord < r.keyWord)
	{
		check= true;
	}
	return check;
}

bool WordItem::operator > (const WordItem & r )
{
	bool check = false;
	if ((*this).keyWord > r.keyWord)
	{
		check= true;
	}
	return check;
}

bool WordItem::operator == (const WordItem & r ) const 
{
	bool check = false;
	if ((*this).keyWord == r.keyWord)
	{
		check = true;
	}
	return check;
}

bool WordItem::operator <= (const WordItem & r) const
{
	bool check = false;
	if ((*this).keyWord <= r.keyWord)
	{
		check = true;
	}
	return check;

}
#endif