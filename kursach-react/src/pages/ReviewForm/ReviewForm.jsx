import React, { useState, useEffect } from 'react';
import { addReview, getPlatforms } from '../../api';
import './ReviewForm.css';

function ReviewForm({ gameId, onSubmit }) {
  const [rating, setRating] = useState(5);
  const [comment, setComment] = useState('');
  const [platform, setPlatform] = useState('');
  const [platforms, setPlatforms] = useState([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [success, setSuccess] = useState('');

  useEffect(() => {
    loadPlatforms();
  }, []);

  const loadPlatforms = async () => {
    try {
      const response = await getPlatforms();
      setPlatforms(response.data);
      if (response.data.length > 0) {
        setPlatform(response.data[0].idPlatform);
      }
    } catch (err) {
      console.error('Error loading platforms:', err);
    }
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setError('');
    setSuccess('');
    setLoading(true);

    try {
      if (!platform) {
        setError('Please select a platform');
        setLoading(false);
        return;
      }

      await addReview(gameId, {
        Mark: rating,
        Review_text: comment,
        idPlatform: platform,
      });

      setSuccess('Review submitted successfully!');
      setRating(5);
      setComment('');

      setTimeout(() => {
        setSuccess('');
        onSubmit && onSubmit();
      }, 1500);
    } catch (err) {
      setError(
        err.response?.data?.error || 'Failed to submit review',
      );
    } finally {
      setLoading(false);
    }
  };

  const getScoreClass = (score) => {
    if (score >= 8) return 'positive';
    if (score >= 6) return 'mixed';
    return 'negative';
  };

  return (
    <form className="review-form" onSubmit={handleSubmit}>
      <h3>Write a Review</h3>

      {error && <div className="form-error">{error}</div>}
      {success && <div className="form-success">{success}</div>}

      <div className="form-group">
        <label htmlFor="rating">Your Score</label>
        <div className="rating-input-container">
          <div className={`rating-preview ${getScoreClass(rating)}`}>
            {rating}
          </div>
          <div className="rating-controls">
            <input
              id="rating"
              type="range"
              min="1"
              max="10"
              value={rating}
              onChange={(e) => setRating(parseInt(e.target.value, 10))}
              className="rating-slider"
            />
            <div className="rating-labels">
              <span>1</span>
              <span>10</span>
            </div>
          </div>
        </div>
      </div>

      <div className="form-group">
        <label htmlFor="platform">Platform</label>
        <select
          id="platform"
          value={platform}
          onChange={(e) => setPlatform(parseInt(e.target.value, 10))}
          className="form-select"
        >
          <option value="">Select platform</option>
          {platforms.map((p) => (
            <option key={p.idPlatform} value={p.idPlatform}>
              {p.Platform_name}
            </option>
          ))}
        </select>
      </div>

      <div className="form-group">
        <label htmlFor="comment">Review (Optional)</label>
        <textarea
          id="comment"
          value={comment}
          onChange={(e) => setComment(e.target.value)}
          placeholder="Share your thoughts about this game..."
          rows="6"
          className="form-textarea"
        />
      </div>

      <button type="submit" disabled={loading} className="submit-btn">
        {loading ? 'Submitting...' : 'Submit Review'}
      </button>
    </form>
  );
}

export default ReviewForm;
