#include "../headers/Review.hpp"
#include <sstream>
#include <iostream>

Review::Review(std::shared_ptr<DatabaseConnection> database) : db(database) {
    if (!db) {
        throw std::runtime_error("ERROR: Database connection is null!");
    }
}

bool Review::addReview(int idUser, int idGame, int idPlatform,
                      const std::string& Review_text, int mark) {
    try {
        if (idUser <= 0 || idGame <= 0 || idPlatform <= 0 || mark < 1 || mark > 10) {
            std::cerr << "ERROR: Invalid review parameters!\n";
            return false;
        }

        std::ostringstream query;
        query << "INSERT INTO review (idUser, idGame, idPlatform, Review_text, Mark) "
              << "VALUES (" << idUser << ", " << idGame << ", " << idPlatform << ", '"
              << Review_text << "', " << mark << ");";

        bool result = db->executeQuery(query.str());
        if (result) {
            std::cout << "[✓] Review added successfully!\n";
        } else {
            std::cerr << "✗ Failed to add review!\n";
        }
        return result;
    } catch (const std::exception& e) {
        std::cerr << "ERROR in addReview: " << e.what() << "\n";
        return false;
    }
}

bool Review::deleteReview(int reviewId) {
    try {
        if (reviewId <= 0) {
            std::cerr << "ERROR: Invalid review ID!\n";
            return false;
        }

        std::ostringstream query;
        query << "DELETE FROM review WHERE idReview = " << reviewId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in deleteReview: " << e.what() << "\n";
        return false;
    }
}

bool Review::updateReview(int reviewId, const std::string& Review_text, int mark) {
    try {
        if (reviewId <= 0 || mark < 1 || mark > 10) {
            std::cerr << "ERROR: Invalid parameters!\n";
            return false;
        }

        std::ostringstream query;
        query << "UPDATE review SET Review_text = '" << Review_text << "', Mark = " << mark
              << " WHERE idReview = " << reviewId << ";";

        return db->executeQuery(query.str());
    } catch (const std::exception& e) {
        std::cerr << "ERROR in updateReview: " << e.what() << "\n";
        return false;
    }
}

