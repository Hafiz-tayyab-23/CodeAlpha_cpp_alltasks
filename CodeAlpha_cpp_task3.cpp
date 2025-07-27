/*
TASK 3: Banking System
        Simple CLI banking system with Customer, Account, and Transaction classes.
*/

#include <iostream> // For input/output operations
#include <vector>   // For using std::vector
#include <string>   // For using std::string
#include <ctime>    // For date and time functions
#include <iomanip>  // For output formatting
#include <fstream>  // For file input/output
using namespace std;

// Transaction class represents a single transaction (deposit, withdrawal, transfer)
class Transaction
{
public:
    string type;    // Type of transaction
    double amount;  // Amount involved
    string date;    // Date and time of transaction
    string details; // Additional details (e.g., transfer info)
    Transaction(const string &t, double a, const string &d, const string &det = "")
        : type(t), amount(a), date(d), details(det) {}
};

// Account class represents a bank account for a customer
class Account
{
public:
    string accountNumber;             // Unique account number
    double balance;                   // Current balance
    vector<Transaction> transactions; // List of transactions for this account

    Account(const string &accNo) : accountNumber(accNo), balance(0.0) {}

    // Deposit money into the account
    void deposit(double amount)
    {
        balance += amount;
        transactions.emplace_back("Deposit", amount, currentDateTime());
    }

    // Withdraw money from the account (returns false if insufficient funds)
    bool withdraw(double amount)
    {
        if (amount > balance)
            return false;
        balance -= amount;
        transactions.emplace_back("Withdrawal", amount, currentDateTime());
        return true;
    }

    // Add a custom transaction (used for transfers)
    void addTransaction(const string &type, double amount, const string &details = "")
    {
        transactions.emplace_back(type, amount, currentDateTime(), details);
    }

    // Print recent transactions (default: last 5)
    void printTransactions(int recent = 5)
    {
        cout << "\n============================ Recent Transactions ===========================\n";
        cout << "| Date & Time         | Type           | Amount     | Details              |\n";
        cout << "----------------------------------------------------------------------------\n";
        int start = max(0, (int)transactions.size() - recent);
        for (int i = start; i < transactions.size(); ++i)
        {
            cout << "| " << setw(19) << left << transactions[i].date
                 << " | " << setw(14) << left << transactions[i].type
                 << " | " << setw(9) << left << fixed << setprecision(2) << transactions[i].amount
                 << "  | " << setw(20) << left << (transactions[i].details.empty() ? "-" : transactions[i].details)
                 << " |" << endl;
        }
        cout << "============================================================================\n";
    }

    // Get current date and time as string
    static string currentDateTime()
    {
        time_t now = time(0);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return buf;
    }
};

// Customer class represents a bank customer
class Customer
{
public:
    string name;              // Customer's name
    string customerId;        // Unique customer ID
    string password;          // Password for login
    vector<Account> accounts; // List of accounts owned by the customer

    Customer(const string &n, const string &id, const string &pw) : name(n), customerId(id), password(pw) {}
    Customer(const string &n, const string &id) : name(n), customerId(id), password("") {}

    // Find an account by account number
    Account *getAccount(const string &accNo)
    {
        for (auto &acc : accounts)
            if (acc.accountNumber == accNo)
                return &acc;
        return nullptr;
    }
};

// Global list of all customers
vector<Customer> customers;
// Counters for generating unique customer and account IDs
int nextCustomerCounter = 1;
int nextAccountCounter = 1;

// Save all customer and account data to a file for persistence
void saveData()
{
    ofstream fout("bank_data.txt");
    fout << nextCustomerCounter << ' ' << nextAccountCounter << '\n';
    fout << customers.size() << '\n';
    for (const auto &cust : customers)
    {
        fout << cust.customerId << '\n'
             << cust.name << '\n'
             << cust.password << '\n';
        fout << cust.accounts.size() << '\n';
        for (const auto &acc : cust.accounts)
        {
            fout << acc.accountNumber << ' ' << acc.balance << '\n';
            fout << acc.transactions.size() << '\n';
            for (const auto &t : acc.transactions)
            {
                fout << t.type << '\n'
                     << t.amount << '\n'
                     << t.date << '\n'
                     << t.details << '\n';
            }
        }
    }
    fout.close();
}

