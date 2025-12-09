import axios from 'axios';

// Use environment variable or fallback to localhost for development
// In production, REACT_APP_API_URL should be set to your deployed backend URL
const API_BASE_URL = process.env.REACT_APP_API_URL || 
  (process.env.NODE_ENV === 'production' 
    ? 'https://your-backend-url.com/api'  // Replace with your actual backend URL
    : 'http://localhost:3001/api');

console.log('API Base URL:', API_BASE_URL);
console.log('Environment:', process.env.NODE_ENV);

const api = axios.create({
  baseURL: API_BASE_URL,
  headers: {
    'Content-Type': 'application/json',
  },
  timeout: 10000, // 10 seconds timeout
});

// Add request interceptor for debugging
api.interceptors.request.use(
  (config) => {
    console.log(`Making ${config.method.toUpperCase()} request to:`, config.url);
    return config;
  },
  (error) => {
    console.error('Request error:', error);
    return Promise.reject(error);
  }
);

// Add response interceptor for error handling
api.interceptors.response.use(
  (response) => response,
  (error) => {
    if (error.code === 'ERR_NETWORK') {
      console.error('Network error - check if backend is running and CORS is configured');
      console.error('Backend URL:', API_BASE_URL);
    }
    return Promise.reject(error);
  }
);

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
