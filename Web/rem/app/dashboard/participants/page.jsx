"use client";

import { useEffect, useState } from "react";
import { ImageUploader } from "@/components";
import "./participants.css" // css de la lista

export default function ParticipantsDashboard() {
    const [participants, setParticipants] = useState([]);
    const [file, setFile] = useState(null);
    const [form, setForm] = useState({
        name: "",
        description: "",
    });

    useEffect(() => {
        fetchParticipants();
    }, []);

    async function fetchParticipants() {
        const res = await fetch("/api/participants");
        const data = await res.json();
        setParticipants(data);
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

        const res = await fetch("/api/participants", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                name: form.name,
                description: form.description,
                image: imageUrl,
            }),
        });

        if (res.ok) {
            setForm({ name: "", description: "" });
            setFile(null);
            fetchParticipants();
        }
    }


    async function deleteParticipant(id) {
        if (!confirm("¿Seguro que querés borrar este participante?")) return;
        const res = await fetch(`/api/participants/${id}`, { method: "DELETE" });
        if (res.ok) fetchParticipants();
    }

    return (
        <div className="dashboard-card">
            <h1>Dashboard de Participantes</h1>

            <form onSubmit={handleSubmit} className="dashboard-form">
                <input
                    type="text"
                    placeholder="Nombre"
                    value={form.name}
                    onChange={(e) => setForm({ ...form, name: e.target.value })}
                    required
                />
                <textarea
                    placeholder="Descripción"
                    value={form.description}
                    onChange={(e) => setForm({ ...form, description: e.target.value })}
                />
                <ImageUploader key={form.image || "empty"} onFileSelect={setFile} required/>
                <button type="submit">Agregar participante</button>
            </form>

            <ul className="dashboard-list">
                {participants.map((p) => (
                    <li key={p.id} className="dashboard-item">
                        <div className="dashboard-item-info">
                            {p.image && (
                                <img
                                    src={p.image}
                                    alt={p.name}
                                    className="dashboard-thumb"
                                />
                            )}
                            <span>
                                <strong>{p.name}</strong> — {p.description}
                            </span>
                        </div>
                        <button className="btn-delete" onClick={() => deleteParticipant(p.id)}>
                            <span className="material-symbols-sharp">delete</span>
                            Eliminar
                        </button>
                    </li>
                ))}
            </ul>
        </div>
    );
}
