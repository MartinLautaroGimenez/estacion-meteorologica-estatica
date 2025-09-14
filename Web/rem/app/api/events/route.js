import { NextResponse } from "next/server";
import { readData, saveData } from "@/lib/db";

const FILE = "events.json";

export async function GET() {
    const events = await readData(FILE);
    return NextResponse.json(events);
}

export async function POST(req) {
    const body = await req.json();
    const events = await readData(FILE);

    const newEvent = { id: Date.now().toString(), ...body };
    events.push(newEvent);

    await saveData(FILE, events);
    return NextResponse.json(newEvent, { status: 201 });
}
