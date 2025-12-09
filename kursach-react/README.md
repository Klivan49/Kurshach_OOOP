# GameCritic - Game Rating Website

A Metacritic-style game rating and review website built with React and Node.js.

## Features

- Browse and search games
- View game details with Metascore ratings
- Write and read user reviews
- Filter games by category, platform, and rating
- User authentication and authorization
- Admin/Moderator capabilities

## Tech Stack

### Frontend
- React 19
- Axios for API calls
- CSS3 with modern styling

### Backend
- Node.js with Express
- MySQL database
- JWT authentication
- bcryptjs for password hashing

## Setup Instructions

### Prerequisites
- Node.js (v14 or higher)
- MySQL database
- npm or yarn

### Database Setup

1. Make sure MySQL is running
2. Import the database schema from `AdminPanel/kursach.session.sql`
3. Update database credentials in `server/.env`

### Backend Setup

1. Navigate to the server directory:
```bash
cd server
```

2. Install dependencies:
```bash
npm install
```

3. Create a `.env` file (copy from `.env.example`):
```env
DB_HOST=127.0.0.1
DB_PORT=3306
DB_USER=root
DB_PASSWORD=your_password
DB_NAME=game_rating
JWT_SECRET=your-secret-key-change-this-in-production
PORT=3001
```

4. Start the server:
```bash
npm start
```

For development with auto-reload:
```bash
npm run dev
```

The server will run on `http://localhost:3001`

### Frontend Setup

1. Navigate to the frontend directory:
```bash
cd kursach-react
```

2. Install dependencies:
```bash
npm install
```

3. Start the development server:
```bash
npm start
```

The app will open in your browser at `http://localhost:3000`

## API Endpoints

### Authentication
- `POST /api/auth/register` - Register a new user
- `POST /api/auth/login` - Login user
- `GET /api/auth/me` - Get current user (requires auth)

### Games
- `GET /api/games` - Get all games (with filters)
- `GET /api/games/:id` - Get game by ID
- `POST /api/games` - Create game (moderator/admin)
- `PUT /api/games/:id` - Update game (moderator/admin)
- `DELETE /api/games/:id` - Delete game (admin)

### Reviews
- `GET /api/reviews/game/:gameId` - Get reviews for a game
- `POST /api/reviews/game/:gameId` - Create review (requires auth)
- `DELETE /api/reviews/:id` - Delete review (own review or moderator)

### Categories & Platforms
- `GET /api/categories` - Get all categories
- `GET /api/platforms` - Get all platforms

## Default Admin Account

Username: `admin`
Password: `1234` (as per database schema)

**Note:** Change the default password in production!

## Project Structure

```
kursach-react/
├── server/                 # Node.js backend
│   ├── config/            # Database configuration
│   ├── middleware/       # Auth middleware
│   ├── routes/           # API routes
│   └── server.js         # Main server file
├── src/
│   ├── pages/            # React components
│   │   ├── Header/       # Header component
│   │   ├── HomePage/     # Main page
│   │   ├── GameCard/     # Game card component
│   │   ├── GameDetail/   # Game detail modal
│   │   ├── ReviewForm/   # Review form
│   │   ├── AuthModal/    # Authentication modal
│   │   ├── Sidebar/      # Filter sidebar
│   │   └── SearchBar/    # Search component
│   ├── api.js           # API client
│   └── App.jsx          # Main app component
└── package.json
```

## Development

The project uses:
- React Hooks for state management
- Axios for HTTP requests
- JWT tokens stored in localStorage
- Responsive design for mobile and desktop

## License

This project is for educational purposes.
