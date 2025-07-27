/*
TASK 2: Login and Registration System
        This program allows users to register and login. Credentials are stored in a file (users.txt).
*/

#include <iostream>
#include <fstream> // for file operations
#include <string>
using namespace std;

// Simple hashing function to obfuscate the password (for demonstration only, not secure)
string simpleHash(const string &password)
{
    string hash = "";
    for (char c : password)
        hash += to_string((int)c + 7) + "-"; // Add 7 to ASCII value and concatenate
    return hash;
}

// Checks if the given username already exists in the users.txt file
bool usernameExists(const string &username)
{
    ifstream fin("users.txt"); // Open the file for reading
    string line, user, pass;
    while (getline(fin, line)) // Read each line from the file
    {
        size_t sep = line.find(',');
        if (sep != string::npos)
        {
            user = line.substr(0, sep);
            if (user == username)
                return true; // Username found
        }
    }
    return false; // Username not found
}

// Handles user registration: input, validation, and storing credentials
void registerUser()
{
    string username, password;
    cout << "\n--- Registration ---\n";
    cout << "Enter username: ";
    cin >> username;
    if (username.empty())
    {
        cout << "Username cannot be empty!\n";
        return;
    }
    if (usernameExists(username))
    {
        cout << "Username already exists!\n";
        return;
    }
    cout << "Enter password: ";
    cin >> password;
    if (password.length() < 4)
    {
        cout << "Password must be at least 4 characters!\n";
        return;
    }
    // Store username and hashed password in users.txt
    ofstream fout("users.txt", ios::app);
    fout << username << "," << simpleHash(password) << endl;
    cout << "Registration successful!\n";
}

// Handles user login: input, credential check, and feedback
void loginUser()
{
    string username, password;
    cout << "\n--- Login ---\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    ifstream fin("users.txt");
    string line, user, pass;
    bool found = false; 
    string hash = simpleHash(password); // Hash the input password for comparison
    while (getline(fin, line))
    {
        size_t sep = line.find(',');
        if (sep != string::npos)
        {
            user = line.substr(0, sep); // Extract username
            pass = line.substr(sep + 1); // Extract hashed password
            if (user == username && pass == hash)
            {
                found = true; // Credentials match
                break;
            }
        }
    }
    if (found)
        cout << "Login successful!\n";
    else
        cout << "Invalid username or password!\n";
}

// Main menu loop for the login and registration system
int main()
{
    int choice;
    cout << "\n==== Login & Registration System ====" << endl;
    do
    {
        cout << "\n1. Register\n2. Login\n0. Exit\nChoose an option: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            registerUser(); // Call registration
            break;
        case 2:
            loginUser(); // Call login
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option!\n";
        }
    } while (choice != 0);
    return 0;
}