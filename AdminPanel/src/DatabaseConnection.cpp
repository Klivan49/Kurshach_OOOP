#include "../headers/DatabaseConnection.hpp"
#include <iostream>

DatabaseConnection::DatabaseConnection(const std::string& host, 
                                       const std::string& user,
                                       const std::string& password,
                                       const std::string& database)
    : host(host), user(user), password(password), database(database), isConnected(false) {
    
    try {
        // Инициализировать объект MYSQL
        connection = mysql_init(NULL);
        
        if (!connection) {
            std::cerr << "ERROR: mysql_init failed!\n";
            throw std::runtime_error("Failed to initialize MySQL");
        }
        
        // Подключиться к БД
        if (!mysql_real_connect(connection,
                               host.c_str(),
                               user.c_str(),
                               password.c_str(),
                               database.c_str(),
                               3306, NULL, 0)) {
            std::string error = std::string("MySQL Connection Error: ") + mysql_error(connection);
            std::cerr << "ERROR: " << error << "\n";
            mysql_close(connection);
            throw std::runtime_error(error);
        }
        
        isConnected = true;
        std::cout << "[✓] Database connection established successfully!\n";
    } catch (const std::exception& e) {
        isConnected = false;
        std::cerr << "ERROR in DatabaseConnection constructor: " << e.what() << "\n";
        throw;
    }
}

DatabaseConnection::~DatabaseConnection() {
    try {
        disconnect();
    } catch (const std::exception& e) {
        std::cerr << "ERROR in ~DatabaseConnection: " << e.what() << "\n";
    }
}

bool DatabaseConnection::isActive() const {
    return isConnected && connection != nullptr;
}

bool DatabaseConnection::executeQuery(const std::string& query) {
    try {
        if (!isConnected || !connection) {
            std::cerr << "ERROR: Database not connected!\n";
            return false;
        }

        if (query.empty()) {
            std::cerr << "ERROR: Empty query!\n";
            return false;
        }

        int result = mysql_query(connection, query.c_str());
        if (result != 0) {
            std::cerr << "ERROR: Query failed: " << mysql_error(connection) << "\n";
            std::cerr << "Query: " << query << "\n";
            return false;
        }

        return true;
    } catch (const std::exception& e) {
        std::cerr << "ERROR in executeQuery: " << e.what() << "\n";
        return false;
    }
}

MYSQL_RES* DatabaseConnection::getQueryResult(const std::string& query) {
    try {
        if (!isConnected || !connection) {
            std::cerr << "ERROR: Database not connected!\n";
            return nullptr;
        }

        if (query.empty()) {
            std::cerr << "ERROR: Empty query!\n";
            return nullptr;
        }

        int result = mysql_query(connection, query.c_str());
        if (result != 0) {
            std::cerr << "ERROR: Query failed: " << mysql_error(connection) << "\n";
            return nullptr;
        }

        MYSQL_RES* resultSet = mysql_store_result(connection);
        if (!resultSet) {
            std::cerr << "ERROR: Failed to store result: " << mysql_error(connection) << "\n";
            return nullptr;
        }

        return resultSet;
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getQueryResult: " << e.what() << "\n";
        return nullptr;
    }
}

void DatabaseConnection::freeResult(MYSQL_RES* result) {
    try {
        if (result) {
            mysql_free_result(result);
        }
    } catch (const std::exception& e) {
        std::cerr << "ERROR in freeResult: " << e.what() << "\n";
    }
}

void DatabaseConnection::disconnect() {
    try {
        if (isConnected && connection) {
            mysql_close(connection);
            connection = nullptr;
            isConnected = false;
            std::cout << "[✓] Database disconnected.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "ERROR in disconnect: " << e.what() << "\n";
    }
}

std::string DatabaseConnection::getLastError() const {
    if (connection) {
        return std::string(mysql_error(connection));
    }
    return "Connection not initialized";
}

