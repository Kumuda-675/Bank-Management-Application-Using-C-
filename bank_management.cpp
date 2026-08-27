#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class BankAccount
{
private:
    int accountNumber;
    string customerName;
    string pin;
    double balance;

public:
    BankAccount()
    {
        accountNumber = 0;
        customerName = "";
        pin = "";
        balance = 0;
    }

    BankAccount(int accNo, string name, string userPin, double initialBalance)
    {
        accountNumber = accNo;
        customerName = name;
        pin = userPin;
        balance = initialBalance;
    }

    int getAccountNumber()
    {
        return accountNumber;
    }

    string getCustomerName()
    {
        return customerName;
    }

    string getPin()
    {
        return pin;
    }

    double getBalance()
    {
        return balance;
    }

    bool deposit(double amount)
    {
        if (amount <= 0)
        {
            cout << "Invalid deposit amount!\n";
            return false;
        }

        balance += amount;
        cout << "Deposit successful!\n";
        return true;
    }

    bool withdraw(double amount)
    {
        if (amount <= 0)
        {
            cout << "Invalid withdrawal amount!\n";
            return false;
        }

        if (amount > balance)
        {
            cout << "Insufficient balance!\n";
            return false;
        }

        balance -= amount;
        cout << "Withdrawal successful!\n";
        return true;
    }

    void checkBalance()
    {
        cout << "\nCurrent Balance: Rs. " << balance << endl;
    }

    void displayDetails()
    {
        cout << "\n----- ACCOUNT DETAILS -----\n";
        cout << "Account Number: " << accountNumber << endl;
        cout << "Customer Name : " << customerName << endl;
        cout << "Balance       : Rs. " << balance << endl;
        cout << "---------------------------\n";
    }
};

class BankManagement
{
private:
    vector<BankAccount> accounts;
    const string filename = "bank_data.txt";

public:
    BankManagement()
    {
        loadData();
    }

    void saveData()
    {
        ofstream file(filename);

        if (!file)
        {
            cout << "Error opening file!\n";
            return;
        }

        for (BankAccount &account : accounts)
        {
            file << account.getAccountNumber() << "|"
                 << account.getCustomerName() << "|"
                 << account.getPin() << "|"
                 << account.getBalance() << endl;
        }

        file.close();
    }

    void loadData()
    {
        ifstream file(filename);

        if (!file)
        {
            return;
        }

        string line;

        while (getline(file, line))
        {
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            size_t pos3 = line.find('|', pos2 + 1);

            if (pos1 != string::npos &&
                pos2 != string::npos &&
                pos3 != string::npos)
            {
                int accNo = stoi(line.substr(0, pos1));
                string name = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string userPin = line.substr(pos2 + 1, pos3 - pos2 - 1);
                double balance = stod(line.substr(pos3 + 1));

                accounts.push_back(
                    BankAccount(accNo, name, userPin, balance)
                );
            }
        }

        file.close();
    }

    bool accountExists(int accNo)
    {
        for (BankAccount &account : accounts)
        {
            if (account.getAccountNumber() == accNo)
            {
                return true;
            }
        }

        return false;
    }

    void createAccount()
    {
        int accNo;
        string name;
        string userPin;
        double initialBalance;

        cout << "\nEnter Account Number: ";
        cin >> accNo;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid account number!\n";
            return;
        }

        if (accountExists(accNo))
        {
            cout << "Account number already exists!\n";
            return;
        }

        cin.ignore();

        cout << "Enter Customer Name: ";
        getline(cin, name);

        cout << "Create PIN/Password: ";
        cin >> userPin;

        cout << "Enter Initial Balance: ";
        cin >> initialBalance;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid balance!\n";
            return;
        }

        if (initialBalance < 0)
        {
            cout << "Initial balance cannot be negative!\n";
            return;
        }

        accounts.push_back(
            BankAccount(accNo, name, userPin, initialBalance)
        );

        saveData();

        cout << "\nAccount created successfully!\n";
    }

    void login()
    {
        int accNo;
        string userPin;

        cout << "\nEnter Account Number: ";
        cin >> accNo;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid account number!\n";
            return;
        }

        cout << "Enter PIN/Password: ";
        cin >> userPin;

        for (BankAccount &account : accounts)
        {
            if (account.getAccountNumber() == accNo &&
                account.getPin() == userPin)
            {
                cout << "\nLogin successful. Welcome, "
                     << account.getCustomerName() << "!\n";

                accountMenu(account);
                return;
            }
        }

        cout << "Invalid account number or PIN!\n";
    }

    void accountMenu(BankAccount &account)
    {
        int choice;
        double amount;

        do
        {
            cout << "\n===== BANK ACCOUNT MENU =====\n";
            cout << "1. Deposit Money\n";
            cout << "2. Withdraw Money\n";
            cout << "3. Check Balance\n";
            cout << "4. Display Account Details\n";
            cout << "5. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input!\n";
                continue;
            }

            switch (choice)
            {
                case 1:
                    cout << "Enter deposit amount: ";
                    cin >> amount;

                    if (!cin.fail() && account.deposit(amount))
                    {
                        saveData();
                    }
                    else if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid amount!\n";
                    }
                    break;

                case 2:
                    cout << "Enter withdrawal amount: ";
                    cin >> amount;

                    if (!cin.fail() && account.withdraw(amount))
                    {
                        saveData();
                    }
                    else if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid amount!\n";
                    }
                    break;

                case 3:
                    account.checkBalance();
                    break;

                case 4:
                    account.displayDetails();
                    break;

                case 5:
                    cout << "Logged out successfully!\n";
                    break;

                default:
                    cout << "Invalid choice!\n";
            }

        } while (choice != 5);
    }

    void mainMenu()
    {
        int choice;

        do
        {
            cout << "\n=================================\n";
            cout << "   BANK MANAGEMENT APPLICATION\n";
            cout << "=================================\n";
            cout << "1. Create Account\n";
            cout << "2. Login to Account\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Please enter a number.\n";
                continue;
            }

            switch (choice)
            {
                case 1:
                    createAccount();
                    break;

                case 2:
                    login();
                    break;

                case 3:
                    cout << "\nThank you for using the Bank Management Application!\n";
                    break;

                default:
                    cout << "Invalid choice! Please try again.\n";
            }

        } while (choice != 3);
    }
};

int main()
{
    BankManagement bank;
    bank.mainMenu();

    return 0;
}
