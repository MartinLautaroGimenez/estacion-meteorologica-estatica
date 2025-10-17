import NextAuth from "next-auth";
import CredentialsProvider from "next-auth/providers/credentials";
import bcrypt from "bcryptjs";
import { prisma } from "@/lib/prisma"; // conexión Prisma

export const authOptions = {
    providers: [
        CredentialsProvider({
            name: "Credenciales",
            credentials: {
                email: { label: "Email", type: "text" },
                password: { label: "Contraseña", type: "password" },
            },
            async authorize(credentials) {
                if (!credentials?.email || !credentials?.password) return null;

                // Buscar el usuario en la base de datos
                const user = await prisma.webadmins.findUnique({
                    where: { email: credentials.email },
                });

                if (!user) return null;

                // Validar la contraseña
                const isValid = await bcrypt.compare(credentials.password, user.clave);
                if (!isValid) return null;

                // Mapear esAdmin (0 o 1) a "admin" o "user"
                const role = user.esAdmin === 1 ? "admin" : "user";

                // Devolver datos que irán al token
                return {
                    id: user.idwebadmins,
                    email: user.email,
                    role,
                };
            },
        }),
    ],

    pages: {
        signIn: "/login",
    },

    callbacks: {
        // Guardar información personalizada en el JWT
        async jwt({ token, user }) {
            if (user) {
                token.id = user.id;
                token.role = user.role;
            }
            return token;
        },

        // Pasar la información del token a la sesión
        async session({ session, token }) {
            if (token) {
                session.user.id = token.id;
                session.user.role = token.role;
            }
            return session;
        },
    },

    session: {
        strategy: "jwt",
    },

    secret: process.env.NEXTAUTH_SECRET,
};

// Exportar para que Next.js lo use como API
const handler = NextAuth(authOptions);
export { handler as GET, handler as POST };
