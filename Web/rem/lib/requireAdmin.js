export async function requireAdmin(req) {
    const session = await getServerSession(authOptions);
    if (!session || session.user.role !== "admin") {
        throw new Error("No autorizado");
    }
    return session;
}
