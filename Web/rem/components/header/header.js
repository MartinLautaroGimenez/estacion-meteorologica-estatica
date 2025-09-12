"use client"; //Para renderizar en el lado del cliente
import { useEffect, useState } from "react";
import "./header.css";

export default function Header() {
  const [theme, setTheme] = useState("light");
  const [scrolled, setScrolled] = useState(false);

  // Al cargar, recupera el tema guardado en localStorage
  useEffect(() => {
    const savedTheme = localStorage.getItem("theme") || "light";
    setTheme(savedTheme);
    document.body.classList.toggle("dark-theme", savedTheme === "dark");
  }, []);
  // Función para alternar tema
  const toggleTheme = () => {
    const newTheme = theme === "light" ? "dark" : "light";
    setTheme(newTheme);
    document.body.classList.toggle("dark-theme", newTheme === "dark");
    localStorage.setItem("theme", newTheme);
  };
  // Efecto para manejar el scroll y cambiar la clase del header
  useEffect(() => {
    const handleScroll = () => {
      setScrolled(window.scrollY > 20);
    }

    window.addEventListener("scroll", handleScroll);
    return () => {
      window.removeEventListener("scroll", handleScroll);
    }
  })

  return (
    <>
      <header className={scrolled ? "scrolled" : ""}>
        <div className="logo">
          <img src="https://i.ibb.co/4NyW2qt/logo.png" alt="" />
          <h2 className="etech2">
            Estaciones Meteorológicas ETec UM
            <span className="danger"></span>
          </h2>
        </div>

        <div className="navbar">
          <a href="/">
            <span className="material-symbols-sharp">home</span>
            <h3>Inicio</h3>
          </a>
          <a href="/datos">
            <span className="material-symbols-sharp">
              format_list_bulleted
            </span>
            <h3>Datos</h3>
          </a>
          <a href="/about_us">
            <span className="material-symbols-sharp">info</span>
            <h3>Sobre nosotros</h3>
          </a>
        </div>

        {/* Botón theme toggler */}
        <div className="theme-toggler" onClick={toggleTheme}>
          <span
            className={`material-symbols-sharp ${
              theme === "light" ? "active" : ""
            }`}
          >
            light_mode
          </span>
          <span
            className={`material-symbols-sharp ${
              theme === "dark" ? "active" : ""
            }`}
          >
            dark_mode
          </span>
        </div>
      </header>
    </>
  );
}
