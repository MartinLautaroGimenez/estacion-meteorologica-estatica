const apiUrl = 'https://emetec.wetec.um.edu.ar/graph';

let myChart1, myChart2, myChart3;

// Función asincrónica para obtener y procesar datos
async function fetchData() {
    try {
        const response = await fetch(apiUrl, {
            headers: {
                'Access-Control-Allow-Origin': '*'  // Permitir CORS, si es necesario
            }
        });

        if (!response.ok) {
            throw new Error('Network response was not ok ' + response.statusText);
        } // algo

          // Destruir los gráficos antes de recrearlos
          if (myChart1) {
            myChart1.destroy();
          }
          if (myChart2) {
            myChart2.destroy();
          }
          if (myChart3) {
            myChart3.destroy();
          }
        
        const data = await response.json();
        const ultimos10Datos = data.slice(-10);
        
        const numMuestras = document.getElementById('numMuestras').value;
        const ultimosNdatos = data.slice(-numMuestras); // Filtrar por muestras seleccionadas
        console.log(data); // Registro para verificar que los datos se obtienen correctamente
        displayData(data); // Función para mostrar los datos (puedes definirla más adelante)
        createTemperatureHumidityChart(ultimosNdatos); // Función para crear gráfico de temperatura y humedad
    } catch (error) {
        console.error('There has been a problem with your fetch operation:', error);
    }
}

// Función para formatear la hora a partir de una cadena de fecha
function formatTime(dateString) {
    const timePart = dateString.split(' ')[1]; // Obtener la parte de tiempo (HH:MM)
    return timePart;
}

// Función para crear gráfico de temperatura y humedad
function createTemperatureHumidityChart(data) {
    // Configuración del primer gráfico (Temperatura y Humedad Relativa)
    const labels = data.map(d => formatTime(d.Fecha));
    const temperatures = data.map(d => parseFloat(d["Temperatura Promedio"]));
    const humidities = data.map(d => parseFloat(d["Humedad"]));

    const ctx = document.getElementById('myChart1').getContext('2d');
    myChart1 = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
            datasets: [
                {
                    label: 'Temperatura',
                    data: temperatures,
                    backgroundColor: 'rgba(103, 58, 183, 0.2)',
                    borderColor: 'rgb(103, 58, 183)',
                    fill: false,
                    borderWidth: 2,
                },
                {
                    label: 'Humedad Relativa',
                    data: humidities,
                    backgroundColor: 'rgba(74, 170, 246, 0.2)',
                    borderColor: 'rgb(74, 170, 246)',
                    fill: false,
                    borderWidth: 2,
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            devicePixelRatio: window.devicePixelRatio,
            animation: {
                show: {
                    animations: {
                        x: { from: 0 },
                        y: { from: 0 }
                    }
                },
                hide: {
                    animations: {
                        x: { to: 0 },
                        y: { to: 0 }
                    }
                }
            },
            scales: {
                y: {
                    beginAtZero: true
                }
            }
        }
    });

    // Configuración del segundo gráfico (Presión Atmosférica)
    const bpa = data.map(d => parseFloat(d["Presion"]));
    const a = data.map(d => parseFloat(d["Altitud"]));
    const bp0 = data.map(d => parseFloat(d["Presion Nivel Mar"]));
    const ctx2 = document.getElementById('myChart2').getContext('2d');
    myChart2 = new Chart(ctx2, {
        type: 'line',
        data: {
            labels: labels,
            datasets: [
                {
                    label: 'Presión Atmosférica',
                    data: bpa,
                    backgroundColor: 'rgba(229, 212, 249, 0.2)',
                    borderColor: 'rgb(2, 119, 189)',
                    fill: true,
                    borderWidth: 2.5,
                },
                {
                    label: 'Presión A nivel de Mar',
                    data: bp0,
                    backgroundColor: 'rgba(29, 12, 249, 0.3)',
                    borderColor: 'rgb(2, 119, 189)',
                    fill: true,
                    borderWidth: 2.5,
                },
                {
                    label: 'Altitud',
                    data: a,
                    backgroundColor: 'rgba(109, 162, 9, 0.2)',
                    borderColor: 'rgb(2, 119, 189)',
                    fill: true,
                    borderWidth: 2.5,
                },
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            devicePixelRatio: window.devicePixelRatio,
            animations: {
                tension: {
                    duration: 1000,
                    easing: 'linear',
                    from: 1,
                    to: 0,
                    loop: false
                }
            },
            scales: {
                y: {
                    beginAtZero: true
                }
            }
        }
    });

    // Configuración del tercer gráfico (Velocidad del viento)
    const ppmco2 = data.map(d => parseFloat(d["ppm CO2"]));
    const velviento = data.map(d => parseFloat(d["Velocidad del viento"]));
    const ctx3 = document.getElementById('myChart3').getContext('2d');
    myChart3 = new Chart(ctx3, {
        type: 'bar',
        data: {
            labels: labels,
            datasets: [
                {
                    label: 'Velocidad del viento',
                    data: velviento,
                    backgroundColor: 'rgba(255, 159, 64, 0.2)',
                    borderColor: 'rgb(255, 159, 64)',
                    fill: true,
                    borderWidth: 2.5,
                },
                {
                    label: 'Partes Por Millón de CO2',
                    data: ppmco2,
                    backgroundColor: 'rgba(55, 159, 64, 0.2)',
                    borderColor: 'rgb(255, 159, 64)',
                    fill: true,
                    borderWidth: 2.5,
                },
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            devicePixelRatio: window.devicePixelRatio,
            scales: {
                y: {
                    beginAtZero: true
                }
            },
            animations: {
                tension: {
                    duration: 1000,
                    easing: 'easeInOutQuart',
                    from: 0,
                    to: 1,
                    loop: false
                }
            }
        }
    });
}

// Función para mostrar datos (puedes expandir esta función según necesites)
function displayData(data) {
    // Puedes agregar funcionalidad adicional de visualización aquí
}

// Llamar a la función para obtener datos cuando el DOM esté completamente cargado
document.addEventListener('DOMContentLoaded', fetchData);

