import React, { useEffect, useState } from 'react';
import { getGames, getGameById } from '../../api';
import Sidebar from '../Sidebar/Sidebar';
import SearchBar from '../SearchBar/SearchBar';
import GameCard from '../GameCard/GameCard';
import GameDetail from '../GameDetail/GameDetail';
import './HomePage.css';

function HomePage({ user }) {
  const [games, setGames] = useState([]);
  const [selectedGame, setSelectedGame] = useState(null);
  const [filters, setFilters] = useState({
    search: '',
    sortBy: 'rating',
    category: null,
    platform: null,
    minRating: null,
  });
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    loadGames();
  }, [filters]);

  const loadGames = async () => {
    try {
      setLoading(true);
      const params = {
        search: filters.search || undefined,
        category: filters.category || undefined,
        platform: filters.platform || undefined,
        sortBy: filters.sortBy,
        minRating: filters.minRating || undefined,
      };
      const response = await getGames(params);
      setGames(response.data || []);
    } catch (error) {
      console.error('Error loading games:', error);
      // Set empty array on error
      setGames([]);
      // Show user-friendly error message
      if (error.code === 'ERR_NETWORK') {
        console.error('Cannot connect to backend. Make sure backend is running and accessible.');
      }
    } finally {
      setLoading(false);
    }
  };

  const handleSearch = (query) => {
    setFilters((prev) => ({ ...prev, search: query }));
  };

  const handleFiltersChange = (newFilters) => {
    setFilters(newFilters);
  };

  const handleGameClick = async (game) => {
    // Load full game details
    try {
      const response = await getGameById(game.idGame);
      setSelectedGame(response.data);
    } catch (error) {
      console.error('Error loading game details:', error);
      setSelectedGame(game);
    }
  };

  const handleCloseDetail = () => {
    setSelectedGame(null);
  };

  const handleReviewAdded = () => {
    loadGames();
    if (selectedGame) {
      handleGameClick(selectedGame);
    }
  };

  return (
    <main className="home-page">
      <div className="home-hero">
        <div className="hero-content">
          <h1>Game Reviews & Ratings</h1>
          <p>Discover, rate, and review the best games</p>
        </div>
      </div>

      <div className="layout">
        <Sidebar filters={filters} onFiltersChange={handleFiltersChange} />

        <section className="content">
          <SearchBar onSearch={handleSearch} />

          {loading ? (
            <div className="loading">Loading games...</div>
          ) : games.length === 0 ? (
            <div className="no-games">No games found</div>
          ) : (
            <>
              <div className="games-header">
                <h2>All Games</h2>
                <span className="games-count">{games.length} games</span>
              </div>
              <div className="games-grid">
                {games.map((game) => (
                  <GameCard
                    key={game.idGame}
                    game={game}
                    onClick={() => handleGameClick(game)}
                  />
                ))}
              </div>
            </>
          )}
        </section>
      </div>

      {selectedGame && (
        <GameDetail
          game={selectedGame}
          user={user}
          onClose={handleCloseDetail}
          onReviewAdded={handleReviewAdded}
        />
      )}
    </main>
  );
}

export default HomePage;