std::vector<ReviewData> Review::getAllReviews() {
    std::vector<ReviewData> reviews;
    try {
        std::string query = "SELECT idReview, idUser, idGame, idPlatform, Review_text, Mark, IsActive "
                           "FROM review;";
        MYSQL_RES* result = db->getQueryResult(query);

        if (!result) {
            std::cerr << "ERROR: Failed to get reviews from database!\n";
            return reviews;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            ReviewData review;
            review.idReview = std::stoi(row[0] ? row[0] : "0");
            review.idUser = std::stoi(row[1] ? row[1] : "0");
            review.idGame = std::stoi(row[2] ? row[2] : "0");
            review.idPlatform = std::stoi(row[3] ? row[3] : "0");
            review.reviewText = row[4] ? row[4] : "";
            review.mark = std::stoi(row[5] ? row[5] : "0");
            review.isActive = (row[6] && std::string(row[6]) == "1");
            reviews.push_back(review);
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getAllReviews: " << e.what() << "\n";
    }
    return reviews;
}

ReviewData Review::getReviewById(int reviewId) {
    ReviewData review = {-1, 0, 0, 0, "", 0, 0, "", "", false};
    try {
        if (reviewId <= 0) {
            return review;
        }

        std::ostringstream query;
        query << "SELECT idReview, idUser, idGame, idPlatform, Review_text, Mark, IsActive "
              << "FROM review WHERE idReview = " << reviewId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return review;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            review.idReview = std::stoi(row[0] ? row[0] : "0");
            review.idUser = std::stoi(row[1] ? row[1] : "0");
            review.idGame = std::stoi(row[2] ? row[2] : "0");
            review.idPlatform = std::stoi(row[3] ? row[3] : "0");
            review.reviewText = row[4] ? row[4] : "";
            review.mark = std::stoi(row[5] ? row[5] : "0");
            review.isActive = (row[6] && std::string(row[6]) == "1");
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getReviewById: " << e.what() << "\n";
    }
    return review;
}

std::vector<ReviewData> Review::getGameReviews(int gameId) {
    std::vector<ReviewData> reviews;
    try {
        if (gameId <= 0) {
            return reviews;
        }

        std::ostringstream query;
        query << "SELECT idReview, idUser, idGame, idPlatform, Review_text, Mark, IsActive "
              << "FROM review WHERE idGame = " << gameId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return reviews;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            ReviewData review;
            review.idReview = std::stoi(row[0] ? row[0] : "0");
            review.idUser = std::stoi(row[1] ? row[1] : "0");
            review.idGame = std::stoi(row[2] ? row[2] : "0");
            review.idPlatform = std::stoi(row[3] ? row[3] : "0");
            review.reviewText = row[4] ? row[4] : "";
            review.mark = std::stoi(row[5] ? row[5] : "0");
            review.isActive = (row[6] && std::string(row[6]) == "1");
            reviews.push_back(review);
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getGameReviews: " << e.what() << "\n";
    }
    return reviews;
}

std::vector<ReviewData> Review::getUserReviews(int userId) {
    std::vector<ReviewData> reviews;
    try {
        if (userId <= 0) {
            return reviews;
        }

        std::ostringstream query;
        query << "SELECT idReview, idUser, idGame, idPlatform, Review_text, Mark, IsActive "
              << "FROM review WHERE idUser = " << userId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return reviews;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            ReviewData review;
            review.idReview = std::stoi(row[0] ? row[0] : "0");
            review.idUser = std::stoi(row[1] ? row[1] : "0");
            review.idGame = std::stoi(row[2] ? row[2] : "0");
            review.idPlatform = std::stoi(row[3] ? row[3] : "0");
            review.reviewText = row[4] ? row[4] : "";
            review.mark = std::stoi(row[5] ? row[5] : "0");
            review.isActive = (row[6] && std::string(row[6]) == "1");
            reviews.push_back(review);
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getUserReviews: " << e.what() << "\n";
    }
    return reviews;
}

std::vector<ReviewData> Review::getPlatformReviews(int platformId) {
    std::vector<ReviewData> reviews;
    try {
        if (platformId <= 0) {
            return reviews;
        }

        std::ostringstream query;
        query << "SELECT idReview, idUser, idGame, idPlatform, Review_text, Mark, IsActive "
              << "FROM review WHERE idPlatform = " << platformId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return reviews;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            ReviewData review;
            review.idReview = std::stoi(row[0] ? row[0] : "0");
            review.idUser = std::stoi(row[1] ? row[1] : "0");
            review.idGame = std::stoi(row[2] ? row[2] : "0");
            review.idPlatform = std::stoi(row[3] ? row[3] : "0");
            review.reviewText = row[4] ? row[4] : "";
            review.mark = std::stoi(row[5] ? row[5] : "0");
            review.isActive = (row[6] && std::string(row[6]) == "1");
            reviews.push_back(review);
        }

        db->freeResult(result);
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getPlatformReviews: " << e.what() << "\n";
    }
    return reviews;
}

double Review::getAverageRating(int gameId) {
    try {
        if (gameId <= 0) {
            return 0.0;
        }

        std::ostringstream query;
        query << "SELECT AVG(Mark) FROM review WHERE idGame = " << gameId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
        if (!result) {
            return 0.0;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        double avg = 0.0;
        if (row && row[0]) {
            avg = std::stod(row[0]);
        }

        db->freeResult(result);
        return avg;
    } catch (const std::exception& e) {
        std::cerr << "ERROR in getAverageRating: " << e.what() << "\n";
        return 0.0;
    }
}

int Review::getReviewCount() {
    try {
        std::string query = "SELECT COUNT(*) FROM review;";
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
        std::cerr << "ERROR in getReviewCount: " << e.what() << "\n";
        return 0;
    }
}

int Review::getGameReviewCount(int gameId) {
    try {
        if (gameId <= 0) {
            return 0;
        }

        std::ostringstream query;
        query << "SELECT COUNT(*) FROM review WHERE idGame = " << gameId << ";";

        MYSQL_RES* result = db->getQueryResult(query.str());
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
        std::cerr << "ERROR in getGameReviewCount: " << e.what() << "\n";
        return 0;
    }
}





