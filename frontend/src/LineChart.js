import React, { useEffect, useRef } from 'react';
import Chart from 'chart.js/auto';

const LineChart = ({ data, name }) => {
  const chartRef = useRef(null);

  useEffect(() => {
    if (!chartRef.current) return;

    // Destroy the existing chart instance
    if (chartRef.current.chart) {
      chartRef.current.chart.destroy();
    }

    const values = data.map(([value]) => parseFloat(value));
    const timestamps = data.map(([, timestamp]) => parseInt(timestamp));
    
    // Convert timestamps to days
    const days = timestamps.map(timestamp => {
      const date = new Date(timestamp);
      return `${date.getDate()}/${date.getMonth() + 1}/${date.getFullYear()}`;
    });

    const ctx = chartRef.current.getContext('2d');

    const newChart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: days,
        datasets: [{
          label: name,
          data: values,
          borderColor: 'rgba(75, 192, 192, 1)',
          borderWidth: 2,
          fill: false,
        }],
      },
      options: {
        scales: {
          x: {
            type: 'category',
            labels: days, // Use the converted days as labels
            position: 'bottom',
          },
          y: {
            min: 0,
          },
        },
      },
    });

    // Save the new chart instance in the ref
    chartRef.current.chart = newChart;
  }, [data]);

  return <canvas ref={chartRef} width="400" height="200" />;
};

export default LineChart;
