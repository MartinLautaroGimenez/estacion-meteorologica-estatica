import { NextResponse } from "next/server";
import { readFile, writeFile } from "fs/promises";
import path from "path";

const filePath = path.join(process.cwd(), "config", "events.json");

async function readEvents() {
    const data = await readFile(filePath, "utf8");
    return JSON.parse(data);
}

async function saveEvents(events) {
    await writeFile(filePath, JSON.stringify(events, null, 2));
}

// GET /api/events/:id
export async function GET(req, { params }) {
    const id = await params.id;
    const events = await readEvents();
    const event = events.find((e) => e.id === id);

    if (!event) return NextResponse.json({ error: "Not found" }, { status: 404 });

    return NextResponse.json(event);
}

// PUT /api/events/:id
export async function PUT(req, { params }) {
    const id = await params.id;
    const body = await req.json();
    const events = await readEvents();

    const index = events.findIndex((e) => e.id === id);
    if (index === -1)
        return NextResponse.json({ error: "Not found" }, { status: 404 });

    events[index] = { ...events[index], ...body };
    await saveEvents(events);

    return NextResponse.json(events[index]);
}

// DELETE /api/events/:id
export async function DELETE(req, { params }) {
    const id = await params.id;
    const events = await readEvents();

    const filtered = events.filter((e) => e.id !== id);
    if (filtered.length === events.length)
        return NextResponse.json({ error: "Not found" }, { status: 404 });

    await saveEvents(filtered);
    return NextResponse.json({ success: true });
}
