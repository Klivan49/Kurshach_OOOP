import React, { useEffect, useState } from 'react';
import { getCategories } from '../../api';
import './Sidebar.css';

function Sidebar({ filters, onFiltersChange }) {
  const [categories, setCategories] = useState([]);

  useEffect(() => {
    loadFilters();
  }, []);

  const loadFilters = async () => {
    try {
      const [catsRes, platRes] = await Promise.all([
        getCategories()
      ]);
      setCategories(catsRes.data);
    } catch (error) {
      console.error('Error loading filters:', error);
    }
  };

  const handleCategoryChange = (categoryId) => {
    const newCategory = filters.category === categoryId ? null : categoryId;
    onFiltersChange({ ...filters, category: newCategory });
  };

  const handleSortChange = (sortBy) => {
    onFiltersChange({ ...filters, sortBy });
  };

  const handleMinRatingChange = (minRating) => {
    onFiltersChange({ ...filters, minRating: minRating || null });
  };

  return (
    <aside className="sidebar">
      <div className="sidebar-section">
        <h3>Sort By</h3>
        <div className="filter-options">
          <label className="radio-option">
            <input
              type="radio"
              name="sort"
              checked={filters.sortBy === 'rating'}
              onChange={() => handleSortChange('rating')}
            />
            <span>Metascore</span>
          </label>
          <label className="radio-option">
            <input
              type="radio"
              name="sort"
              checked={filters.sortBy === 'newest'}
              onChange={() => handleSortChange('newest')}
            />
            <span>Release Date</span>
          </label>
          <label className="radio-option">
            <input
              type="radio"
              name="sort"
              checked={filters.sortBy === 'name'}
              onChange={() => handleSortChange('name')}
            />
            <span>Name</span>
          </label>
          <label className="radio-option">
            <input
              type="radio"
              name="sort"
              checked={filters.sortBy === 'reviews'}
              onChange={() => handleSortChange('reviews')}
            />
            <span>Most Reviews</span>
          </label>
        </div>
      </div>

      <div className="sidebar-section">
        <h3>Category</h3>
        <div className="filter-options">
          <label className="checkbox-option">
            <input
              type="checkbox"
              checked={filters.category === null}
              onChange={() => handleCategoryChange(null)}
            />
            <span>All</span>
          </label>
          {categories.map((cat) => (
            <label key={cat.idCategory} className="checkbox-option">
              <input
                type="checkbox"
                checked={filters.category === cat.idCategory}
                onChange={() => handleCategoryChange(cat.idCategory)}
              />
              <span>{cat.CategoryName}</span>
            </label>
          ))}
        </div>
      </div>
      <div className="sidebar-section">
        <h3>Minimum Score</h3>
        <div className="filter-options">
          <input
            type="range"
            min="0"
            max="10"
            step="0.5"
            value={filters.minRating || 0}
            onChange={(e) => handleMinRatingChange(parseFloat(e.target.value))}
            className="rating-slider"
          />
          <div className="rating-display">
            {filters.minRating ? `${filters.minRating.toFixed(1)}+` : 'Any'}
          </div>
        </div>
      </div>
    </aside>
  );
}

export default Sidebar;

