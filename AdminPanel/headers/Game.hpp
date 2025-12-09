#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <string>
#include <vector>
#include "DatabaseConnection.hpp"

struct CategoryData {
    int idCategory;
    std::string categoryName;
    std::string description;
};

struct GameData {
    int idGame;
    std::string name;
    int releaseYear;
    std::string description;
    double productionCost;
    int idCategory;
    std::string imageURL;
    bool isActive;
};

class Game {
private:
    std::shared_ptr<DatabaseConnection> db;

public:
    explicit Game(std::shared_ptr<DatabaseConnection> database);
    
    // Game CRUD operations
    bool addGame(const std::string& name, int releaseYear, const std::string& description,
                double productionCost, int idCategory, const std::string& imageURL = "");
    bool deleteGame(int gameId);
    bool updateGame(int gameId, const std::string& name, int releaseYear,
                   const std::string& description, double productionCost, const std::string& imageURL = "");
    
    // Category operations
    bool addCategory(const std::string& categoryName, const std::string& description);
    bool deleteCategory(int categoryId);
    bool updateCategory(int categoryId, const std::string& categoryName,
                       const std::string& description);
    
    // Read operations
    std::vector<GameData> getAllGames();
    GameData getGameById(int gameId);
    std::vector<GameData> getGamesByCategory(int categoryId);
    std::vector<CategoryData> getAllCategories();
    CategoryData getCategoryById(int categoryId);
    int getGameCount();
    int getCategoryCount();
};

#endif // GAME_HPP