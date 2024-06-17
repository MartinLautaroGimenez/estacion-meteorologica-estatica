// scripts.js
window.onload = function () {
    // Temperature Chart
    var ctxTemp = document.getElementById('temperatureChart').getContext('2d');
    var temperatureChart = new Chart(ctxTemp, {
        type: 'line',
        data: {
            labels: ['18:00', '21:00', '00:00', '03:00', '06:00', '09:00', '12:00', '15:00'],
            datasets: [{
                label: 'Temperature',
                data: [20, 19, 18, 17, 16, 15, 19, 18],
                borderColor: 'rgba(255, 99, 132, 1)',
                borderWidth: 1,
                fill: false
            }]
        },
        options: {
            scales: {
                y: {
                    beginAtZero: false
                }
            }
        }
    });

    // Pressure Chart
    var ctxPressure = document.getElementById('pressureChart').getContext('2d');
    var pressureChart = new Chart(ctxPressure, {
        type: 'line',
        data: {
            labels: ['18:00', '21:00', '00:00', '03:00', '06:00', '09:00', '12:00', '15:00'],
            datasets: [{
                label: 'Pressure',
                data: [930, 932, 934, 936, 938, 940, 938, 940],
                borderColor: 'rgba(54, 162, 235, 1)',
                borderWidth: 1,
                fill: false
            }]
        },
        options: {
            scales: {
                y: {
                    beginAtZero: false
                }
            }
        }
    });
};
