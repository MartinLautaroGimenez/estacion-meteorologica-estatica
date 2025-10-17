import { getServerSession } from "next-auth/next";
import { authOptions } from "./authOptions";

export async function requireAdmin(req) {
    const session = await getServerSession(authOptions);
    console.log("Session in requireAdmin:", session);
    if (!session || session.user.role !== "admin") {
        throw new Error("No autorizado");
    }
    return session;
}
