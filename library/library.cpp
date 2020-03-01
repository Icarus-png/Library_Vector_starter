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

//global vectors
vector<book> books;
vector<patron> patrons;

/*
 * clear books and patrons containers
 * then reload them from disk 
 */
void reloadAllData(){
	loadBooks(books, BOOKFILE.c_str());
	loadPatrons(patrons, PATRONFILE.c_str());
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
	reloadAllData();
	int booksOut = howmanybooksdoesPatronHaveCheckedOut(patronid);

	//check if patron is enrolled
	if (booksOut == PATRON_NOT_ENROLLED) {
		return PATRON_NOT_ENROLLED;
	}

	//check if patron has max number of books out
	if (booksOut == MAX_BOOKS_ALLOWED_OUT) {
		return TOO_MANY_OUT;
	}

	//check if book is in collection, record location if it is
	bool inCol = false;
	for (int i = 0; i < books.size(); i++) {
		if (bookid == books[i].book_id) {
			inCol = true;

			//checkout process: book
			books[i].loaned_to_patron_id = patronid;
			books[i].state = OUT;
		}
	}

	if (!inCol) {
		return BOOK_NOT_IN_COLLECTION;
	}

	booksOut += 1;
	//checkout process: patron
	for (int i = 0; i < patrons.size(); i++) {
		if (patrons[i].patron_id == patronid) {
			patrons[i].number_books_checked_out = booksOut;
		}
	}

	//save
	savePatrons(patrons, PATRONFILE.c_str());
	saveBooks(books, BOOKFILE.c_str());
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
	reloadAllData();

	//check if book is in collection
	bool inCol = false;
	for (int i = 0; i < books.size(); i++) {
		if (books[i].book_id == bookid) {
			inCol = true;
		}
	}
	if (!inCol) {
		return BOOK_NOT_IN_COLLECTION;
	}

	//fix patron data
	int loc;
	for (int i = 0; i < books.size(); i++) {
		if (books[i].book_id == bookid) {
			loc = i;
		}
	}
	for (int i = 0; i < patrons.size(); i++) {
		if (patrons[i].patron_id == books[loc].loaned_to_patron_id) {
			patrons[i].number_books_checked_out--;
		}
	}
	//change book state, change loaned to patron to no one
	books[loc].state = IN;
	books[loc].loaned_to_patron_id = NO_ONE;
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
	reloadAllData();

	//initialize person
	patron person;
	person.name = name;
	person.number_books_checked_out = 0;
	person.patron_id = patrons.size();
	patrons.push_back(person);
	savePatrons(patrons, PATRONFILE.c_str());
	return person.patron_id;
}

/*
 * the number of books in the books container
 * (ie. if 3 books returns 3)
 * 
 */
int numbBooks(){
	reloadAllData();
	return books.size();
}

/*
 * the number of patrons in the patrons container
 * (ie. if 3 patrons returns 3)
 */
int numbPatrons(){
	reloadAllData();
	return patrons.size();
}

/*the number of books patron has checked out
 *
 *returns a positive number indicating how many books are checked out 
 *        or PATRON_NOT_ENROLLED         
 */
int howmanybooksdoesPatronHaveCheckedOut(int patronid){
	int booksOut;
	bool enrolled = false;
	for (int i = 0; i < patrons.size(); i++) {
		if (patronid == patrons[i].patron_id){
			enrolled = true;
			booksOut = patrons[i].number_books_checked_out;
		}
	}

	if(!enrolled) {
		return PATRON_NOT_ENROLLED;
	}

	return booksOut;
}

/* search through patrons container to see if patronid is there
 * if so returns the name associated with patronid in the variable name
 * 
 * returns SUCCESS found it and name in name
 *         PATRON_NOT_ENROLLED no patron with this patronid
 */
int whatIsPatronName(std::string &name,int patronid){
	bool enrolled = false;
	for (int i = 0; i < patrons.size(); i++) {
		if (patronid == patrons[i].patron_id) {
			name = patrons[i].name;
			enrolled = true;
		}
	}
	if (!enrolled) {
		return PATRON_NOT_ENROLLED;
	}
	return SUCCESS;
}

