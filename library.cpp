// Shaan Kohli
// This program integrates a library system fully online. 
// A variety of options are given to choose from to help 
// keep your library clean and organized. 

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

using namespace std;

// PROTOTYPES
void addItem(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus);
void clearLibrary(vector<string> &bookTitle, vector<string> &bookNumber, vector<string> &checkoutStatus);
void displayLibrary(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus);
string getIntegerString(int status);
void invalidEntries(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus);
void loadFromFile(vector<string> &bookTitle, vector<string> &bookNumber, vector<string> &checkoutStatus);
void outputToFile(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus);
void printCheckoutStats(vector<string> &bookTitle, vector<string> &bookNumber, vector<string> &checkoutStatus);
void printMenu();
void removeEntry(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus);
void searchEntry(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus);
bool validISBN(const string &isbnNumber);

int main()
{
    string command;

    // vectors
    vector<string> bookTitle;
    vector<string> bookNumber;
    vector<string> checkoutStatus;
    
    cout << "Welcome to the Library Management System" << endl;
    cout << "----------------------------------------" << endl;
    cout << endl;

    do 
    {
        // Output the menu and acquire a user selection
        printMenu();
        cout << endl << "Enter a command (case does not matter): ";

        // We use getline for all user input to avoid needing to handle
        // input buffer issues relating to using both >> and getline
        getline(cin, command);
        cout << endl;

        // Execute non-exit commands
        if (command == "a" || command == "A") {
            addItem(bookTitle, bookNumber, checkoutStatus);
        }
        else if (command == "c" || command == "C") {
            clearLibrary(bookTitle, bookNumber, checkoutStatus);
        }
        else if (command == "d" || command == "D") {
            displayLibrary(bookTitle, bookNumber, checkoutStatus);
        }
        else if (command == "i" || command == "I") {
            invalidEntries(bookTitle, bookNumber, checkoutStatus);
        }
        else if (command == "l" || command == "L") {
            loadFromFile(bookTitle, bookNumber, checkoutStatus);
        }
        else if (command == "o" || command == "O") {
            outputToFile(bookTitle, bookNumber, checkoutStatus);
        }
        else if (command == "p" || command == "P") {
            printCheckoutStats(bookTitle, bookNumber, checkoutStatus);
        }
        else if (command == "r" || command == "R") {
            removeEntry(bookTitle, bookNumber, checkoutStatus);
        }
        else if (command == "s" || command == "S") {
            searchEntry(bookTitle, bookNumber, checkoutStatus);
        }
        cout << endl;
    } while (!(command == "x" || command == "X"));

    return 0;
}

// Adds item to user's library
void addItem(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus) {
    string newTitle, newNumber;

    cout << "What is the book title? ";
    getline(cin, newTitle);
    cout << endl;

    while (newTitle.find(',') != string::npos) {
        cout << "The book title cannot contain commas." << endl;
        return;
    }

    cout << "What is the 13-digit ISBN (with hyphens)? ";
    getline(cin, newNumber);
    cout << endl;

    // after added entry, status now becomes in library since it is = to 0
    string newStatus = "0";

    bookTitle.push_back(newTitle);
    bookNumber.push_back(newNumber);
    checkoutStatus.push_back(newStatus);
    cout << "The Following Entry Was Added" << endl;
    cout << "-----------------------------" << endl;
    cout << newTitle << " --- " << newNumber << " --- " << getIntegerString(stoi(newStatus)) << endl;
}

// clears library completely
void clearLibrary(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus) {
    bookTitle.clear();
    bookNumber.clear();
    checkoutStatus.clear();
    cout << "Your library is now empty." << endl;
}

// displays the user's current library
void displayLibrary(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus) {
    cout << "Your Current Library" << endl;
    cout << "--------------------" << endl;

    size_t size = bookTitle.size();   

    if (size == 0) {
        cout << "The library has no books." << endl;
        return;
    }

    for (int i = 0; i < size; i++) {
        // Changes from integer representation to string representation
        string statusStr = getIntegerString(stoi(checkoutStatus.at(i)));

        cout << bookTitle.at(i) << " --- " << bookNumber.at(i) << " --- " << statusStr << endl;
    }
}

// Helper function for display function to output string for corresponding integer
string getIntegerString(int status) {
    switch (status) {
        case 0:
            return "In Library";
        case 1:
            return "Checked Out";
        case 2:
            return "On Loan";
        case 3:
            return "Unknown State";
        default:
            return "Invalid State";
    }
}

