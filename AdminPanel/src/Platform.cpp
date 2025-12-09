#include "../headers/Platform.hpp"
#include <sstream>
#include <iostream>

Platform::Platform(std::shared_ptr<DatabaseConnection> database) : db(database) {
    if (!db) {
        throw std::runtime_error("ERROR: Database connection is null!");
    }
}

bool Platform::addPlatform(const std::string& Platform_name, const std::string& type,
                          const std::string& description) {
    try {
        if (Platform_name.empty() || type.empty()) {
            std::cerr << "ERROR: Platform name and type cannot be empty!\n";
            return false;
        }

        std::ostringstream query;
        query << "INSERT INTO platform (Platform_name, Type, Description) "
              << "VALUES ('" << Platform_name << "', '" << type << "', '" << description << "');";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in addPlatform: " << e.what() << "\n";
        return false;
    }
}

bool Platform::deletePlatform(int platformId) {
    try {
        if (platformId <= 0) {
            std::cerr << "ERROR: Invalid platform ID!\n";
            return false;
        }

        std::ostringstream query;
        query << "DELETE FROM platform WHERE idPlatform = " << platformId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in deletePlatform: " << e.what() << "\n";
        return false;
    }
}

bool Platform::updatePlatform(int platformId, const std::string& Platform_name,
                             const std::string& type, const std::string& description) {
    try {
        if (platformId <= 0 || Platform_name.empty() || type.empty()) {
            std::cerr << "ERROR: Invalid parameters!\n";
            return false;
        }

        std::ostringstream query;
        query << "UPDATE platform SET Platform_name = '" << Platform_name << "', Type = '"
              << type << "', Description = '" << description << "' WHERE idPlatform = "
              << platformId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in updatePlatform: " << e.what() << "\n";
        return false;
    }
}

bool Platform::addGameToPlatform(int gameId, int platformId, const std::string& releaseDate,
                                double price) {
    try {
        if (gameId <= 0 || platformId <= 0 || price < 0) {
            std::cerr << "ERROR: Invalid parameters!\n";
            return false;
        }

        std::ostringstream query;
        query << "INSERT INTO game_s_platfo (idGame, idPlatform, ReleaseDate, Price) "
              << "VALUES (" << gameId << ", " << platformId << ", '" << releaseDate << "', "
              << price << ");";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in addGameToPlatform: " << e.what() << "\n";
        return false;
    }
}

bool Platform::removeGameFromPlatform(int gameId, int platformId) {
    try {
        if (gameId <= 0 || platformId <= 0) {
            std::cerr << "ERROR: Invalid parameters!\n";
            return false;
        }

        std::ostringstream query;
        query << "DELETE FROM game_s_platfo WHERE idGame = " << gameId << " AND idPlatform = "
              << platformId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in removeGameFromPlatform: " << e.what() << "\n";
        return false;
    }
}

bool Platform::updateGamePlatformPrice(int gameId, int platformId, double newPrice) {
    try {
        if (gameId <= 0 || platformId <= 0 || newPrice < 0) {
            std::cerr << "ERROR: Invalid parameters!\n";
            return false;
        }

        std::ostringstream query;
        query << "UPDATE game_s_platfo SET Price = " << newPrice << " WHERE idGame = " << gameId
              << " AND idPlatform = " << platformId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in updateGamePlatformPrice: " << e.what() << "\n";
        return false;
    }
}

std::vector<PlatformData> Platform::getAllPlatforms() {
    std::vector<PlatformData> platforms;
    try {
        std::string query = "SELECT idPlatform, Platform_name, Type, Description FROM platform;";
        MYSQL_RES* result = db->getQueryResult(query);

        if (!result) {
            std::cerr << "ERROR: Failed to get platforms from database!\n";
            return platforms;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            PlatformData platform;
            platform.idPlatform = std::stoi(row[0] ? row[0] : "0");
            platform.platformName = row[1] ? row[1] : "";
            platform.type = row[2] ? row[2] : "";
            platform.description = row[3] ? row[3] : "";
            platforms.push_back(platform);
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getAllPlatforms: " << e.what() << "\n";
    }
    return platforms;
}

PlatformData Platform::getPlatformById(int platformId) {
    PlatformData platform = {-1, "", "", ""};
    try {
        if (platformId <= 0) {
            return platform;
        }

        std::ostringstream query;
        query << "SELECT idPlatform, Platform_name, Type, Description FROM platform WHERE idPlatform = "
              << platformId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return platform;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            platform.idPlatform = std::stoi(row[0] ? row[0] : "0");
            platform.platformName = row[1] ? row[1] : "";
            platform.type = row[2] ? row[2] : "";
            platform.description = row[3] ? row[3] : "";
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getPlatformById: " << e.what() << "\n";
    }
    return platform;
}

std::vector<GamePlatformData> Platform::getGamesPlatforms(int gameId) {
    std::vector<GamePlatformData> results;
    try {
        if (gameId <= 0) {
            return results;
        }

        std::ostringstream query;
        query << "SELECT idGame, idPlatform, ReleaseDate, Price FROM game_s_platfo WHERE idGame = "
              << gameId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return results;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            GamePlatformData gp;
            gp.idGame = std::stoi(row[0] ? row[0] : "0");
            gp.idPlatform = std::stoi(row[1] ? row[1] : "0");
            gp.releaseDate = row[2] ? row[2] : "";
            gp.price = std::stod(row[3] ? row[3] : "0.0");
            results.push_back(gp);
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getGamesPlatforms: " << e.what() << "\n";
    }
    return results;
}

std::vector<GamePlatformData> Platform::getPlatformGames(int platformId) {
    std::vector<GamePlatformData> results;
    try {
        if (platformId <= 0) {
            return results;
        }

        std::ostringstream query;
        query << "SELECT idGame, idPlatform, ReleaseDate, Price FROM game_s_platfo WHERE idPlatform = "
              << platformId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return results;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            GamePlatformData gp;
            gp.idGame = std::stoi(row[0] ? row[0] : "0");
            gp.idPlatform = std::stoi(row[1] ? row[1] : "0");
            gp.releaseDate = row[2] ? row[2] : "";
            gp.price = std::stod(row[3] ? row[3] : "0.0");
            results.push_back(gp);
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getPlatformGames: " << e.what() << "\n";
    }
    return results;
}

int Platform::getPlatformCount() {
    try {
        std::string query = "SELECT COUNT(*) FROM platform;";
        MYSQL_RES* result = db->getQueryResult(query);
        
        if (!result) {
            std::cerr << "ERROR: Failed to get platform count from database!\n";
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
        std::cerr << "ERROR in getPlatformCount: " << e.what() << "\n";
        return 0;
    }
}



