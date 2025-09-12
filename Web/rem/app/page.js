"use client";

import { SUMMARY_CARDS_CONFIG } from "@/config";
import { DataCard, CategoryDropdown } from "@/components";
import { showPopup } from "@/lib";
import { useState } from "react";
import { usePollingApi } from "@/hooks";

export default function Home() {
    const [categoria, setCategoria] = useState('emetec');
    const { data, error, loading } = usePollingApi(categoria); // 15 minutos por defecto
    console.log(data);

    const invokemap = () => {
        // TODO: Función para abrir mapa
    }
    const renderCell = (value) => {
        if (
            value === null ||
            value === undefined ||
            value === "" ||
            value === "null" ||
            value === "undefined"
        ) {
            return "No disponible";
        }
        return value;
    };

    return (
        <>
            <CategoryDropdown set={setCategoria} estadoNodo={!error} />
            <div className="subjects">
                {SUMMARY_CARDS_CONFIG.map(cfg => (
                    <DataCard
                        key={cfg.title}
                        icon={cfg.icon}
                        title={cfg.title}
                        value={data?.[0][cfg.dataKey]} //El último valor
                        className={cfg.className}
                        unit={cfg.unit}
                        onClick={cfg.showChart ? () => showPopup(cfg.title, cfg.dataKey, cfg.color) : null}
                    />
                ))}
            </div>
            {/* Tabla de Detalles */}
            <div className="timetableinicio" id="timetable">
                <div>
                    <h2 className="tableh2">Últimos 10 Datos</h2>
                </div>
                <div className="table-responsive">
                    <table id="tablaDetalles">
                        <thead>
                            <tr>
                                <th>Dato</th>
                                <th>Fecha</th>
                                <th>Temperatura</th>
                                <th>Humedad relativa</th>
                                <th>Presión atmosferica</th>
                                <th>Punto de rocio</th>
                                <th>Calidad del aire</th>
                            </tr>
                        </thead>
                        <tbody id="tablaDetallesCuerpo">
                            {data?.map((element) => (
                                <tr key={element["Dato N°"]}>
                                    <td>{renderCell(element["Dato N°"])}</td>
                                    <td>{renderCell(element["Fecha"])}</td>
                                    <td>{renderCell(element["Temperatura Promedio"])}</td>
                                    <td>{renderCell(element["Humedad"])}</td>
                                    <td>{renderCell(element["Presion"])}</td>
                                    <td>{renderCell(element["Temperatura punto de rocio"])}</td>
                                    <td>{renderCell(element["Calidad del aire"])}</td>
                                </tr>
                            ))}
                        </tbody>
                    </table>
                </div>
            </div>
        </>
    );
}
