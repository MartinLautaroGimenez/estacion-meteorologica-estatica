export async function obtenerDatos(url) {
    try {
        const response = await fetch(url, {
        headers: { 'Access-Control-Allow-Origin': '*' }
        });
        if (!response.ok) throw new Error('HTTP ' + response.status);
        return await response.json();
    } catch (error) {
        console.error('Error fetching data:', error);
        return null;
    }
}