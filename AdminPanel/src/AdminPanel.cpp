#include "../headers/AdminPanel.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

AdminPanel::AdminPanel(std::shared_ptr<DatabaseConnection> connection,
                      std::shared_ptr<User> user,
                      std::shared_ptr<Game> game,
                      std::shared_ptr<Review> review,
                      std::shared_ptr<Platform> platform)
    : db(connection), userMgr(user), gameMgr(game), reviewMgr(review), platformMgr(platform) {
    if (!db || !userMgr || !gameMgr || !reviewMgr || !platformMgr) {
        throw std::runtime_error("ERROR: Null pointers passed to AdminPanel constructor!");
    }
}

// ============= DISPLAY FUNCTIONS =============

void AdminPanel::displayAvailableUsers() {
    try {
        std::vector<UserData> users = userMgr->getAllUsers();
        if (users.empty()) {
            std::cout << "[ℹ] No users in database!\n";
            return;
        }

        std::cout << "\n════════════════════════════════════════════════════════\n";
        std::cout << "          AVAILABLE USERS                               \n";
        std::cout << "════════════════════════════════════════════════════════\n";
        std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Username" 
                  << std::setw(15) << "Privilege" << "Status\n";
        std::cout << "──────────────────────────────────────────────────────\n";

        for (const auto& user : users) {
            std::cout << std::left << std::setw(5) << user.idUser 
                      << std::setw(20) << user.username 
                      << std::setw(15) << user.privilege 
                      << (user.isActive ? "Active ✓" : "Inactive ✗") << "\n";
        }
        std::cout << "\n";
    } catch (const std::exception& e) {
        std::cerr << "ERROR displaying users: " << e.what() << "\n";
    }
}

void AdminPanel::displayAvailableGames() {
    try {
        std::vector<GameData> games = gameMgr->getAllGames();
        if (games.empty()) {
            std::cout << "[ℹ] No games in database!\n";
            return;
        }

        std::cout << "\n══════════════════════════════════════════════════════════════════════════════\n";
        std::cout << "                        AVAILABLE GAMES                                      \n";
        std::cout << "══════════════════════════════════════════════════════════════════════════════\n";
        std::cout << std::left << std::setw(5) << "ID" << std::setw(25) << "Name" 
                  << std::setw(10) << "Year" << std::setw(12) << "Category" 
                  << std::setw(8) << "Image" << "Status\n";
        std::cout << "────────────────────────────────────────────────────────────────────────────────\n";

        for (const auto& game : games) {
            std::string categoryName = "Unknown";
            try {
                CategoryData cat = gameMgr->getCategoryById(game.idCategory);
                if (cat.idCategory > 0) {
                    categoryName = cat.categoryName;
                }
            } catch (...) {}

            std::string imageStatus = game.imageURL.empty() ? "No" : "Yes";

            std::cout << std::left << std::setw(5) << game.idGame 
                      << std::setw(25) << (game.name.length() > 24 ? game.name.substr(0, 21) + "..." : game.name)
                      << std::setw(10) << game.releaseYear 
                      << std::setw(12) << categoryName 
                      << std::setw(8) << imageStatus
                      << (game.isActive ? "Active ✓" : "Inactive ✗") << "\n";
        }
        std::cout << "\n";
    } catch (const std::exception& e) {
        std::cerr << "ERROR displaying games: " << e.what() << "\n";
    }
}

void AdminPanel::displayAvailablePlatforms() {
    try {
        std::vector<PlatformData> platforms = platformMgr->getAllPlatforms();
        if (platforms.empty()) {
            std::cout << "[ℹ] No platforms in database!\n";
            return;
        }

        std::cout << "\n════════════════════════════════════════════════════════\n";
        std::cout << "           AVAILABLE PLATFORMS                          \n";
        std::cout << "════════════════════════════════════════════════════════\n";
        std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Platform Name" 
                  << std::setw(15) << "Type" << "\n";
        std::cout << "──────────────────────────────────────────────────────\n";

        for (const auto& platform : platforms) {
            std::cout << std::left << std::setw(5) << platform.idPlatform 
                      << std::setw(20) << platform.platformName 
                      << std::setw(15) << platform.type << "\n";
        }
        std::cout << "\n";
    } catch (const std::exception& e) {
        std::cerr << "ERROR displaying platforms: " << e.what() << "\n";
    }
}

