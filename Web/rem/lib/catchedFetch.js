export async function catchedFetch(url, options = {}) {
    try {
        const res = await fetch(url, {
            ...options,
            headers: {
                "Content-Type": "application/json",
                ...(options.headers || {}),
            },
            credentials: "include",
        });

        if (!res.ok) {
            let errorMessage = `Error ${res.status}`;
            try {
                const errorData = await res.json();
                errorMessage = errorData.error || errorMessage;
            } catch {
                // se deja el mensaje por defecto
            }
            throw new Error(errorMessage);
        }

        // Si hay contenido vacío (204 No Content)
        if (res.status === 204) {
            return null;
        }

        return await res.json();
    } catch (err) {
        console.error("apiFetch error:", err);
        throw err;
    }
}
