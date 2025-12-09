const express = require('express');
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');
const pool = require('../config/database');
const { authenticateToken } = require('../middleware/auth');

const router = express.Router();

// Register
router.post('/register', async (req, res) => {
  try {
    const { username, password, email } = req.body;

    if (!username || !password) {
      return res.status(400).json({ error: 'Username and password are required' });
    }

    if (password.length < 6) {
      return res.status(400).json({ error: 'Password must be at least 6 characters' });
    }

    // Check if user exists
    const [existingUsers] = await pool.execute(
      'SELECT idUser FROM user WHERE Username = ?',
      [username]
    );

    if (existingUsers.length > 0) {
      return res.status(400).json({ error: 'Username already exists' });
    }

    // Hash password (SHA1 for compatibility with existing admin panel)
    const crypto = require('crypto');
    const passwordHash = crypto.createHash('sha1').update(password).digest('hex');

    // Insert user
    const [result] = await pool.execute(
      'INSERT INTO user (Username, PasswordHash, Privilege) VALUES (?, ?, ?)',
      [username, passwordHash, 'user']
    );

    const token = jwt.sign(
      { idUser: result.insertId, username, privilege: 'user' },
      process.env.JWT_SECRET || 'your-secret-key',
      { expiresIn: '7d' }
    );

    res.status(201).json({
      message: 'User registered successfully',
      token,
      user: {
        idUser: result.insertId,
        username,
        privilege: 'user'
      }
    });
  } catch (error) {
    console.error('Registration error:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

// Login
router.post('/login', async (req, res) => {
  try {
    const { username, password } = req.body;

    if (!username || !password) {
      return res.status(400).json({ error: 'Username and password are required' });
    }

    // Find user
    const [users] = await pool.execute(
      'SELECT idUser, Username, PasswordHash, Privilege, IsActive FROM user WHERE Username = ?',
      [username]
    );

    if (users.length === 0) {
      return res.status(401).json({ error: 'Invalid credentials' });
    }

    const user = users[0];

    if (!user.IsActive) {
      return res.status(403).json({ error: 'Account is disabled' });
    }

    // Verify password (SHA1 for compatibility)
    const crypto = require('crypto');
    const passwordHash = crypto.createHash('sha1').update(password).digest('hex');

    if (passwordHash !== user.PasswordHash) {
      return res.status(401).json({ error: 'Invalid credentials' });
    }

    // Update last login
    await pool.execute(
      'UPDATE user SET LastLoginDate = NOW() WHERE idUser = ?',
      [user.idUser]
    );

    const token = jwt.sign(
      { idUser: user.idUser, username: user.Username, privilege: user.Privilege },
      process.env.JWT_SECRET || 'your-secret-key',
      { expiresIn: '7d' }
    );

    res.json({
      message: 'Login successful',
      token,
      user: {
        idUser: user.idUser,
        username: user.Username,
        privilege: user.Privilege
      }
    });
  } catch (error) {
    console.error('Login error:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

// Get current user
router.get('/me', authenticateToken, async (req, res) => {
  try {
    const [users] = await pool.execute(
      'SELECT idUser, Username, Privilege, RegistrationDate, LastLoginDate FROM user WHERE idUser = ?',
      [req.user.idUser]
    );

    if (users.length === 0) {
      return res.status(404).json({ error: 'User not found' });
    }

    res.json({ user: users[0] });
  } catch (error) {
    console.error('Get user error:', error);
    res.status(500).json({ error: 'Internal server error' });
  }
});

module.exports = router;

