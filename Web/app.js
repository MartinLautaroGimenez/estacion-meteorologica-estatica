// Selección de elementos del DOM
const sideMenu = document.querySelector("aside");
const themeToggler = document.querySelector(".theme-toggler");
const nextDay = document.getElementById('nextDay');
const prevDay = document.getElementById('prevDay');
const categoriaDropdown = document.getElementById('categoriaDropdown');

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
    document.body.classList.add('dark-theme');
    themeToggler.querySelector('span:nth-child(1)').classList.remove('active');
    themeToggler.querySelector('span:nth-child(2)').classList.add('active');
}

// Función para establecer cookies
function setCookie(name, value, days) {
    const date = new Date();
    date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
    const expires = "expires=" + date.toUTCString();
    document.cookie = name + "=" + value + ";" + expires + ";path=/";
}

// Función para obtener el valor de una cookie
function getCookie(name) {
    const decodedCookie = decodeURIComponent(document.cookie);
    const cookies = decodedCookie.split(';');
    name = name + "=";
    for (let cookie of cookies) {
        cookie = cookie.trim();
        if (cookie.indexOf(name) === 0) {
            return cookie.substring(name.length, cookie.length);
        }
    }
    return "";
}

// Cargar el valor del dropdown desde la cookie si existe
window.addEventListener('load', function() {
    const savedValue = getCookie('categoriaDropdown');
    if (savedValue) {
        categoriaDropdown.value = savedValue;
    } else {
        categoriaDropdown.value = "emetec"; // Valor default si no hay cookie
    }
    categoriaDropdown.dispatchEvent(new Event('change')); // Disparar cambio para cargar datos iniciales
});

//APIs
const apiUrl = 'https://emetec.wetec.um.edu.ar/datos';
const apiTempUrl = 'https://emetec.wetec.um.edu.ar/datosemm';
const apiEMM3 = 'https://emetec.wetec.um.edu.ar/datosemm3';
const apiEMM4 = 'https://emetec.wetec.um.edu.ar/datosemm4';
const apiSJ = 'https://emetec.wetec.um.edu.ar/datosemesj';

// Event listener para el dropdown
categoriaDropdown.addEventListener('change', function() {
    const selectedValue = categoriaDropdown.value;
    setCookie('categoriaDropdown', selectedValue, 30); // Guardar selección en cookie
    if (selectedValue === 'emetec') {
        obtenerDatos(apiUrl);
    } else if (selectedValue === 'test') {
        obtenerDatos(apiTempUrl);
    } else if (selectedValue === 'emm3') {
        obtenerDatos(apiEMM3);
    } else if (selectedValue === 'emm4'){
        obtenerDatos(apiEMM4);
    } else if (selectedValue === 'sj'){
        obtenerDatos(apiSJ);
    }
});

// Función para obtener datos
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

        const ultimoDato = ultimos10Datos[0];
        
        // Función para manejar valores nulos
        const obtenerValor = (valor) => (
            valor == null || valor === "Proximamente" || valor <= 0
                ? "No disponible"
                : valor
        );
        
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
        document.getElementById('Altitud').textContent = obtenerValor(ultimoDato["Altitud"]) + "mts";
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
    } catch (error) {
        console.error('Error al obtener los datos:', error);
    }
}
