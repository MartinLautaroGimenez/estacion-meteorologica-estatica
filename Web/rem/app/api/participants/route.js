import { NextResponse } from "next/server";
import { prisma } from "@/lib/prisma";

export async function GET() {
  const participants = await prisma.participantes.findMany();
  return NextResponse.json(participants);
}

export async function POST(req) {
  const data = await req.json();
  const newParticipant = await prisma.participantes.create({ data });
  return NextResponse.json(newParticipant, { status: 201 });
}
