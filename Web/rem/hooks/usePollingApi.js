import { useState, useEffect } from "react";
import {APIS} from "@/models";

/**
 * @param {string} apiKey - clave del endpoint definido en APIs
 * @param {number} intervalMs - intervalo en milisegundos (900000 para 15min)
 */
export function usePollingApi(apiKey, intervalMs = 900000) {
    const [data, setData] = useState(null);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState(null);

    useEffect(() => {
        let cancel = false;

        async function fetchData() {
            try {
                const url = APIS[apiKey];
                if (!url) throw new Error(`API "${apiKey}" no encontrada`);

                const res = await fetch(url);
                if (!res.ok) throw new Error("Error en la petición");

                const json = await res.json();
                if (!cancel) setData(json);
            } catch (err) {
                if (!cancel) setError(err.message);
            } finally {
                if (!cancel) setLoading(false);
            }
        }
        fetchData();
        const interval = setInterval(fetchData, intervalMs);
        return () => {
            cancel = true;
            clearInterval(interval);
        };
    }, [apiKey, intervalMs]);
    return { data, loading, error };
}