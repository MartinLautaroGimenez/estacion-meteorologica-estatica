import NextAuth from "next-auth";
import CredentialsProvider from "next-auth/providers/credentials";
import { readData } from "@/lib/db"; // tu helper para leer JSON
import bcrypt from "bcrypt";

const FILE = "users.json";

export const authOptions = {
    providers: [
        CredentialsProvider({
            name: "Credenciales",
            credentials: {
                email: { label: "Email", type: "text" },
                password: { label: "Contraseña", type: "password" },
            },
            async authorize(credentials) {
                const users = await readData(FILE);
                const user = users.find(u => u.email === credentials.email);
                if (user) {
                    const isValid = await bcrypt.compare(credentials.password, user.password);
                    if (isValid) {
                        return { id: user.id, email: user.email, role: user.role };
                    }
                }
                return null;
            }
        }),
    ],
    pages: {
        signIn: "/login", // página personalizada
    },
    callbacks: {
        async jwt({ token, user }) {
            if (user) {
                token.id = user.id;
                token.role = user.role;
            }
            return token;
        },
        async session({ session, token }) {
            if (token) {
                session.user.id = token.id;
                session.user.role = token.role;
            }
            return session;
        },
    },
    secret: process.env.NEXTAUTH_SECRET,
};

const handler = NextAuth(authOptions);

export { handler as GET, handler as POST };
