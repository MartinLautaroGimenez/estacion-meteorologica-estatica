const express = require('express');
const fs = require('fs');
const axios = require('axios');
const XLSX = require('xlsx');
const https = require('https');

const app = express();
const port = 3000;

// Función para convertir JSON a Excel
function jsonToExcel(jsonData, outputPath) {
    // Crear un libro de Excel
    const workbook = XLSX.utils.book_new();
    
    // Convertir JSON a una hoja de Excel
    const worksheet = XLSX.utils.json_to_sheet(jsonData);

    // Añadir la hoja al libro
    XLSX.utils.book_append_sheet(workbook, worksheet, 'Sheet1');

    // Escribir el libro en un archivo
    XLSX.writeFile(workbook, outputPath);

    console.log(`Archivo Excel generado en: ${outputPath}`);
}

// URL que devuelve el JSON
const url = 'http://emetec.wetec.um.edu.ar/excelpt';

// Ruta de salida del archivo Excel
const outputPath = 'output.xlsx';

// Configuración de Axios para evitar la verificación del certificado
const axiosConfig = {
    httpsAgent: new https.Agent({ rejectUnauthorized: false })
};

// Ruta para la página HTML
app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
});

// Ruta para la descarga del archivo Excel
app.get('/download', (req, res) => {
    // Hacer la solicitud HTTP para obtener el JSON
    axios.get(url, axiosConfig)
        .then(response => {
            const jsonData = response.data; // JSON obtenido de la respuesta
            // Llamar a la función para convertir el JSON en Excel
            jsonToExcel(jsonData, outputPath);
            // Descargar el archivo Excel al hacer clic en el botón
            res.download(outputPath, 'data.xlsx', (err) => {
                if (err) {
                    console.error('Error al descargar el archivo:', err);
                } else {
                    console.log('Archivo descargado con éxito');
                    // Eliminar el archivo después de la descarga
                    fs.unlinkSync(outputPath);
                }
            });
        })
        .catch(error => {
            console.error('Error al obtener el JSON:', error);
            res.status(500).send('Error interno del servidor');
        });
});

// Iniciar el servidor
app.listen(port, () => {
    console.log(`Servidor en ejecución en http://localhost:${port}`);
});