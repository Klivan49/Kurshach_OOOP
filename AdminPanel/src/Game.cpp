#include "../headers/Game.hpp"
#include <sstream>
#include <iostream>

Game::Game(std::shared_ptr<DatabaseConnection> database) : db(database) {
    if (!db) {
        throw std::runtime_error("ERROR: Database connection is null!");
    }
}

bool Game::addGame(const std::string& name, int Release_year, const std::string& description,
                   double Production_cost, int idCategory, const std::string& imageURL) {
    try {
        if (name.empty() || Release_year < 1990 || Release_year > 2100 || idCategory <= 0) {
            std::cerr << "ERROR: Invalid game parameters!\n";
            return false;
        }

        // Escape single quotes in strings for SQL
        std::string escapedName = name;
        std::string escapedDesc = description;
        std::string escapedImageURL = imageURL;
        size_t pos = 0;
        while ((pos = escapedName.find("'", pos)) != std::string::npos) {
            escapedName.replace(pos, 1, "''");
            pos += 2;
        }
        pos = 0;
        while ((pos = escapedDesc.find("'", pos)) != std::string::npos) {
            escapedDesc.replace(pos, 1, "''");
            pos += 2;
        }
        pos = 0;
        while ((pos = escapedImageURL.find("'", pos)) != std::string::npos) {
            escapedImageURL.replace(pos, 1, "''");
            pos += 2;
        }

        std::ostringstream query;
        query << "INSERT INTO game (Name, Release_year, Description, Production_cost, idCategory, ImageURL) "
              << "VALUES ('" << escapedName << "', " << Release_year << ", '" << escapedDesc << "', "
              << Production_cost << ", " << idCategory << ", ";
        if (imageURL.empty()) {
            query << "NULL";
        } else {
            query << "'" << escapedImageURL << "'";
        }
        query << ");";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in addGame: " << e.what() << "\n";
        return false;
    }
}

bool Game::deleteGame(int gameId) {
    try {
        if (gameId <= 0) {
            std::cerr << "ERROR: Invalid game ID!\n";
            return false;
        }

        std::ostringstream query;
        query << "DELETE FROM game WHERE idGame = " << gameId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in deleteGame: " << e.what() << "\n";
        return false;
    }
}

bool Game::updateGame(int gameId, const std::string& name, int Release_year,
                     const std::string& description, double Production_cost, const std::string& imageURL) {
    try {
        if (gameId <= 0 || name.empty()) {
            std::cerr << "ERROR: Invalid parameters!\n";
            return false;
        }

        // Escape single quotes
        std::string escapedName = name;
        std::string escapedDesc = description;
        std::string escapedImageURL = imageURL;
        size_t pos = 0;
        while ((pos = escapedName.find("'", pos)) != std::string::npos) {
            escapedName.replace(pos, 1, "''");
            pos += 2;
        }
        pos = 0;
        while ((pos = escapedDesc.find("'", pos)) != std::string::npos) {
            escapedDesc.replace(pos, 1, "''");
            pos += 2;
        }
        pos = 0;
        while ((pos = escapedImageURL.find("'", pos)) != std::string::npos) {
            escapedImageURL.replace(pos, 1, "''");
            pos += 2;
        }

        std::ostringstream query;
        query << "UPDATE game SET Name = '" << escapedName << "', Release_year = " << Release_year
              << ", Description = '" << escapedDesc << "', Production_cost = "
              << Production_cost << ", ImageURL = ";
        if (imageURL.empty()) {
            query << "NULL";
        } else {
            query << "'" << escapedImageURL << "'";
        }
        query << " WHERE idGame = " << gameId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in updateGame: " << e.what() << "\n";
        return false;
    }
}

bool Game::addCategory(const std::string& categoryName, const std::string& description) {
    try {
        if (categoryName.empty()) {
            std::cerr << "ERROR: Category name cannot be empty!\n";
            return false;
        }

        std::ostringstream query;
        query << "INSERT INTO category (CategoryName, Description) "
              << "VALUES ('" << categoryName << "', '" << description << "');";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in addCategory: " << e.what() << "\n";
        return false;
    }
}

bool Game::deleteCategory(int categoryId) {
    try {
        if (categoryId <= 0) {
            std::cerr << "ERROR: Invalid category ID!\n";
            return false;
        }

        std::ostringstream query;
        query << "DELETE FROM category WHERE idCategory = " << categoryId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in deleteCategory: " << e.what() << "\n";
        return false;
    }
}

bool Game::updateCategory(int categoryId, const std::string& categoryName,
                         const std::string& description) {
    try {
        if (categoryId <= 0 || categoryName.empty()) {
            std::cerr << "ERROR: Invalid parameters!\n";
            return false;
        }

        std::ostringstream query;
        query << "UPDATE category SET CategoryName = '" << categoryName
              << "', Description = '" << description << "' WHERE idCategory = "
              << categoryId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in updateCategory: " << e.what() << "\n";
        return false;
    }
}

