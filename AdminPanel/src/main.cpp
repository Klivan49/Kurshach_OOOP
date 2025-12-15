#include <iostream>
#include <memory>
#include <filesystem>
#include <exception>
#include <fstream>
#include <nlohmann/json.hpp>

#include "../headers/DatabaseConnection.hpp"
#include "../headers/User.hpp"
#include "../headers/Game.hpp"
#include "../headers/Review.hpp"
#include "../headers/Platform.hpp"
#include "../headers/AdminPanel.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

void printStartupInfo() {
    std::cout << "\n";
    std::cout << "════════════════════════════════════════════════════════════\n";
    std::cout << "                                                            \n";
    std::cout << "        GAME RATING ADMIN PANEL v2.0                        \n";
    std::cout << "        Enhanced with Image Support                         \n";
    std::cout << "                                                            \n";
    std::cout << "════════════════════════════════════════════════════════════\n\n";
}

int main() {
    try {
        printStartupInfo();

        // ========== DATABASE CONNECTION ==========
        std::cout << "[*] Connecting to database...\n";
        std::shared_ptr<DatabaseConnection> db;

        std::ifstream file("config.json");

        if (!file.is_open()) {
            std::cerr << "[✗] ERROR: Could not open config.json\n";
            std::cerr << "[*] Current working directory: " << fs::current_path() << "\n";
            std::cerr << "[*] Looked for file at: " << fs::absolute("../config.json") << "\n";
            throw std::runtime_error("config.json not found");
        }

        json obj = json::parse(file);

        std::string host = obj["database"]["host"].get<std::string>();
        std::string user = obj["database"]["user"].get<std::string>();
        std::string password = obj["database"]["password"].get<std::string>();
        std::string database = obj["database"]["database"].get<std::string>();
        
        try {
            db = std::make_shared<DatabaseConnection>(
                host,
                user,
                password,  
                database
            );
        } catch (const std::exception& e) {
            std::cerr << "\n[✗] FATAL ERROR: Cannot connect to database!\n";
            std::cerr << "    " << e.what() << "\n";
            std::cerr << "\n    Please check:\n";
            std::cerr << "    1. MySQL Server is running\n";
            std::cerr << "    2. Database 'game_rating' exists\n";
            std::cerr << "    3. Username and password are correct\n";
            std::cerr << "    4. Port 3306 is accessible\n\n";
            return 1;
        }

        // ========== MANAGERS INITIALIZATION ==========
        std::cout << "[*] Initializing managers...\n";
        
        std::shared_ptr<User> userMgr;
        try {
            userMgr = std::make_shared<User>(db);
            std::cout << "    [✓] User manager initialized\n";
        } catch (const std::exception& e) {
            std::cerr << "    [✗] Failed to initialize User manager: " << e.what() << "\n";
            return 1;
        }

        std::shared_ptr<Game> gameMgr;
        try {
            gameMgr = std::make_shared<Game>(db);
            std::cout << "    [✓] Game manager initialized\n";
        } catch (const std::exception& e) {
            std::cerr << "    [✗] Failed to initialize Game manager: " << e.what() << "\n";
            return 1;
        }

        std::shared_ptr<Review> reviewMgr;
        try {
            reviewMgr = std::make_shared<Review>(db);
            std::cout << "    [✓] Review manager initialized\n";
        } catch (const std::exception& e) {
            std::cerr << "    [✗] Failed to initialize Review manager: " << e.what() << "\n";
            return 1;
        }

        std::shared_ptr<Platform> platformMgr;
        try {
            platformMgr = std::make_shared<Platform>(db);
            std::cout << "    [✓] Platform manager initialized\n";
        } catch (const std::exception& e) {
            std::cerr << "    [✗] Failed to initialize Platform manager: " << e.what() << "\n";
            return 1;
        }

        // ========== ADMIN PANEL INITIALIZATION ==========
        std::cout << "[*] Starting Admin Panel...\n\n";
        
        std::shared_ptr<AdminPanel> panel;
        try {
            panel = std::make_shared<AdminPanel>(db, userMgr, gameMgr, reviewMgr, platformMgr);
        } catch (const std::exception& e) {
            std::cerr << "[✗] Failed to initialize Admin Panel: " << e.what() << "\n";
            return 1;
        }

        // ========== MAIN APPLICATION LOOP ==========
        std::cout << "[✓] All systems initialized successfully!\n";
        std::cout << "[*] Entering main application loop...\n\n";
        
        try {
            panel->run();
        } catch (const std::exception& e) {
            std::cerr << "\n[✗] FATAL ERROR in main application: " << e.what() << "\n";
            return 1;
        }

        // ========== CLEANUP ==========
        std::cout << "\n[*] Shutting down...\n";
        panel = nullptr;
        userMgr = nullptr;
        gameMgr = nullptr;
        reviewMgr = nullptr;
        platformMgr = nullptr;
        db = nullptr;
        
        std::cout << "[✓] Application closed successfully.\n\n";
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n[✗] CATASTROPHIC ERROR: " << e.what() << "\n";
        std::cerr << "Application will terminate.\n\n";
        return 1;
    } catch (...) {
        std::cerr << "\n[✗] UNKNOWN CATASTROPHIC ERROR!\n";
        std::cerr << "Application will terminate.\n\n";
        return 1;
    }
}
