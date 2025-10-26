import { NextResponse } from "next/server";
import { writeFile, unlink } from "fs/promises";
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

export async function DELETE(req) {
    try {
        const { searchParams } = new URL(req.url);
        const filename = searchParams.get("filename")?.replace(/^\/?uploads\//, "");

        if (!filename) {
            return NextResponse.json({ error: "Missing filename parameter" }, { status: 400 });
        }

        const filePath = path.join(process.cwd(), "public", "uploads", filename);
        await unlink(filePath);

        return NextResponse.json({ message: "File deleted successfully" });
    } catch (error) {
        if (error.code === "ENOENT") {
            return NextResponse.json({ error: "File not found" }, { status: 404 });
        }
        console.error("Error deleting file:", error);
        return NextResponse.json({ error: "Delete failed" }, { status: 500 });
    }
}