document.addEventListener('DOMContentLoaded', () => {
  // ─── Endpoints ───────────────────────────────────────────────────────────────
  const apiUrl     = 'https://emetec.wetec.um.edu.ar/datos';
  const apiTempUrl = 'https://emetec.wetec.um.edu.ar/datosemm';
  const apiSJ      = 'https://emetec.wetec.um.edu.ar/datosemesj';

  let ultimos10DatosGlobal = [];
  let ultimoDatoGlobal    = {};

  // ─── Helpers ─────────────────────────────────────────────────────────────────
  function setText(id, value) {
    const el = document.getElementById(id);
    if (el) el.textContent = value;
    else console.warn(`No existe elemento con id="${id}"`);
  }

  // ─── Captura del dropdown y listener ─────────────────────────────────────────
  const categoriaDropdown = document.getElementById('categoriaDropdown');
  if (!categoriaDropdown) {
    console.error('Falta el elemento <select id="categoriaDropdown">');
    return;
  }

  categoriaDropdown.addEventListener('change', () => {
    const selectedValue = categoriaDropdown.value;
    setCookie('categoriaDropdown', selectedValue, 30);

    if (selectedValue === 'emetec') {
      obtenerDatos(apiUrl);
    } else if (selectedValue === 'test') {
      obtenerDatos(apiTempUrl);
    } else if (selectedValue === 'sj') {
      obtenerDatos(apiSJ);
    }

    verificarEstadoNodo();
  });

  // ─── Carga inicial ────────────────────────────────────────────────────────────
  obtenerDatos(apiUrl);
  verificarEstadoNodo();

  // ─── Función para obtener datos ──────────────────────────────────────────────
  async function obtenerDatos(url) {
    try {
      const response = await fetch(url, {
        headers: { 'Access-Control-Allow-Origin': '*' }
      });
      if (!response.ok) throw new Error('HTTP ' + response.status);

      const data = await response.json();
      const ultimos10Datos = data.slice(-10);
      ultimos10DatosGlobal = ultimos10Datos;
      ultimoDatoGlobal    = ultimos10Datos[0];

      // normalizador de valores “no disponibles”
      const obtenerValor = v =>
        (v === null ||
         v === 'null' ||
         v === 'Proximamente' ||
         v === 0 ||
         v === '0' ||
         v === -1 ||
         v === '-1' ||
         v === undefined ||
         v <= 0 ||
         v === 'NaN')
        ? 'No disponible'
        : v;

      // Actualizo tarjetas principales
      setText('datoNumero', obtenerValor(ultimoDatoGlobal['Dato N°']));
      setText('fecha',      obtenerValor(ultimoDatoGlobal['Fecha']));
      setText('battery',
        (obtenerValor(ultimoDatoGlobal['Nivel de bateria']) === 'No disponible')
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Nivel de bateria'])} V`
      );
      setText('temperatura',
        (obtenerValor(ultimoDatoGlobal['Temperatura Promedio']) === 'No disponible')
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Temperatura Promedio'])} °C`
      );
      setText('stermica',
        (obtenerValor(ultimoDatoGlobal['Sensacion T']) === 'No disponible')
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Sensacion T'])} °C`
      );
      setText('presion',
        (obtenerValor(ultimoDatoGlobal['Presion']) === 'No disponible')
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Presion'])} hPa`
      );
      setText('humedad',
        (obtenerValor(ultimoDatoGlobal['Humedad']) === 'No disponible')
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Humedad'])} %`
      );
      setText('nivelLuz',
        (obtenerValor(ultimoDatoGlobal['Nivel de luz']) === 'No disponible')
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Nivel de luz'])} lm`
      );
      setText('procio',
        (obtenerValor(ultimoDatoGlobal['Temperatura punto de rocio']) === 'No disponible')
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Temperatura punto de rocio'])} °C`
      );
      setText('ppmco2',
        (obtenerValor(ultimoDatoGlobal['ppm CO2']) === 'No disponible')
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['ppm CO2'])} ppm`
      );
      setText('calidadAire',      obtenerValor(ultimoDatoGlobal['Calidad del aire']));
      setText('velocidadViento',  obtenerValor(ultimoDatoGlobal['Velocidad del viento']));
      setText('direccionViento',  obtenerValor(ultimoDatoGlobal['Direccion del viento']));
      setText('sensorLluvia',     obtenerValor(ultimoDatoGlobal['Sensor de lluvia']));
      setText('Altitud',          obtenerValor(ultimoDatoGlobal['Altitud']));
      // Fechas secundarias
      setText('fecha2', obtenerValor(ultimoDatoGlobal['Fecha']));
      setText('fecha3', obtenerValor(ultimoDatoGlobal['Fecha']));
      setText('fecha4', obtenerValor(ultimoDatoGlobal['Fecha']));
      setText('fecha5', obtenerValor(ultimoDatoGlobal['Fecha']));

      // Tabla de detalles
      const tablaDetallesCuerpo = document.getElementById('tablaDetallesCuerpo');
      if (tablaDetallesCuerpo) {
        tablaDetallesCuerpo.innerHTML = '';
        ultimos10Datos.forEach(dato => {
          const fila = document.createElement('tr');
          fila.innerHTML = `
            <td>${obtenerValor(dato['Dato N°'])}</td>
            <td>${obtenerValor(dato['Fecha'])}</td>
            <td>${obtenerValor(dato['Temperatura Promedio'])} °C</td>
            <td>${obtenerValor(dato['Sensacion T'])} °C</td>
            <td>${obtenerValor(dato['Presion'])} hPa</td>
            <td>${obtenerValor(dato['Humedad'])} %</td>
            <td>${obtenerValor(dato['Nivel de luz'])} lm</td>
          `;
          tablaDetallesCuerpo.appendChild(fila);
        });
      }

      actualizarGrafico();
    }
    catch (err) {
      console.error('Error al obtener datos:', err);
    }
  }

  // ─── Actualiza datos para el gráfico según categoría ────────────────────────
  function actualizarGrafico() {
    const selectedValue = categoriaDropdown.value;
    let key   = '';
    let color = '';
    let title = '';

    if (selectedValue === 'emetec') {
      key   = 'Temperatura Promedio';
      color = '79, 126, 255';
      title = 'Temperatura promedio';
    }
    else if (selectedValue === 'test') {
      key   = 'Presion';
      color = '153, 102, 255';
      title = 'Presión promedio';
    }
    else if (selectedValue === 'sj') {
      key   = 'Humedad';
      color = '255, 159, 64';
      title = 'Humedad promedio';
    }

    mostrarPopup('#popup-grafico', title, key, color);
  }

  // ─── Muestra un popup con gráfico Chart.js ─────────────────────────────────
  function mostrarPopup(idPopup, titulo, key, color) {
    $(idPopup).on('click', () => {
      const labels = ultimos10DatosGlobal.map(d => d['Fecha']).reverse();
      const data   = ultimos10DatosGlobal.map(d => parseFloat(d[key]) || null).reverse();

      Swal.fire({
        title: titulo,
        width: 600,
        html: `
          <div style="display:flex; flex-direction:row; align-items:flex-start;">
            <div style="flex:2; padding-right:20px;">
              <canvas id="grafico${titulo.replace(/\s/g,'')}" width="400" height="220"></canvas>
            </div>
          </div>
          <p style="font-size:12px; color:gray; margin-top:10px;">
            Nota: si faltan datos, el gráfico puede no verse perfecto.
          </p>
        `,
        icon: 'info',
        confirmButtonText: 'Cerrar',
        didOpen: () => {
          const ctx = document
            .getElementById(`grafico${titulo.replace(/\s/g,'')}`)
            .getContext('2d');
          new Chart(ctx, {
            type: 'line',
            data: {
              labels,
              datasets: [{
                label: `${titulo} (últimos 10)`,
                data,
                borderColor: `rgba(${color},1)`,
                backgroundColor: `rgba(${color},0.2)`,
                fill: true
              }]
            },
            options: {
              responsive: true,
              maintainAspectRatio: false,
              scales: {
                x: { title: { display:true, text:'Fecha' } },
                y: { title: { display:true, text:titulo } }
              }
            }
          });
        }
      });
    });
  }

  // ─── Mapa interactivo con Leaflet ───────────────────────────────────────────
  function invokemap() {
    Swal.fire({
      title: 'Mapa Interactivo',
      html: '<div id="map" style="height:300px;"></div>',
      width: 600,
      didOpen: () => {
        const map = L.map('map').setView([-32.969924, -68.844069], 12);
        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
          maxZoom: 19,
          attribution: '© OpenStreetMap'
        }).addTo(map);

        const locations = [
          { lat:-32.951849, lng:-68.851420, popup:'EME: ETec' },
          { lat:-32.985219, lng:-68.897708, popup:'EMM: Colegio San Jorge' }
        ];

        locations.forEach(loc => {
          const marker = L.marker([loc.lat, loc.lng]).addTo(map);
          const btnId  = `btn-${loc.popup.replace(/\s+/g,'')}`;
          const html   = `
            <h3 style="color:black;">${loc.popup}</h3><br>
            <button id="${btnId}"
              style="margin-top:5px;padding:4px 8px;
                     background:#007bff;color:white;
                     border:none;border-radius:4px;cursor:pointer;">
              Seleccionar nodo
            </button>
          `;
          marker.bindPopup(html);
          marker.on('popupopen', () => {
            setTimeout(() => {
              const btn = document.getElementById(btnId);
              if (!btn) return;
              btn.addEventListener('click', () => {
                let sel = '';
                if (loc.popup.includes('ETec')) sel = 'emetec';
                else if (loc.popup.includes('San Jorge')) sel = 'sj';
                categoriaDropdown.value = sel;
                setCookie('categoriaDropdown', sel, 30);
                categoriaDropdown.dispatchEvent(new Event('change'));
                Swal.close();
              });
            }, 100);
          });
        });
      }
    });
  }

  // Exponer al scope global
  window.invokemap = invokemap;

  // ─── Verificar estado del nodo EMEtec ────────────────────────────────────────
  async function verificarEstadoNodo() {
    const estadoElemento = document.getElementById('estadoNodo');
    if (!estadoElemento) {
      console.warn('No existe #estadoNodo en el HTML');
      return;
    }
    if (categoriaDropdown.value !== 'emetec') {
      estadoElemento.style.display = 'none';
      return;
    }

    estadoElemento.style.display = 'flex';
    estadoElemento.className   = 'card-estado-nodo compact';
    estadoElemento.innerHTML   = '';

    try {
      const resp = await fetch('https://emetec.wetec.um.edu.ar/emsdb');
      if (!resp.ok) throw new Error('HTTP ' + resp.status);
      const data = await resp.json();
      const emetec = data.find(e => e.Nombre.includes('ETec'));

      if (!emetec) {
        estadoElemento.classList.add('unknown');
        estadoElemento.innerHTML = `
          <i class="fa-solid fa-circle-question"></i>
          <span>Estado del nodo desconocido</span>
        `;
      } else {
        const enLinea = emetec.Estado === 'En Linea';
        estadoElemento.classList.add(enLinea ? 'online' : 'offline');
        estadoElemento.innerHTML = enLinea
          ? `<i class="fa-solid fa-circle-check"></i>
             <span class="estado-texto">Nodo en línea</span>`
          : `<i class="fa-solid fa-circle-xmark"></i>
             <span class="estado-texto">Nodo fuera de línea</span>`;
      }
    }
    catch (err) {
      console.error('Error al verificar estado:', err);
      estadoElemento.classList.add('unknown');
      estadoElemento.innerHTML = `
        <i class="fa-solid fa-triangle-exclamation"></i>
        <span>No se pudo obtener el estado del nodo</span>
      `;
    }
  }
});