// Checks for all possible entries that are invalid and prints to user
void invalidEntries(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus) {
    vector<string> invalidTitle;
    vector<string> invalidNumber;
    vector<string> invalidStatus;

    cout << "The Following Library Entries Have Invalid Data" << endl;
    cout << "-----------------------------------------------" << endl;

    if (bookTitle.size() == 0) {
        cout << "The library has no books." << endl;
        return;
    }
    
    // puts all info in vectors if ISBN or checkout status is invalid
    for (int i = 0; i < bookNumber.size(); i++) {
        int status = stoi(checkoutStatus.at(i));
        if (!validISBN(bookNumber.at(i)) || status < 0 || status > 3) {
            invalidTitle.push_back(bookTitle.at(i));
            invalidNumber.push_back(bookNumber.at(i));
            invalidStatus.push_back(checkoutStatus.at(i));
        }
    }

    if (invalidTitle.size() == 0) {
        cout << "The library has no invalid entries." << endl;
        return;
    }

    for (int j = 0; j < invalidTitle.size(); j++) {
        cout << invalidTitle.at(j) << " --- " << invalidNumber.at(j) << " --- " << getIntegerString(stoi(invalidStatus.at(j))) << endl;
    }
}

// GETS ACCESS TO CONTENTS OF FILE AND STORES IN VECTORS
void loadFromFile(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus) {
    
    string filename;
    cout << "What database to read from? ";
    getline(cin, filename);

    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cout << endl << "Could not find the database file." << endl;
        return;
    }

    string line;
    int lineCount = 0;

    while (getline(inputFile, line)) {
        string title, number, status;
        // gets acces to first comma
        size_t commaFound = line.find(',');

        size_t nextComma;

        if (commaFound != string::npos) {
            title = line.substr(0, commaFound);

            // sets line to everything after the 1st comma
            line = line.substr(commaFound + 1);

            nextComma = line.find(',');

            if (nextComma != string::npos) {
                number = line.substr(0, nextComma);
                status = line.substr(nextComma + 1);
            }
            else {
                number = line;
                status = "0";
            }
            
        }
        // default values
        else {
            number = "000-0-00-000000-0";
            status = "0";
            title = line;
        }

        // removes extra whitespace
        if (number.front() == ' ') {
            number.erase(0, 1);
        }
        if (status.front() == ' ') {
            status.erase(0, 1);
        }

        bookTitle.push_back(title);
        bookNumber.push_back(number);
        checkoutStatus.push_back(status);

        lineCount++;
    }

    inputFile.close();

    cout << endl << "Read in " << lineCount << " lines from the file." << endl;
}

// creates a new file to output data onto
void outputToFile(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus) {

    string newFile;
    cout << "Enter output file: ";
    getline(cin, newFile);

    ofstream outputFile(newFile);

    size_t size = bookTitle.size();

    if (!outputFile.is_open()) {
        cout << "Could not open database file for writing" << endl;
        return;
    }

    for (int i = 0; i < size; i++) {
        outputFile << bookTitle.at(i) << ", " << bookNumber.at(i) << "," << checkoutStatus.at(i) << endl;
    }

        outputFile.close();
}

// prints checkout info to screen
void printCheckoutStats(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus) {

        size_t size = bookTitle.size();
        int libraryCount = 0;
        int checkedOutCount = 0;
        int loanCount = 0;
        int unknownCount = 0;
        int otherCount = 0;

        for (size_t i = 0; i < checkoutStatus.size(); i++) {
            if (checkoutStatus.at(i) == "0") {
                libraryCount++;
            }
            else if (checkoutStatus.at(i) == "1") {
                checkedOutCount++;
            }
            else if (checkoutStatus.at(i) == "2") {
                loanCount++;
            }
            else if (checkoutStatus.at(i) == "3") {
                unknownCount++;
            }
            else {
                otherCount++;
            }
        }

        cout << "Your Current Library's Stats" << endl;
        cout << "----------------------------" << endl;
        cout << "Total Books: " << size << endl;
        cout << "   In Library: " << libraryCount << endl;
        cout << "   Checked Out: " << checkedOutCount << endl;
        cout << "   On Loan: " << loanCount << endl;
        cout << "   Unknown: " << unknownCount << endl;
        cout << "   Other: " << otherCount << endl;
}

