// Configuración de las tarjetas que se muestran en el resumen de datos, en la página inicial
export const SUMMARY_CARDS_CONFIG = [
    { icon: "device_thermostat", title: "Temperatura", dataKey: "Temperatura Promedio", className: "qw", unit: "°C" },
    { icon: "water_drop", title: "Humedad Relativa", dataKey: "Humedad", className: "eg", unit: "%" },
    { icon: "cloud", title: "Presión Atmosférica", dataKey: "Presion", className: "cs", unit: "hPa" },
    { icon: "dew_point", title: "Punto de rocío", dataKey: "Temperatura punto de rocio", className: "cs", unit: "°C" },
    { icon: "altitude", title: "Altitud", dataKey: "Altitud", className: "cs", unit: "m" },
    { icon: "air", title: "Calidad del aire", dataKey: "Calidad del aire", className: "eg", unit: '' },
];