// Load all customer and account data from the file
void loadData()
{
    ifstream fin("bank_data.txt");
    if (!fin)
        return;
    size_t nCustomers, nAccounts, nTrans;
    fin >> nextCustomerCounter >> nextAccountCounter;
    fin >> nCustomers;
    string dummy;
    getline(fin, dummy); // consume newline
    for (size_t i = 0; i < nCustomers; ++i)
    {
        string cid, name, password;
        getline(fin, cid);
        getline(fin, name);
        getline(fin, password);
        customers.emplace_back(name, cid, password);
        fin >> nAccounts;
        getline(fin, dummy);
        for (size_t j = 0; j < nAccounts; ++j)
        {
            string accNo;
            double bal;
            fin >> accNo >> bal;
            customers.back().accounts.emplace_back(accNo);
            customers.back().accounts.back().balance = bal;
            fin >> nTrans;
            getline(fin, dummy);
            for (size_t k = 0; k < nTrans; ++k)
            {
                string type, date, details;
                double amt;
                getline(fin, type);
                fin >> amt;
                getline(fin, dummy);
                getline(fin, date);
                getline(fin, details);
                customers.back().accounts.back().transactions.emplace_back(type, amt, date, details);
            }
        }
    }
    fin.close();
}

// Generate a new unique customer ID
string generateCustomerId()
{
    char buf[20];
    sprintf(buf, "CUS-%04d", nextCustomerCounter);
    return string(buf);
}

// Generate a new unique account number
string generateAccountNumber()
{
    char buf[24];
    sprintf(buf, "ACC-NO-%05d", nextAccountCounter);
    return string(buf);
}

// Find a customer by their customer ID
Customer *findCustomer(const string &id)
{
    for (auto &c : customers)
        if (c.customerId == id)
            return &c;
    return nullptr;
}

// Register a new customer (asks for name and password, creates one account)
void registerCustomer()
{
    string name, password;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Set a password: ";
    getline(cin, password);
    string cid = generateCustomerId();
    customers.emplace_back(name, cid, password);
    string accNo = generateAccountNumber();
    customers.back().accounts.emplace_back(accNo);
    cout << "Registration successful!\nYour Customer ID: " << cid << "\nYour Account Number: " << accNo << endl;
    ++nextCustomerCounter;
    ++nextAccountCounter;
    saveData();
}

// Login a customer (asks for ID and password, returns pointer if successful)
Customer *loginCustomer()
{
    string cid, password;
    cout << "Enter Customer ID: ";
    cin >> cid;
    cout << "Enter Password: ";
    cin.ignore();
    getline(cin, password);
    Customer *cust = findCustomer(cid);
    if (!cust || cust->password != password)
    {
        cout << "Invalid ID or password!\n";
        return nullptr;
    }
    cout << "Login successful. Welcome, " << cust->name << "!\n";
    return cust;
}

// (Legacy) Create a customer without password (not used in new flow)
void createCustomer()
{
    string name;
    cout << "Enter customer name: ";
    cin.ignore();
    getline(cin, name);
    string cid = generateCustomerId();
    customers.emplace_back(name, cid);
    cout << "Customer created. ID: " << cid << endl;
    ++nextCustomerCounter;
}

// (Legacy) Create an account for an existing customer (not used in new flow)
void createAccount()
{
    string cid;
    cout << "Enter customer ID: ";
    cin >> cid;
    Customer *cust = findCustomer(cid);
    if (!cust)
    {
        cout << "Customer not found!\n";
        return;
    }
    string accNo = generateAccountNumber();
    cust->accounts.emplace_back(accNo);
    cout << "Account created. Account Number: " << accNo << endl;
    ++nextAccountCounter;
}

// Deposit money into one of the logged-in customer's accounts
void deposit(Customer *cust)
{
    if (!cust)
        return;
    if (cust->accounts.empty())
    {
        cout << "No accounts found!\n";
        return;
    }
    cout << "Your Accounts:\n";
    for (size_t i = 0; i < cust->accounts.size(); ++i)
        cout << i + 1 << ". " << cust->accounts[i].accountNumber << "\n";
    int idx;
    cout << "Select account (number): ";
    cin >> idx;
    if (idx < 1 || idx > cust->accounts.size())
    {
        cout << "Invalid selection!\n";
        return;
    }
    double amt;
    cout << "Enter amount to deposit: ";
    cin >> amt;
    cust->accounts[idx - 1].deposit(amt);
    cout << "Deposit successful. New balance: " << cust->accounts[idx - 1].balance << endl;
    saveData();
}

// Withdraw money from one of the logged-in customer's accounts
void withdraw(Customer *cust)
{
    if (!cust)
        return;
    if (cust->accounts.empty())
    {
        cout << "No accounts found!\n";
        return;
    }
    cout << "Your Accounts:\n";
    for (size_t i = 0; i < cust->accounts.size(); ++i)
        cout << i + 1 << ". " << cust->accounts[i].accountNumber << "\n";
    int idx;
    cout << "Select account (number): ";
    cin >> idx;
    if (idx < 1 || idx > cust->accounts.size())
    {
        cout << "Invalid selection!\n";
        return;
    }
    double amt;
    cout << "Enter amount to withdraw: ";
    cin >> amt;
    if (cust->accounts[idx - 1].withdraw(amt))
        cout << "Withdrawal successful. New balance: " << cust->accounts[idx - 1].balance << endl;
    else
        cout << "Insufficient balance!\n";
    saveData();
}

