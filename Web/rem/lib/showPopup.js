import Swal from "sweetalert2";
import Chart from "chart.js/auto";

export function showPopup(titulo, key, color, data) {
    const labels = data?.map(d => d['Fecha']).reverse();
    const values = data?.map(d => parseFloat(d[key]) || null).reverse();
    Swal.fire({
        title: titulo,
        width: 600,
        html: `
        <div style="display:flex; flex-direction:row; align-items:flex-start;">
            <div style="flex:2; padding-right:20px;">
                <canvas id="grafico${titulo.replace(/\s/g, '')}" width="400" height="220"></canvas>
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
                .getElementById(`grafico${titulo.replace(/\s/g, '')}`)
                .getContext('2d');
            new Chart(ctx, {
                type: 'line',
                data: { labels, datasets: [{ label: `${titulo} (últimos 10)`, data: values, borderColor: `rgba(${color},1)`, backgroundColor: `rgba(${color},0.2)`, fill: true }] },
                options: {
                    responsive: true, maintainAspectRatio: false,
                    scales: { x: { title: { display: true, text: 'Fecha' } }, y: { title: { display: true, text: titulo } } }
                }
            });
        }
    });
};