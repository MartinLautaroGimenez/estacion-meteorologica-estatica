// Esperar a que el DOM esté completamente cargado
document.addEventListener('DOMContentLoaded', function() {
    const apiUrl = 'https://emetec.wetec.um.edu.ar/datos';

    async function obtenerDatos() {
        try {
            const response = await fetch(apiUrl, {
                headers: {
                    'Access-Control-Allow-Origin': '*'
                }
            });

            }

            const data = await response.json();
            const ultimos10Datos = data.slice(-10);

            const ultimoDato = ultimos10Datos[ultimos10Datos.length - 1];
            document.getElementById('datoNumero').textContent = ultimoDato["Dato N°"];
            document.getElementById('fecha').textContent = ultimoDato["Fecha"];
            document.getElementById('battery').textContent = ultimoDato["Nivel de bateria"] + ' V';
            document.getElementById('temperatura').textContent = ultimoDato["Temperatura Promedio"] + ' °C';
            document.getElementById('stermica').textContent = ultimoDato["Sensacion T"] + ' °C';
            document.getElementById('presion').textContent = ultimoDato["Presion"] + ' hPa';
            document.getElementById('humedad').textContent = ultimoDato["Humedad"] + ' %';
            document.getElementById('nivelLuz').textContent = ultimoDato["Nivel de luz"] + ' lm';
            document.getElementById('procio').textContent = ultimoDato["Temperatura punto de rocio"] + ' °C';
            document.getElementById('ppmco2').textContent = ultimoDato["ppm CO2"] + ' ppm';
            document.getElementById('calidadAire').textContent = ultimoDato["Calidad del aire"];
            document.getElementById('velocidadViento').textContent = ultimoDato["Velocidad del viento"];
            document.getElementById('direccionViento').textContent = ultimoDato["Direccion del viento"];
            document.getElementById('sensorLluvia').textContent = ultimoDato["Sensor de lluvia"];
            document.getElementById('fecha2').textContent = ultimoDato["Fecha"];
            document.getElementById('fecha3').textContent = ultimoDato["Fecha"];
            document.getElementById('fecha4').textContent = ultimoDato["Fecha"];
            document.getElementById('fecha5').textContent = ultimoDato["Fecha"];

            const tablaDetallesCuerpo = document.getElementById('tablaDetallesCuerpo');
            tablaDetallesCuerpo.innerHTML = '';

            ultimos10Datos.forEach(dato => {
                const fila = document.createElement('tr');
                fila.innerHTML = `
                    <td>${dato["Dato N°"]}</td>
                    <td>${dato["Fecha"]}</td>
                    <td>${dato["Nivel de bateria"]} V</td>
                    <td>${dato["Temperatura Promedio"]} °C</td>
                    <td>${dato["Presion"]} hPa</td>
                    <td>${dato["Humedad"]} %</td>
                    <td>${dato["Nivel de luz"]} lm</td>
                    <td>${dato["Calidad del aire"]}</td>
                    <td>${dato["ppm CO2"]} ppm</td>
                `;
                tablaDetallesCuerpo.appendChild(fila);
            });
        }
    }

    obtenerDatos();
});
