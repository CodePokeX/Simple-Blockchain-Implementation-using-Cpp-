#include <iostream>
#include "Blockchain.h"
#include "User.h"
#include <queue>

void registerUser() {
    std::string username, password;
    double initialBalance;
    std::cout << "Enter new username: ";
    std::cin >> username;
    if (User::userExists(username)) {
        std::cout << "User already exists.\n";
        return;
    }
    std::cout << "Enter password: ";
    std::cin >> password;
    std::cout << "Enter initial balance: ";
    std::cin >> initialBalance;
    if (initialBalance < 0) {
        std::cout << "Invalid Initial Balance!\n";
        return;
    }
    User newUser(username, password, initialBalance);
    newUser.saveToFile();
    std::cout << "Registration successful!\n";
}

bool loginUser(std::string& loggedInUser) {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (User::userExists(username)) {
        User user = User::loadUser(username);
        if (user.authenticate(password)) {
            std::cout << "Login successful!\n";
            loggedInUser = username;
            return true;
        }
    }
    std::cout << "Login failed. Incorrect username or password.\n";
    return false;
}

void userAccount(Blockchain& blockchain, const std::string& sender) {
    bool loggedIn = true;
    while (loggedIn) {
        int choice;
        std::cout << "\n--- User Menu ---\n";
        std::cout << "1. Check Balance\n";
        std::cout << "2. Make a Transaction\n";
        std::cout << "3. Logout\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            User user = User::loadUser(sender);
            std::cout << "Your balance is: " << user.getBalance() << "\n";
        } else if (choice == 2) {
            std::string receiver;
            double amount;
            std::cout << "Enter receiver username: ";
            std::cin >> receiver;
            std::cout << "Enter amount: ";
            std::cin >> amount;

            if(amount <= 0){
                std::cout<<"Invalid amount!"<<std::endl;
            }
            else{

                if (!User::userExists(receiver)) {
                    std::cout << "Receiver does not exist.\n";
                } else {
                    User senderUser = User::loadUser(sender);
                    if(senderUser.getUsername() == User::loadUser(receiver).getUsername()){
                        std::cout<< "Invalid Transaction!"<<std::endl;
                    }
                    else{

                        if (senderUser.getBalance() >= amount) {
                            User receiverUser = User::loadUser(receiver);
                            
                            blockchain.addBlock(sender, receiver, amount);
                            std::cout << "Transaction added and block mined!\n";
                            
                            senderUser.updateBalance(-amount);  // Deduct from sender
                            receiverUser.updateBalance(amount);  // Credit to receiver
                            
                            User::updateUserFile(senderUser);   // Update both users in file
                            User::updateUserFile(receiverUser);
                        } else {
                            std::cout << "Insufficient balance.\n";
                        }
                    }
                }
            }
        } else if (choice == 3) {
            loggedIn = false;
        }
    }
}

int main() {
    Blockchain blockchain;
    std::string loggedInUser;

    

    while (true) {
        int choice;
        std::cout << "\n--- Main Menu ---\n";
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";
        std::cout << "3. Print Blockchain\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            if (loginUser(loggedInUser)) {
                userAccount(blockchain, loggedInUser);
            }
        } else if (choice == 3) {
            blockchain.printBlockchain();
        } else if (choice == 4) {
            break;
        } else {
            std::cout << "Invalid choice, please try again.\n";
        }
    }

    return 0;
}
