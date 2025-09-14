import { NextResponse } from "next/server";
import { readFile, writeFile } from "fs/promises";
import path from "path";
import { getServerSession } from "next-auth";
import { authOptions } from "@/lib/nextauth";
import { requireAdmin } from '@/lib'

const filePath = path.join(process.cwd(), "config", "users.json");

async function readUsers() {
    const data = await readFile(filePath, "utf8");
    return JSON.parse(data);
}

async function saveUsers(users) {
    await writeFile(filePath, JSON.stringify(users, null, 2));
}

// GET /api/users/:id
export async function GET(req, { params }) {
    try {
        const session = requireAdmin(req)
        const id = await params.id;
        const users = await readUsers();
        const user = users.find((e) => e.id === id);

        if (!user) return NextResponse.json({ error: "Not found" }, { status: 404 });

        return NextResponse.json(user);
    } catch (e) {
        return new Response(JSON.stringify({ error: e.message }), { status: 403 });
    }
}

// PUT /api/users/:id
export async function PUT(req, { params }) {
    try {
        const session = requireAdmin(req)
        const id = await params.id;
        const body = await req.json();
        const users = await readUsers();

        const index = users.findIndex((e) => e.id === id);
        if (index === -1)
            return NextResponse.json({ error: "Not found" }, { status: 404 });

        users[index] = { ...users[index], ...body };
        await saveUsers(users);

        return NextResponse.json(users[index]);
    } catch (e) {
        return new Response(JSON.stringify({ error: e.message }), { status: 403 });
    }
}

// DELETE /api/users/:id
export async function DELETE(req, { params }) {
    try {
        const session = requireAdmin(req)
        const id = await params.id;
        const users = await readUsers();

        const filtered = users.filter((e) => e.id !== id);
        if (filtered.length === users.length)
            return NextResponse.json({ error: "Not found" }, { status: 404 });
        await saveUsers(filtered);
        return NextResponse.json({ success: true });
    } catch (e) {
        return new Response(JSON.stringify({ error: e.message }), { status: 403 });
    }
}
