const express = require('express');
const pool = require('../config/database');

const router = express.Router();

// Get all platforms
router.get('/', async (req, res) => {
  try {
    const [platforms] = await pool.execute(
      'SELECT * FROM platform ORDER BY Platform_name'
    );
    res.json(platforms);
  } catch (error) {
    console.error('Get platforms error:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

module.exports = router;

