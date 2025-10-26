"use client";

import { useEffect, useState } from "react";
import { useSession } from "next-auth/react";

export default function UsersDashboard() {
    const { data: session, status } = useSession();
    const isAdmin = session?.user?.role === "admin";
    const [users, setUsers] = useState([]);
    const [form, setForm] = useState({
        email: "",
        role: "editor",
        clave: "",
    });

    useEffect(() => {
        if (isAdmin) fetchUsers();
    }, [isAdmin]);

    async function fetchUsers() {
        const res = await fetch("/api/users");
        const data = await res.json();
        setUsers(data);
        console.log("fetched users", data);
    }

    async function handleSubmit(e) {
        e.preventDefault();
        const res = await fetch("/api/users", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(form),
        });
        if (res.ok) {
            setForm({ email: "", role: "editor", clave: "" });
            fetchUsers();
        }
    }

    async function deleteUser(id) {
        if (!confirm("¿Seguro que querés borrar este usuario?")) return;
        const res = await fetch(`/api/users/${id}`, { method: "DELETE" });
        if (res.ok) fetchUsers();
    }

    if (status === "loading") {
        return <p>Cargando...</p>;
    }

    if (!isAdmin) {
        return (
            <div className="dashboard-card">
                <h1>No autorizado</h1>
                <p>No tenés permisos para acceder a esta sección.</p>
            </div>
        );
    }

    return (
        <div className="dashboard-card">
            <h1>Usuarios</h1>
            <form onSubmit={handleSubmit} className="dashboard-form">
                <input
                    type="email"
                    placeholder="Email"
                    value={form.email}
                    onChange={(e) => setForm({ ...form, email: e.target.value })}
                    required
                />
                <select
                    value={form.role}
                    onChange={(e) => setForm({ ...form, role: e.target.value })}
                >
                    <option value="admin">Admin</option>
                    <option value="editor">Editor</option>
                </select>
                <input
                    type="password"
                    placeholder="Contraseña"
                    value={form.clave}
                    onChange={(e) => setForm({ ...form, clave: e.target.value })}
                    required
                />
                <button type="submit">Agregar usuario</button>
            </form>

            <ul className="dashboard-list">
                {users.map((u) => (
                    <li key={u.idwebadmins}>
                        <span>
                            <strong>{u.email}</strong> ({u.role})
                        </span>
                        <button className="btn-delete" onClick={() => deleteUser(u.idwebadmins)}>
                            <span className="material-symbols-sharp">delete</span>
                            Eliminar
                        </button>
                    </li>
                ))}
            </ul>
        </div>
    );
}
