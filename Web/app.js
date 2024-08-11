// Selección de elementos del DOM
const sideMenu = document.querySelector("aside");
const themeToggler = document.querySelector(".theme-toggler");
const nextDay = document.getElementById('nextDay');
const prevDay = document.getElementById('prevDay');

// Función para alternar entre temas oscuro y claro
themeToggler.onclick = function() {
    document.body.classList.toggle('dark-theme'); // Alternar clase dark-theme en body
    themeToggler.querySelector('span:nth-child(1)').classList.toggle('active'); // Alternar estado activo de primer span
    themeToggler.querySelector('span:nth-child(2)').classList.toggle('active'); // Alternar estado activo de segundo span
    
    // Guardar preferencia de tema en el almacenamiento local
    const isDarkTheme = document.body.classList.contains('dark-theme');
    localStorage.setItem('theme', isDarkTheme ? 'dark' : 'light');
}

// Comprobar si hay un tema guardado en el almacenamiento local
const savedTheme = localStorage.getItem('theme');
if (savedTheme === 'dark') {
    // Aplicar tema oscuro al cargar la página
    document.body.classList.add('dark-theme');
    themeToggler.querySelector('span:nth-child(1)').classList.remove('active'); // Desactivar primer span
    themeToggler.querySelector('span:nth-child(2)').classList.add('active'); // Activar segundo span
}

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

            if (!response.ok) {
                throw new Error('Error en la solicitud HTTP: ' + response.statusText);
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

        } catch (error) {
            console.error('Error al obtener los datos:', error);
        }
    }

    obtenerDatos();
});


