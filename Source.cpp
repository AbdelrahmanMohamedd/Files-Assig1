#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

void print_menu();
void add_book();
void delete_book();
void update_book();
void print_book();
void print_all();

bool check_file(string file_name);


class Book
{
public:
	char ISBN[5];
	string title;
	string author_name;
	double price;
	int year;
	int num_of_pages;

	Book()
	{
	}
	Book(string ISBN, string title, string author_name, double price, int year, int num_of_pages)
	{
		int length = ISBN.length();
		for (int i = 0; i < length; i++)
		{
			this->ISBN[i] = ISBN[i];
		}
		this->ISBN[length] = '\0';
		this->title = title;
		this->author_name = author_name;
		this->price = price;
		this->year = year;
		this->num_of_pages = num_of_pages;
	}
	friend istream& operator>> (istream& in, Book& temp);
	friend ostream& operator<< (ostream& out, Book temp);
};
int main()
{
	string select;
	print_menu();
	while(true)
	{
		cout << "Enter A Choice From The Menu :";
		cin >> select;
		if (select == "1")
		{
			add_book();
		}
		else if (select == "2")
		{
			delete_book();
		}
		else if (select == "3")
		{
			update_book();
		}
		else if (select == "4")
		{
			print_book();
		}
		else if(select == "5")
		{
			print_all();
		}
		else if (select == "E" || select == "e")
		{
			return 0;
		}
		else
		{
			cin.ignore(1000, '\n');
			cout << "Invalid Input." << endl;
		}
	}
	//system("Pause");
	return 0;
}