void AdminPanel::displayAvailableCategories() {
    try {
        std::vector<CategoryData> categories = gameMgr->getAllCategories();
        if (categories.empty()) {
            std::cout << "[ℹ] No categories in database!\n";
            return;
        }

        std::cout << "\n════════════════════════════════════════════════════════\n";
        std::cout << "            AVAILABLE CATEGORIES                        \n";
        std::cout << "════════════════════════════════════════════════════════\n";
        std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Category Name" 
                  << "Description\n";
        std::cout << "──────────────────────────────────────────────────────\n";

        for (const auto& cat : categories) {
            std::cout << std::left << std::setw(5) << cat.idCategory 
                      << std::setw(20) << cat.categoryName 
                      << cat.description << "\n";
        }
        std::cout << "\n";
    } catch (const std::exception& e) {
        std::cerr << "ERROR displaying categories: " << e.what() << "\n";
    }
}

// ============= VALIDATION FUNCTIONS =============

bool AdminPanel::validateUserId(int userId) {
    if (userId <= 0) return false;
    try {
        UserData user = userMgr->getUserById(userId);
        return user.idUser > 0;
    } catch (...) {
        return false;
    }
}

bool AdminPanel::validateGameId(int gameId) {
    if (gameId <= 0) return false;
    try {
        GameData game = gameMgr->getGameById(gameId);
        return game.idGame > 0;
    } catch (...) {
        return false;
    }
}

bool AdminPanel::validatePlatformId(int platformId) {
    if (platformId <= 0) return false;
    try {
        PlatformData platform = platformMgr->getPlatformById(platformId);
        return platform.idPlatform > 0;
    } catch (...) {
        return false;
    }
}

bool AdminPanel::validateCategoryId(int categoryId) {
    if (categoryId <= 0) return false;
    try {
        CategoryData category = gameMgr->getCategoryById(categoryId);
        return category.idCategory > 0;
    } catch (...) {
        return false;
    }
}

bool AdminPanel::validateMarkRange(int mark) {
    return mark >= 1 && mark <= 10;
}

bool AdminPanel::validateStringInput(const std::string& input, int minLen, int maxLen) {
    int len = input.length();
    return len >= minLen && len <= maxLen && !input.empty();
}

// ============= USER INPUT FUNCTIONS =============

int AdminPanel::getValidatedUserId() {
    displayAvailableUsers();
    int userId;
    while (true) {
        std::cout << "Enter user ID (from list above): ";
        if (!(std::cin >> userId)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cerr << "✗ Invalid input! Please enter a number.\n";
            continue;
        }
        
        if (validateUserId(userId)) {
            std::cout << "[✓] User ID valid!\n";
            return userId;
        } else {
            std::cerr << "✗ ERROR: User ID " << userId << " does not exist!\n";
            std::cout << "[↻] Please try again.\n\n";
        }
    }
}

int AdminPanel::getValidatedGameId() {
    displayAvailableGames();
    int gameId;
    while (true) {
        std::cout << "Enter game ID (from list above): ";
        if (!(std::cin >> gameId)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cerr << "✗ Invalid input! Please enter a number.\n";
            continue;
        }
        
        if (validateGameId(gameId)) {
            std::cout << "[✓] Game ID valid!\n";
            return gameId;
        } else {
            std::cerr << "✗ ERROR: Game ID " << gameId << " does not exist!\n";
            std::cout << "[↻] Please try again.\n\n";
        }
    }
}

int AdminPanel::getValidatedPlatformId() {
    displayAvailablePlatforms();
    int platformId;
    while (true) {
        std::cout << "Enter platform ID (from list above): ";
        if (!(std::cin >> platformId)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cerr << "✗ Invalid input! Please enter a number.\n";
            continue;
        }
        
        if (validatePlatformId(platformId)) {
            std::cout << "[✓] Platform ID valid!\n";
            return platformId;
        } else {
            std::cerr << "✗ ERROR: Platform ID " << platformId << " does not exist!\n";
            std::cout << "[↻] Please try again.\n\n";
        }
    }
}

