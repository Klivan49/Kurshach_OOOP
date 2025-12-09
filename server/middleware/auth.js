const jwt = require('jsonwebtoken');

const authenticateToken = (req, res, next) => {
  const authHeader = req.headers['authorization'];
  const token = authHeader && authHeader.split(' ')[1];

  if (!token) {
    return res.status(401).json({ error: 'Access token required' });
  }

  jwt.verify(token, process.env.JWT_SECRET || 'your-secret-key', (err, user) => {
    if (err) {
      return res.status(403).json({ error: 'Invalid or expired token' });
    }
    req.user = user;
    next();
  });
};

const requireAdmin = (req, res, next) => {
  if (req.user && req.user.privilege === 'admin') {
    next();
  } else {
    res.status(403).json({ error: 'Admin access required' });
  }
};

const requireModerator = (req, res, next) => {
  if (req.user && (req.user.privilege === 'admin' || req.user.privilege === 'moderator')) {
    next();
  } else {
    res.status(403).json({ error: 'Moderator access required' });
  }
};

module.exports = {
  authenticateToken,
  requireAdmin,
  requireModerator
};

