import React, { useEffect, useState } from 'react';
import { Line } from 'react-chartjs-2';
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  Filler,
} from 'chart.js';
import axios from 'axios';
import './Dashboard.css';

ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  Filler
);

const Dashboard = () => {
  const [chartData, setChartData] = useState({
    labels: [],
    datasets: [],
  });
  const [currentHumidity, setCurrentHumidity] = useState(null);
  const [status, setStatus] = useState('Loading...');
  const [statusGradient, setStatusGradient] = useState('var(--info-gradient)');
  const [isLoading, setIsLoading] = useState(true);

  const getStatus = (value) => {
    if (value < 30) return { 
      text: '🔥 Too Dry', 
      gradient: 'var(--danger-gradient)',
      description: 'Soil needs water immediately'
    };
    if (value > 70) return { 
      text: '💧 Too Wet', 
      gradient: 'var(--info-gradient)',
      description: 'Reduce watering frequency'
    };
    return { 
      text: '✨ Perfect', 
      gradient: 'var(--success-gradient)',
      description: 'Optimal moisture level'
    };
  };

  const fetchData = async () => {
    try {
      const response = await axios.get('http://localhost:5000/api/data');
      const data = response.data.reverse();

      const labels = data.map((d) => 
        new Date(d.timestamp).toLocaleTimeString([], { 
          hour: '2-digit', 
          minute: '2-digit' 
        })
      );
      const values = data.map((d) => d.value);

      setChartData({
        labels,
        datasets: [
          {
            label: 'Humidity',
            data: values,
            borderColor: '#667eea',
            backgroundColor: (context) => {
              const ctx = context.chart.ctx;
              const gradient = ctx.createLinearGradient(0, 0, 0, 300);
              gradient.addColorStop(0, 'rgba(102, 126, 234, 0.4)');
              gradient.addColorStop(0.5, 'rgba(102, 126, 234, 0.2)');
              gradient.addColorStop(1, 'rgba(102, 126, 234, 0)');
              return gradient;
            },
            tension: 0.4,
            fill: true,
            pointRadius: 0,
            pointHoverRadius: 6,
            pointHoverBackgroundColor: '#667eea',
            pointHoverBorderColor: '#fff',
            pointHoverBorderWidth: 2,
            borderWidth: 3,
          },
        ],
      });

      if (data.length > 0) {
        const latest = data[data.length - 1].value;
        setCurrentHumidity(latest);
        const statusInfo = getStatus(latest);
        setStatus(statusInfo.text);
        setStatusGradient(statusInfo.gradient);
        setIsLoading(false);
      }
    } catch (error) {
      console.error('Error fetching data:', error);
    }
  };

  useEffect(() => {
    fetchData();
    const interval = setInterval(fetchData, 2000);
    return () => clearInterval(interval);
  }, []);

  const chartOptions = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
      legend: {
        display: false,
      },
      tooltip: {
        enabled: true,
        mode: 'index',
        intersect: false,
        backgroundColor: 'rgba(15, 15, 35, 0.95)',
        titleColor: '#ffffff',
        bodyColor: '#a0a0b8',
        borderColor: 'rgba(102, 126, 234, 0.5)',
        borderWidth: 1,
        padding: 12,
        displayColors: false,
        titleFont: {
          size: 14,
          weight: '600',
        },
        bodyFont: {
          size: 13,
        },
        callbacks: {
          label: (context) => `${context.parsed.y.toFixed(1)}% humidity`,
        },
      },
    },
    scales: {
      x: {
        grid: {
          display: false,
          drawBorder: false,
        },
        ticks: {
          maxTicksLimit: 8,
          color: '#6b6b8a',
          font: {
            size: 11,
          },
        },
      },
      y: {
        beginAtZero: true,
        max: 100,
        grid: {
          color: 'rgba(255, 255, 255, 0.05)',
          drawBorder: false,
        },
        ticks: {
          color: '#6b6b8a',
          font: {
            size: 11,
          },
          callback: (value) => `${value}%`,
        },
      },
    },
    interaction: {
      mode: 'nearest',
      axis: 'x',
      intersect: false,
    },
  };

  return (
    <div className="container">
      <header className="dashboard-header">
        <div className="logo-container">
          <div className="logo-icon">🌱</div>
          <h1 className="logo-text">
            Smart<span className="logo-accent">Garden</span>
          </h1>
        </div>
        <p className="subtitle">Real-time Soil Monitoring System</p>
      </header>

      <div className="dashboard-grid">
        {/* Main Status Card */}
        <div className="glass-card status-card">
          <div className="card-header">
            <h2 className="card-title">Current Status</h2>
            <div className="pulse-indicator"></div>
          </div>
          
          <div className="humidity-display">
            {!isLoading && currentHumidity !== null ? (
              <>
                <div className="humidity-value-container">
                  <span className="humidity-value">{currentHumidity.toFixed(0)}</span>
                  <span className="humidity-unit">%</span>
                </div>
                <div className="humidity-label">Soil Moisture</div>
              </>
            ) : (
              <div className="loading-spinner"></div>
            )}
          </div>

          <div 
            className="status-badge"
            style={{ background: statusGradient }}
          >
            {status}
          </div>

          <div className="status-description">
            {currentHumidity !== null && getStatus(currentHumidity).description}
          </div>
        </div>

        {/* Chart Card */}
        <div className="glass-card chart-card">
          <div className="card-header">
            <h2 className="card-title">Historical Data</h2>
            <div className="chart-legend">
              <span className="legend-dot"></span>
              <span className="legend-text">Last 100 readings</span>
            </div>
          </div>
          
          <div className="chart-container">
            <Line options={chartOptions} data={chartData} />
          </div>
        </div>

        {/* Stats Cards */}
        <div className="glass-card stats-card">
          <div className="stat-icon">📊</div>
          <div className="stat-content">
            <div className="stat-label">Average</div>
            <div className="stat-value">
              {chartData.datasets[0]?.data.length > 0
                ? (chartData.datasets[0].data.reduce((a, b) => a + b, 0) / 
                   chartData.datasets[0].data.length).toFixed(1)
                : '--'}%
            </div>
          </div>
        </div>

        <div className="glass-card stats-card">
          <div className="stat-icon">📈</div>
          <div className="stat-content">
            <div className="stat-label">Peak</div>
            <div className="stat-value">
              {chartData.datasets[0]?.data.length > 0
                ? Math.max(...chartData.datasets[0].data).toFixed(1)
                : '--'}%
            </div>
          </div>
        </div>

        <div className="glass-card stats-card">
          <div className="stat-icon">📉</div>
          <div className="stat-content">
            <div className="stat-label">Lowest</div>
            <div className="stat-value">
              {chartData.datasets[0]?.data.length > 0
                ? Math.min(...chartData.datasets[0].data).toFixed(1)
                : '--'}%
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default Dashboard;
