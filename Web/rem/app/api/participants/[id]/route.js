import { NextResponse } from "next/server";
import { readFile, writeFile } from "fs/promises";
import path from "path";

const filePath = path.join(process.cwd(), "config", "participants.json");

async function readParticipants() {
    const data = await readFile(filePath, "utf8");
    return JSON.parse(data);
}

async function saveParticipants(participants) {
    await writeFile(filePath, JSON.stringify(participants, null, 2));
}

// GET /api/participants/:id
export async function GET(req, { params }) {
    const id = await params.id;
    const participants = await readParticipants();
    const participant = participants.find((e) => e.id === id);

    if (!participant) return NextResponse.json({ error: "Not found" }, { status: 404 });

    return NextResponse.json(participant);
}

// PUT /api/participants/:id
export async function PUT(req, { params }) {
    const id = await params.id;
    const body = await req.json();
    const participants = await readParticipants();

    const index = participants.findIndex((e) => e.id === id);
    if (index === -1)
        return NextResponse.json({ error: "Not found" }, { status: 404 });

    participants[index] = { ...participants[index], ...body };
    await saveParticipants(participants);

    return NextResponse.json(participants[index]);
}

// DELETE /api/participants/:id
export async function DELETE(req, { params }) {
    const id = await params.id;
    const participants = await readParticipants();

    const filtered = participants.filter((e) => e.id !== id);
    if (filtered.length === participants.length)
        return NextResponse.json({ error: "Not found" }, { status: 404 });

    await saveParticipants(filtered);
    return NextResponse.json({ success: true });
}
