import React, { useEffect, useState } from 'react';
import { setAuthToken, getCurrentUser } from './api';
import Header from './pages/Header/Header';
import HomePage from './pages/HomePage/HomePage';
import './App.css';

function App() {
  const [user, setUser] = useState(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    const token = localStorage.getItem('token');
    if (token) {
      setAuthToken(token);
      loadUser();
    } else {
      setLoading(false);
    }
  }, []);

  const loadUser = async () => {
    try {
      const response = await getCurrentUser();
      setUser(response.data.user);
    } catch (error) {
      localStorage.removeItem('token');
      setAuthToken(null);
    } finally {
      setLoading(false);
    }
  };

  const handleLogin = (userData, token) => {
    localStorage.setItem('token', token);
    setAuthToken(token);
    setUser(userData);
  };

  const handleLogout = () => {
    localStorage.removeItem('token');
    setAuthToken(null);
    setUser(null);
  };

  if (loading) {
    return <div className="loading">Loading...</div>;
  }

  return (
    <div className="app">
      <Header user={user} onLogout={handleLogout} onLogin={handleLogin} />
      <HomePage user={user} />
    </div>
  );
}

export default App;
