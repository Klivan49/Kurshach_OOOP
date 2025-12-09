import React from 'react';
import './GameCard.css';

function GameCard({ game, onClick }) {
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
  const reviewCount = game.reviewCount || 0;

  return (
    <div className="game-card" onClick={onClick}>
      <div className="game-image-container">
        <img
          src={
            game.image ||
            `https://via.placeholder.com/200x300/1a1a1a/66cc33?text=${encodeURIComponent(game.Name.substring(0, 20))}`
          }
          alt={game.Name}
          className="game-image"
        />
        {avgScore && (
          <div className={`game-score ${getScoreClass(avgScore)}`}>
            <span className="score-value">{avgScore}</span>
          </div>
        )}
      </div>

      <div className="game-info">
        <h3 className="game-title">{game.Name}</h3>
        <div className="game-meta">
          <span className="game-category">{game.CategoryName || 'Uncategorized'}</span>
          {game.Release_year && (
            <span className="game-year">{game.Release_year}</span>
          )}
        </div>
        <div className="game-footer">
          <span className="review-count">
            {reviewCount} {reviewCount === 1 ? 'review' : 'reviews'}
          </span>
          {game.platforms && (
            <span className="platforms">{game.platforms.split(',').slice(0, 2).join(', ')}</span>
          )}
        </div>
      </div>
    </div>
  );
}

export default GameCard;
