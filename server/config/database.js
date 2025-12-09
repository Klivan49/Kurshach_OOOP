const mysql = require('mysql2/promise');
const path = require('path');
require('dotenv').config({ path: path.join(__dirname, '..', '.env') });

// Используем MYSQL_* переменные от Railway, или DB_* для локального тестирования
const config = {
  host: process.env.MYSQLHOST || process.env.DB_HOST,
  port: parseInt(process.env.MYSQLPORT || process.env.DB_PORT) || 3306,
  user: process.env.MYSQLUSER || process.env.DB_USER,
  password: process.env.MYSQLPASSWORD || process.env.DB_PASSWORD,
  database: process.env.MYSQLDATABASE || process.env.DB_NAME
};

console.log('Database config:', {
  host: config.host,
  port: config.port,
  user: config.user,
  database: config.database,
  environment: process.env.NODE_ENV || 'development',
  hasPassword: !!config.password
});

const pool = mysql.createPool({
  ...config,
  charset: 'utf8mb4',
  waitForConnections: true,
  connectionLimit: 10,
  queueLimit: 0,
  enableKeepAlive: true,
  keepAliveInitialDelayMs: 0,
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
    process.exit(1);
  });

module.exports = pool;
