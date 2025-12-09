const mysql = require('mysql2/promise');
const path = require('path');
require('dotenv').config({ path: path.join(__dirname, '..', '.env') });

// Debug: log environment variables (without password)
console.log('Database config:', {
  host: process.env.MYSQLHOST || process.env.DB_HOST || '127.0.0.1',
  port: process.env.MYSQLPORT || process.env.DB_PORT || 3306,
  user: process.env.MYSQLUSER || process.env.DB_USER || 'root',
  database: process.env.MYSQLDATABASE || process.env.DB_NAME || 'game_rating',
  environment: process.env.NODE_ENV || 'development',
  hasPassword: !!(process.env.MYSQLPASSWORD || process.env.DB_PASSWORD)
});

const pool = mysql.createPool({
  host: process.env.MYSQLHOST || process.env.DB_HOST || '127.0.0.1',
  port: parseInt(process.env.MYSQLPORT || process.env.DB_PORT) || 3306,
  user: process.env.MYSQLUSER || process.env.DB_USER || 'root',
  password: process.env.MYSQLPASSWORD || process.env.DB_PASSWORD || '',
  database: process.env.MYSQLDATABASE || process.env.DB_NAME || 'game_rating',
  charset: 'utf8mb4',
  waitForConnections: true,
  connectionLimit: 10,
  queueLimit: 0,
  enableKeepAlive: true,
  keepAliveInitialDelayMs: 0,
  // Важно для Railway
  ssl: process.env.NODE_ENV === 'production' ? 'require' : undefined
});

// Test connection
pool.getConnection()
  .then(connection => {
    console.log('✅ Database connected successfully');
    connection.release();
  })
  .catch(err => {
    console.error('❌ Database connection error:', err.message);
    console.error('Error code:', err.code);
    if (err.code === 'ER_ACCESS_DENIED_ERROR') {
      console.error('⚠️  Check your MYSQLPASSWORD (Railway) or DB_PASSWORD (.env) in environment');
    }
    if (err.code === 'PROTOCOL_CONNECTION_LOST') {
      console.error('⚠️  Connection timeout. Check if MYSQLHOST is correct');
    }
  });

module.exports = pool;
