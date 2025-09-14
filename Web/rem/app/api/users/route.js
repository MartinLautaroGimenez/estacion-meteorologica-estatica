import { NextResponse } from "next/server";
import { readData, saveData } from "@/lib/db";
import { requireAdmin } from "@/lib";
import bcrypt from "bcrypt";

const FILE = "users.json";
const SALT_ROUNDS = 10;

export async function GET() {
    const users = await readData(FILE);
    return NextResponse.json(users);
}

export async function POST(req) {
    try {
        const session = requireAdmin(req)

        const body = await req.json();
        const users = await readData(FILE);
        const hashedPassword = await bcrypt.hash(body.password, SALT_ROUNDS)

        const newUser = {
            id: Date.now().toString(),
            email: body.email,
            role: body.role,
            password: hashedPassword
        };
        users.push(newUser);

        await saveData(FILE, users);
        return NextResponse.json(newUser, { status: 201 });
    } catch (e) {
        return new Response(JSON.stringify({ error: e.message }), { status: 403 });
    }
}
