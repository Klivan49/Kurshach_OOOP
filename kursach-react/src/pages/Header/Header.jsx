import React, { useState } from 'react';
import AuthModal from '../AuthModal/AuthModal';
import './Header.css';

function Header({ user, onLogout, onLogin }) {
  const [showAuthModal, setShowAuthModal] = useState(false);

  const handleLogout = () => {
    onLogout();
  };

  const handleAuthSuccess = (userData, token) => {
    onLogin(userData, token);
    setShowAuthModal(false);
  };

  return (
    <header className="header">
      <div className="header-container">
        <div className="logo">
          <h1>KlivanCritic</h1>
          <span className="logo-subtitle">Game Reviews & Ratings</span>
        </div>

        <div className="auth-section">
          {user ? (
            <div className="user-info">
              <span className="username">{user.Username}</span>
              {user.Privilege === 'admin' && (
                <span className="badge admin-badge">Admin</span>
              )}
              <button onClick={handleLogout} className="logout-btn">
                Sign Out
              </button>
            </div>
          ) : (
            <button
              onClick={() => setShowAuthModal(true)}
              className="login-btn"
            >
              Sign In
            </button>
          )}
        </div>
      </div>

      {showAuthModal && (
        <AuthModal
          onClose={() => setShowAuthModal(false)}
          onSuccess={handleAuthSuccess}
        />
      )}
    </header>
  );
}

export default Header;
