"use client";
import "./datos.css";
import { showPopup } from "@/lib";
import { useState } from "react";
import { usePollingApi } from "@/hooks";
import { NODES } from "@/models";
import { CARDS_CONFIG } from "@/config";
import { DataCard, CategoryDropdown } from "@/components";

// TODO: completar el manejp de cookies  y localstorage para guardar selección y datos de nodos
export default () => {
    const [categoria, setCategoria] = useState('emetec');
    const { data, error, loading } = usePollingApi(categoria); // 15 minutos por defecto
    console.log(data);

    return (
        <>
            <CategoryDropdown set={setCategoria}/>
            <div
                className="subjects"
                style={{ gridTemplateColumns: "repeat(5, 1fr) !important" }}
            >
                <DataCard
                    key={"popup-fecha"}
                    icon={"location_on"}
                    title={"Ubicación"}
                    value={NODES[categoria][0]}
                    className={"eg"}
                    onClick={()=> window.location.href = NODES[categoria][1]}
                    style={{ fontSize: "1rem" }}
                />
                {CARDS_CONFIG.map(cfg => (
                <DataCard
                    key={cfg.id}
                    icon={cfg.icon}
                    title={cfg.title}
                    value={data?.[0][cfg.dataKey]}   // el último valor
                    className={cfg.className}
                    unit={cfg.unit}
                    onClick={cfg.showChart? () => showPopup(cfg.title, cfg.dataKey, cfg.color, data) : null}
                />
                ))}
            </div>
        </>
    );
};
