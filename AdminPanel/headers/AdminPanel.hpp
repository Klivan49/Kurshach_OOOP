#ifndef ADMINPANEL_HPP
#define ADMINPANEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "DatabaseConnection.hpp"
#include "User.hpp"
#include "Game.hpp"
#include "Review.hpp"
#include "Platform.hpp"
#include <sstream>

// Для работы со стрелочками и Enter на Windows/Linux/macOS
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

class AdminPanel {
private:
    std::shared_ptr<DatabaseConnection> db;
    std::shared_ptr<User> userMgr;
    std::shared_ptr<Game> gameMgr;
    std::shared_ptr<Review> reviewMgr;
    std::shared_ptr<Platform> platformMgr;

    // Текущее состояние меню
    enum MenuState {
        MAIN_MENU,
        USER_MENU,
        GAME_MENU,
        REVIEW_MENU,
        PLATFORM_MENU,
        STATISTICS_MENU,
        EXIT
    };

    MenuState currentMenu = MAIN_MENU;
    int selectedOption = 0;

    // Вспомогательные функции
    void clearScreen();
    int getKeyPress(); // Получить нажатую клавишу (стрелочка, Enter, ESC)
    void displayMenu(const std::vector<std::string>& options, int selected);
    void printHeader(const std::string& title);
    void printSeparator();
    void pause();

    // Тройная буферизация
    // Обработчики меню
    void handleMainMenu();
    void handleUserMenu();
    void handleGameMenu();
    void handleReviewMenu();
    void handlePlatformMenu();
    void handleStatistics();

    // User операции
    void userShowAll();
    void userAddNew();
    void userDelete();
    void userUpdatePrivilege();

    // Game операции
    void gameShowAll();
    void gameAddNew();
    void gameDelete();
    void gameShowCategories();
    void gameAddCategory();

    // Platform операции
    void platformShowAll();
    void platformAddNew();
    void platformLinkGame();

    // Review операции
    void reviewShowAll();
    void reviewAddNew();
    void reviewDelete();
    void reviewByGame();

    // Utility
    std::string getUserInput(const std::string& prompt = "");
    int getIntInput(const std::string& prompt = "");
    double getDoubleInput(const std::string& prompt = "");
    bool askConfirmation(const std::string& question);

    private:
    // Display functions - показываем списки
    void displayAvailableUsers();
    void displayAvailableGames();
    void displayAvailablePlatforms();
    void displayAvailableCategories();

    // Validation functions - везде проверки
    bool validateUserId(int userId);
    bool validateGameId(int gameId);
    bool validatePlatformId(int platformId);
    bool validateCategoryId(int categoryId);
    bool validateMarkRange(int mark);
    bool validateStringInput(const std::string& input, int minLen, int maxLen);

    // Input functions - ввод с проверкой
    int getValidatedUserId();
    int getValidatedGameId();
    int getValidatedPlatformId();
    int getValidatedCategoryId();
    int getValidatedMark();
    std::string getValidatedText(const std::string& prompt, int minLen, int maxLen);
    std::string getValidatedUsername();
    std::string getValidatedPassword();

    // User operations
    void showAllUsers();
    void addNewUser();
    void deleteUser();
    void updateUserPrivilege();

    // Game operations
    void showAllGames();
    void addNewGame();
    void updateGame();
    void deleteGame();
    void manageCategories();

    // Review operations
    void addReview();
    void viewGameReviews();

    // Platform operations
    void showAllPlatforms();
    void addNewPlatform();
    void deletePlatform();

public:
    AdminPanel(std::shared_ptr<DatabaseConnection> connection,
               std::shared_ptr<User> user,
               std::shared_ptr<Game> game,
               std::shared_ptr<Review> review,
               std::shared_ptr<Platform> platform);

    void run();
};

#endif