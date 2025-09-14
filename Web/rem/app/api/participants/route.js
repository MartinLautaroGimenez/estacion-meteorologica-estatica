import { NextResponse } from "next/server";
import { readData, saveData } from "@/lib/db";

const FILE = "participants.json";

export async function GET() {
  const participants = await readData(FILE);
  return NextResponse.json(participants);
}

export async function POST(req) {
  const body = await req.json();
  const participants = await readData(FILE);

  const newParticipant = { id: Date.now().toString(), ...body };
  participants.push(newParticipant);

  await saveData(FILE, participants);
  return NextResponse.json(newParticipant, { status: 201 });
}
