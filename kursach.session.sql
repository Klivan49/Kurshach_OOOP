-- =====================================================
-- Game Rating Database Schema (ИСПРАВЛЕННАЯ)
-- =====================================================

-- Create database
CREATE DATABASE IF NOT EXISTS game_rating;
USE game_rating;

-- =====================================================
-- User Table
-- =====================================================
CREATE TABLE IF NOT EXISTS user (
    idUser INT AUTO_INCREMENT PRIMARY KEY,
    Username VARCHAR(255) UNIQUE NOT NULL,
    PasswordHash VARCHAR(255) NOT NULL,
    Privilege ENUM('admin', 'moderator', 'user') DEFAULT 'user',
    RegistrationDate DATETIME DEFAULT CURRENT_TIMESTAMP,
    IsActive TINYINT DEFAULT 1,
    LastLoginDate DATETIME,
    INDEX idx_username (Username),
    INDEX idx_active (IsActive)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================
-- Category Table
-- =====================================================
CREATE TABLE IF NOT EXISTS category (
    idCategory INT AUTO_INCREMENT PRIMARY KEY,
    CategoryName VARCHAR(100) NOT NULL UNIQUE,
    Description TEXT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================
-- Game Table
-- =====================================================
CREATE TABLE IF NOT EXISTS game (
    idGame INT AUTO_INCREMENT PRIMARY KEY,
    Name VARCHAR(255) NOT NULL,
    Release_year INT,
    Description TEXT,
    ImageURL VARCHAR(500) NULL,
    Production_cost DECIMAL(12,2),
    idCategory INT NOT NULL,
    CreatedDate DATETIME DEFAULT CURRENT_TIMESTAMP,
    UpdatedDate DATETIME ON UPDATE CURRENT_TIMESTAMP,
    IsActive TINYINT DEFAULT 1,
    FOREIGN KEY (idCategory) REFERENCES category(idCategory) ON DELETE CASCADE,
    INDEX idx_category (idCategory),
    INDEX idx_active (IsActive),
    INDEX idx_name (Name)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================
-- Platform Table
-- =====================================================
CREATE TABLE IF NOT EXISTS platform (
    idPlatform INT AUTO_INCREMENT PRIMARY KEY,
    Platform_name VARCHAR(100) NOT NULL UNIQUE,
    Type VARCHAR(50),
    Description TEXT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================
-- Game-Platform Junction Table
-- =====================================================
CREATE TABLE IF NOT EXISTS game_s_platfo (
    idGame INT NOT NULL,
    idPlatform INT NOT NULL,
    ReleaseDate DATE,
    Price DECIMAL(8,2),
    PRIMARY KEY (idGame, idPlatform),
    FOREIGN KEY (idGame) REFERENCES game(idGame) ON DELETE CASCADE,
    FOREIGN KEY (idPlatform) REFERENCES platform(idPlatform) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================
-- Review Table
-- =====================================================
CREATE TABLE IF NOT EXISTS review (
    idReview INT AUTO_INCREMENT PRIMARY KEY,
    idUser INT NOT NULL,
    idGame INT NOT NULL,
    idPlatform INT NOT NULL,
    Review_text TEXT,
    Mark INT CHECK (Mark >= 1 AND Mark <= 10),
    AvgMark DECIMAL(3,2),
    ReviewDate DATETIME DEFAULT CURRENT_TIMESTAMP,
    Validation VARCHAR(50),
    IsActive TINYINT DEFAULT 1,
    FOREIGN KEY (idUser) REFERENCES user(idUser) ON DELETE CASCADE,
    FOREIGN KEY (idGame) REFERENCES game(idGame) ON DELETE CASCADE,
    FOREIGN KEY (idPlatform) REFERENCES platform(idPlatform) ON DELETE CASCADE,
    INDEX idx_game (idGame),
    INDEX idx_user (idUser),
    INDEX idx_platform (idPlatform),
    INDEX idx_active (IsActive)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================
-- Activity Log Table - ИСПРАВЛЕННАЯ ВЕРСИЯ
-- =====================================================
-- ВАРИАНТ 1: idUser может быть NULL (рекомендуется)
CREATE TABLE IF NOT EXISTS activitylog (
    idLog INT AUTO_INCREMENT PRIMARY KEY,
    idUser INT,
    Action VARCHAR(255),
    TableName VARCHAR(100),
    RecordID INT,
    OldValue TEXT,
    NewValue TEXT,
    Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    IPAddress VARCHAR(45),
    FOREIGN KEY (idUser) REFERENCES user(idUser) ON DELETE SET NULL,
    INDEX idx_user (idUser),
    INDEX idx_timestamp (Timestamp),
    INDEX idx_action (Action)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================
-- Sample Data
-- =====================================================

-- Insert sample categories
INSERT INTO category (CategoryName, Description) VALUES
    ('RPG', 'Role-Playing Games'),
    ('Action', 'Action Adventure Games'),
    ('Strategy', 'Strategic Games'),
    ('Sports', 'Sports and Racing'),
    ('Puzzle', 'Puzzle Games');

-- Insert sample platforms
INSERT INTO platform (Platform_name, Type, Description) VALUES
    ('PlayStation 5', 'Console', 'Sony PS5'),
    ('Xbox Series X', 'Console', 'Microsoft Xbox'),
    ('Nintendo Switch', 'Console', 'Nintendo portable'),
    ('PC Steam', 'PC', 'Valve Steam Platform'),
    ('Epic Games Store', 'PC', 'Epic Games Platform');

-- Insert sample user
INSERT INTO user (Username, PasswordHash, Privilege) VALUES
    ('admin', '7110eda4d09e062aa5e4a390b0a572ac0d2c64d7', 'admin');

-- =====================================================
-- End of Schema (FIXED VERSION)
-- =====================================================INSERT INTO game (
    idGame,
    Name,
    Release_year,
    Description,
    Production_cost,
    idCategory,
    CreatedDate,
    UpdatedDate,
    IsActive
  )
VALUES (
    idGame:int,
    'Name:varchar',
    Release_year:int,
    'Description:text',
    'Production_cost:decimal',
    idCategory:int,
    'CreatedDate:datetime',
    'UpdatedDate:datetime',
    'IsActive:tinyint'
  );