"use client"
import { useEffect } from "react"
import { NODES } from "@/models"
import "./mapModal.css"

export const MapModal = ({ isOpen, onClose }) => {
    useEffect(() => {
        if (!isOpen) return

        const loadLeaflet = async () => {
            if (typeof window === "undefined") return

            // Cargar CSS de Leaflet
            if (!document.querySelector('link[href*="leaflet.css"]')) {
                const link = document.createElement("link")
                link.rel = "stylesheet"
                link.href = "https://unpkg.com/leaflet@1.9.4/dist/leaflet.css"
                document.head.appendChild(link)
            }

            // Cargar JS de Leaflet
            if (!window.L) {
                const script = document.createElement("script")
                script.src = "https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"
                script.onload = () => initMap()
                document.head.appendChild(script)
            } else {
                initMap()
            }
        }

        const initMap = () => {
            const L = window.L
            if (!L) return

            // Limpiar mapa existente
            const mapContainer = document.getElementById("nodes-map")
            if (!mapContainer) return

            mapContainer.innerHTML = ""

            // Calcular centro del mapa basado en todos los nodos
            const nodes = Object.values(NODES)
            const avgLat = nodes.reduce((sum, node) => sum + node[2], 0) / nodes.length
            const avgLng = nodes.reduce((sum, node) => sum + node[3], 0) / nodes.length

            // Crear mapa
            const map = L.map("nodes-map").setView([avgLat, avgLng], 12)

            // Agregar tiles de OpenStreetMap
            L.tileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png", {
                attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors',
            }).addTo(map)

            // Agregar marcadores para cada nodo
            Object.entries(NODES).forEach(([key, [name, mapUrl, lat, lng]]) => {
                const marker = L.marker([lat, lng]).addTo(map)
                marker.bindPopup(`
                    <div style="text-align: center;">
                        <strong>${name}</strong><br/>
                        <a href="${mapUrl}" target="_blank" rel="noopener noreferrer" style="color: #2563eb; text-decoration: none;">
                        Ver en Google Maps →
                        </a>
                    </div>
                `)
            })
        }

        loadLeaflet()
    }, [isOpen])

    if (!isOpen) return null

    return (
        <div className="modal-overlay" onClick={onClose}>
            <div className="modal-content" onClick={(e) => e.stopPropagation()}>
                <div className="modal-header">
                    <h2>Ubicación de Nodos Meteorológicos</h2>
                    <button className="modal-close" onClick={onClose}>
                        <span className="material-symbols-sharp">close</span>
                    </button>
                </div>
                <div className="modal-body">
                    <div id="nodes-map" className="single-map-container"></div>
                </div>
            </div>
        </div>
    )
}
