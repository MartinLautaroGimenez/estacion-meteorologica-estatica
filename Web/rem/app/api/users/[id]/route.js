import { NextResponse } from "next/server";
import { prisma } from "@/lib/prisma";
import { requireAdmin } from "@/lib/requireAdmin";
import bcrypt from "bcryptjs";

const SALT_ROUNDS = 10;

// GET /api/users/:id
export async function GET(req, { params }) {
    try {
        await requireAdmin(req);

        const id = parseInt(params.id, 10);
        if (isNaN(id)) {
            return NextResponse.json({ error: "Invalid ID" }, { status: 400 });
        }

        const user = await prisma.webadmins.findUnique({
            where: { idwebadmins: id },
            select: {
                idwebadmins: true,
                email: true,
                esAdmin: true,
            },
        });

        if (!user) {
            return NextResponse.json({ error: "Not found" }, { status: 404 });
        }

        return NextResponse.json(user);
    } catch (e) {
        console.error("GET /users/:id error:", e);
        return NextResponse.json({ error: e.message }, { status: 403 });
    }
}

// PUT /api/users/:id
export async function PUT(req, { params }) {
    try {
        await requireAdmin(req);

        const id = parseInt(params.id, 10);
        if (isNaN(id)) {
            return NextResponse.json({ error: "Invalid ID" }, { status: 400 });
        }

        const body = await req.json();
        const dataToUpdate = {};

        if (body.email) dataToUpdate.email = body.email;
        if (body.role !== undefined) dataToUpdate.esAdmin = body.role === "admin" ? 1 : 0;
        if (body.password) {
            dataToUpdate.clave = await bcrypt.hash(body.password, SALT_ROUNDS);
        }

        const updatedUser = await prisma.webadmins.update({
            where: { idwebadmins: id },
            data: dataToUpdate,
            select: {
                idwebadmins: true,
                email: true,
                esAdmin: true,
            },
        });

        return NextResponse.json(updatedUser);
    } catch (e) {
        console.error("PUT /users/:id error:", e);
        if (e.code === "P2025") {
            // Prisma: record not found
            return NextResponse.json({ error: "Not found" }, { status: 404 });
        }
        return NextResponse.json({ error: e.message }, { status: 403 });
    }
}

// DELETE /api/users/:id
export async function DELETE(req, { params }) {
    try {
        //await requireAdmin(req);
        let { id } = await params
        id = parseInt(id, 10);
        if (isNaN(id)) {
            return NextResponse.json({ error: "Invalid ID" }, { status: 400 });
        }

        await prisma.webadmins.delete({
            where: { idwebadmins: id },
        });

        return NextResponse.json({ success: true });
    } catch (e) {
        console.error("DELETE /users/:id error:", e);
        if (e.code === "P2025") {
            return NextResponse.json({ error: "Not found" }, { status: 404 });
        }
        return NextResponse.json({ error: e.message }, { status: 403 });
    }
}
