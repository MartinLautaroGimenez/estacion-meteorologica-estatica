const apiUrl = 'https://emetec.wetec.um.edu.ar/excel';

async function descargarDatosEnXLSX() {
  try {
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