std::vector<GameData> Game::getAllGames() {
    std::vector<GameData> games;
    try {
        std::string query = "SELECT idGame, Name, Release_year, Description, Production_cost, idCategory, ImageURL, IsActive "
                           "FROM game;";
        MYSQL_RES* result = db->getQueryResult(query);

        if (!result) {
            std::cerr << "ERROR: Failed to get games from database!\n";
            return games;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            GameData game;
            game.idGame = std::stoi(row[0] ? row[0] : "0");
            game.name = row[1] ? row[1] : "";
            game.releaseYear = std::stoi(row[2] ? row[2] : "0");
            game.description = row[3] ? row[3] : "";
            game.productionCost = std::stod(row[4] ? row[4] : "0.0");
            game.idCategory = std::stoi(row[5] ? row[5] : "0");
            game.imageURL = row[6] ? row[6] : "";
            game.isActive = (row[7] && std::string(row[7]) == "1");
            games.push_back(game);
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getAllGames: " << e.what() << "\n";
    }
    return games;
}

GameData Game::getGameById(int gameId) {
    GameData game = {-1, "", 0, "", 0.0, 0, "", false};
    try {
        if (gameId <= 0) {
            std::cerr << "ERROR: Invalid game ID!\n";
            return game;
        }

        std::ostringstream query;
        query << "SELECT idGame, Name, Release_year, Description, Production_cost, idCategory, ImageURL, IsActive "
              << "FROM game WHERE idGame = " << gameId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return game;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            game.idGame = std::stoi(row[0] ? row[0] : "0");
            game.name = row[1] ? row[1] : "";
            game.releaseYear = std::stoi(row[2] ? row[2] : "0");
            game.description = row[3] ? row[3] : "";
            game.productionCost = std::stod(row[4] ? row[4] : "0.0");
            game.idCategory = std::stoi(row[5] ? row[5] : "0");
            game.imageURL = row[6] ? row[6] : "";
            game.isActive = (row[7] && std::string(row[7]) == "1");
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getGameById: " << e.what() << "\n";
    }
    return game;
}

std::vector<GameData> Game::getGamesByCategory(int categoryId) {
    std::vector<GameData> games;
    try {
        if (categoryId <= 0) {
            std::cerr << "ERROR: Invalid category ID!\n";
            return games;
        }

        std::ostringstream query;
        query << "SELECT idGame, Name, Release_year, Description, Production_cost, idCategory, ImageURL, IsActive "
              << "FROM game WHERE idCategory = " << categoryId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return games;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            GameData game;
            game.idGame = std::stoi(row[0] ? row[0] : "0");
            game.name = row[1] ? row[1] : "";
            game.releaseYear = std::stoi(row[2] ? row[2] : "0");
            game.description = row[3] ? row[3] : "";
            game.productionCost = std::stod(row[4] ? row[4] : "0.0");
            game.idCategory = std::stoi(row[5] ? row[5] : "0");
            game.imageURL = row[6] ? row[6] : "";
            game.isActive = (row[7] && std::string(row[7]) == "1");
            games.push_back(game);
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getGamesByCategory: " << e.what() << "\n";
    }
    return games;
}

std::vector<CategoryData> Game::getAllCategories() {
    std::vector<CategoryData> categories;
    try {
        std::string query = "SELECT idCategory, CategoryName, Description FROM category;";
        MYSQL_RES* result = db->getQueryResult(query);

        if (!result) {
            std::cerr << "ERROR: Failed to get categories from database!\n";
            return categories;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            CategoryData category;
            category.idCategory = std::stoi(row[0] ? row[0] : "0");
            category.categoryName = row[1] ? row[1] : "";
            category.description = row[2] ? row[2] : "";
            categories.push_back(category);
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getAllCategories: " << e.what() << "\n";
    }
    return categories;
}

CategoryData Game::getCategoryById(int categoryId) {
    CategoryData category = {-1, "", ""};
    try {
        if (categoryId <= 0) {
            std::cerr << "ERROR: Invalid category ID!\n";
            return category;
        }

        std::ostringstream query;
        query << "SELECT idCategory, CategoryName, Description FROM category WHERE idCategory = "
              << categoryId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return category;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            category.idCategory = std::stoi(row[0] ? row[0] : "0");
            category.categoryName = row[1] ? row[1] : "";
            category.description = row[2] ? row[2] : "";
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getCategoryById: " << e.what() << "\n";
    }
    return category;
}

int Game::getGameCount() {
    try {
        std::string query = "SELECT COUNT(*) FROM game;";
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
        std::cerr << "ERROR in getGameCount: " << e.what() << "\n";
        return 0;
    }
}

int Game::getCategoryCount() {
    try {
        std::string query = "SELECT COUNT(*) FROM category;";
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
        std::cerr << "ERROR in getCategoryCount: " << e.what() << "\n";
        return 0;
    }
}





