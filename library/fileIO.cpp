#include <iostream>
#include <fstream>
#include "../includes_usr/fileIO.h"
using namespace std;
/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char* filename)
{
	fstream file;
	file.open(filename);
	book libBook;
	string line;

	if (!file.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}

	books.clear();
	while (!file.eof()) {

		getline(file, line, ',');
		//when we get to last line in the file we need to break out of the loop
		if (line == "") {
			break;
		}
		//string to int
		libBook.book_id = stoi(line);

		getline(file, line, ',');
		libBook.title = line;

		getline(file, line, ',');
		libBook.author = line;

		getline(file, line, ',');
		int state = stoi(line);
		if (state == 0) {
			libBook.state = UNKNOWN;
		}
		else if (state == 1) {
			libBook.state = IN;
		}
		else {
			libBook.state = OUT;
		}

		//last element in line
		getline(file, line, '\n');
		libBook.loaned_to_patron_id = stoi(line);

		books.push_back(libBook);
		//if (books.empty()) {
		//	return NO_BOOKS_IN_LIBRARY;
		//}
	}
	if (books.empty()) {
		return NO_BOOKS_IN_LIBRARY;
	}

	file.close();
	return SUCCESS;
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char* filename)
{
	ofstream ofile;
	ofile.open(filename);
	string toOut;

	//place-holders because I'm dumb and get confused
	string bookid;
	string booktitle;
	string bookauthor;
	string bookstate;
	string patronid;

	if (!ofile.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}

	if (books.empty()) {
		return NO_BOOKS_IN_LIBRARY;
	}

	for (int i = 0; i < books.size(); i++) {
		bookid = to_string(books[i].book_id);
		booktitle = books[i].title;
		bookauthor = books[i].author;
		bookstate = to_string(books[i].state);
		patronid = to_string(books[i].loaned_to_patron_id);

		toOut = bookid + "," + booktitle + "," + bookauthor + "," + bookstate + "," + patronid;
		ofile << toOut << endl;
	}

	ofile.close();
	return SUCCESS;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char* filename)
{
	fstream file;
	file.open(filename);

	if (!file.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}

	patrons.clear();
	while (!file.eof()) {
		string line;
		patron person;

		getline(file, line, ',');
		//when we get to last, blank line we need to break out of while
		if (line == "") {
			break;
		}
		person.patron_id = stoi(line);

		getline(file, line, ',');
		person.name = line;

		//last element in line
		getline(file, line, '\n');
		person.number_books_checked_out = stoi(line);

		patrons.push_back(person);
		//if (patrons.empty()) {
		//	return NO_PATRONS_IN_LIBRARY;
		//}
	}
	if (patrons.empty()) {
		return NO_PATRONS_IN_LIBRARY;
	}

	file.close();
	return SUCCESS;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char* filename)
{
	ofstream ofile;
	ofile.open(filename);
	string toOut;

	//place-holders once again
	string patronid;
	string patronName;
	string patronsBooks;

	if (!ofile.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}

	if (patrons.empty()) {
		return NO_PATRONS_IN_LIBRARY;
	}

	for (int i = 0; i < patrons.size(); i++) {
		patronid = to_string(patrons[i].patron_id);
		patronName = patrons[i].name;
		patronsBooks = to_string(patrons[i].number_books_checked_out);

		toOut = patronid + "," + patronName + "," + patronsBooks;
		ofile << toOut << endl;
	}

	ofile.close();
	return SUCCESS;
}
