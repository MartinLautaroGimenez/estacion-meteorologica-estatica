let apiEMEtec = 'https://emetec.wetec.um.edu.ar/exceleme';
let apiTest = 'https://emetec.wetec.um.edu.ar/excelemm';
let apiSJexcel = 'https://emetec.wetec.um.edu.ar/excelemesj';

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

// Función para descargar los datos en XLSX usando la selección guardada en cookie
async function descargarDatosEnXLSX() {
    try {
        // Obtener el valor del dropdown desde la cookie
        let selectedCategory = getCookie('categoriaDropdown');
        if (selectedCategory === "") {
            selectedCategory = 'emetec';
        }

        let apiUrl;
        if (selectedCategory === 'emetec') {
            apiUrl = apiEMEtec;
        } else if (selectedCategory === 'test') {
            apiUrl = apiTest;
        } else if (selectedCategory === 'sj') {
            apiUrl = apiSJexcel;
        }
        else {
            console.error('Categoría inválida:', selectedCategory);
            return;
        }

        const response = await fetch(apiUrl, {
            headers: {
                'Access-Control-Allow-Origin': '*' // Permitir CORS, si es necesario
            }
        });

        if (response.ok) {
            const datos = await response.json();
            const workbook = XLSX.utils.book_new();
            const worksheet = XLSX.utils.json_to_sheet(datos);
            XLSX.utils.book_append_sheet(workbook, worksheet, 'Datos');
            const archivoXLSX = XLSX.write(workbook, { bookType: 'xlsx', type: 'array' });

            // Convertir el archivo XLSX a un blob
            const blob = new Blob([archivoXLSX], { type: 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet' });

            // Descargar el archivo XLSX
            const url = URL.createObjectURL(blob);
            const link = document.createElement('a');
            link.href = url;
            link.download = 'datos.xlsx';
            document.body.appendChild(link);
            link.click();
            document.body.removeChild(link);

            // Limpiar
            URL.revokeObjectURL(url);
        } else {
            console.error('Error al descargar los datos:', response.status);
        }
    } catch (error) {
        console.error('Error al descargar los datos:', error);
    }
}

// Event listener para el dropdown
document.getElementById('categoriaDropdown').addEventListener('change', function() {
    const selectedValue = this.value;
    setCookie('categoriaDropdown', selectedValue, 30); // Guardar selección en cookie
});