int AdminPanel::getValidatedMark() {
    int mark;
    while (true) {
        std::cout << "Enter mark (1-10): ";
        if (!(std::cin >> mark)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cerr << "✗ Invalid input! Please enter a number.\n";
            continue;
        }
        
        if (validateMarkRange(mark)) {
            std::cout << "[✓] Mark valid!\n";
            return mark;
        } else {
            std::cerr << "✗ ERROR: Mark must be between 1-10!\n";
            std::cout << "[↻] Please try again.\n";
        }
    }
}


std::string AdminPanel::getValidatedText(const std::string& prompt, int minLen, int maxLen) {
    std::string text;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, text);

        if (validateStringInput(text, minLen, maxLen)) {
            std::cout << "[✓] Text valid!\n";
            return text;
        } else {
            std::cerr << "✗ ERROR: Text must be " << minLen << "-" << maxLen << " characters!\n";
            std::cout << "[↻] Please try again.\n";
        }
    }
}

std::string AdminPanel::getValidatedUsername() {
    std::string username;

    while (true) {
        std::cout << "Enter username (3-50 characters): ";
        std::getline(std::cin, username);

        // Проверка длины
        if (!validateStringInput(username, 3, 50)) {
            std::cerr << "✗ ERROR: Username must be 3-50 characters!\n";
            std::cout << "[↻] Please try again.\n";
            continue;
        }

        // Проверка уникальности в БД
        if (userMgr->userExists(username)) {
            std::cerr << "✗ ERROR: Username '" << username << "' already exists!\n";
            std::cout << "[↻] Please try again.\n";
            continue;
        }

        std::cout << "[✓] Username valid and available!\n";
        return username;
    }
}


std::string AdminPanel::getValidatedPassword() {
    std::string password;

    while (true) {
        std::cout << "Enter password (6-100 characters): ";
        std::getline(std::cin, password);

        if (validateStringInput(password, 6, 100)) {
            std::cout << "[✓] Password valid!\n";
            return password;
        } else {
            std::cerr << "✗ ERROR: Password must be 6-100 characters!\n";
            std::cout << "[↻] Please try again.\n";
        }
    }
}

// ============= SCREEN FUNCTIONS =============

void AdminPanel::clearScreen() {
    try {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    } catch (...) {}
}

void AdminPanel::printHeader(const std::string& title) {
    std::cout << "\n════════════════════════════════════════════════\n";
    std::cout << " " << std::left << std::setw(56) << title << "\n";
    std::cout << "════════════════════════════════════════════════\n\n";
}

int AdminPanel::getKeyPress() {
    #ifdef _WIN32
        return _getch();
    #else
        struct termios oldSettings, newSettings;
        tcgetattr(STDIN_FILENO, &oldSettings);
        newSettings = oldSettings;
        newSettings.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
        
        int ch = std::getchar();
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
        return ch;
    #endif
}


void AdminPanel::displayMenu(const std::vector<std::string>& options, int selected) {
    for (int i = 0; i < static_cast<int>(options.size()); ++i) {
        if (i == selected) {
            std::cout << "▶ " << (i + 1) << ". " << options[i] << " ◀\n";
        } else {
            std::cout << "  " << (i + 1) << ". " << options[i] << "\n";
        }
    }
}


// ============= MAIN MENU =============

