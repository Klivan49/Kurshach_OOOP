# Quick Start Guide

## 1. Database Setup

Make sure MySQL is running and import the schema:
```bash
mysql -u root -p < AdminPanel/kursach.session.sql
```

Or use MySQL Workbench to import `AdminPanel/kursach.session.sql`

## 2. Backend Setup

```bash
cd server
npm install
```

Create `.env` file:
```env
DB_HOST=127.0.0.1
DB_PORT=3306
DB_USER=root
DB_PASSWORD=49K|ivan1708
DB_NAME=game_rating
JWT_SECRET=game-rating-secret-key-2024
PORT=3001
```

Start backend:
```bash
npm start
```

Backend runs on http://localhost:3001

## 3. Frontend Setup

```bash
cd kursach-react
npm install
npm start
```

Frontend runs on http://localhost:3000

## 4. Default Login

- Username: `admin`
- Password: `1234` (from database schema)

## Troubleshooting

- Make sure MySQL is running
- Check database credentials in `server/.env`
- Ensure backend is running before starting frontend
- Check browser console for errors

