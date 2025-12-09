#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <memory>
#include <string>
#include <vector>
#include "DatabaseConnection.hpp"

struct PlatformData {
    int idPlatform;
    std::string platformName;
    std::string type;
    std::string description;
};

struct GamePlatformData {
    int idGame;
    int idPlatform;
    std::string releaseDate;
    double price;
};

class Platform {
private:
    std::shared_ptr<DatabaseConnection> db;

public:
    explicit Platform(std::shared_ptr<DatabaseConnection> database);
    
    // Platform CRUD operations
    bool addPlatform(const std::string& platformName, const std::string& type,
                    const std::string& description);
    bool deletePlatform(int platformId);
    bool updatePlatform(int platformId, const std::string& platformName,
                       const std::string& type, const std::string& description);
    
    // Game-Platform operations
    bool addGameToPlatform(int gameId, int platformId, const std::string& releaseDate, double price);
    bool removeGameFromPlatform(int gameId, int platformId);
    bool updateGamePlatformPrice(int gameId, int platformId, double newPrice);
    
    // Read operations
    std::vector<PlatformData> getAllPlatforms();
    PlatformData getPlatformById(int platformId);
    std::vector<GamePlatformData> getGamesPlatforms(int gameId);
    std::vector<GamePlatformData> getPlatformGames(int platformId);
    int getPlatformCount();
};

#endif // PLATFORM_HPP