void AdminPanel::run() {
    int selectedOption = 0;

    while (true) {
        clearScreen();
        printHeader("GAME RATING ADMIN PANEL");
        
        // Display statistics
        try {
            int gameCount = gameMgr->getGameCount();
            int categoryCount = gameMgr->getCategoryCount();
            int userCount = userMgr->getUserCount();
            int platformCount = platformMgr->getPlatformCount();
            
            std::cout << "════════════════════════════════════════════════════════════\n";
            std::cout << "                    STATISTICS                             \n";
            std::cout << "════════════════════════════════════════════════════════════\n";
            std::cout << "  Games: " << std::setw(47) << std::left << gameCount << "\n";
            std::cout << "  Categories: " << std::setw(43) << std::left << categoryCount << "\n";
            std::cout << "  Users: " << std::setw(48) << std::left << userCount << "\n";
            std::cout << "  Platforms: " << std::setw(44) << std::left << platformCount << "\n";
            std::cout << "════════════════════════════════════════════════════════════\n\n";
        } catch (...) {
            // Ignore errors in statistics
        }

        std::vector<std::string> mainMenuOptions = {
            "User Management",
            "Game Management",
            "Platform Management",
            "Review Management",
            "Exit"
        };

        displayMenu(mainMenuOptions, selectedOption);
        std::cout << "\n[↑/↓ Navigate] [Enter Select] [ESC Exit]\n";


        int key = getKeyPress();

        if (key == 27) {  // ESC
            break;
        }

        if (key == 224) {  // Arrow keys (Windows)
            key = getKeyPress();
            if (key == 72) selectedOption--;     // Up
            else if (key == 80) selectedOption++; // Down
        } else if (key == 13) {  // Enter
            switch (selectedOption) {
                case 0: handleUserMenu(); break;
                case 1: handleGameMenu(); break;
                case 2: handlePlatformMenu(); break;
                case 3: handleReviewMenu(); break;
                case 4: return;
                default: break;
            }
            selectedOption = 0;
        }

        // Wrap around
        if (selectedOption < 0) selectedOption = mainMenuOptions.size() - 1;
        if (selectedOption >= static_cast<int>(mainMenuOptions.size())) selectedOption = 0;
    }

    std::cout << "\n[✓] Goodbye!\n";
}

// ============= USER MENU =============

void AdminPanel::handleUserMenu() {
    int selectedOption = 0;

    while (true) {
        clearScreen();
        printHeader("User Management");

        std::vector<std::string> options = {
            "Show All Users",
            "Add New User",
            "Delete User",
            "Update User Privilege",
            "Back to Main Menu"
        };

        displayMenu(options, selectedOption);
        std::cout << "\n[↑/↓ Navigate] [Enter Select] [ESC Back]\n";

        int key = getKeyPress();

        if (key == 27) {  // ESC
            return;
        }

        if (key == 224) {  // Arrow keys
            key = getKeyPress();
            if (key == 72) selectedOption--;
            else if (key == 80) selectedOption++;
        } else if (key == 13) {  // Enter
            switch (selectedOption) {
                case 0: showAllUsers(); break;
                case 1: addNewUser(); break;
                case 2: deleteUser(); break;
                case 3: updateUserPrivilege(); break;
                case 4: return;
                default: break;
            }
        }

        if (selectedOption < 0) selectedOption = options.size() - 1;
        if (selectedOption >= static_cast<int>(options.size())) selectedOption = 0;
    }
}

void AdminPanel::showAllUsers() {
    clearScreen();
    printHeader("All Users");
    displayAvailableUsers();
    std::cout << "Press any key to continue...";
    std::getchar();
}

void AdminPanel::addNewUser() {
    clearScreen();
    printHeader("Add New User");

    std::cout << "[ℹ] Username: 3-50 characters\n";
    std::cout << "[ℹ] Password: 6-100 characters\n";
    std::cout << "[ℹ] Privilege: admin, moderator, or user\n\n";

    std::string username = getValidatedUsername();
    std::string password = getValidatedPassword();

    std::string privilege;
    while (true) {
        std::cout << "Enter privilege (admin/moderator/user): ";
        std::getline(std::cin, privilege);
        if (privilege == "admin" || privilege == "moderator" || privilege == "user") {
            std::cout << "[✓] Privilege valid!\n";
            break;
        } else {
            std::cerr << "✗ ERROR: Invalid privilege!\n";
        }
    }

    std::string hashedPassword = userMgr->hashPassword(password);
    if (userMgr->addUser(username, hashedPassword, privilege)) {
        std::cout << "\n[✓] User added successfully!\n";
    } else {
        std::cerr << "\n✗ Failed to add user!\n";
    }

    std::cout << "Press any key to continue...";
    std::getchar();
}

void AdminPanel::deleteUser() {
    clearScreen();
    printHeader("Delete User");

    int userId = getValidatedUserId();
    
    if (userMgr->deleteUser(userId)) {
        std::cout << "\n[✓] User deleted successfully!\n";
    } else {
        std::cerr << "\n✗ Failed to delete user!\n";
    }

    std::cout << "Press any key to continue...";
    std::getchar();
}

