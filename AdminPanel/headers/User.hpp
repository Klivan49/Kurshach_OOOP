#ifndef USER_HPP
#define USER_HPP

#include <memory>
#include <string>
#include <vector>
#include "DatabaseConnection.hpp"

struct UserData {
    int idUser;
    std::string username;
    std::string passwordHash;
    std::string privilege;
    bool isActive;
};

class User {
private:
    std::shared_ptr<DatabaseConnection> db;

public:
    explicit User(std::shared_ptr<DatabaseConnection> database);
    
    // User CRUD operations
    bool addUser(const std::string& username, const std::string& passwordHash, 
                const std::string& privilege);
    bool deleteUser(int userId);
    bool updateUserPrivilege(int userId, const std::string& newPrivilege);
    bool updateUser(int userId, const std::string& username, const std::string& privilege);
    
    // Read operations
    std::vector<UserData> getAllUsers();
    UserData getUserById(int userId);
    UserData getUserByUsername(const std::string& username);
    int getUserCount();
    
    // Utility operations
    bool userExists(const std::string& username);
    std::string hashPassword(const std::string& password);
    bool verifyPassword(const std::string& plainPassword, const std::string& hashedPassword);
};

#endif // USER_HPP