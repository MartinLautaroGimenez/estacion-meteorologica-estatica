"use client";

import { useEffect, useState } from "react";
import { ImageUploader } from "@/components";
import "./events.css" // css de la lista

export default function EventsDashboard() {
    const [events, setEvents] = useState([]);
    const [file, setFile] = useState(null);
    const [form, setForm] = useState({ title: "", date: "", description: "" });

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

        let imageUrl = null;
        if (file) {
            const formData = new FormData();
            formData.append("file", file);

            const uploadRes = await fetch("/api/upload", {
                method: "POST",
                body: formData,
            });

            const uploadData = await uploadRes.json();
            imageUrl = uploadData.url;
        } else {
            alert("Por favor selecciona una imagen antes de enviar")
            return
        }

        const res = await fetch("/api/events", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                title: form.title,
                date: form.date,
                description: form.description,
                image: imageUrl,
            }),
        });

        if (res.ok) {
            setForm({ title: "", date: "", description: "", image: null });
            setFile(null);
            fetchEvents();
        }
    }

    async function deleteEvent(id) {
        if (!confirm("¿Seguro que querés borrar este evento?")) return;
        const res = await fetch(`/api/events/${id}`, { method: "DELETE" });
        if (res.ok) fetchEvents();
    }

    return (
        <div className="dashboard-card">
            <h1>Dashboard de Eventos</h1>

            <form onSubmit={handleSubmit} className="dashboard-form">
                <input
                    type="text"
                    placeholder="Título"
                    value={form.title}
                    onChange={(e) => setForm({ ...form, title: e.target.value })}
                    required
                />
                <input
                    type="date"
                    value={form.date}
                    onChange={(e) => setForm({ ...form, date: e.target.value })}
                    required
                />
                <textarea
                    placeholder="Descripción"
                    value={form.description}
                    onChange={(e) => setForm({ ...form, description: e.target.value })}
                    required
                />
                <ImageUploader key={form.image || "empty"} onFileSelect={setFile} />
                <button type="submit">Agregar evento</button>
            </form>

            <ul className="dashboard-list-events">
                {events.map((ev) => (
                    <li key={ev.id} className="dashboard-event-card">
                        {ev.image && (
                            <img
                                src={ev.image}
                                alt={ev.title}
                                className="dashboard-event-thumb"
                            />
                        )}
                        <div className="dashboard-event-info">
                            <h3>{ev.title}</h3>
                            <p>{ev.date}</p>
                            <p>{ev.description}</p>
                        </div>
                        <button className="btn-delete" onClick={() => deleteEvent(ev.id)}>
                            <span className="material-symbols-sharp">delete</span>
                        </button>
                    </li>
                ))}
            </ul>
        </div>
    );
}
