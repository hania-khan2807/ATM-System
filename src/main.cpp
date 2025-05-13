#include <iostream>
#include <string>
#include <limits>
#include "ATM.h"
#include "Account.h"
#include "Transaction.h"
#include "utils.h"

using namespace std;

// Function prototypes
void displayWelcomeScreen();
void displayMainMenu();
Account* authenticateUser(ATM& atm);
void handleBalanceInquiry(Account* account);
void handleDeposit(ATM& atm, Account* account);
void handleWithdrawal(ATM& atm, Account* account);
void handleTransactionHistory(Account* account);
void clearInputBuffer();
void pressEnterToContinue();

int main() {
    // Initialize ATM with sample accounts
    ATM atm;
    
    // Create and add sample accounts
    Account* acc1 = new Account("123456789", "1234", 1000.00);
    Account* acc2 = new Account("987654321", "4321", 500.00);
    atm.addAccount(acc1);
    atm.addAccount(acc2);

    // Display welcome screen
    displayWelcomeScreen();

    // Authenticate user
    Account* currentAccount = authenticateUser(atm);
    if (!currentAccount) {
        cout << "\nAuthentication failed. Exiting..." << endl;
        return 1;
    }

    // Main application loop
    bool running = true;
    while (running) {
        displayMainMenu();
        
        int choice;
        cout << "Enter your choice (1-5): ";
        
        // Validate input
        while (!(cin >> choice) || choice < 1 || choice > 5) {
            clearInputBuffer();
            cout << "Invalid input. Please enter a number between 1 and 5: ";
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                handleBalanceInquiry(currentAccount);
                break;
            case 2:
                handleDeposit(atm, currentAccount);
                break;
            case 3:
                handleWithdrawal(atm, currentAccount);
                break;
            case 4:
                handleTransactionHistory(currentAccount);
                break;
            case 5:
                cout << "\nThank you for using our ATM. Goodbye!" << endl;
                running = false;
                break;
        }

        if (running) {
            pressEnterToContinue();
        }
    }

    return 0;
}

void displayWelcomeScreen() {
    system("cls"); // For Windows to clear the terminal (use "clear" on Linux/Mac)
    cout << "==============================================================" << endl;
    cout << "||                                                          ||" << endl;
    cout << "||                 🌟  WELCOME TO  🌟                         ||" << endl;
    cout << "||                                                          ||" << endl;
    cout << "||             💳  BANK ATM MANAGEMENT SYSTEM  💳            ||" << endl;
    cout << "||                                                          ||" << endl;
    cout << "||               Powered by C++ and OOP 🧠                  ||" << endl;
    cout << "||                                                          ||" << endl;
    cout << "==============================================================" << endl << endl;
}


void displayMainMenu() {
    cout << "\nATM MAIN MENU" << endl;
    cout << "1. Check Account Balance" << endl;
    cout << "2. Deposit Funds" << endl;
    cout << "3. Withdraw Funds" << endl;
    cout << "4. View Transaction History" << endl;
    cout << "5. Exit" << endl;
}

Account* authenticateUser(ATM& atm) {
    string accountNumber, pin;
    int attempts = 3;

    while (attempts > 0) {
        cout << "\nPlease enter your account number: ";
        getline(cin, accountNumber);

        cout << "Enter your PIN: ";
        getline(cin, pin);

        Account* account = atm.findAccount(accountNumber);
        if (account && account->verifyPin(pin)) {
            cout << "\nAuthentication successful. Welcome!" << endl;
            return account;
        }

        attempts--;
        if (attempts > 0) {
            cout << "\nInvalid account number or PIN. " << attempts 
                 << " attempts remaining." << endl;
        }
    }

    return nullptr;
}

void handleBalanceInquiry(Account* account) {
    cout << "\nACCOUNT BALANCE" << endl;
    cout << "----------------" << endl;
    cout << "Account: " << utils::maskAccountNumber(account->getAccountNumber()) << endl;
    cout << "Available Balance: " << utils::formatCurrency(account->getBalance()) << endl;
}

void handleDeposit(ATM& atm, Account* account) {
    cout << "\nDEPOSIT FUNDS" << endl;
    cout << "--------------" << endl;
    cout << "Current Balance: " << utils::formatCurrency(account->getBalance()) << endl;

    double amount;
    cout << "\nEnter amount to deposit (max $10,000): $";
    
    while (!(cin >> amount) || !utils::isValidAmount(amount)) {
        clearInputBuffer();
        cout << "Invalid amount. Please enter a positive amount up to $10,000: $";
    }
    clearInputBuffer();

    // Confirm deposit
    cout << "\nYou are about to deposit " << utils::formatCurrency(amount) << endl;
    cout << "Confirm? (Y/N): ";
    char confirm;
    cin >> confirm;
    clearInputBuffer();

    if (toupper(confirm) == 'Y') {
        account->updateBalance(amount);
        account->addTransaction(new Transaction("Deposit", amount));
        atm.enqueueDeposit(account);
        
        cout << "\nDeposit successful!" << endl;
        cout << "New Balance: " << utils::formatCurrency(account->getBalance()) << endl;
    } else {
        cout << "Deposit canceled." << endl;
    }
}

void handleWithdrawal(ATM& atm, Account* account) {
    cout << "\nWITHDRAW FUNDS" << endl;
    cout << "---------------" << endl;
    cout << "Current Balance: " << utils::formatCurrency(account->getBalance()) << endl;

    double amount;
    cout << "\nEnter amount to withdraw (max $10,000): $";
    
    while (!(cin >> amount) || !utils::isValidAmount(amount)) {
        clearInputBuffer();
        cout << "Invalid amount. Please enter a positive amount up to $10,000: $";
    }
    clearInputBuffer();

    if (amount > account->getBalance()) {
        cout << "\nInsufficient funds. Your current balance is " 
             << utils::formatCurrency(account->getBalance()) << endl;
        return;
    }

    // Confirm withdrawal
    cout << "\nYou are about to withdraw " << utils::formatCurrency(amount) << endl;
    cout << "Confirm? (Y/N): ";
    char confirm;
    cin >> confirm;
    clearInputBuffer();

    if (toupper(confirm) == 'Y') {
        account->updateBalance(-amount);
        account->addTransaction(new Transaction("Withdrawal", amount));
        atm.enqueueWithdrawal(account);
        
        cout << "\nWithdrawal successful!" << endl;
        cout << "Please take your cash." << endl;
        cout << "New Balance: " << utils::formatCurrency(account->getBalance()) << endl;
    } else {
        cout << "Withdrawal canceled." << endl;
    }
}

void handleTransactionHistory(Account* account) {
    account->displayTransactionHistory();
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pressEnterToContinue() {
    cout << "\nPress Enter to continue...";
    clearInputBuffer();
    cin.get();
}