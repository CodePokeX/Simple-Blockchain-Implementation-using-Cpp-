#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    User(const std::string& username, const std::string& password, double initialBalance = 0.0);
    
    bool authenticate(const std::string& password) const;
    void saveToFile() const;
    bool updateBalance(double amount);  // Adjust balance
    double getBalance() const;
    std::string getUsername() const;
    
    static bool userExists(const std::string& username);
    static User loadUser(const std::string& username);
    static void updateUserFile(const User& updatedUser);  // New function to update user data in the file

private:
    std::string username;
    std::string passwordHash;
    double balance;
    
    std::string hashPassword(const std::string& password) const;
};

#endif
