// APIs
const apiUrl = 'https://emetec.wetec.um.edu.ar/datos';
const apiTempUrl = 'https://emetec.wetec.um.edu.ar/datosemm';
const apiSJ = 'https://emetec.wetec.um.edu.ar/datosemesj';
let ultimos10DatosGlobal = [];
let ultimoDatoGlobal = {};

// Event listener para el dropdown
categoriaDropdown.addEventListener('change', function () {
    const selectedValue = categoriaDropdown.value;
    setCookie('categoriaDropdown', selectedValue, 30); // Guardar selección en cookie
    if (selectedValue === 'emetec') {
        obtenerDatos(apiUrl);
    } else if (selectedValue === 'test') {
        obtenerDatos(apiTempUrl);
    } else if (selectedValue === 'sj') {
        obtenerDatos(apiSJ);
    }
});

// Función para obtener datos
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

        ultimos10DatosGlobal = ultimos10Datos;
        console.log('Ultimos 10 datos:', ultimos10DatosGlobal);

        const ultimoDato = ultimos10Datos[0];
        ultimoDatoGlobal = ultimos10Datos[0];

        console.log('Ultimo dato:', ultimoDatoGlobal);

        // Función para manejar valores nulos o inválidos
        const obtenerValor = (valor) => {
            return (
                valor === null ||
                valor === "null" ||
                valor === "Proximamente" ||
                valor === 0 ||
                valor === "0" ||
                valor === -1 ||
                valor === "-1" ||
                valor === undefined ||
                valor <= 0 ||
                valor === "NaN"
            ) ? "No disponible" : valor;
        };

        document.getElementById('datoNumero').textContent = obtenerValor(ultimoDato["Dato N°"]);
        document.getElementById('fecha').textContent = obtenerValor(ultimoDato["Fecha"]);
        document.getElementById('battery').textContent =
            obtenerValor(ultimoDato["Nivel de bateria"]) === "No disponible"
                ? "No disponible"
                : obtenerValor(ultimoDato["Nivel de bateria"]) + " V";

        document.getElementById('temperatura').textContent =
            obtenerValor(ultimoDato["Temperatura Promedio"]) === "No disponible"
                ? "No disponible"
                : obtenerValor(ultimoDato["Temperatura Promedio"]) + " °C";

        document.getElementById('stermica').textContent =
            obtenerValor(ultimoDato["Sensacion T"]) === "No disponible"
                ? "No disponible"
                : obtenerValor(ultimoDato["Sensacion T"]) + " °C";

        document.getElementById('presion').textContent =
            obtenerValor(ultimoDato["Presion"]) === "No disponible"
                ? "No disponible"
                : obtenerValor(ultimoDato["Presion"]) + " hPa";

        document.getElementById('humedad').textContent =
            obtenerValor(ultimoDato["Humedad"]) === "No disponible"
                ? "No disponible"
                : obtenerValor(ultimoDato["Humedad"]) + " %";

        document.getElementById('nivelLuz').textContent =
            obtenerValor(ultimoDato["Nivel de luz"]) === "No disponible"
                ? "No disponible"
                : obtenerValor(ultimoDato["Nivel de luz"]) + " lm";

        document.getElementById('procio').textContent =
            obtenerValor(ultimoDato["Temperatura punto de rocio"]) === "No disponible"
                ? "No disponible"
                : obtenerValor(ultimoDato["Temperatura punto de rocio"]) + " °C";

        document.getElementById('ppmco2').textContent =
            obtenerValor(ultimoDato["ppm CO2"]) === "No disponible"
                ? "No disponible"
                : obtenerValor(ultimoDato["ppm CO2"]) + " ppm";

        document.getElementById('calidadAire').textContent = obtenerValor(ultimoDato["Calidad del aire"]);
        document.getElementById('velocidadViento').textContent = obtenerValor(ultimoDato["Velocidad del viento"]);
        document.getElementById('direccionViento').textContent = obtenerValor(ultimoDato["Direccion del viento"]);
        document.getElementById('sensorLluvia').textContent = obtenerValor(ultimoDato["Sensor de lluvia"]);
        document.getElementById('Altitud').textContent = obtenerValor(ultimoDato["Altitud"]);
        document.getElementById('fecha2').textContent = obtenerValor(ultimoDato["Fecha"]);
        document.getElementById('fecha3').textContent = obtenerValor(ultimoDato["Fecha"]);
        document.getElementById('fecha4').textContent = obtenerValor(ultimoDato["Fecha"]);
        document.getElementById('fecha5').textContent = obtenerValor(ultimoDato["Fecha"]);

        const tablaDetallesCuerpo = document.getElementById('tablaDetallesCuerpo');
        tablaDetallesCuerpo.innerHTML = '';

        ultimos10Datos.forEach(dato => {
            const fila = document.createElement('tr');
            fila.innerHTML = `
                <td>${obtenerValor(dato["Dato N°"])}</td>
                <td>${obtenerValor(dato["Fecha"])}</td>
                <td>${obtenerValor(dato["Temperatura Promedio"])} °C</td>
                <td>${obtenerValor(dato["Sensacion T"])} °C</td>
                <td>${obtenerValor(dato["Presion"])} hPa</td>
                <td>${obtenerValor(dato["Humedad"])} %</td>
                <td>${obtenerValor(dato["Nivel de luz"])} lm</td>
            `;
            tablaDetallesCuerpo.appendChild(fila);
        });

        actualizarGrafico();
    } catch (error) {
        console.error('Error al obtener los datos:', error);
    }
}

