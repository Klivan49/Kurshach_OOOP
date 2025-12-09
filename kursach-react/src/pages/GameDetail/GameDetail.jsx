import React, { useState, useEffect } from 'react';
import { getReviews, deleteReview } from '../../api';
import ReviewForm from '../ReviewForm/ReviewForm';
import './GameDetail.css';

function GameDetail({ game, user, onClose, onReviewAdded }) {
  const [reviews, setReviews] = useState([]);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    if (game) {
      loadReviews();
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [game]);

  const loadReviews = async () => {
    try {
      setLoading(true);
      const response = await getReviews(game.idGame);
      setReviews(response.data);
    } catch (error) {
      console.error('Error loading reviews:', error);
    } finally {
      setLoading(false);
    }
  };

  const handleReviewAdded = () => {
    loadReviews();
    onReviewAdded && onReviewAdded();
  };

  const handleDeleteReview = async (reviewId) => {
    if (window.confirm('Are you sure you want to delete this review?')) {
      try {
        await deleteReview(reviewId);
        loadReviews();
        onReviewAdded && onReviewAdded();
      } catch (error) {
        console.error('Error deleting review:', error);
        alert('Failed to delete review');
      }
    }
  };

  const getScoreColor = (score) => {
    if (score >= 8) return '#66cc33';
    if (score >= 6) return '#ffbb00';
    return '#ff4444';
  };

  const getScoreClass = (score) => {
    if (score >= 8) return 'positive';
    if (score >= 6) return 'mixed';
    return 'negative';
  };

  const avgScore = game.AvgMark ? Number(game.AvgMark).toFixed(1) : null;

  return (
    <div className="game-detail-overlay" onClick={onClose}>
      <div className="game-detail" onClick={(e) => e.stopPropagation()}>
        <button className="close-btn" onClick={onClose}>×</button>

        <div className="detail-header">
          <div className="detail-image">
            <img
              src={
                game.image ||
                `https://via.placeholder.com/300x450/1a1a1a/66cc33?text=${encodeURIComponent(game.Name)}`
              }
              alt={game.Name}
            />
          </div>

          <div className="detail-info">
            <h1 className="detail-title">{game.Name}</h1>

            <div className="detail-meta">
              <div className="meta-item">
                <span className="meta-label">Genre:</span>
                <span className="meta-value">{game.CategoryName || 'N/A'}</span>
              </div>
              <div className="meta-item">
                <span className="meta-label">Release:</span>
                <span className="meta-value">{game.Release_year || 'N/A'}</span>
              </div>
              {game.Production_cost && (
                <div className="meta-item">
                  <span className="meta-label">Budget:</span>
                  <span className="meta-value">
                    ${Number(game.Production_cost).toLocaleString()}
                  </span>
                </div>
              )}
            </div>

            <div className="score-section">
              {avgScore ? (
                <>
                  <div className={`metascore ${getScoreClass(avgScore)}`}>
                    <span className="metascore-value">{avgScore}</span>
                  </div>
                  <div className="score-info">
                    <p className="score-label">Metascore</p>
                    <p className="review-count">
                      Based on {reviews.length} {reviews.length === 1 ? 'review' : 'reviews'}
                    </p>
                  </div>
                </>
              ) : (
                <div className="no-score">
                  <p>No reviews yet</p>
                  <p className="review-count">Be the first to review!</p>
                </div>
              )}
            </div>

            {game.Description && (
              <div className="description">
                <h3>Summary</h3>
                <p>{game.Description}</p>
              </div>
            )}
          </div>
        </div>

        <div className="detail-reviews">
          <div className="reviews-header">
            <h2>User Reviews</h2>
            {user && (
              <button
                className="add-review-btn"
                onClick={() => {
                  document.querySelector('.review-form')?.scrollIntoView({ behavior: 'smooth' });
                }}
              >
                Write a Review
              </button>
            )}
          </div>

          {user && (
            <div className="review-form-container">
              <ReviewForm gameId={game.idGame} onSubmit={handleReviewAdded} />
            </div>
          )}

          {loading ? (
            <div className="loading">Loading reviews...</div>
          ) : reviews.length > 0 ? (
            <div className="reviews-list">
              {reviews.map((review) => (
                <div key={review.idReview} className="review-item">
                  <div className="review-header">
                    <div className="review-user">
                      <span className="review-username">{review.Username}</span>
                      <span className="review-platform">{review.Platform_name}</span>
                    </div>
                    <div className={`review-score ${getScoreClass(review.Mark)}`}>
                      {review.Mark}
                    </div>
                  </div>
                  <div className="review-date">
                    {new Date(review.ReviewDate).toLocaleDateString('en-US', {
                      year: 'numeric',
                      month: 'long',
                      day: 'numeric'
                    })}
                  </div>
                  {review.Review_text && (
                    <div className="review-text">{review.Review_text}</div>
                  )}
                  {(user && (user.idUser === review.idUser || user.Privilege === 'admin' || user.Privilege === 'moderator')) && (
                    <button
                      className="delete-review-btn"
                      onClick={() => handleDeleteReview(review.idReview)}
                    >
                      Delete
                    </button>
                  )}
                </div>
              ))}
            </div>
          ) : (
            <p className="no-reviews">
              {user
                ? 'Be the first to review this game!'
                : 'Sign in to leave a review'}
            </p>
          )}
        </div>
      </div>
    </div>
  );
}

export default GameDetail;
