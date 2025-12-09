#ifndef DATABASE_CONNECTION_HPP
#define DATABASE_CONNECTION_HPP

#include <mysql.h>
#include <string>
#include <vector>
#include <stdexcept>

class DatabaseConnection {
private:
    MYSQL* connection;
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    bool isConnected;

public:
    DatabaseConnection(const std::string& h, const std::string& u, 
                      const std::string& p, const std::string& d);
    ~DatabaseConnection();

    bool connect();
    void disconnect();
    bool isActive() const;
    MYSQL* getConnection();
    
    // Query execution methods
    bool executeQuery(const std::string& query);
    MYSQL_RES* getQueryResult(const std::string& query);
    void freeResult(MYSQL_RES* result);
    std::string getLastError() const;
};

#endif // DATABASE_CONNECTION_HPP