// Función para actualizar el gráfico según el popup seleccionado
function actualizarGrafico() {
    const selectedValue = categoriaDropdown.value;
    let data = [];
    let color = '';
    let label = '';

    if (selectedValue === 'emetec') {
        data = ultimos10DatosGlobal.map(dato => dato["Temperatura Promedio"]);
        color = '79, 126, 255';
        label = 'Temperatura promedio';
    } else if (selectedValue === 'test') {
        data = ultimos10DatosGlobal.map(dato => dato["Presion"]);
        color = '153, 102, 255';
        label = 'Presión promedio';
    } else if (selectedValue === 'emm3') {
        data = ultimos10DatosGlobal.map(dato => dato["Humedad"]);
        color = '255, 159, 64';
        label = 'Humedad promedio';
    }

    mostrarPopup('#popup-grafico', 'Gráfico', label, data, color);
}

// Función para mostrar el popup con gráfico
function mostrarPopup(idPopup, titulo, key, color) {
    $(idPopup).on('click', function () {
        const labels = ultimos10DatosGlobal.map(dato => dato["Fecha"]);
        const data = ultimos10DatosGlobal.map(dato => parseFloat(dato[key]) || null);

        Swal.fire({
            title: titulo,
            width: 600,
            html: `
                <div style="display: flex; flex-direction: row; align-items: flex-start;">
                    <div style="flex: 2; padding-right: 20px;">
                        <canvas id="grafico${titulo}" width="400" height="220" style="width: 100%; height: auto;"></canvas>
                    </div>
                </div>
                <p style="font-size: 12px; color: gray; margin-top: 10px;">Nota: Los gráficos pueden no mostrarse correctamente si los datos son insuficientes o están mal formateados.</p>

            `,
            icon: 'info',
            confirmButtonText: 'Cerrar',
            customClass: {
                popup: 'popup-content',
            },
            didOpen: function () {
                const ctx = document.getElementById(`grafico${titulo}`).getContext('2d');
                new Chart(ctx, {
                    type: 'line',
                    data: {
                        labels: labels.reverse(),
                        datasets: [{
                            label: `${titulo} (Últimos 10 registros)`,
                            data: data.reverse(),
                            borderColor: `rgba(${color}, 1)`,
                            backgroundColor: `rgba(${color}, 0.2)`,
                            fill: true,
                        }]
                    },
                    options: {
                        responsive: true,
                        maintainAspectRatio: false,
                        scales: {
                            x: {
                                type: 'category',
                                title: {
                                    display: true,
                                    text: 'Fecha'
                                }
                            },
                            y: {
                                beginAtZero: false,
                                title: {
                                    display: true,
                                    text: titulo
                                }
                            }
                        }
                    }
                });
            }
        });
    });
}