void AdminPanel::updateUserPrivilege() {
    clearScreen();
    printHeader("Update User Privilege");

    int userId = getValidatedUserId();

    std::string privilege;
    while (true) {
        std::cout << "Enter new privilege (admin/moderator/user): ";
        std::getline(std::cin, privilege);
        if (privilege == "admin" || privilege == "moderator" || privilege == "user") {
            std::cout << "[✓] Privilege valid!\n";
            break;
        } else {
            std::cerr << "✗ ERROR: Invalid privilege!\n";
        }
    }

    if (userMgr->updateUser(userId, "", privilege)) {
        std::cout << "\n[✓] Privilege updated successfully!\n";
    } else {
        std::cerr << "\n✗ Failed to update privilege!\n";
    }

    std::cout << "Press any key to continue...";
    std::getchar();
}

// ============= GAME MENU =============

void AdminPanel::handleGameMenu() {
    int selectedOption = 0;

    while (true) {
        clearScreen();
        printHeader("Game Management");

        std::vector<std::string> options = {
            "Show All Games",
            "Add New Game",
            "Update Game",
            "Delete Game",
            "Manage Categories",
            "Back to Main Menu"
        };

        displayMenu(options, selectedOption);
        std::cout << "\n[↑/↓ Navigate] [Enter Select] [ESC Back]\n";

        int key = getKeyPress();

        if (key == 27) return;

        if (key == 224) {
            key = getKeyPress();
            if (key == 72) selectedOption--;
            else if (key == 80) selectedOption++;
        } else if (key == 13) {
            switch (selectedOption) {
                case 0: showAllGames(); break;
                case 1: addNewGame(); break;
                case 2: updateGame(); break;
                case 3: deleteGame(); break;
                case 4: manageCategories(); break;
                case 5: return;
                default: break;
            }
        }

        if (selectedOption < 0) selectedOption = options.size() - 1;
        if (selectedOption >= static_cast<int>(options.size())) selectedOption = 0;
    }
}

void AdminPanel::showAllGames() {
    clearScreen();
    printHeader("All Games");
    displayAvailableGames();
    std::cout << "Press any key to continue...";
    std::getchar();
}

void AdminPanel::addNewGame() {
    clearScreen();
    printHeader("Add New Game");

    displayAvailableCategories();

    std::string name = getValidatedText("Enter game name (3-255 chars): ", 3, 255);

    int releaseYear;
    while (true) {
        std::cout << "Enter release year (1990-2100): ";
        if (std::cin >> releaseYear && releaseYear >= 1990 && releaseYear <= 2100) {
            std::cout << "[✓] Year valid!\n";
            break;
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cerr << "✗ ERROR: Year must be 1990-2100!\n";
        }
    }
    std::cin.ignore();

    std::string description = getValidatedText("Enter description (10-1000 chars): ", 10, 1000);

    double productionCost;
    while (true) {
        std::cout << "Enter production cost (0 or more): ";
        if (std::cin >> productionCost && productionCost >= 0) {
            std::cout << "[✓] Cost valid!\n";
            break;
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cerr << "✗ ERROR: Cost must be 0 or more!\n";
        }
    }
    std::cin.ignore();

    std::cout << "\n[ℹ] Image URL (optional, press Enter to skip):\n";
    std::cout << "[ℹ] Examples:\n";
    std::cout << "    - https://cdn.cloudflare.steamstatic.com/steam/apps/730/header.jpg\n";
    std::cout << "    - https://images.igdb.com/igdb/image/upload/t_cover_big/co1wyy.jpg\n";
    std::cout << "    - Leave empty for placeholder\n\n";
    std::string imageURL;
    std::cout << "Enter image URL (or press Enter to skip): ";
    std::getline(std::cin, imageURL);
    if (!imageURL.empty()) {
        std::cout << "[✓] Image URL set!\n";
    } else {
        std::cout << "[ℹ] No image URL - will use placeholder\n";
    }

    int categoryId = getValidatedCategoryId();

    if (gameMgr->addGame(name, releaseYear, description, productionCost, categoryId, imageURL)) {
        std::cout << "\n[✓] Game added successfully!\n";
        if (!imageURL.empty()) {
            std::cout << "[✓] Image URL saved: " << imageURL << "\n";
        }
    } else {
        std::cerr << "\n✗ Failed to add game!\n";
    }

    std::cout << "\nPress any key to continue...";
    std::getchar();
}

