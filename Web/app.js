document.addEventListener('DOMContentLoaded', function() {
    const apiUrl = 'https://emetec.wetec.um.edu.ar/datos';
    const apiTempUrl = 'https://emetec.wetec.um.edu.ar/datostruchos';

    async function obtenerDatos(url) {
        try {
            const response = await fetch(url, {
                headers: {
                    'Access-Control-Allow-Origin': '*'
                }
            });

            if (!response.ok) {
                throw new Error('Error en la solicitud HTTP: ' + response.statusText);
            }

            const data = await response.json();
            const ultimos10Datos = data.slice(-10);

            // Obtener el último dato
            const ultimoDato = ultimos10Datos[ultimos10Datos.length - 1];
            // Obtener el primer dato
            const primerDato = data[0]; // Acceder al primer elemento

            // Mostrar el último dato en el DOM
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

            // Mostrar el primer dato en el DOM (ejemplo)
            document.getElementById('primerDatoNumero').textContent = primerDato["Dato N°"]; // Asegúrate de tener un elemento con este ID
            document.getElementById('primerFecha').textContent = primerDato["Fecha"]; // Asegúrate de tener un elemento con este ID

            // Repetir para otros campos según sea necesario...

            // Actualizar la tabla de detalles
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

    // Event listener para el dropdown
    categoriaDropdown.addEventListener('change', function() {
        const selectedValue = categoriaDropdown.value;
        if (selectedValue === 'emetec') {
            obtenerDatos(apiUrl); // Usar la URL de temperatura
        } else {
            obtenerDatos(apiTempUrl); // Usar la URL por defecto
        }
    });

    // Llamar a obtenerDatos al cargar la página para mostrar datos iniciales
    obtenerDatos(apiUrl);
});