//Prints out the main menu of the management system
void printMenu()
{
    cout << "Library Management Menu" << endl;
    cout << "-----------------------" << endl;
    cout << "A - Add Item To Library" << endl;
    cout << "C - Clear The Library Of All Entries" << endl;
    cout << "D - Display Library Entries" << endl;    
    cout << "I - List Invalid Library Entries" << endl;
    cout << "L - Load Library From File" << endl;    
    cout << "O - Output Library To File" << endl;
    cout << "P - Print Out Checkout Stats" << endl;
    cout << "R - Remove A Library Entry" << endl;      
    cout << "S - Search For A Library Entry" << endl;    
    cout << "X - Exit Program";
    cout << endl;
}

// Removes an entry in library of the user's choice
void removeEntry(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus) {
    string search;
    string choice;
    int num;

    cout << "Would you like remove by (1) name or (2) ISBN." << endl;
    cout << "Enter the numeric choice: ";
    getline(cin, choice);
    num = stoi(choice);

    if (num == 1) {
        cout << endl << "Enter the book name: ";
        getline(cin, search);
        cout << endl;
    }
    else if (num == 2) {
        cout << endl << "Enter the book 13-digit ISBN (with dashes): ";
        getline(cin, search);
        cout << endl;
    }
    else {
        cout << endl << "Invalid remove by choice option." << endl;
        return;
    }

    cout << "The Following Entry Was Removed From The Library" << endl;
    cout << "------------------------------------------------" << endl;

    int lastMatch = -1;

    // gets the last match of the user input
    for (size_t i = 0; i < bookTitle.size(); i++) {
        if ((num == 1 && bookTitle[i] == search) || (num == 2 && bookNumber[i] == search)) {
            lastMatch = i;
        }
    }

    // removes user input as long as last match was found
    if (lastMatch != -1) {
        string status = getIntegerString(stoi(checkoutStatus.at(lastMatch)));
        cout << bookTitle.at(lastMatch) << " --- " << bookNumber.at(lastMatch) << " --- " << status << endl;
        bookTitle.erase(bookTitle.begin() + lastMatch);
        bookNumber.erase(bookNumber.begin() + lastMatch);
        checkoutStatus.erase(checkoutStatus.begin() + lastMatch);
    }
    else {
        cout << "No matching entry found in the library." << endl;   
    }
}

// searches for user input in library
void searchEntry(vector<string>& bookTitle, vector<string>& bookNumber, vector<string>& checkoutStatus) {
    string search;
    string choice;
    int num;

    cout << "Would you like to search by (1) name or (2) ISBN." << endl;
    cout << "Enter the numeric choice: ";
    getline(cin, choice);
    num = stoi(choice);

    if (num == 1) {
        cout << endl << "Enter the book name: ";
        getline(cin, search);
        cout << endl;
    }
    else if (num == 2) {
        cout << endl << "Enter the book 13-digit ISBN (with dashes): ";
        getline(cin, search);
        cout << endl;
    }
    else {
        cout << endl << "Invalid search by choice option." << endl;
        return;
    }

    cout << "The Following Are Your Search Results" << endl;
    cout << "-------------------------------------" << endl;

    int lastMatch = -1;

    // finds last match of search
    for (size_t i = 0; i < bookTitle.size(); i++) {
        if ((num == 1 && bookTitle[i] == search) || (num == 2 && bookNumber[i] == search)) {
            lastMatch = i;
        }
    }
    // if last match was found, then it shows on screen
    if (lastMatch != -1) {
        string status = getIntegerString(stoi(checkoutStatus.at(lastMatch)));
        cout << bookTitle.at(lastMatch) << " --- " << bookNumber.at(lastMatch) << " --- " << status << endl;
    }
    else {
        cout << "No matching entry found in the library." << endl;
    }
}

// checks to make sure that you use a valid ISBN number when adding an entry
bool validISBN(const string& isbnNumber) {

    string tempISBN;
    int digitCount = 0;

    // ISBN number must contain 13 digits
    for (int j = 0; j < isbnNumber.length(); j++) {
        if (isdigit(isbnNumber.at(j))) {
            digitCount++;
        }
    }
    if (digitCount != 13) {
        return false;
    }

    // ISBN number without the last digit
    for (int i = 0; i < isbnNumber.length() - 1; i++) {
        if (isdigit(isbnNumber.at(i))) {
            tempISBN += isbnNumber.at(i);
        }
        
    }
   
    int totalSum = 0;
    int weight = 1;

    // check digit calculation
    for (int k = 0; k < tempISBN.length(); k++) {
            
        totalSum += weight * (tempISBN.at(k) - '0');
        weight = (weight == 1) ? 3 : 1;
  
    }

    int lastNum = isbnNumber.back() - '0';

    int checkNum = 10 - (totalSum % 10);
 
    return (checkNum == lastNum);
}
