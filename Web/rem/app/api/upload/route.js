import { NextResponse } from "next/server";
import { writeFile } from "fs/promises";
import path from "path";

export async function POST(req) {
    try {
        // Obtener los datos del formData
        const formData = await req.formData();
        const file = formData.get("file");

        if (!file) {
            return NextResponse.json({ error: "No file received" }, { status: 400 });
        }

        // Convertir a buffer
        const bytes = await file.arrayBuffer();
        const buffer = Buffer.from(bytes);

        // Generar nombre único
        const filename = `${Date.now()}-${file.name}`;

        // Definir ruta donde guardar
        const filePath = path.join(process.cwd(), "public", "uploads", filename);

        // Guardar en disco
        await writeFile(filePath, buffer);

        // Devolver URL accesible públicamente
        return NextResponse.json({ url: `/uploads/${filename}` });
    } catch (error) {
        console.error("Error uploading file:", error);
        return NextResponse.json({ error: "Upload failed" }, { status: 500 });
    }
}