int AdminPanel::getValidatedCategoryId() {
    displayAvailableCategories();
    int categoryId;
    while (true) {
        std::cout << "Enter category ID (from list above): ";
        if (!(std::cin >> categoryId)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cerr << "✗ Invalid input!\n";
            continue;
        }
        if (validateCategoryId(categoryId)) {
            std::cout << "[✓] Category ID valid!\n";
            return categoryId;
        } else {
            std::cerr << "✗ ERROR: Category ID does not exist!\n";
        }
    }
}

void AdminPanel::updateGame() {
    clearScreen();
    printHeader("Update Game");

    int gameId = getValidatedGameId();
    
    GameData currentGame = gameMgr->getGameById(gameId);
    if (currentGame.idGame <= 0) {
        std::cerr << "\n✗ ERROR: Game not found!\n";
        std::cout << "Press any key to continue...";
        std::getchar();
        return;
    }

    std::cout << "\n[ℹ] Current game information:\n";
    std::cout << "    Name: " << currentGame.name << "\n";
    std::cout << "    Year: " << currentGame.releaseYear << "\n";
    std::cout << "    Description: " << (currentGame.description.length() > 50 ? currentGame.description.substr(0, 47) + "..." : currentGame.description) << "\n";
    std::cout << "    Cost: $" << currentGame.productionCost << "\n";
    std::cout << "    Image URL: " << (currentGame.imageURL.empty() ? "Not set" : currentGame.imageURL) << "\n\n";

    std::cout << "[ℹ] Enter new values (press Enter to keep current value):\n\n";

    std::string name;
    std::cout << "Enter game name (current: " << currentGame.name << "): ";
    std::getline(std::cin, name);
    if (name.empty()) {
        name = currentGame.name;
    } else if (!validateStringInput(name, 3, 255)) {
        std::cerr << "✗ Invalid name length! Keeping current value.\n";
        name = currentGame.name;
    }

    int releaseYear = currentGame.releaseYear;
    std::string yearInput;
    std::cout << "Enter release year (current: " << currentGame.releaseYear << "): ";
    std::getline(std::cin, yearInput);
    if (!yearInput.empty()) {
        try {
            int newYear = std::stoi(yearInput);
            if (newYear >= 1990 && newYear <= 2100) {
                releaseYear = newYear;
            } else {
                std::cerr << "✗ Invalid year! Keeping current value.\n";
            }
        } catch (...) {
            std::cerr << "✗ Invalid input! Keeping current value.\n";
        }
    }

    std::string description;
    std::cout << "Enter description (current: " << (currentGame.description.length() > 30 ? currentGame.description.substr(0, 27) + "..." : currentGame.description) << "): ";
    std::getline(std::cin, description);
    if (description.empty()) {
        description = currentGame.description;
    } else if (!validateStringInput(description, 10, 1000)) {
        std::cerr << "✗ Invalid description length! Keeping current value.\n";
        description = currentGame.description;
    }

    double productionCost = currentGame.productionCost;
    std::string costInput;
    std::cout << "Enter production cost (current: $" << currentGame.productionCost << "): ";
    std::getline(std::cin, costInput);
    if (!costInput.empty()) {
        try {
            double newCost = std::stod(costInput);
            if (newCost >= 0) {
                productionCost = newCost;
            } else {
                std::cerr << "✗ Invalid cost! Keeping current value.\n";
            }
        } catch (...) {
            std::cerr << "✗ Invalid input! Keeping current value.\n";
        }
    }

    std::string imageURL = currentGame.imageURL;
    std::cout << "\n[ℹ] Image URL (current: " << (currentGame.imageURL.empty() ? "Not set" : currentGame.imageURL) << "):\n";
    std::cout << "    Enter new URL or 'clear' to remove, or press Enter to keep: ";
    std::string imageInput;
    std::getline(std::cin, imageInput);
    if (imageInput == "clear" || imageInput == "CLEAR") {
        imageURL = "";
        std::cout << "[✓] Image URL will be cleared\n";
    } else if (!imageInput.empty()) {
        imageURL = imageInput;
        std::cout << "[✓] Image URL updated\n";
    }

    if (gameMgr->updateGame(gameId, name, releaseYear, description, productionCost, imageURL)) {
        std::cout << "\n[✓] Game updated successfully!\n";
    } else {
        std::cerr << "\n✗ Failed to update game!\n";
    }

    std::cout << "Press any key to continue...";
    std::getchar();
}

