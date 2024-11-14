#include "User.h"
#include "SHA256.h"
#include <fstream>
#include <sstream>
#include <vector>

User::User(const std::string& username, const std::string& password, double initialBalance)
    : username(username), passwordHash(hashPassword(password)), balance(initialBalance) {}

std::string User::hashPassword(const std::string& password) const {
    return SHA256::hash(password);
}

bool User::authenticate(const std::string& password) const {
    return this->passwordHash == hashPassword(password);
}

void User::saveToFile() const {
    std::ofstream file("users.txt", std::ios::app);
    file << username << " " << passwordHash << " " << balance << "\n";
    file.close();
}

bool User::userExists(const std::string& username) {
    std::ifstream file("users.txt");
    std::string storedUsername, storedHash;
    double storedBalance;
    while (file >> storedUsername >> storedHash >> storedBalance) {
        if (storedUsername == username) {
            return true;
        }
    }
    return false;
}

User User::loadUser(const std::string& username) {
    std::ifstream file("users.txt");
    std::string storedUsername, storedHash;
    double storedBalance;
    while (file >> storedUsername >> storedHash >> storedBalance) {
        if (storedUsername == username) {
            User user(storedUsername, "");  // Create a user object without setting password
            user.passwordHash = storedHash;
            user.balance = storedBalance;
            return user;
        }
    }
    throw std::runtime_error("User not found");
}

// Updates user data in users.txt without duplicating
void User::updateUserFile(const User& updatedUser) {
    std::ifstream file("users.txt");
    std::vector<User> users;
    std::string storedUsername, storedHash;
    double storedBalance;

    // Read existing users
    while (file >> storedUsername >> storedHash >> storedBalance) {
        User user(storedUsername, "");  // Create user without hashing new password
        user.passwordHash = storedHash;
        user.balance = storedBalance;
        if (storedUsername == updatedUser.username) {
            user.balance = updatedUser.balance;  // Update balance for the specified user
        }
        users.push_back(user);
    }
    file.close();

    // Rewrite file with updated users
    std::ofstream outFile("users.txt", std::ios::trunc);
    for (const User& user : users) {
        outFile << user.username << " " << user.passwordHash << " " << user.balance << "\n";
    }
    outFile.close();
}

bool User::updateBalance(double amount) {
    if (balance + amount < 0) return false;  // Prevent negative balance
    balance += amount;
    return true;
}

double User::getBalance() const {
    return balance;
}

std::string User::getUsername() const{
    return username;
}
