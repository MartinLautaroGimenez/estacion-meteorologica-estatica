import { NextResponse } from "next/server";
import { prisma } from "@/lib/prisma";
import { requireAdmin } from "@/lib/requireAdmin";
import bcrypt from "bcryptjs";

const SALT_ROUNDS = 10;

// GET /api/users
export async function GET() {
    try {
        const webadmins = await prisma.webadmins.findMany();
        return NextResponse.json(webadmins);
    } catch (error) {
        console.error("Error fetching users:", error);
        return NextResponse.json({ error: "Server error" }, { status: 500 });
    }
}

// POST /api/users
export async function POST(req) {
    try {
        const session = await requireAdmin(req);

        const body = await req.json();
        if (!body.email || !body.clave) {
            return NextResponse.json({ error: "Missing email or password" }, { status: 400 });
        }

        const hashedclave = await bcrypt.hash(body.clave, SALT_ROUNDS);

        const newUser = await prisma.webadmins.create({
            data: {
                email: body.email,
                esAdmin: body.role === "admin" ? 1 : 0,
                clave: hashedclave,
            },
        });

        return NextResponse.json(
            { id: newUser.idwebadmins, email: newUser.email, esAdmin: newUser.esAdmin },
            { status: 201 }
        );
    } catch (e) {
        console.error("Error creating user:", e);
        return NextResponse.json({ error: e.message }, { status: 403 });
    }
}