void AdminPanel::deleteGame() {
    clearScreen();
    printHeader("Delete Game");

    int gameId = getValidatedGameId();

    if (gameMgr->deleteGame(gameId)) {
        std::cout << "\n[✓] Game deleted successfully!\n";
    } else {
        std::cerr << "\n✗ Failed to delete game!\n";
    }

    std::cout << "Press any key to continue...";
    std::getchar();
}

void AdminPanel::manageCategories() {
    int selectedOption = 0;

    while (true) {
        clearScreen();
        printHeader("Manage Categories");

        std::vector<std::string> options = {
            "Show All Categories",
            "Add New Category",
            "Delete Category",
            "Back"
        };

        displayMenu(options, selectedOption);

        int key = getKeyPress();

        if (key == 27) return;

        if (key == 224) {
            key = getKeyPress();
            if (key == 72) selectedOption--;
            else if (key == 80) selectedOption++;
        } else if (key == 13) {
            switch (selectedOption) {
                case 0: {
                    clearScreen();
                    printHeader("All Categories");
                    displayAvailableCategories();
                    std::cout << "Press any key to continue...";
                    std::getchar();
                    break;
                }
                case 1: {
                    clearScreen();
                    printHeader("Add New Category");
                    std::string name = getValidatedText("Enter category name (3-100 chars): ", 3, 100);
                    std::string desc = getValidatedText("Enter description (10-500 chars): ", 10, 500);
                    
                    if (gameMgr->addCategory(name, desc)) {
                        std::cout << "\n[✓] Category added!\n";
                    } else {
                        std::cerr << "\n✗ Failed to add category!\n";
                    }
                    std::cout << "Press any key to continue...";
                    std::getchar();
                    break;
                }
                case 2: {
                    clearScreen();
                    printHeader("Delete Category");
                    int categoryId = getValidatedCategoryId();
                    if (gameMgr->deleteCategory(categoryId)) {
                        std::cout << "\n[✓] Category deleted!\n";
                    } else {
                        std::cerr << "\n✗ Failed to delete category!\n";
                    }
                    std::cout << "Press any key to continue...";
                    std::getchar();
                    break;
                }
                case 3: return;
                default: break;
            }
        }

        if (selectedOption < 0) selectedOption = options.size() - 1;
        if (selectedOption >= static_cast<int>(options.size())) selectedOption = 0;
    }
}

// ============= REVIEW MENU =============

void AdminPanel::handleReviewMenu() {
    int selectedOption = 0;

    while (true) {
        clearScreen();
        printHeader("Review Management");

        std::vector<std::string> options = {
            "Add Review",
            "View Game Reviews",
            "Back to Main Menu"
        };

        displayMenu(options, selectedOption);
        std::cout << "\n[↑/↓ Navigate] [Enter Select] [ESC Back]\n";

        int key = getKeyPress();

        if (key == 27) return;

        if (key == 224) {
            key = getKeyPress();
            if (key == 72) selectedOption--;
            else if (key == 80) selectedOption++;
        } else if (key == 13) {
            switch (selectedOption) {
                case 0: addReview(); break;
                case 1: viewGameReviews(); break;
                case 2: return;
                default: break;
            }
        }

        if (selectedOption < 0) selectedOption = options.size() - 1;
        if (selectedOption >= static_cast<int>(options.size())) selectedOption = 0;
    }
}

