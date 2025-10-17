"use client";

import { useState } from "react";
import { signIn } from "next-auth/react";
import { useRouter } from "next/navigation";
import "./login.css";

export default function LoginPage() {
    const [email, setEmail] = useState("");
    const [password, setPassword] = useState("");
    const [error, setError] = useState(null);
    const router = useRouter();

    async function handleSubmit(e) {
        e.preventDefault();
        setError(null);

        const res = await signIn("credentials", {
            email,
            password,
            redirect: false,
        });
        console.log("SignIn response:", res);

        if (res?.error) {
            setError("Credenciales inválidas");
        } else {
            router.push("/dashboard");
        }
    }

    return (
        <div className="login-container">
            <div className="login-card">
                <h1 className="login-title">Iniciar sesión</h1>

                <form onSubmit={handleSubmit} className="login-form">
                    <div>
                        <label className="block text-sm font-medium text-gray-700">
                            Email
                        </label>
                        <input
                            type="email"
                            value={email}
                            className="login-input"
                            onChange={(e) => setEmail(e.target.value)}
                            required
                        />
                    </div>

                    <div>
                        <label className="block text-sm font-medium text-gray-700">
                            Contraseña
                        </label>
                        <input
                            type="password"
                            value={password}
                            className="login-input"
                            onChange={(e) => setPassword(e.target.value)}
                            required
                        />
                    </div>

                    <button type="submit" className="login-button">
                        <span className="material-symbols-sharp">login</span>
                        Entrar
                    </button>
                </form>

                {error && <p className="login-error">{error}</p>}
            </div>
        </div>
    );
}
