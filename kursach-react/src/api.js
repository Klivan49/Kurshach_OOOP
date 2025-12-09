import axios from 'axios';

// Use environment variable or fallback to localhost for development
const API_BASE_URL = process.env.REACT_APP_API_URL || 'http://localhost:3001/api';

const api = axios.create({
  baseURL: API_BASE_URL,
  headers: {
    'Content-Type': 'application/json',
  },
});

export const setAuthToken = (token) => {
  if (token) {
    api.defaults.headers.common['Authorization'] = `Bearer ${token}`;
  } else {
    delete api.defaults.headers.common['Authorization'];
  }
};

// Auth
export const register = (userData) => api.post('/auth/register', userData);
export const login = (credentials) => api.post('/auth/login', credentials);
export const getCurrentUser = () => api.get('/auth/me');

// Games
export const getGames = (params) => api.get('/games', { params });
export const getGameById = (id) => api.get(`/games/${id}`);
export const createGame = (gameData) => api.post('/games', gameData);
export const updateGame = (id, gameData) => api.put(`/games/${id}`, gameData);
export const deleteGame = (id) => api.delete(`/games/${id}`);

// Categories
export const getCategories = () => api.get('/categories');

// Platforms
export const getPlatforms = () => api.get('/platforms');

// Reviews
export const getReviews = (gameId) => api.get(`/reviews/game/${gameId}`);
export const addReview = (gameId, reviewData) =>
  api.post(`/reviews/game/${gameId}`, reviewData);
export const deleteReview = (reviewId) => api.delete(`/reviews/${reviewId}`);

export default api;
