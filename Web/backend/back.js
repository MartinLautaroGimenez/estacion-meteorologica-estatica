// back.js – ¡Tu back.js arreglado! 🚀
document.addEventListener('DOMContentLoaded', () => {
  // ─── Endpoints ───────────────────────────────────────────────────────────────────────────
  const apiUrl     = 'https://emetec.wetec.um.edu.ar/datos';
  const apiTempUrl = 'https://emetec.wetec.um.edu.ar/datosemm';
  const apiSJ      = 'https://emetec.wetec.um.edu.ar/datosemesj';

  let ultimos10DatosGlobal = [];
  let ultimoDatoGlobal    = {};

  // ─── Helper para evitar null.innerHTML ───────────────────────────────────────────────────
  function setText(id, value) {
    const el = document.getElementById(id);
    if (el) {
      el.textContent = value;
    } else {
      console.warn(`❓ No existe elemento con id="${id}"`);
    }
  }

  // ─── Captura del dropdown y listener ─────────────────────────────────────────────────────
  const categoriaDropdown = document.getElementById('categoriaDropdown');
  if (!categoriaDropdown) {
    console.error('🚨 Falta el <select id="categoriaDropdown">. Abortando script.');
    return;
  }

  categoriaDropdown.addEventListener('change', () => {
    const sel = categoriaDropdown.value;
    setCookie('categoriaDropdown', sel, 30);

    if (sel === 'emetec')      obtenerDatos(apiUrl);
    else if (sel === 'test')   obtenerDatos(apiTempUrl);
    else if (sel === 'sj')     obtenerDatos(apiSJ);
    else console.warn(`⚠️ Valor desconocido: "${sel}"`);

    verificarEstadoNodo();
  });

  // ─── Carga inicial ────────────────────────────────────────────────────────────────────────
  obtenerDatos(apiUrl);
  verificarEstadoNodo();

  // ─── Función para obtener datos ──────────────────────────────────────────────────────────
  async function obtenerDatos(url) {
    try {
      const resp = await fetch(url, { headers: { 'Access-Control-Allow-Origin': '*' } });
      if (!resp.ok) throw new Error('HTTP ' + resp.status);

      const data = await resp.json();
      const arr  = data.slice(-10);
      ultimos10DatosGlobal = arr;
      ultimoDatoGlobal    = arr[0] || {};

      const norm = v =>
        (v == null || v === 'null' || v === 'Proximamente' ||
         v === 0    || v === '0'    || v === -1         ||
         v === '-1' || v === undefined || v <= 0        ||
         v === 'NaN')
        ? 'No disponible'
        : v;

      // ── Actualizo tarjetas principales ──────────────────────────────────────────────
      setText('datoNumero', norm(ultimoDatoGlobal['Dato N°']));
      setText('fecha',      norm(ultimoDatoGlobal['Fecha']));
      setText('battery',
        norm(ultimoDatoGlobal['Nivel de bateria']) === 'No disponible'
          ? 'No disponible'
          : `${norm(ultimoDatoGlobal['Nivel de bateria'])} V`);
      setText('temperatura',
        norm(ultimoDatoGlobal['Temperatura Promedio']) === 'No disponible'
          ? 'No disponible'
          : `${norm(ultimoDatoGlobal['Temperatura Promedio'])} °C`);
      setText('stermica',
        norm(ultimoDatoGlobal['Sensacion T']) === 'No disponible'
          ? 'No disponible'
          : `${norm(ultimoDatoGlobal['Sensacion T'])} °C`);
      setText('presion',
        norm(ultimoDatoGlobal['Presion']) === 'No disponible'
          ? 'No disponible'
          : `${norm(ultimoDatoGlobal['Presion'])} hPa`);
      setText('humedad',
        norm(ultimoDatoGlobal['Humedad']) === 'No disponible'
          ? 'No disponible'
          : `${norm(ultimoDatoGlobal['Humedad'])} %`);
      setText('nivelLuz',
        norm(ultimoDatoGlobal['Nivel de luz']) === 'No disponible'
          ? 'No disponible'
          : `${norm(ultimoDatoGlobal['Nivel de luz'])} lm`);
      setText('procio',
        norm(ultimoDatoGlobal['Temperatura punto de rocio']) === 'No disponible'
          ? 'No disponible'
          : `${norm(ultimoDatoGlobal['Temperatura punto de rocio'])} °C`);
      setText('ppmco2',
        norm(ultimoDatoGlobal['ppm CO2']) === 'No disponible'
          ? 'No disponible'
          : `${norm(ultimoDatoGlobal['ppm CO2'])} ppm`);
      setText('calidadAire',     norm(ultimoDatoGlobal['Calidad del aire']));
      setText('velocidadViento', norm(ultimoDatoGlobal['Velocidad del viento']));
      setText('direccionViento', norm(ultimoDatoGlobal['Direccion del viento']));
      setText('sensorLluvia',    norm(ultimoDatoGlobal['Sensor de lluvia']));
      setText('Altitud',         norm(ultimoDatoGlobal['Altitud']));

      // Fechas secundarias
      ['fecha2','fecha3','fecha4','fecha5', 'fecha6'].forEach(id =>
        setText(id, norm(ultimoDatoGlobal['Fecha']))
      );

      // ── Tabla de detalles ──────────────────────────────────────────────────────────
      const tbody = document.getElementById('tablaDetallesCuerpo');
      if (tbody) {
        tbody.innerHTML = '';
        arr.forEach(dato => {
          const tr = document.createElement('tr');
          tr.innerHTML = `
            <td>${norm(dato['Dato N°'])}</td>
            <td>${norm(dato['Fecha'])}</td>
            <td>${norm(dato['Temperatura Promedio'])} °C</td>
            <td>${norm(dato['Humedad'])} %</td>
            <td>${norm(dato['Presion'])} hPa</td>
            <td>${norm(dato['Temperatura punto de rocio'])} °C</td>
            <td>${norm(dato['Calidad del aire'])}</td>
          `;
          tbody.appendChild(tr);
        });
      } else {
        console.warn('❓ No existe #tablaDetallesCuerpo');
      }

      actualizarGrafico();
    } catch (err) {
      console.error('Error al obtener datos:', err);
    }
  }

  // ─── Actualizar gráfico según categoría ────────────────────────────────────────────────
  function actualizarGrafico() {
    const sel = categoriaDropdown.value;
    let key, color, title;
    if (sel === 'emetec') { key = 'Temperatura Promedio'; color = '79,126,255'; title = 'Temperatura promedio'; }
    else if (sel === 'test') { key = 'Presion'; color = '153,102,255'; title = 'Presión promedio'; }
    else if (sel === 'sj')   { key = 'Humedad'; color = '255,159,64'; title = 'Humedad promedio'; }
    else return;
    mostrarPopup('#popup-grafico', title, key, color);
  }

  // ─── Mostrar popup con Chart.js ────────────────────────────────────────────────────────
  function mostrarPopup(id, titulo, key, color) {
    $(id).on('click', () => {
      const labels = ultimos10DatosGlobal.map(d => d['Fecha']).reverse();
      const data   = ultimos10DatosGlobal.map(d => parseFloat(d[key]) || null).reverse();
      Swal.fire({
        title: titulo,
        width: 600,
        html: `
          <div style="display:flex;">
            <canvas id="grafico${titulo.replace(/\s/g,'')}" width="400" height="220"></canvas>
          </div>
          <p style="font-size:12px;color:gray;margin-top:10px;">
            Nota: datos faltantes pueden distorsionar el gráfico.
          </p>`,
        icon: 'info',
        confirmButtonText: 'Cerrar',
        didOpen: () => {
          const ctx = document
            .getElementById(`grafico${titulo.replace(/\s/g,'')}`)
            .getContext('2d');
          new Chart(ctx, {
            type: 'line',
            data: { labels, datasets: [{ label:`${titulo} (últimos 10)`, data, borderColor:`rgba(${color},1)`, backgroundColor:`rgba(${color},0.2)`, fill:true }] },
            options: { responsive:true, maintainAspectRatio:false,
              scales: { x:{ title:{ display:true, text:'Fecha' }}, y:{ title:{ display:true, text:titulo }} }
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

  // ─── Mapa interactivo con Leaflet ───────────────────────────────────────────────────────
  function invokemap() {
    Swal.fire({
      title: 'Mapa Interactivo',
      html: '<div id="map" style="height:300px;"></div>',
      width: 600,
      didOpen: () => {
        const map = L.map('map').setView([-32.969924, -68.844069], 12);
        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { maxZoom:19 }).addTo(map);
        [
          { lat:-32.951849, lng:-68.851420, popup:'EME: ETec' },
          { lat:-32.985219, lng:-68.897708, popup:'EMM: Colegio San Jorge' }
        ].forEach(loc => {
          const marker = L.marker([loc.lat, loc.lng]).addTo(map);
          const btnId  = `btn-${loc.popup.replace(/\s+/g,'')}`;
          const html   = `<h3>${loc.popup}</h3><button id="${btnId}" style="margin-top:5px;">Seleccionar nodo</button>`;
          marker.bindPopup(html);
          marker.on('popupopen', () => {
            setTimeout(() => {
              const btn = document.getElementById(btnId);
              btn?.addEventListener('click', () => {
                const sel = loc.popup.includes('ETec') ? 'emetec' : 'sj';
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
  window.invokemap = invokemap;

  // ─── Verificar estado del nodo EMEtec ──────────────────────────────────────────────────
  async function verificarEstadoNodo() {
    const estadoEl = document.getElementById('estadoNodo');
    if (!estadoEl) {
      console.warn('❓ No existe #estadoNodo. Omite verificarEstadoNodo.');
      return;
    }
    if (categoriaDropdown.value !== 'emetec') {
      estadoEl.style.display = 'none';
      return;
    }

    estadoEl.style.display = 'flex';
    estadoEl.className   = 'card-estado-nodo compact';
    estadoEl.innerHTML   = ''; // safe porque ya existe

    try {
      const res  = await fetch('https://emetec.wetec.um.edu.ar/emsdb');
      if (!res.ok) throw new Error('HTTP ' + res.status);
      const db   = await res.json();
      const nodo = db.find(e => e.Nombre.includes('ETec'));

      if (!nodo) {
        estadoEl.classList.add('unknown');
        estadoEl.innerHTML = `<i class="fa-solid fa-circle-question"></i><span>Estado desconocido</span>`;
      } else {
        const online = nodo.Estado === 'En Linea';
        estadoEl.classList.add(online ? 'online' : 'offline');
        estadoEl.innerHTML = online
          ? `<i class="fa-solid fa-circle-check"></i><span class="estado-texto">Nodo en línea</span>`
          : `<i class="fa-solid fa-circle-xmark"></i><span class="estado-texto">Nodo fuera de línea</span>`;
      }
    } catch (err) {
      console.error('Error al verificar estado:', err);
      estadoEl.classList.add('unknown');
      estadoEl.innerHTML = `<i class="fa-solid fa-triangle-exclamation"></i><span>No se pudo obtener estado</span>`;
    }
  }

});
