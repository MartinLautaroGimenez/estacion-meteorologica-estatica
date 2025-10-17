import { NextResponse } from "next/server";
import { prisma } from "@/lib/prisma";

// GET /api/participants/:id
export async function GET(req, props) {
    const params = await props.params;
    try {
        const id = Number(params.id);
        const participant = await prisma.participantes.findUnique({
            where: { idparticipantes: id },
        });

        if (!participant) {
            return NextResponse.json({ error: "Not found" }, { status: 404 });
        }

        return NextResponse.json(participant);
    } catch (error) {
        console.error("Error fetching participant:", error);
        return NextResponse.json({ error: "Server error" }, { status: 500 });
    }
}

// PUT /api/participants/:id
export async function PUT(req, props) {
    const params = await props.params;
    try {
        const id = Number(params.id);
        const body = await req.json();

        const updated = await prisma.participantes.update({
            where: { idparticipantes: id },
            data: {
                nombre: body.nombre,
                descripcion: body.descripcion,
                imagen: body.imagen,
            },
        });

        return NextResponse.json(updated);
    } catch (error) {
        console.error("Error updating participant:", error);
        if (error.code === "P2025") {
            return NextResponse.json({ error: "Not found" }, { status: 404 });
        }
        return NextResponse.json({ error: "Server error" }, { status: 500 });
    }
}

// DELETE /api/participants/:id
export async function DELETE(req, props) {
    const params = await props.params;
    try {
        const id = Number(params.id);

        await prisma.participantes.delete({
            where: { idparticipantes: id },
        });

        return NextResponse.json({ success: true });
    } catch (error) {
        console.error("Error deleting participant:", error);
        if (error.code === "P2025") {
            return NextResponse.json({ error: "Not found" }, { status: 404 });
        }
        return NextResponse.json({ error: "Server error" }, { status: 500 });
    }
}
