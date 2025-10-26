"use client";

import { useEffect, useState } from "react";
import { ImageUploader } from "@/components";
import "./events.css"

export default function EventsDashboard() {
    const [events, setEvents] = useState([]);
    const [file, setFile] = useState(null);
    const [form, setForm] = useState({ titulo: "", fecha: "", descripcion: "" });

    useEffect(() => {
        fetchEvents();
    }, []);

    async function fetchEvents() {
        const res = await fetch("/api/events");
        const data = await res.json();
        setEvents(data);
    }

    async function handleSubmit(e) {
        e.preventDefault();

        let imagenUrl = null;
        if (file) {
            const formData = new FormData();
            formData.append("file", file);

            const uploadRes = await fetch("/api/upload", {
                method: "POST",
                body: formData,
            });

            const uploadData = await uploadRes.json();
            imagenUrl = uploadData.url;
        } else {
            alert("Por favor selecciona una imagen antes de enviar")
            return
        }

        const res = await fetch("/api/events", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                titulo: form.titulo,
                fecha: new Date(form.fecha),
                descripcion: form.descripcion,
                imagen: imagenUrl,
            }),
        });

        if (res.ok) {
            setForm({ titulo: "", fecha: "", descripcion: "", imagen: null });
            setFile(null);
            fetchEvents();
        }
    }

    async function deleteEvent(id, imagen) {
        if (!confirm("¿Seguro que querés borrar este evento?")) return;
        await fetch(`/api/upload?filename=${encodeURIComponent(imagen)}`, {
            method: "DELETE",
        });

        const res = await fetch(`/api/events/${id}`, { method: "DELETE" });
        if (res.ok) fetchEvents();
    }

    return (
        <div className="dashboard-card">
            <h1>Eventos</h1>

            <form onSubmit={handleSubmit} className="dashboard-form">
                <input
                    type="text"
                    placeholder="Título"
                    value={form.titulo}
                    onChange={(e) => setForm({ ...form, titulo: e.target.value })}
                    required
                />
                <input
                    type="date"
                    value={form.fecha}
                    onChange={(e) => setForm({ ...form, fecha: e.target.value })}
                    required
                />
                <textarea
                    placeholder="Descripción"
                    value={form.descripcion}
                    onChange={(e) => setForm({ ...form, descripcion: e.target.value })}
                    required
                />
                <ImageUploader key={form.image || "empty"} onFileSelect={setFile} />
                <button type="submit">Agregar evento</button>
            </form>

            <ul className="dashboard-list-events">
                {events.map((ev) => (
                    <li key={ev.ideventos} className="dashboard-event-card">
                        {ev.imagen && (
                            <img
                                src={ev.imagen}
                                alt={ev.titulo}
                                className="dashboard-event-thumb"
                            />
                        )}
                        <div className="dashboard-event-info">
                            <h3>{ev.titulo}</h3>
                            <p>{ev.fecha}</p>
                            <p>{ev.descripcion}</p>
                        </div>
                        <button className="btn-delete" onClick={() => deleteEvent(ev.ideventos, ev.imagen)}>
                            <span className="material-symbols-sharp">delete</span>
                        </button>
                    </li>
                ))}
            </ul>
        </div>
    );
}
