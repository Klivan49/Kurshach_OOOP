const express = require('express');
const pool = require('../config/database');
const { authenticateToken, requireAdmin, requireModerator } = require('../middleware/auth');

const router = express.Router();

// Get all games with filters
router.get('/', async (req, res) => {
  try {
    const { category, platform, search, sortBy = 'rating', minRating } = req.query;

    let query = `
      SELECT 
        g.idGame,
        g.Name,
        g.Release_year,
        g.Description,
        g.Production_cost,
        g.idCategory,
        g.CreatedDate,
        g.ImageURL,
        c.CategoryName,
        COALESCE(AVG(r.Mark), 0) as AvgMark,
        COUNT(DISTINCT r.idReview) as reviewCount,
        GROUP_CONCAT(DISTINCT p.Platform_name) as platforms
      FROM game g
      LEFT JOIN category c ON g.idCategory = c.idCategory
      LEFT JOIN review r ON g.idGame = r.idGame AND r.IsActive = 1
      LEFT JOIN platform p ON r.idPlatform = p.idPlatform
      WHERE g.IsActive = 1
    `;

    const params = [];

    if (category) {
      query += ' AND g.idCategory = ?';
      params.push(category);
    }

    if (platform) {
      query += ' AND EXISTS (SELECT 1 FROM review r2 WHERE r2.idGame = g.idGame AND r2.idPlatform = ? AND r2.IsActive = 1)';
      params.push(platform);
    }

    if (search) {
      query += ' AND g.Name LIKE ?';
      params.push(`%${search}%`);
    }

    query += ' GROUP BY g.idGame';

    if (minRating) {
      query += ' HAVING AvgMark >= ?';
      params.push(minRating);
    }

    // Sorting
    switch (sortBy) {
      case 'rating':
        query += ' ORDER BY AvgMark DESC, reviewCount DESC';
        break;
      case 'newest':
        query += ' ORDER BY g.Release_year DESC';
        break;
      case 'name':
        query += ' ORDER BY g.Name ASC';
        break;
      case 'reviews':
        query += ' ORDER BY reviewCount DESC';
        break;
      default:
        query += ' ORDER BY AvgMark DESC';
    }

    const [games] = await pool.execute(query, params);

    res.json(games);
  } catch (error) {
    console.error('Get games error:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

// Get game by ID
router.get('/:id', async (req, res) => {
  try {
    const { id } = req.params;

    const [games] = await pool.execute(
      `SELECT 
        g.*,
        c.CategoryName,
        COALESCE(AVG(r.Mark), 0) as AvgMark,
        COUNT(DISTINCT r.idReview) as reviewCount
      FROM game g
      LEFT JOIN category c ON g.idCategory = c.idCategory
      LEFT JOIN review r ON g.idGame = r.idGame AND r.IsActive = 1
      WHERE g.idGame = ? AND g.IsActive = 1
      GROUP BY g.idGame`,
      [id]
    );

    if (games.length === 0) {
      return res.status(404).json({ error: 'Game not found' });
    }

    // Get platforms for this game
    const [platforms] = await pool.execute(
      `SELECT DISTINCT p.idPlatform, p.Platform_name, p.Type
      FROM platform p
      INNER JOIN review r ON p.idPlatform = r.idPlatform
      WHERE r.idGame = ? AND r.IsActive = 1`,
      [id]
    );

    res.json({ ...games[0], platforms });
  } catch (error) {
    console.error('Get game error:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

// Create game (Admin/Moderator only)
router.post('/', authenticateToken, requireModerator, async (req, res) => {
  try {
    const { Name, Release_year, Description, Production_cost, idCategory, ImageURL } = req.body;

    if (!Name || !idCategory) {
      return res.status(400).json({ error: 'Name and category are required' });
    }

    const [result] = await pool.execute(
      'INSERT INTO game (Name, Release_year, Description, Production_cost, idCategory, ImageURL) VALUES (?, ?, ?, ?, ?, ?)',
      [Name, Release_year || null, Description || null, Production_cost || null, idCategory, ImageURL || null]
    );

    // Log activity
    await pool.execute(
      'INSERT INTO activitylog (idUser, Action, TableName, RecordID, NewValue) VALUES (?, ?, ?, ?, ?)',
      [req.user.idUser, 'CREATE', 'game', result.insertId, JSON.stringify(req.body)]
    );

    res.status(201).json({ message: 'Game created successfully', idGame: result.insertId });
  } catch (error) {
    console.error('Create game error:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

// Update game (Admin/Moderator only)
router.put('/:id', authenticateToken, requireModerator, async (req, res) => {
  try {
    const { id } = req.params;
    const { Name, Release_year, Description, Production_cost, idCategory, ImageURL } = req.body;

    // Get old value
    const [oldGames] = await pool.execute('SELECT * FROM game WHERE idGame = ?', [id]);
    if (oldGames.length === 0) {
      return res.status(404).json({ error: 'Game not found' });
    }

    await pool.execute(
      'UPDATE game SET Name = ?, Release_year = ?, Description = ?, Production_cost = ?, idCategory = ?, ImageURL = ? WHERE idGame = ?',
      [Name, Release_year, Description, Production_cost, idCategory, ImageURL || null, id]
    );

    // Log activity
    await pool.execute(
      'INSERT INTO activitylog (idUser, Action, TableName, RecordID, OldValue, NewValue) VALUES (?, ?, ?, ?, ?, ?)',
      [req.user.idUser, 'UPDATE', 'game', id, JSON.stringify(oldGames[0]), JSON.stringify(req.body)]
    );

    res.json({ message: 'Game updated successfully' });
  } catch (error) {
    console.error('Update game error:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

// Delete game (Admin only)
router.delete('/:id', authenticateToken, requireAdmin, async (req, res) => {
  try {
    const { id } = req.params;

    // Get old value
    const [oldGames] = await pool.execute('SELECT * FROM game WHERE idGame = ?', [id]);
    if (oldGames.length === 0) {
      return res.status(404).json({ error: 'Game not found' });
    }

    // Soft delete
    await pool.execute('UPDATE game SET IsActive = 0 WHERE idGame = ?', [id]);

    // Log activity
    await pool.execute(
      'INSERT INTO activitylog (idUser, Action, TableName, RecordID, OldValue) VALUES (?, ?, ?, ?, ?)',
      [req.user.idUser, 'DELETE', 'game', id, JSON.stringify(oldGames[0])]
    );

    res.json({ message: 'Game deleted successfully' });
  } catch (error) {
    console.error('Delete game error:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

module.exports = router;

