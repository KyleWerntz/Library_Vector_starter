#include "../includes_usr/fileIO.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char* filename)
{
	books.clear();

	try	{
		ifstream fileIn(filename);
		string line, word, temp;
		vector<string> row;
		book new_book = book();

		if (!fileIn.is_open())
			return COULD_NOT_OPEN_FILE;
		if (fileIn.peek() == ifstream::traits_type::eof())
			return NO_BOOKS_IN_LIBRARY;

		while (fileIn.peek() != ifstream::traits_type::eof())	{
			getline(fileIn, line);
			stringstream s(line);
			row.clear();

			while (getline(s, word, ','))	{
				row.push_back(word);
			}

			int tempt = 0;

			string book_id = row[0];
			stringstream bookid(book_id);
			bookid >> tempt;
			new_book.book_id = tempt;

			new_book.title = row[1];
			new_book.author = row[2];

			string book_state = row[3];
			stringstream bookstate(book_state);
			bookstate >> tempt;
			switch(tempt)	{
			case 0:
				new_book.state = UNKNOWN;
				break;
			case 1:
				new_book.state = IN;
				break;
			case 2:
				new_book.state = OUT;
				break;
			}

			string patron_id = row[4];
			stringstream patronid(patron_id);
			patronid >> tempt;
			new_book.loaned_to_patron_id = tempt;

			books.push_back(new_book);
		}
		return SUCCESS;
	} catch (fstream::failure& err)	{
		return COULD_NOT_OPEN_FILE;
	}

	return COULD_NOT_OPEN_FILE;
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char* filename)
{
	if (books.size() == 0)
		return NO_BOOKS_IN_LIBRARY;

	try	{
		ofstream fileIn(filename);

		if (!fileIn.is_open())
			return COULD_NOT_OPEN_FILE;

		for (int i = 0; i < books.size(); i++)	{
			fileIn << books[i].book_id << ","
					<< books[i].title << ","
					<< books[i].author << ","
					<< books[i].state << ","
					<< books[i].loaned_to_patron_id
					<< "\n";
		}

		return SUCCESS;

	} catch (fstream::failure& err)	{
		return COULD_NOT_OPEN_FILE;
	}

	return COULD_NOT_OPEN_FILE;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char* filename)
{
	patrons.clear();

	try	{
		ifstream fileIn(filename);
		string line, word, temp;
		vector<string> row;
		patron new_patron = patron();

		if (!fileIn.is_open())
			return COULD_NOT_OPEN_FILE;
		if (fileIn.peek() == ifstream::traits_type::eof())
			return NO_PATRONS_IN_LIBRARY;

		while (fileIn.peek() != ifstream::traits_type::eof())	{
			getline(fileIn, line);
			stringstream s(line);
			row.clear();

			while (getline(s, word, ','))	{
				row.push_back(word);
			}

			int tempt = 0;

			string patron_id = row[0];
			stringstream patronid(patron_id);
			patronid >> tempt;
			new_patron.patron_id = tempt;

			new_patron.name = row[1];

			string books_out = row[2];
			stringstream booksout(books_out);
			booksout >> tempt;
			new_patron.number_books_checked_out = tempt;

			patrons.push_back(new_patron);
		}
		return SUCCESS;
	} catch (fstream::failure& err)	{
		return COULD_NOT_OPEN_FILE;
	}
	return COULD_NOT_OPEN_FILE;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char* filename)
{
	if (patrons.size() == 0)
			return NO_BOOKS_IN_LIBRARY;

		try	{
			ofstream fileIn(filename, ios::out);

			for (int i = 0; i < patrons.size(); i++)	{
				fileIn << patrons[i].patron_id << ","
						<< patrons[i].name << ","
						<< patrons[i].number_books_checked_out
						<< "\n";
			}

			return SUCCESS;

		} catch (fstream::failure& err)	{
			return COULD_NOT_OPEN_FILE;
		}

		return SUCCESS;
}
