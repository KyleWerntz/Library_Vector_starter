#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iostream>

#include "../includes_usr/library.h"
#include "../includes_usr/datastructures.h"
#include "../includes_usr/fileIO.h"
using namespace std;

//NOTE: please ensure patron and book data are loaded from disk before calling the following
//NOTE: also make sure you save patron and book data to disk any time you make a change to them
//NOTE: for files where data is stored see constants.h BOOKFILE and PATRONFILE

vector<book> books;
vector<patron> patrons;

/*
 * clear books and patrons containers
 * then reload them from disk 
 */
void reloadAllData(){
	books.clear();
	patrons.clear();

	char bookFile[BOOKFILE.size() + 1];
	copy(BOOKFILE.begin(), BOOKFILE.end(), bookFile);
	loadBooks(books, bookFile);

	char patronFile[PATRONFILE.size() + 1];
	copy(PATRONFILE.begin(), PATRONFILE.end(), patronFile);
	loadPatrons(patrons, patronFile);
}

/* checkout a book to a patron
 * first load books and patrons from disk
 * make sure patron enrolled (patronid is assigned to a patron in patrons container)
 * make sure book in collection (bookid is assigned to a book in books container)
 * 
 * see if patron can check out any more books 
 * 	if not return TOO_MANY_OUT patron has the MAX_BOOKS_ALLOWED_OUT
 * 	
 * if so then check the book out to the patron, set the following fields for the book in the
 * books container
 * book.loaned_to_patron_id = patronid;
 * book.state = OUT;
 * 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 *         PATRON_NOT_ENROLLED
 * 		   BOOK_NOT_IN_COLLECTION
 *         TOO_MANY_OUT patron has the max number of books allowed checked out
 */
int checkout(int bookid, int patronid){

	//reloadAllData();
	//cout << "b size: " << books.size() << endl;

	int bookLocation = -1;
	int patronLocation = -1;

	if (patrons.size() == 0)
		return PATRON_NOT_ENROLLED;

	for (int i = 0; i < patrons.size(); i++)	{
		if (patronid == patrons[i].patron_id)	{
			if (patrons[i].number_books_checked_out == MAX_BOOKS_ALLOWED_OUT)
				return TOO_MANY_OUT;
			patronLocation = i;
			break;
		}
		else if (i + 1 == patrons.size())	{
			if (patronid != patrons[i].patron_id)
				return PATRON_NOT_ENROLLED;
		}
	}

	for (int i = 0; i < books.size(); i++)	{
		if (bookid == books[i].book_id)	{
			if (books[i].state == book_checkout_state::OUT)
				return BOOK_NOT_IN_COLLECTION;
			bookLocation = i;
			break;
		}
		else if (i + 1 == patrons.size())	{
			if (bookid != books[i].book_id)
				return BOOK_NOT_IN_COLLECTION;
		}
	}

	if (bookLocation >= 0)	{
		books[bookLocation].loaned_to_patron_id = patronid;
		books[bookLocation].state = OUT;
	}
	if (patronLocation >= 0)	{
		patrons[patronLocation].number_books_checked_out++;
	}

	return SUCCESS;
}

/* check a book back in 
 * first load books and patrons from disk
 * make sure book in collection (bookid is assigned to a book in books container)
 *  	
 * if so find the the patron the book is checked out to and decrement his/hers number_books_checked_out
 * then check the book back in by marking the book.loaned_to_patron_id = NO_ONE and the book.state = IN;
 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 * 		   BOOK_NOT_IN_COLLECTION
 */
int checkin(int bookid){

	//reloadAllData();

	int patronid = -1;

	if (patrons.size() == 0)
		return PATRON_NOT_ENROLLED;

	for (int i = 0; i < books.size(); i++)	{
		if (bookid == books[i].book_id)	{
			books[i].loaned_to_patron_id = NO_ONE;
			books[i].state = IN;
			patronid = books[i].loaned_to_patron_id;
			break;
		}
		else if (i + 1 == patrons.size())	{
			if (bookid != books[i].book_id)
				return BOOK_NOT_IN_COLLECTION;
		}
	}

	for (int i = 0; i < patrons.size(); i++)	{
		if (patronid == patrons[i].patron_id)	{
			patrons[i].number_books_checked_out--;
		}
	}

	return SUCCESS;
}

/*
 * enroll a patron, duplicate names are fine as patrons are uniquely identified by their patronid
 * first load books and patrons from disk
 * create a patron object, initialize its fields as appropriate, assign him/her the nextPatronID
 * then push the patron onto the patrons container
 * save all changes to the patrons container to disk
 * return 
 *    the patron_id of the person added
 */
int enroll(std::string &name){

	patron new_patron = patron();

	new_patron.name = name;
	int new_id = patrons.size();
	new_patron.patron_id = new_id;

	patrons.push_back(new_patron);

	return new_id;
}

/*
 * the number of books in the books container
 * (ie. if 3 books returns 3)
 * 
 */
int numbBooks(){
	return books.size();
}

/*
 * the number of patrons in the patrons container
 * (ie. if 3 patrons returns 3)
 */
int numbPatrons(){
	return patrons.size();
}

/*the number of books patron has checked out
 *
 *returns a positive number indicating how many books are checked out 
 *        or PATRON_NOT_ENROLLED         
 */
int howmanybooksdoesPatronHaveCheckedOut(int patronid){

	for (int i = 0; i < patrons.size(); i++)	{
		if (patronid == patrons[i].patron_id)	{
			return patrons[i].number_books_checked_out;
		}
		else if (i + 1 == patrons.size())	{
			if (patronid != patrons[i].patron_id)
				return PATRON_NOT_ENROLLED;
		}
	}

	return 0;
}

/* search through patrons container to see if patronid is there
 * if so returns the name associated with patronid in the variable name
 * 
 * returns SUCCESS found it and name in name
 *         PATRON_NOT_ENROLLED no patron with this patronid
 */
int whatIsPatronName(std::string &name,int patronid){

	for (int i = 0; i < patrons.size(); i++)	{
		if (patrons[i].name == name)	{
			cout << patrons[i].name << endl;;
			return SUCCESS;
		}
		else if (i + 1 == patrons.size())	{
			return PATRON_NOT_ENROLLED;
		}
	}

	return SUCCESS;
}