void print_menu()
{
	cout << "1- Add Book." << endl
		 << "2- Delete Book." << endl
		 << "3- Update Book." << endl
		 << "4- Print Book." << endl
		 << "5- Print All." << endl
		 << "E- Exit The Program." << endl;
}
void add_book()
{
	ofstream file;
	file.open("book.txt", ios ::out | ios::app);

	if (file.fail())
	{
		cout << "File Opening Failed." << endl;
		return;
	}
	else
	{
		Book b;
		cin >> b;

		file.seekp(0, ios::end);
		int sizee = strlen(b.ISBN);

		file.write((char*)&sizee, sizeof(sizee));
		file.write(b.ISBN, sizee);
		file << '\0';

		sizee = b.title.length();
		file.write((char*)(&sizee), sizeof(sizee));
		file.write((char*)(&b.title[0]), sizee);

		sizee = b.author_name.length();
		file.write((char*)(&sizee), sizeof(sizee));
		file.write((char*)(&b.author_name[0]), sizee);

		sizee = sizeof(b.price);
		file.write((char*)(&sizee), sizeof(sizee));
		file.write((char*)&b.price, sizee);

		sizee = sizeof(b.year);
		file.write((char*)&sizee, sizeof(sizee));
		file.write((char*)&b.year, sizee);

		sizee = sizeof(b.num_of_pages);
		file.write((char*)&sizee, sizeof(sizee));
		file.write((char*)&b.num_of_pages, sizee);
		// Add Parameter To The End of The Record.
		file << '#';
		file.close();
	}
}
void delete_book()
{
	// Checking The File.
	if (!check_file("book.txt"))
	{
		return;
	}
	else
	{
		//Reading the ISBN from the user.
		char input_ISBN[5];
		cout << "Enter The ISBN(4 Characters Max) :";
		cin.ignore(1000, '\n');
		cin.getline(input_ISBN, 5, '\n');
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
		// Opening the file for reading.
		fstream delete_record;
		delete_record.open("book.txt", ios::out | ios::in);
		delete_record.seekg(0, ios::beg);
		//Filling a vector with the books of the file.
		vector<Book> file_books;
		Book temp_book;
		int sizee = 0;
		int num_of_books = 0;
		while (!(delete_record.eof()))
		{
			delete_record.read((char*)&sizee, sizeof(sizee));
			delete_record.read(temp_book.ISBN, sizee + 1);

			delete_record.read((char*)&sizee, sizeof(sizee));
			temp_book.title.resize(sizee);
			delete_record.read((char*)&temp_book.title[0], sizee);

			delete_record.read((char*)&sizee, sizeof(sizee));
			temp_book.author_name.resize(sizee);
			delete_record.read((char*)&temp_book.author_name[0], sizee);

			delete_record.read((char*)&sizee, sizeof(sizee));
			delete_record.read((char*)&temp_book.price, sizee);

			delete_record.read((char*)&sizee, sizeof(sizee));
			delete_record.read((char*)&temp_book.year, sizee);

			delete_record.read((char*)&sizee, sizeof(sizee));
			delete_record.read((char*)&temp_book.num_of_pages, sizee);

			delete_record.ignore(); //For Ignoring #.

			file_books.push_back(temp_book);
			num_of_books++;
			if (delete_record.peek() == EOF)
			{
				break;
			}
		}
		//Searching for the desired record.
		int book_index = -1;
		for (int i = 0; i < num_of_books; i++)
		{
			if (strcmp(file_books[i].ISBN, input_ISBN) == 0)
			{
				book_index = i;
				break;
			}
		}
		if (book_index == -1)
		{
			cout << "Book Not Found." << endl;
		}
		else
		{
			cout << "Book Found." << endl;
			delete_record.close();
			delete_record.open("book.txt", ios::out | ios::trunc);
			for (int i = 0; i < num_of_books; i++)
			{
				if (i == book_index)
				{
					continue;
				}
				sizee = strlen(file_books[i].ISBN);
				delete_record.write((char*)&sizee, sizeof(sizee));
				delete_record.write(file_books[i].ISBN, sizee);
				delete_record << '\0';

				sizee = file_books[i].title.length();
				delete_record.write((char*)&sizee, sizeof(sizee));
				delete_record.write((char*)&file_books[i].title[0], sizee);

				sizee = file_books[i].author_name.length();
				delete_record.write((char*)&sizee, sizeof(sizee));
				delete_record.write((char*)&file_books[i].author_name[0], sizee);

				sizee = sizeof(file_books[i].price);
				delete_record.write((char*)&sizee, sizeof(sizee));
				delete_record.write((char*)&file_books[i].price, sizee);

				sizee = sizeof(file_books[i].year);
				delete_record.write((char*)&sizee, sizeof(sizee));
				delete_record.write((char*)&file_books[i].year, sizee);

				sizee = sizeof(file_books[i].num_of_pages);
				delete_record.write((char*)&sizee, sizeof(sizee));
				delete_record.write((char*)&file_books[i].num_of_pages, sizee);

				delete_record << '#';
			}
			cout << "Book Deleted Successfully." << endl;
		}
		delete_record.close();
	}
}
void update_book()
{
	// Checking The File.
	if (!check_file("book.txt"))
	{
		return;
	}
	else
	{
		//Reading the ISBN from the user.
		char input_ISBN[5];
		cout << "Enter The ISBN(4 Characters Max) :";
		cin.ignore(1000, '\n');
		cin.getline(input_ISBN, 5, '\n');
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
		// Opening the file for reading.
		fstream update_record;
		update_record.open("book.txt", ios::out | ios::in);
		update_record.seekg(0, ios::beg);
		//Filling a vector with the books of the file.
		vector<Book> file_books;
		Book temp_book;
		int sizee = 0;
		int num_of_books = 0;
		while (!(update_record.eof()))
		{
			update_record.read((char*)&sizee, sizeof(sizee));
			update_record.read(temp_book.ISBN, sizee + 1);

			update_record.read((char*)&sizee, sizeof(sizee));
			temp_book.title.resize(sizee);
			update_record.read((char*)&temp_book.title[0], sizee);

			update_record.read((char*)&sizee, sizeof(sizee));
			temp_book.author_name.resize(sizee);
			update_record.read((char*)&temp_book.author_name[0], sizee);

			update_record.read((char*)&sizee, sizeof(sizee));
			update_record.read((char*)&temp_book.price, sizee);

			update_record.read((char*)&sizee, sizeof(sizee));
			update_record.read((char*)&temp_book.year, sizee);

			update_record.read((char*)&sizee, sizeof(sizee));
			update_record.read((char*)&temp_book.num_of_pages, sizee);

			update_record.ignore(); //For Ignoring #.

			file_books.push_back(temp_book);
			num_of_books++;

			if (update_record.peek() == EOF)
			{
				break;
			}
		}
		//Searching for the desired record.
		int book_index = -1;
		for (int i = 0; i < num_of_books; i++)
		{
			if (strcmp(file_books[i].ISBN, input_ISBN) == 0)
			{
				book_index = i;
				break;
			}
		}
		if (book_index == -1)
		{
			cout << "Book Not Found." << endl;
		}
		else
		{
			cout << "Book Found." << endl;
			cout << "Enter The New Data." << endl;
			cin >> file_books[book_index];
			update_record.close();
			update_record.open("book.txt", ios::out | ios::trunc);
			for (int i = 0; i < num_of_books; i++)
			{
				sizee = strlen(file_books[i].ISBN);
				update_record.write((char*)&sizee, sizeof(sizee));
				update_record.write(file_books[i].ISBN, sizee);
				update_record << '\0';

				sizee = file_books[i].title.length();
				update_record.write((char*)&sizee, sizeof(sizee));
				update_record.write((char*)&file_books[i].title[0], sizee);

				sizee = file_books[i].author_name.length();
				update_record.write((char*)&sizee, sizeof(sizee));
				update_record.write((char*)&file_books[i].author_name[0], sizee);

				sizee = sizeof(file_books[i].price);
				update_record.write((char*)&sizee, sizeof(sizee));
				update_record.write((char*)&file_books[i].price, sizee);

				sizee = sizeof(file_books[i].year);
				update_record.write((char*)&sizee, sizeof(sizee));
				update_record.write((char*)&file_books[i].year, sizee);

				sizee = sizeof(file_books[i].num_of_pages);
				update_record.write((char*)&sizee, sizeof(sizee));
				update_record.write((char*)&file_books[i].num_of_pages, sizee);

				update_record << '#';
			}
		}
		update_record.close();
	}
}
void print_book()
{
	// Checking The File.
	if (!check_file("book.txt"))
	{
		return;
	}
	else
	{
		cin.ignore(1000, '\n'); // For Cleaning The \n from the last cin.
		// Reading The Title From The User.
		string title;
		cout << "Enter The Title of The Book :";
		getline(cin, title, '\n');
		// Getting to The Specific Record.
		ifstream read;
		read.open("book.txt");
		read.seekg(0, ios::beg);

		Book temp;
		int sizee = 0; // Default Value.
		int bytes_back = 0;
		bool found = false;
		while(!(read.eof()))
		{
			read.read((char*)&sizee, sizeof(sizee));
			read.read(temp.ISBN, sizee + 1);
			bytes_back = sizeof(sizee) + sizee + 1;

			read.read((char*)&sizee, sizeof(sizee));
			temp.title.resize(sizee);
			read.read((char*)&temp.title[0], sizee);
			bytes_back += sizeof(sizee) + sizee;

			if(temp.title == title)
			{
				found = true;
				break;
			}

			char eat_character;
			while (true)
			{
				read.get(eat_character);
				if (eat_character == '#')
				{
					break;
				}
			}
		}
		if (found == false)
		{
			cout << "Book Not Found." << endl;
		}
		else
		{
			read.seekg(-1 * bytes_back, ios::cur);
			// Reading The Record From The File.
			read.read((char*)&sizee, sizeof(sizee));
			read.read(temp.ISBN, sizee + 1);

			read.read((char*)&sizee, sizeof(sizee));
			temp.title.resize(sizee);
			read.read((char*)&temp.title[0], sizee);

			read.read((char*)&sizee, sizeof(sizee));
			temp.author_name.resize(sizee);
			read.read((char*)&temp.author_name[0], sizee);

			read.read((char*)&sizee, sizeof(sizee));
			read.read((char*)&temp.price, sizee);

			read.read((char*)&sizee, sizeof(sizee));
			read.read((char*)&temp.year, sizee);

			read.read((char*)&sizee, sizeof(sizee));
			read.read((char*)&temp.num_of_pages, sizee);

			cout << temp << endl;
		}
		read.close();
	}
}
void print_all()
{
	// Checking The File.
	if (!check_file("book.txt"))
	{
		return;
	}
	else
	{
		ifstream print_file;
		print_file.open("Book.txt");
		print_file.seekg(0, ios::beg);

		Book temp;
		int sizee;
		int book_number = 1;
		while (!(print_file.eof()))
		{
			
			print_file.read((char*)&sizee, sizeof(sizee));
			print_file.read(temp.ISBN, sizee + 1);

			print_file.read((char*)&sizee, sizeof(sizee));
			temp.title.resize(sizee);
			print_file.read((char*)&temp.title[0], sizee);

			print_file.read((char*)&sizee, sizeof(sizee));
			temp.author_name.resize(sizee);
			print_file.read((char*)&temp.author_name[0], sizee);

			print_file.read((char*)&sizee, sizeof(sizee));
			print_file.read((char*)&temp.price, sizee);

			print_file.read((char*)&sizee, sizeof(sizee));
			print_file.read((char*)&temp.year, sizee);

			print_file.read((char*)&sizee, sizeof(sizee));
			print_file.read((char*)&temp.num_of_pages, sizee);

			print_file.ignore(); // For Removing The # Symbol.

			cout << "Book(" <<book_number << ")" << endl
				 << temp << endl;
			
			book_number++;
			if (print_file.peek() == EOF)
			{
				break;
			}
		}
	}
}
bool check_file(string file_name)
{
	ifstream file;
	file.open(file_name.c_str());

	if (file.fail())
	{
		cout << "File Opening Failed, Make Sure That The File Is Created." << endl;
		file.close();
		return false;
	}
	file.seekg(0, ios::end);
	int temp = file.tellg();
	if (temp == 0)
	{
		cout << "File IS Empty, Add Some Books or Check The file." << endl;
		file.close();
		return false;
	}
	file.close();
	return true;
}
istream& operator>> (istream& in, Book& temp)
{
	cout << "Enter The ISBN (4 Characters Max) :";
	if (cin.peek() == '\n')
	{
		cin.ignore(1000, '\n');
	}
	cin.getline(temp.ISBN, 5, '\n');
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');
	}

	cout << "Enter The Title :";
	getline(in, temp.title, '\n');

	cout << "Enter The Author Name :";
	getline(in, temp.author_name, '\n');

	cout << "Enter The Price :";
	in >> temp.price;

	cout << "Enter The Year :";
	in >> temp.year;

	cout << "Enter The Number of Pages :";
	in >> temp.num_of_pages;
	return in;
}
ostream& operator<< (ostream& out, Book temp)
{
	cout << "ISBN :" << temp.ISBN << endl;
	cout << "Title :" << temp.title << endl;
	cout << "Author Name :" << temp.author_name << endl;
	cout << "Price :" << temp.price << endl;
	cout << "Year :" << temp.year << endl;
	cout << "Number of Pages :" << temp.num_of_pages << endl;

	return out;
}
