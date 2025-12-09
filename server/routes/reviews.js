const express = require('express');
const pool = require('../config/database');
const { authenticateToken, requireModerator } = require('../middleware/auth');

const router = express.Router();

// Get reviews for a game
router.get('/game/:gameId', async (req, res) => {
  try {
    const { gameId } = req.params;

    const [reviews] = await pool.execute(
      `SELECT 
        r.idReview,
        r.idUser,
        r.idGame,
        r.idPlatform,
        r.Review_text,
        r.Mark,
        r.ReviewDate,
        r.Validation,
        u.Username,
        p.Platform_name
      FROM review r
      INNER JOIN user u ON r.idUser = u.idUser
      INNER JOIN platform p ON r.idPlatform = p.idPlatform
      WHERE r.idGame = ? AND r.IsActive = 1
      ORDER BY r.ReviewDate DESC`,
      [gameId]
    );

    res.json(reviews);
  } catch (error) {
    console.error('Get reviews error:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

// Create review
router.post('/game/:gameId', authenticateToken, async (req, res) => {
  try {
    const { gameId } = req.params;
    const { Mark, Review_text, idPlatform } = req.body;

    if (!Mark || !idPlatform) {
      return res.status(400).json({ error: 'Mark and platform are required' });
    }

    if (Mark < 1 || Mark > 10) {
      return res.status(400).json({ error: 'Mark must be between 1 and 10' });
    }

    // Check if user already reviewed this game on this platform
    const [existing] = await pool.execute(
      'SELECT idReview FROM review WHERE idUser = ? AND idGame = ? AND idPlatform = ? AND IsActive = 1',
      [req.user.idUser, gameId, idPlatform]
    );

    if (existing.length > 0) {
      return res.status(400).json({ error: 'You have already reviewed this game on this platform' });
    }

    const [result] = await pool.execute(
      'INSERT INTO review (idUser, idGame, idPlatform, Review_text, Mark) VALUES (?, ?, ?, ?, ?)',
      [req.user.idUser, gameId, idPlatform, Review_text || null, Mark]
    );

    // Calculate average mark for this game (including the new review)
    const [marks] = await pool.execute(
      'SELECT AVG(Mark) as avgMark FROM review WHERE idGame = ? AND IsActive = 1',
      [gameId]
    );

    const avgMark = marks[0].avgMark ? parseFloat(marks[0].avgMark).toFixed(2) : 0;

    // Update all reviews' AvgMark for this game
    await pool.execute(
      'UPDATE review SET AvgMark = ? WHERE idGame = ?',
      [avgMark, gameId]
    );

    // Log activity (optional, may fail if table doesn't exist)
    try {
      await pool.execute(
        'INSERT INTO activitylog (idUser, Action, TableName, RecordID, NewValue) VALUES (?, ?, ?, ?, ?)',
        [req.user.idUser, 'CREATE', 'review', result.insertId, JSON.stringify(req.body)]
      );
    } catch (logError) {
      console.warn('Failed to log activity:', logError.message);
      // Don't fail the request if logging fails
    }

    res.status(201).json({ message: 'Review created successfully', idReview: result.insertId });
  } catch (error) {
    console.error('Create review error:', error);
    console.error('Error details:', {
      message: error.message,
      code: error.code,
      sqlState: error.sqlState,
      sqlMessage: error.sqlMessage
    });
    res.status(500).json({ 
      error: 'Internal server error',
      details: process.env.NODE_ENV === 'development' ? error.message : undefined
    });
  }
});

// Delete review (own review or moderator)
router.delete('/:id', authenticateToken, async (req, res) => {
  try {
    const { id } = req.params;

    // Check if review exists and get owner
    const [reviews] = await pool.execute(
      'SELECT idUser FROM review WHERE idReview = ?',
      [id]
    );

    if (reviews.length === 0) {
      return res.status(404).json({ error: 'Review not found' });
    }

    // Check permissions
    const isOwner = reviews[0].idUser === req.user.idUser;
    const isModerator = req.user.privilege === 'admin' || req.user.privilege === 'moderator';

    if (!isOwner && !isModerator) {
      return res.status(403).json({ error: 'Permission denied' });
    }

    // Soft delete
    await pool.execute('UPDATE review SET IsActive = 0 WHERE idReview = ?', [id]);

    // Log activity
    await pool.execute(
      'INSERT INTO activitylog (idUser, Action, TableName, RecordID) VALUES (?, ?, ?, ?)',
      [req.user.idUser, 'DELETE', 'review', id]
    );

    res.json({ message: 'Review deleted successfully' });
  } catch (error) {
    console.error('Delete review error:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

module.exports = router;