mostrarPopup('#popup-temperatura', 'Temperatura', 'Temperatura Promedio', '79, 126, 255');
mostrarPopup('#popup-bp', 'Presión', 'Presion', '153, 102, 255');
mostrarPopup('#popup-hum', 'Humedad', 'Humedad', '255, 159, 64');
mostrarPopup('#popup-luz', 'Nivel de Luz', 'Nivel de luz', '255, 205, 86');
mostrarPopup('#popup-aire', 'Calidad del Aire', 'Calidad del aire', '54, 162, 235');
mostrarPopup('#popup-voltaje', 'Voltaje de la batería', 'Nivel de bateria', '255, 99, 132');
mostrarPopup('#popup-stermica', 'Sensación Térmica', 'Sensacion T', '0, 255, 255');
mostrarPopup('#popup-rocio', 'Punto de rocío', 'Temperatura punto de rocio', '255, 51, 153');
mostrarPopup('#popup-altitud', 'Altitud', 'Altitud', '102, 255, 102');
mostrarPopup('#popup-co2', 'Partes por millón CO2', 'ppm CO2', '204, 204, 0');
mostrarPopup('#popup-vientoVel', 'Velocidad del viento', 'Velocidad del viento', '0, 204, 255');
mostrarPopup('#popup-vientoDir', 'Dirección del viento', 'Direccion del viento', '153, 255, 255');
mostrarPopup('#popup-lluvia', 'Sensor de lluvia', 'Sensor de lluvia', '179, 179, 255');


function invokemap() {
    Swal.fire({
        title: 'Mapa Interactivo',
        html: '<div id="map"></div>',
        width: '600px',
        customClass: {
            popup: 'custom-swal-popup'
        },
        didOpen: () => {
            const map = L.map('map').setView([-32.96992406345138, -68.8440688212624], 12);
            L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
                maxZoom: 19,
                attribution: '© OpenStreetMap'
            }).addTo(map);

            const locations = [
                { lat: -32.9518493953854, lng: -68.85142021358315, popup: 'EME: ETec' },
                { lat: -32.985218761976896, lng: -68.89770780138929, popup: 'EMM: Colegio San Jorge' }
            ];

            locations.forEach(location => {
                const marker = L.marker([location.lat, location.lng]).addTo(map);
            
                // Generar un ID único para el botón
                const botonId = `btn-${location.popup.replace(/\s+/g, '-')}`;
            
                // Crear el HTML del popup
                const popupContent = `
                    <h3 style=" color: black;">${location.popup}</h3><br>
                    <strong>${location.popup}</strong><br>
                    <button id="${botonId}" style="margin-top: 5px; padding: 4px 8px; background-color: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer;">
                        Seleccionar nodo
                    </button>
                `;
            
                marker.bindPopup(popupContent);
            
                marker.on('popupopen', () => {
                    setTimeout(() => { // Esperar que el DOM del popup esté disponible
                        const btn = document.getElementById(botonId);
                        if (btn) {
                            btn.addEventListener('click', () => {
                                let selectedValue = '';
            
                                if (location.popup.includes('ETec')) {
                                    selectedValue = 'emetec';
                                } else if (location.popup.includes('San Jorge')) {
                                    selectedValue = 'sj';
                                } else if (location.popup.includes('Tincho')) {
                                    selectedValue = 'test';
                                }
            
                                categoriaDropdown.value = selectedValue;
                                setCookie('categoriaDropdown', selectedValue, 30);
            
                                if (selectedValue === 'emetec') {
                                    obtenerDatos(apiUrl);
                                } else if (selectedValue === 'test') {
                                    obtenerDatos(apiTempUrl);
                                } else if (selectedValue === 'sj') {
                                    obtenerDatos(apiSJ);
                                }
            
                                Swal.close(); // Cerrar el mapa popup
                            });
                        }
                    }, 100);
                });
            });
            
            
        }
    });
}
