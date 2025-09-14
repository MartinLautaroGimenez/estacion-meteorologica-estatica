"use client";

import "./dashboard.css";
import Link from "next/link";
import { usePathname } from "next/navigation";
import { SessionProvider } from "next-auth/react";

export default function DashboardLayout({ children }) {
    const pathname = usePathname();

    return (
        <SessionProvider>
            <div className="content dashboard-container">
                {/* Navbar lateral */}
                <aside className="dashboard-navbar">
                    <h2>Dashboard</h2>
                    <nav>
                        <ul>
                            <li>
                                <Link
                                    href="/dashboard"
                                    className={pathname === "/dashboard" ? "active" : ""}
                                >
                                    <span className="material-symbols-sharp">dashboard</span>
                                    Inicio
                                </Link>
                            </li>
                            <li>
                                <Link
                                    href="/dashboard/users"
                                    className={pathname.includes("/dashboard/users") ? "active" : ""}
                                >
                                    <span className="material-symbols-sharp">group</span>
                                    Usuarios
                                </Link>
                            </li>
                            <li>
                                <Link
                                    href="/dashboard/events"
                                    className={pathname.includes("/dashboard/events") ? "active" : ""}
                                >
                                    <span className="material-symbols-sharp">event</span>
                                    Eventos
                                </Link>
                            </li>
                            <li>
                                <Link
                                    href="/dashboard/participants"
                                    className={pathname.includes("/dashboard/participants") ? "active" : ""}
                                >
                                    <span className="material-symbols-sharp">person</span>
                                    Participantes
                                </Link>
                            </li>
                        </ul>
                    </nav>
                </aside>

                {/* Contenido principal */}
                <div className="dashboard-main">{children}</div>
            </div>
        </SessionProvider>
    );
}
