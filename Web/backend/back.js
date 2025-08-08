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

    // Carga datos según selección
    if (selectedValue === 'emetec') {
      obtenerDatos(apiUrl);
    } else if (selectedValue === 'test') {
      obtenerDatos(apiTempUrl);
    } else if (selectedValue === 'sj') {
      obtenerDatos(apiSJ);
    }

    verificarEstadoNodo(); // 🔄 ver estado para EMEtec o EMESJ
  });

  // ─── Carga inicial ────────────────────────────────────────────────────────────
  const inicial = categoriaDropdown.value || 'emetec';
  categoriaDropdown.value = inicial;
  obtenerDatos(inicial === 'sj' ? apiSJ : apiUrl);
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
        (v == null || v === 'null' || v === 'Proximamente' ||
         v === 0 || v === '0' || v === -1 || v === '-1' ||
         v === undefined || isNaN(v) || v <= 0)
        ? 'No disponible'
        : v;

      // Actualizo tarjetas principales
      setText('datoNumero', ultimoDatoGlobal['Dato N°']);
      setText('fecha',      ultimoDatoGlobal['Fecha']);
      setText('battery',
        obtenerValor(ultimoDatoGlobal['Nivel de bateria']) === 'No disponible'
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Nivel de bateria'])} V`
      );
      setText('temperatura',
        obtenerValor(ultimoDatoGlobal['Temperatura Promedio']) === 'No disponible'
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Temperatura Promedio'])} °C`
      );
      setText('stermica',
        obtenerValor(ultimoDatoGlobal['Sensacion T']) === 'No disponible'
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Sensacion T'])} °C`
      );
      setText('presion',
        obtenerValor(ultimoDatoGlobal['Presion']) === 'No disponible'
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Presion'])} hPa`
      );
      setText('humedad',
        obtenerValor(ultimoDatoGlobal['Humedad']) === 'No disponible'
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Humedad'])} %`
      );
      setText('nivelLuz',
        obtenerValor(ultimoDatoGlobal['Nivel de luz']) === 'No disponible'
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Nivel de luz'])} lm`
      );
      setText('procio',
        obtenerValor(ultimoDatoGlobal['Temperatura punto de rocio']) === 'No disponible'
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Temperatura punto de rocio'])} °C`
      );
      setText('ppmco2',
        obtenerValor(ultimoDatoGlobal['ppm CO2']) === 'No disponible'
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['ppm CO2'])} KΩ`
      );
      setText('calidadAire',      obtenerValor(ultimoDatoGlobal['Calidad del aire']));
      setText('velocidadViento',  obtenerValor(ultimoDatoGlobal['Velocidad del viento']));
      setText('direccionViento',  obtenerValor(ultimoDatoGlobal['Direccion del viento']));
      setText('sensorLluvia',     obtenerValor(ultimoDatoGlobal['Sensor de lluvia']));
      setText('Altitud',
        obtenerValor(ultimoDatoGlobal['Altitud']) === 'No disponible'
        ? 'No disponible'
        : `${obtenerValor(ultimoDatoGlobal['Altitud'])} m`
      );

      // Fechas secundarias
      ['fecha2','fecha3','fecha4','fecha5','fecha6']
        .forEach(id => setText(id, ultimoDatoGlobal['Fecha']));

      // Tabla de detalles
      const tbody = document.getElementById('tablaDetallesCuerpo');
      if (tbody) {
        tbody.innerHTML = '';
        ultimos10Datos.forEach(dato => {
          const fila = document.createElement('tr');
          fila.innerHTML = `
            <td>${obtenerValor(dato['Dato N°'])}</td>
            <td>${dato['Fecha']}</td>
            <td>${obtenerValor(dato['Temperatura Promedio'])} °C</td>
            <td>${obtenerValor(dato['Humedad'])} %</td>
            <td>${obtenerValor(dato['Presion'])} hPa</td>
            <td>${obtenerValor(dato['Temperatura punto de rocio'])} °C</td>
            <td>${obtenerValor(dato['Calidad del aire'])}</td>
          `;
          tbody.appendChild(fila);
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
    const sel = categoriaDropdown.value;
    let key='', color='', title='';

    if (sel === 'emetec') {
      key='Temperatura Promedio'; color='79, 126, 255'; title='Temperatura promedio';
    }
    else if (sel === 'test') {
      key='Presion';             color='153, 102, 255'; title='Presión promedio';
    }
    else if (sel === 'sj') {
      key='Humedad';             color='255, 159, 64';  title='Humedad promedio';
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
            data: { labels, datasets: [{ label: `${titulo} (últimos 10)`, data, borderColor:`rgba(${color},1)`, backgroundColor:`rgba(${color},0.2)`, fill:true }] },
            options: { responsive:true, maintainAspectRatio:false,
              scales:{ x:{ title:{ display:true, text:'Fecha' } }, y:{ title:{ display:true, text:titulo } } }
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


  // ─── Muestra mapa con botones de selección ─────────────────────────────────
  function invokemap() {
    Swal.fire({
      title: 'Mapa Interactivo',
      html: '<div id="map" style="height:300px;"></div>',
      width: 600,
      didOpen: () => {
        const map = L.map('map').setView([-32.969924, -68.844069], 12);
        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
          maxZoom: 19, attribution: '© OpenStreetMap'
        }).addTo(map);

        const locations = [
          { lat:-32.951849, lng:-68.851420, popup:'EME: ETec',   sel:'emetec' },
          { lat:-32.985219, lng:-68.897708, popup:'EMM: San Jorge', sel:'sj' }
        ];

        locations.forEach(loc => {
          const marker = L.marker([loc.lat, loc.lng]).addTo(map);
          const btnId  = `btn-${loc.sel}`;
          const html   = `
            <h3>${loc.popup}</h3><br>
            <button id="${btnId}"
              style="margin-top:5px;padding:4px 8px;background:#007bff;color:white;border:none;border-radius:4px;cursor:pointer;">
              Seleccionar nodo
            </button>
          `;
          marker.bindPopup(html);
          marker.on('popupopen', () => {
            setTimeout(() => {
              const btn = document.getElementById(btnId);
              if (!btn) return;
              btn.addEventListener('click', () => {
                categoriaDropdown.value = loc.sel;
                setCookie('categoriaDropdown', loc.sel, 30);
                categoriaDropdown.dispatchEvent(new Event('change'));
                Swal.close();
              });
            }, 100);
          });
        });
      }
    });
  }
  window.invokemap = invokemap;

  // ─── Verificar estado del nodo para EMEtec y EMESJ ───────────────────────────
  async function verificarEstadoNodo() {
    const estadoEl = document.getElementById('estadoNodo');
    const sel = categoriaDropdown.value;
    if (!estadoEl) return console.warn('No existe #estadoNodo');

    // Solo para emetec o sj
    if (sel !== 'emetec' && sel !== 'sj') {
      estadoEl.style.display = 'none';
      return;
    }
    estadoEl.style.display = 'flex';
    estadoEl.className = 'card-estado-nodo compact';
    estadoEl.innerHTML = ''; 

    try {
      const resp = await fetch('https://emetec.wetec.um.edu.ar/emsdb');
      if (!resp.ok) throw new Error('HTTP ' + resp.status);
      const data = await resp.json();

      // Elijo el nodo correcto
      const filtro = sel === 'emetec'
        ? e => e.Nombre.includes('ETec')
        : e => e.Nombre.includes('San Jorge');
      const nodo = data.find(filtro);

      if (!nodo) {
        estadoEl.classList.add('unknown');
        estadoEl.innerHTML = `
          <i class="fa-solid fa-circle-question"></i>
          <span>Estado del nodo desconocido</span>`;
      } else {
        const enLinea = nodo.Estado === 'En Linea';
        estadoEl.classList.add(enLinea ? 'online' : 'offline');
        estadoEl.innerHTML = enLinea
          ? `<i class="fa-solid fa-circle-check"></i>
             <span class="estado-texto">Nodo en línea</span>`
          : `<i class="fa-solid fa-circle-xmark"></i>
             <span class="estado-texto">Nodo fuera de línea</span>`;
      }
    }
    catch (err) {
      console.error('Error al verificar estado:', err);
      estadoEl.classList.add('unknown');
      estadoEl.innerHTML = `
        <i class="fa-solid fa-triangle-exclamation"></i>
        <span>No se pudo obtener el estado del nodo</span>`;
    }
  }
});
