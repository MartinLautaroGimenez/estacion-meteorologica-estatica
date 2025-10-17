import { NextResponse } from "next/server";
import { prisma } from "@/lib/prisma";

export async function GET() {
    const events = await prisma.eventos.findMany();
    return NextResponse.json(events);
}

export async function POST(req) {
    const data = await req.json();
    const newEvent = await prisma.eventos.create({ data })
    return NextResponse.json(newEvent, { status: 201 });
}
