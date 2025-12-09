#include "../headers/User.hpp"
#include <sstream>
#include <iostream>

User::User(std::shared_ptr<DatabaseConnection> database) : db(database) {
    if (!db) {
        throw std::runtime_error("ERROR: Database connection is null!");
    }
}

bool User::addUser(const std::string& username, const std::string& passwordHash,
                   const std::string& privilege) {
    try {
        if (username.empty() || passwordHash.empty()) {
            std::cerr << "ERROR: Username and password cannot be empty!\n";
            return false;
        }

        std::ostringstream query;
        query << "INSERT INTO user (Username, PasswordHash, Privilege) VALUES ('"
              << username << "', '" << passwordHash << "', '" << privilege << "');";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in addUser: " << e.what() << "\n";
        return false;
    }
}

bool User::deleteUser(int userId) {
    try {
        if (userId <= 0) {
            std::cerr << "ERROR: Invalid user ID!\n";
            return false;
        }

        std::ostringstream query;
        query << "DELETE FROM user WHERE idUser = " << userId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in deleteUser: " << e.what() << "\n";
        return false;
    }
}

bool User::updateUserPrivilege(int userId, const std::string& newPrivilege) {
    try {
        if (userId <= 0 || newPrivilege.empty()) {
            std::cerr << "ERROR: Invalid user ID or privilege!\n";
            return false;
        }

        std::ostringstream query;
        query << "UPDATE user SET Privilege = '" << newPrivilege
              << "' WHERE idUser = " << userId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in updateUserPrivilege: " << e.what() << "\n";
        return false;
    }
}

bool User::updateUser(int userId, const std::string& username, const std::string& privilege) {
    try {
        if (userId <= 0 || username.empty() || privilege.empty()) {
            std::cerr << "ERROR: Invalid parameters!\n";
            return false;
        }

        std::ostringstream query;
        query << "UPDATE user SET Username = '" << username << "', Privilege = '"
              << privilege << "' WHERE idUser = " << userId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in updateUser: " << e.what() << "\n";
        return false;
    }
}

std::vector<UserData> User::getAllUsers() {
    std::vector<UserData> users;
    try {
        std::string query = "SELECT idUser, Username, PasswordHash, Privilege, IsActive FROM user;";
        MYSQL_RES* result = db->getQueryResult(query);

        if (!result) {
            std::cerr << "ERROR: Failed to get users from database!\n";
            return users;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            UserData user;
            user.idUser = std::stoi(row[0] ? row[0] : "0");
            user.username = row[1] ? row[1] : "";
            user.passwordHash = row[2] ? row[2] : "";
            user.privilege = row[3] ? row[3] : "user";
            user.isActive = (row[4] && std::string(row[4]) == "1");
            users.push_back(user);
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getAllUsers: " << e.what() << "\n";
    }
    return users;
}

UserData User::getUserById(int userId) {
    UserData user = {-1, "", "", "", false};
    try {
        if (userId <= 0) {
            std::cerr << "ERROR: Invalid user ID!\n";
            return user;
        }

        std::ostringstream query;
        query << "SELECT idUser, Username, PasswordHash, Privilege, IsActive FROM user WHERE idUser = " << userId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return user;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            user.idUser = std::stoi(row[0] ? row[0] : "0");
            user.username = row[1] ? row[1] : "";
            user.passwordHash = row[2] ? row[2] : "";
            user.privilege = row[3] ? row[3] : "user";
            user.isActive = (row[4] && std::string(row[4]) == "1");
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getUserById: " << e.what() << "\n";
    }
    return user;
}

UserData User::getUserByUsername(const std::string& username) {
    UserData user = {-1, "", "", "", false};
    try {
        if (username.empty()) {
            std::cerr << "ERROR: Username cannot be empty!\n";
            return user;
        }

        std::ostringstream query;
        query << "SELECT idUser, Username, PasswordHash, Privilege, IsActive FROM user WHERE Username = '"
              << username << "';";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return user;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            user.idUser = std::stoi(row[0] ? row[0] : "0");
            user.username = row[1] ? row[1] : "";
            user.passwordHash = row[2] ? row[2] : "";
            user.privilege = row[3] ? row[3] : "user";
            user.isActive = (row[4] && std::string(row[4]) == "1");
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getUserByUsername: " << e.what() << "\n";
    }
    return user;
}

int User::getUserCount() {
    try {
        std::string query = "SELECT COUNT(*) FROM user;";
        MYSQL_RES* result = db->getQueryResult(query);

        if (!result) {
            return 0;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        int count = 0;
        if (row && row[0]) {
            count = std::stoi(row[0]);
        }

        db->freeResult(result);
        return count;
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getUserCount: " << e.what() << "\n";
        return 0;
    }
}

bool User::userExists(const std::string& username) {
    try {
        if (username.empty()) {
            return false;
        }

        std::ostringstream query;
        query << "SELECT COUNT(*) FROM user WHERE Username = '" << username << "';";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return false;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        bool exists = (row && row[0] && std::stoi(row[0]) > 0);

        db->freeResult(result);
        return exists;
    } catch (const std::exception& e) {
        std::cerr << "ERROR in userExists: " << e.what() << "\n";
        return false;
    }
}

std::string User::hashPassword(const std::string& password) {
    try {
        if (password.empty()) return "";
        
        std::string hashed;
        const unsigned char XOR_KEY = 0xAB;
        
        for (char c : password) {
            unsigned char encrypted = static_cast<unsigned char>(c) ^ XOR_KEY;
            char hex[3];
            snprintf(hex, sizeof(hex), "%02X", encrypted);
            hashed += hex;
        }
        
        return hashed;
    } catch (const std::exception& e) {
        std::cerr << "ERROR in hashPassword: " << e.what() << "\n";
        return "";
    }
}


bool User::verifyPassword(const std::string& plainPassword, const std::string& hashedPassword) {
    try {
        if (plainPassword.empty() || hashedPassword.empty()) return false;
        
        std::string plainHashed = hashPassword(plainPassword);
        return plainHashed == hashedPassword;
    } catch (const std::exception& e) {
        std::cerr << "ERROR in verifyPassword: " << e.what() << "\n";
        return false;
    }
}



