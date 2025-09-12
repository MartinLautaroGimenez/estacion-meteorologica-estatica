"use client";

import "../globals.css";
import "./aboutUs.css";

import { useEffect } from "react";
import "aos/dist/aos.css";
import AOS from "aos";
import "swiper/css";
import Swiper from "swiper";
import { PopupImage, Timeline } from "@/components";
import { MEMBERS_CONFIG } from "@/config"

export default function AboutUs() {
    useEffect(() => {
        // Inicializar AOS
        AOS.init();

        // Inicializar Swiper
        new Swiper(".swiper-container", {});
    }, []);

    return (
        <>
            {/* Acerca de Nosotros */}
            <section id="acerca" data-aos="fade-up">
                <div className="acerca-container">
                    <div className="acerca-text">
                        <h2>Acerca de Nosotros</h2>
                        <p>
                            En EMETec somos un equipo interdisciplinario conformado por estudiantes y docentes de diversas especialidades técnicas, comprometidos con la innovación tecnológica y la aplicación práctica del conocimiento. Nuestra metodología de trabajo está basada en el desarrollo colaborativo y la experimentación activa en un entorno de taller.
                        </p>
                        <p>
                            Integramos conocimientos de electrónica, informática y diseño para ofrecer soluciones técnicas eficientes y creativas. Apostamos por una formación práctica orientada a proyectos reales, lo que nos impulsa a seguir aprendiendo, investigando y creciendo día a día.
                        </p>
                    </div>
                    <div className="acerca-image">
                        <img
                            src="/images/tucuman.jpg"
                            alt="Foto Acerca de Nosotros"
                        />
                    </div>
                </div>
            </section>

            {/* Nuestra Historia */}
            <section id="historia" data-aos="fade-up">
                <h2>Nuestra Historia</h2>
                <div className="timeline-container">
                    <Timeline/>
                </div>
            </section>

            {/* Misión & Visión */}
            <section id="mision-vision" data-aos="fade-up">
                <h2>Misión &amp; Visión</h2>
                <div className="content">
                    <div className="box">
                        <img
                            src="/images/tucumanbalcon.jpeg"
                            alt="Innovación"
                        />
                        <p>
                            <strong>Misión:</strong> Integrar activamente a estudiantes de distintos niveles académicos y especialidades técnicas, promoviendo el Aprendizaje Basado en Proyectos (ABP). Generamos un espacio colaborativo de formación práctica donde todos cumplen un rol específico, independiente de sus conocimientos previos.
                        </p>
                    </div>
                    <div className="box">
                        <img
                            src="/images/eme.jpeg"
                            alt="Sostenibilidad"
                        />
                        <p>
                            <strong>Visión:</strong> Ser un equipo referente dentro de la Escuela Técnica de la Universidad de Mendoza en la implementación del Aprendizaje Basado en Proyectos, destacando por la calidad técnica, la innovación y la inclusión de estudiantes en procesos reales de investigación, desarrollo tecnológico y resolución creativa de problemas.
                        </p>
                    </div>
                </div>
            </section>

            {/* Nuestro Equipo */}
            <section id="equipo">
                <h2>Nuestro Equipo</h2>
                <p>
                    Conocé a las mentes brillantes detrás de este proyecto. Cada integrante aporta experiencia, creatividad y entusiasmo, construyendo juntos una red de estaciones meteorológicas.
                </p>
                <div className="team-container">
                    {MEMBERS_CONFIG.map((member, index) => (
                        <div className="team-member" key={index}>
                            <PopupImage
                                image={member.img}
                                title={member.name}
                                alt={member.name}
                                description={member.description}
                            />
                        </div>
                    ))}
                </div>
            </section>

            {/* Contacto */}
            <section id="github-cta" data-aos="fade-up">
                <div className="content">
                    <h2>¿Querés una estación meteorológica?</h2>
                    <p>
                        Nuestro proyecto es completamente Open Source, lo que significa que cualquiera puede replicarlo desde la comodidad de su hogar, laboratorio o institución.  Sin embargo, si deseás que nosotros recreemos una para vos, nos podes contactar en la siguiente sección.
                    </p>
                    <a href="https://github.com/MartinLautaroGimenez/estacion-meteorologica-estatica" target="_blank">Sumate al GitHub</a>
                </div>
            </section>
            {/* Contacto (Con tarjetas e íconos + Google Maps) */}
            <section id="contacto">
                <div className="contacto-content">
                    <h2>Contacto</h2>
                    <p>
                        ¿Tenés dudas o querés que nosotros armemos tu estación? A continuación te dejamos
                        nuestros correos, teléfono y un mapa de la ubicación de la Escuela Técnica de la UM.
                    </p>
                    {/* Tarjetas de contacto */}
                    <div className="contact-cards">
                        {/* Tarjeta 1 */}
                        <div className="contact-card">
                            <span className="material-symbols-sharp">mail</span>
                            <div className="contact-info">
                                <h3>Correo Luis Cuesta</h3>
                                <a href="mailto:luis.cuesta@etec.um.edu.ar">luis.cuesta@etec.um.edu.ar</a>
                            </div>
                        </div>
                        {/* Tarjeta 2 */}
                        <div className="contact-card">
                            <span className="material-symbols-sharp">mail</span>
                            <div className="contact-info">
                                <h3>Correo Mario Papetti</h3>
                                <a href="mailto:mario.papetti@etec.um.edu.ar">mario.papetti@etec.um.edu.ar</a>
                            </div>
                        </div>
                        {/* Tarjeta 3 */}
                        <div className="contact-card">
                            <span className="material-symbols-sharp">mail</span>
                            <div className="contact-info">
                                <h3>Correo Martín Gimenez</h3>
                                <a href="mailto:mal.gimenez@alumno.etec.um.edu.ar">mal.gimenez@alumno.etec
                                    .um.edu.ar</a>
                            </div>
                        </div>
                        {/* Teléfono */}
                        <div className="contact-card">
                            <span className="material-symbols-sharp">call</span>
                            <div className="contact-info">
                                <h3>Teléfono</h3>
                                <p>+54 9 261 4392939</p>
                            </div>
                        </div>
                    </div>
                    {/* Contenedor del mapa (Google Maps API) */}
                    <iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d3347.9572308101096!2d-68.85449502525934!3d-32.9521385721644!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x967e0b067323d179%3A0x2f6e98ec0645ef99!2sEscuela%20T%C3%A9cnica%20de%20la%20Universidad%20de%20Mendoza!5e0!3m2!1ses-419!2sar!4v1744426154494!5m2!1ses-419!2sar" width="100%" height={450} style={{ border: 0, borderRadius: '20px' }} allowFullScreen loading="lazy" referrerPolicy="no-referrer-when-downgrade">
                    </iframe>
                </div>
            </section>
        </>
    );
}
