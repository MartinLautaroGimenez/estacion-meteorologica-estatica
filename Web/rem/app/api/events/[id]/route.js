import { NextResponse } from "next/server";
import { prisma } from "@/lib/prisma";

// GET /api/events/:id
export async function GET(req, props) {
    const params = await props.params;
    try {
        const id = Number(params.id);
        const event = await prisma.events.findUnique({
            where: { id },
        });

        if (!event) {
            return NextResponse.json({ error: "Not found" }, { status: 404 });
        }

        return NextResponse.json(event);
    } catch (error) {
        console.error("GET /api/events/:id error:", error);
        return NextResponse.json({ error: "Server error" }, { status: 500 });
    }
}

// PUT /api/events/:id
export async function PUT(req, props) {
    const params = await props.params;
    try {
        const id = Number(params.id);
        const body = await req.json();

        const updated = await prisma.events.update({
            where: { id },
            data: body,
        });

        return NextResponse.json(updated);
    } catch (error) {
        console.error("PUT /api/events/:id error:", error);
        if (error.code === "P2025") {
            return NextResponse.json({ error: "Not found" }, { status: 404 });
        }
        return NextResponse.json({ error: "Server error" }, { status: 500 });
    }
}

// DELETE /api/events/:id
export async function DELETE(req, props) {
    const params = await props.params;
    try {
        const id = Number(params.id);

        await prisma.eventos.delete({
            where: { ideventos: id },
        });

        return NextResponse.json({ success: true });
    } catch (error) {
        console.error("DELETE /api/events/:id error:", error);
        if (error.code === "P2025") {
            return NextResponse.json({ error: "Not found" }, { status: 404 });
        }
        return NextResponse.json({ error: "Server error" }, { status: 500 });
    }
}
