"use client"

import { NODES } from "@/models"
import { descargarDatosEnXLSX } from "@/lib"
import { setCookie } from "@/lib"
import { useState } from "react"
import { MapModal } from "../mapModal/mapModal.jsx"
import "./categoryDropdown.css"

export const CategoryDropdown = ({ set, estadoNodo }) => {
    const [isMapOpen, setIsMapOpen] = useState(false)

    const handleChange = (e) => {
        const selectedValue = e.target.value
        set(selectedValue)
        setCookie("categoriaDropdown", selectedValue, 30)
    }

    return (
        <>
            <div className="zarazap">
                <h1>Datos</h1>
                <h3>Seleccione el nodo meteorológico: </h3>
                <div style={{ display: "flex", gap: "8px", alignItems: "center" }}>
                    <select
                        id="categoriaDropdown"
                        className="dropdown"
                        style={{ marginRight: "0px", flex: 1 }}
                        defaultValue=""
                        onChange={handleChange}
                    >
                        <option value="" disabled>
                            Seleccione nodo meteorológico
                        </option>
                        {Object.keys(NODES).map((key) => (
                            <option value={key} key={key}>
                                {NODES[key][0]}
                            </option>
                        ))}
                    </select>
                    <button onClick={() => setIsMapOpen(true)} className="map-button" title="Ver ubicaciones en el mapa">
                        <span className="material-symbols-sharp">map</span>
                    </button>
                </div>
                <div className="cs" style={{ cursor: "pointer" }} onClick={descargarDatosEnXLSX}>
                    <span className="material-symbols-sharp">download</span>
                    <h3>Descargar Excel</h3>
                </div>
                <div
                    id="estadoNodo"
                    className={estadoNodo ? "card-estado-nodo compact online" : "card-estado-nodo compact offline"}
                    style={{ marginTop: "10px" }}
                >
                    {estadoNodo ? (
                        <>
                            <i className="fa-solid fa-circle-xmark"></i>
                            <span className="estado-texto">Nodo en línea</span>
                        </>
                    ) : (
                        <>
                            <i className="fa-solid fa-circle-check"></i>
                            <span className="estado-texto">Nodo fuera de línea</span>
                        </>
                    )}
                </div>
            </div>
            <MapModal isOpen={isMapOpen} onClose={() => setIsMapOpen(false)} />
        </>
    )
}
