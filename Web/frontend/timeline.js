
  document.addEventListener("DOMContentLoaded", function () {
    var timeline_json = {
      title: {
        text: {
          headline: "Nuestra Historia",
          text: "El recorrido de nuestro equipo en el proyecto de la Estación Meteorológica",
        },
        media: {
          url: "frontend/images/emecompleta.jpeg",
        },
      },
      events: [
        {
          start_date: { year: "2022" },
          text: {
            headline: "Todo comenzó con Nicolás Perez",
            text: "En 2022, el alumno Nicolás Pérez dio el primer paso iniciando el proyecto de la Estación Meteorológica Estática (EME).",
          },
          media: { url: "frontend/images/emebasica.jpeg" },
        },
        {
          start_date: { year: "2022", month: "4" },
          text: {
            headline: "Primeros pasos",
            text: "Nicolás Pérez presentó el proyecto EME en el CATE 2023, mostrando las primeras ideas y prototipos. La aceptación inicial abrió nuevas oportunidades para explorar y expandir el potencial del proyecto, impulsándolo hacia nuevos horizontes.",
          },
          media: { url: "frontend/images/cate2023.jpeg" },
        },
        {
          start_date: { year: "2023", month: "9" },
          text: {
            headline: "Cambio de manos",
            text: "En septiembre de 2023, al egresar Nicolás, el proyecto fue heredado por un nuevo estudiante asegurar su continuidad.",
          },
          media: { url: "frontend/images/emebasica.jpeg" },
        },
        {
          start_date: { year: "2024", month: "3" },
          text: {
            headline: "¡Volvemos al ruedo!",
            text: "En marzo de 2024 retomamos el proyecto fuera del taller Aeroglobe. La EME volvió a cobrar vida y fuerza.",
          },
          media: { url: "frontend/images/heredar.jpg" },
        },
        {
          start_date: { year: "2024", month: "8" },
          text: {
            headline: "Nos fuimos al SASE",
            text: "En agosto participamos del Simposio Argentino de Sistemas Embebidos (SASE). La estación volvió a integrar el taller Aeroglobe ETec.",
          },
          media: { url: "frontend/images/tucuman.jpg" },
        },
        {
          start_date: { year: "2024", month: "9" },
          text: {
            headline: "¡Se suma San Jorge!",
            text: "El Colegio San Jorge de Chacras se interesó en nuestro proyecto y decidió sumarse a la red de estaciones meteorológicas con un nuevo nodo.",
          },
          media: { url: "frontend/images/sj.jpg" },
        },
        {
          start_date: { year: "2024", month: "10" },
          text: {
            headline: "Eclipse + Ciencia = 💥",
            text: "Nuestra estación (EMEtec) fue clave para detectar cambios en las condiciones climáticas durante el eclipse del 2 de octubre.",
          },
          media: { url: "frontend/images/eclipse.jpg" },
        },
        {
          start_date: { year: "2024", month: "11" },
          text: {
            headline: "¡Feria de Ciencias!",
            text: "Nuestro equipo viajó a la Feria de Ciencias Pierre Auger a compartir lo que venimos construyendo juntos.",
          },
          media: { url: "frontend/images/PIERRE.png" },
        },
        {
          start_date: { year: "2024", month: "12" },
          text: {
            headline: "Modo vacaciones ON",
            text: "Con la llegada del verano, dejamos lista la estación para que funcione durante el receso sin problemas.",
          },
          media: { url: "frontend/images/eme.jpg" },
        },
        {
          start_date: { year: "2025", month: "3" },
          text: {
            headline: "¡Volvimos con todo!",
            text: "En marzo de 2025 retomamos con energía. Estamos terminando de dejar operativo el nodo del Colegio San Jorge.",
          },
          media: { url: "frontend/images/2025.png" },
        },
      ],
    };
    window.timeline = new TL.Timeline("timeline-embed", timeline_json);
  });
