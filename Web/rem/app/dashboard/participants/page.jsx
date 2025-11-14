"use client";

import { useEffect, useState } from "react";
import { ImageUploader } from "@/components";
import "./participants.css" // css de la lista

export default function ParticipantsDashboard() {
    const [participants, setParticipants] = useState([]);
    const [file, setFile] = useState(null);
    const [form, setForm] = useState({
        nombre: "",
        descripcion: "",
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

        const res = await fetch("/api/participants", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                nombre: form.nombre,
                descripcion: form.descripcion,
                imagen: imagenUrl,
            }),
        });

        if (res.ok) {
            setForm({ nombre: "", descripcion: "" });
            setFile(null);
            fetchParticipants();
        }
    }


    async function deleteParticipant(id, imagen) {
        console.log("Imagen a borrar:", imagen);
        if (!confirm("¿Seguro que querés borrar este participante?")) return;
        await fetch(`/api/upload?filename=${encodeURIComponent(imagen)}`, {
            method: "DELETE",
        });

        const res = await fetch(`/api/participants/${id}`, { method: "DELETE" });
        if (res.ok) fetchParticipants();
    }

    return (
        <div className="dashboard-card">
            <h1>Participantes</h1>

            <form onSubmit={handleSubmit} className="dashboard-form">
                <input
                    type="text"
                    placeholder="Nombre"
                    value={form.nombre}
                    onChange={(e) => setForm({ ...form, nombre: e.target.value })}
                    required
                />
                <textarea
                    placeholder="Descripción"
                    value={form.descripcion}
                    onChange={(e) => setForm({ ...form, descripcion: e.target.value })}
                />
                <ImageUploader key={form.image || "empty"} onFileSelect={setFile} required/>
                <button type="submit">Agregar participante</button>
            </form>

            <ul className="dashboard-list">
                {participants.map((p) => (
                    <li key={p.idparticipantes} className="dashboard-item">
                        <div className="dashboard-item-info">
                            {p.image && (
                                <img
                                    src={p.image}
                                    alt={p.nombre}
                                    className="dashboard-thumb"
                                />
                            )}
                            <span>
                                <strong>{p.nombre}</strong> — {p.descripcion}
                            </span>
                        </div>
                        <button className="btn-delete" onClick={() => deleteParticipant(p.idparticipantes, p.imagen)}>
                            <span className="material-symbols-sharp">delete</span>
                            Eliminar
                        </button>
                    </li>
                ))}
            </ul>
        </div>
    );
}
