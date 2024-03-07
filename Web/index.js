// Importar los módulos necesarios
const express = require('express');
const mysql = require('mysql');

// Crear una instancia de la aplicación Express
const app = express();

// Configuración de la conexión a la base de datos
const connection = mysql.createConnection({
  host: '127.0.0.1', // Cambiar por tu host
  user: '1234', // Cambiar por tu usuario
  password: '1234', // Cambiar por tu contraseña
  database: 'db_for_test' // Cambiar por el nombre de tu base de datos
});

// Conectar a la base de datos
connection.connect((err) => {
  if (err) {
    console.error('Error de conexión a la base de datos:', err);
    return;
  }
  console.log('Conexión a la base de datos exitosa!');
});

// Ruta para obtener el último dato de la base de datos y mostrarlo en un H1
app.get('/', (req, res) => {
  // Consulta SQL para obtener el último dato (asumiendo que hay una tabla llamada 'datos')
  // Ejemplo de consulta SQL para obtener datos de una tabla
    const sql = 'SELECT * FROM estacion_test ORDER BY `Dato N°` DESC LIMIT 1';


  // Ejecutar la consulta
  connection.query(sql, (err, result) => {
    if (err) {
      console.error('Error al ejecutar la consulta:', err);
      res.status(500).send('Error interno del servidor');
      return;
    }

    // Obtener el resultado de la consulta y mostrarlo en un H1
    const ultimoDato = result[0];
    res.send(`
      <h1>Último dato:</h1>
      <p>Dato N°: ${ultimoDato['Dato N°']}</p>
      <p>Fecha: ${ultimoDato.Fecha}</p>
      <p>Temperatura: ${ultimoDato.Temperatura}</p>
      <p>Presión Atmosférica: ${ultimoDato['Presión Atmosférica']}</p>
      <p>Altitud: ${ultimoDato.Altitud}</p>
      <p>Humedad Relativa: ${ultimoDato['Humedad Relativa']}</p>
    `);
  });
});

// Iniciar el servidor en el puerto 3000
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Servidor iniciado en el puerto ${PORT}`);
});