// Transfer money from one of the logged-in customer's accounts to another account
void transfer(Customer *cust)
{
    if (!cust)
        return;
    if (cust->accounts.empty())
    {
        cout << "No accounts found!\n";
        return;
    }
    cout << "Your Accounts:\n";
    for (size_t i = 0; i < cust->accounts.size(); ++i)
        cout << i + 1 << ". " << cust->accounts[i].accountNumber << "\n";
    int idx;
    cout << "Select your account (number): ";
    cin >> idx;
    if (idx < 1 || idx > cust->accounts.size())
    {
        cout << "Invalid selection!\n";
        return;
    }
    string toCid, toAcc;
    cout << "Enter recipient customer ID: ";
    cin >> toCid;
    Customer *toCust = findCustomer(toCid);
    if (!toCust)
    {
        cout << "Recipient not found!\n";
        return;
    }
    cout << "Enter recipient account number: ";
    cin >> toAcc;
    Account *toAccount = toCust->getAccount(toAcc);
    if (!toAccount)
    {
        cout << "Recipient account not found!\n";
        return;
    }
    double amt;
    cout << "Enter amount to transfer: ";
    cin >> amt;
    Account *fromAccount = &cust->accounts[idx - 1];
    if (fromAccount->withdraw(amt))
    {
        toAccount->deposit(amt);
        fromAccount->addTransaction("Transfer Out", amt, "To Acc: " + toAcc);
        toAccount->addTransaction("Transfer In", amt, "From Acc: " + fromAccount->accountNumber);
        cout << "Transfer successful!\n";
        saveData();
    }
    else
    {
        cout << "Insufficient balance!\n";
    }
}

// Show information and recent transactions for one of the logged-in customer's accounts
void showAccountInfo(Customer *cust)
{
    if (!cust)
        return;
    if (cust->accounts.empty())
    {
        cout << "No accounts found!\n";
        return;
    }
    cout << "Your Accounts:\n";
    for (size_t i = 0; i < cust->accounts.size(); ++i)
        cout << i + 1 << ". " << cust->accounts[i].accountNumber << "\n";
    int idx;
    cout << "Select account (number): ";
    cin >> idx;
    if (idx < 1 || idx > cust->accounts.size())
    {
        cout << "Invalid selection!\n";
        return;
    }
    Account *acc = &cust->accounts[idx - 1];
    cout << "\n=============== Account Information ================\n";
    cout << "| Account Number: " << setw(32) << left << acc->accountNumber << " |" << endl;
    cout << "| Balance:        " << setw(32) << left << acc->balance << " |" << endl;
    cout << "====================================================\n";
    acc->printTransactions();
}

// Main function: entry point of the program
int main()
{
    loadData(); // Load data from file at startup
    int choice;
    while (true)
    {
        // Main menu for registration, login, or exit
        cout << "\n====================================================\n";
        cout << "|                BANKING SYSTEM MENU               |\n";
        cout << "====================================================\n";
        cout << "| 1. Register                                      |\n";
        cout << "| 2. Login                                         |\n";
        cout << "| 3. Exit                                          |\n";
        cout << "====================================================\n";
        cout << "Choose an option: ";
        cin >> choice;
        if (choice == 1)
        {
            registerCustomer();
        }
        else if (choice == 2)
        {
            Customer *loggedIn = loginCustomer();
            if (!loggedIn)
                continue;
            int userChoice;
            do
            {
                // Menu for logged-in customers
                cout << "\n==================== CUSTOMER MENU =================\n";
                cout << "| 1. Deposit                                       |\n";
                cout << "| 2. Withdraw                                      |\n";
                cout << "| 3. Transfer                                      |\n";
                cout << "| 4. Show Account Info                             |\n";
                cout << "| 5. Logout                                        |\n";
                cout << "| 6. Exit                                          |\n";
                cout << "====================================================\n";
                cout << "Choose an option: ";
                cin >> userChoice;
                switch (userChoice)
                {
                case 1:
                    deposit(loggedIn);
                    break;
                case 2:
                    withdraw(loggedIn);
                    break;
                case 3:
                    transfer(loggedIn);
                    break;
                case 4:
                    showAccountInfo(loggedIn);
                    break;
                case 5:
                    cout << "Logging out...\n";
                    break;
                case 6:
                    cout << "Exiting...\n";
                    saveData();
                    return 0;
                default:
                    cout << "Invalid option!\n";
                }
            } while (userChoice != 5);
        }
        else if (choice == 3)
        {
            cout << "Exiting...\n";
            saveData();
            break;
        }
        else
        {
            cout << "Invalid option!\n";
        }
    }
    return 0;
}