void AdminPanel::addReview() {
    clearScreen();
    printHeader("Add Review");

    std::cout << "[ℹ] Select from lists below:\n\n";

    int userId = getValidatedUserId();
    int gameId = getValidatedGameId();
    int platformId = getValidatedPlatformId();
    int mark = getValidatedMark();
    std::string reviewText = getValidatedText("Enter review text (1-500 chars): ", 1, 500);

    if (reviewMgr->addReview(userId, gameId, platformId, reviewText, mark)) {
        std::cout << "\n[✓] Review added successfully!\n";
    } else {
        std::cerr << "\n✗ Failed to add review!\n";
    }

    std::cout << "Press any key to continue...";
    std::getchar();
}

void AdminPanel::viewGameReviews() {
    clearScreen();
    printHeader("View Game Reviews");

    int gameId = getValidatedGameId();

    std::vector<ReviewData> reviews = reviewMgr->getGameReviews(gameId);

    if (reviews.empty()) {
        std::cout << "[ℹ] No reviews for this game.\n";
    } else {
        std::cout << "\n════════════════════════════════════════════════════════════════════\n";
        std::cout << "              GAME REVIEWS                                          \n";
        std::cout << "════════════════════════════════════════════════════════════════════\n";
        std::cout << std::left << std::setw(5) << "ID" << std::setw(15) << "User ID" 
                  << std::setw(8) << "Mark" << "Review Text\n";
        std::cout << "────────────────────────────────────────────────────────────────────\n";

        for (const auto& review : reviews) {
            std::string truncated = review.reviewText.length() > 40 
                ? review.reviewText.substr(0, 37) + "..." 
                : review.reviewText;
            
            std::cout << std::left << std::setw(5) << review.idReview 
                      << std::setw(15) << review.idUser 
                      << std::setw(8) << review.mark 
                      << truncated << "\n";
        }
    }

    std::cout << "\nPress any key to continue...";
    std::getchar();
}

// ============= PLATFORM MENU =============

void AdminPanel::handlePlatformMenu() {
    int selectedOption = 0;

    while (true) {
        clearScreen();
        printHeader("Platform Management");

        std::vector<std::string> options = {
            "Show All Platforms",
            "Add New Platform",
            "Delete Platform",
            "Back to Main Menu"
        };

        displayMenu(options, selectedOption);
        std::cout << "\n[↑/↓ Navigate] [Enter Select] [ESC Back]\n";

        int key = getKeyPress();

        if (key == 27) return;

        if (key == 224) {
            key = getKeyPress();
            if (key == 72) selectedOption--;
            else if (key == 80) selectedOption++;
        } else if (key == 13) {
            switch (selectedOption) {
                case 0: showAllPlatforms(); break;
                case 1: addNewPlatform(); break;
                case 2: deletePlatform(); break;
                case 3: return;
                default: break;
            }
        }

        if (selectedOption < 0) selectedOption = options.size() - 1;
        if (selectedOption >= static_cast<int>(options.size())) selectedOption = 0;
    }
}

void AdminPanel::showAllPlatforms() {
    clearScreen();
    printHeader("All Platforms");
    displayAvailablePlatforms();
    std::cout << "Press any key to continue...";
    std::getchar();
}

void AdminPanel::addNewPlatform() {
    clearScreen();
    printHeader("Add New Platform");

    std::string name = getValidatedText("Enter platform name (2-100 chars): ", 2, 100);
    std::string type = getValidatedText("Enter type (2-50 chars): ", 2, 50);
    std::string description = getValidatedText("Enter description (5-500 chars): ", 5, 500);

    if (platformMgr->addPlatform(name, type, description)) {
        std::cout << "\n[✓] Platform added successfully!\n";
    } else {
        std::cerr << "\n✗ Failed to add platform!\n";
    }

    std::cout << "Press any key to continue...";
    std::getchar();
}

void AdminPanel::deletePlatform() {
    clearScreen();
    printHeader("Delete Platform");

    int platformId = getValidatedPlatformId();

    if (platformMgr->deletePlatform(platformId)) {
        std::cout << "\n[✓] Platform deleted successfully!\n";
    } else {
        std::cerr << "\n✗ Failed to delete platform!\n";
    }

    std::cout << "Press any key to continue...";
    std::getchar();
}