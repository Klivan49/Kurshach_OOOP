#ifndef REVIEW_HPP
#define REVIEW_HPP

#include <memory>
#include <string>
#include <vector>
#include "DatabaseConnection.hpp"

struct ReviewData {
    int idReview;
    int idUser;
    int idGame;
    int idPlatform;
    std::string reviewText;
    int mark;
    double avgMark;
    std::string reviewDate;
    std::string validation;
    bool isActive;
};

class Review {
private:
    std::shared_ptr<DatabaseConnection> db;

public:
    explicit Review(std::shared_ptr<DatabaseConnection> database);
    
    // Review CRUD operations
    bool addReview(int idUser, int idGame, int idPlatform, 
                  const std::string& reviewText, int mark);
    bool deleteReview(int reviewId);
    bool updateReview(int reviewId, const std::string& reviewText, int mark);
    
    // Read operations
    std::vector<ReviewData> getAllReviews();
    ReviewData getReviewById(int reviewId);
    std::vector<ReviewData> getGameReviews(int gameId);
    std::vector<ReviewData> getUserReviews(int userId);
    std::vector<ReviewData> getPlatformReviews(int platformId);
    
    // Statistics
    double getAverageRating(int gameId);
    int getReviewCount();
    int getGameReviewCount(int gameId);
};

#endif // REVIEW_HPP