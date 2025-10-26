// prisma/seed.js
import { PrismaClient } from '@prisma/client';

const globalForPrisma = globalThis;

export const prisma =
    globalForPrisma.prisma ||
    new PrismaClient({
        log: ["query", "error"],
    });

async function main() {
    // Lista de participantes
    const participantes = [
        {
            nombre: "Mario Papetti",
            descripcion:
                "Tutor técnico en electrónica e informática, con años de experiencia orientando proyectos estudiantiles en la implementación de nuevas tecnologías. Su capacidad para unir teoría y práctica garantiza soluciones innovadoras y viables. Gracias a su enfoque cercano y didáctico, impulsa a cada integrante a dar lo mejor de sí, transformando ideas en resultados palpables.",
            imagen: "uploads/mario.jpg",
        },
        {
            nombre: "Luis Cuesta",
            descripcion:
                "Backend Developer con un don especial para la lógica y la arquitectura de software. Su dominio de servidores mantiene las estaciones siempre conectadas y optimizadas. Aporta estabilidad y escalabilidad al proyecto con su rigurosidad técnica y entusiasmo por las buenas prácticas.",
            imagen: "uploads/Luis.jpg",
        },
        {
            nombre: "Santiago Juarez",
            descripcion:
                "Responsable del diseño y la estética de nuestras estaciones meteorológicas. Su enfoque en la usabilidad y la experiencia del usuario asegura que cada estación no solo sea funcional, sino también atractiva y fácil de usar.",
            imagen: "uploads/santiago.jpg",
        },
        {
            nombre: "Martín Gimenez",
            descripcion:
                "Full Stack que integra a la perfección hardware y software. Sus habilidades abarcan desde la configuración de microcontroladores hasta la creación de aplicaciones web (como la presente). Su pasión por la innovación empuja constantemente los límites del rendimiento y la experiencia de usuario.",
            imagen: "uploads/martin.jpg",
        },
        {
            nombre: "Augusto Massut",
            descripcion:
                "Estudiante de sexto año de electrónica, enfocado en sensado ambiental y soluciones eco-tecnológicas. Su curiosidad innata y espíritu inquieto lo llevan a investigar y probar componentes para lograr mediciones cada vez más precisas.",
            imagen: "uploads/augusto.jpg",
        },
        {
            nombre: "Sofia Rojas",
            descripcion:
                "Estudiante de quinto año de electrónica con gran talento para el montaje y la detección de fallas.",
            imagen: "uploads/sofia.jpg",
        },
        {
            nombre: "Ivo Bartolomeo",
            descripcion:
                "Estudiante de sexto año de electrónica, experto en soldadura, calibración y mantenimiento de circuitos. Su paciencia y precisión técnica aseguran un funcionamiento impecable en cada etapa.",
            imagen: "uploads/ivo.jpg",
        },
    ];

    // Insertar los registros en la base de datos
    for (const p of participantes) {
        await prisma.participantes.create({ data: p });
    }

    console.log("✅ Participantes agregados correctamente.");
}

main()
    .catch((e) => {
        console.error("❌ Error al ejecutar el seed:", e);
    })
    .finally(async () => {
        await prisma.$disconnect();
    });
