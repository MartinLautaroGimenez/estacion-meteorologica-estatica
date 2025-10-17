import NextAuth from "next-auth";
import CredentialsProvider from "next-auth/providers/credentials";
import bcrypt from "bcryptjs";

export const authOptions = {
    providers: [
        CredentialsProvider({
            name: "Credentials",
            credentials: {
                email: { label: "Email", type: "email" },
                password: { label: "Contraseña", type: "password" }
            },
            async authorize(credentials) {
                const user = await prisma.webadmins.findUnique({
                    where: { email: credentials.email }
                });

                if (!user) return null;

                const isValid = await bcrypt.compare(credentials.password, user.clave);
                if (!isValid) return null;

                const role = user.esAdmin === 1 ? "admin" : "editor";

                return {
                    id: user.idwebadmins,
                    email: user.email,
                    role
                };
            }
        })
    ],
    session: {
        strategy: "jwt"
    },
    secret: process.env.NEXTAUTH_SECRET,
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
        }
    }
};

export default NextAuth(authOptions);
