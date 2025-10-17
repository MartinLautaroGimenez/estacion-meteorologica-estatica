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
                const res = await fetch(`${process.env.NEXTAUTH_URL}/api/users`);
                const users = await res.json();

                const user = users.find(u => u.email === credentials.email);
                if (user && await bcrypt.compare(credentials.password, user.password)) {
                    return { id: user.id, email: user.email, role: user.role };
                }
                return null;
            }
        })
    ],
    session: {
        strategy: "jwt"
    },
    secret: process.env.NEXTAUTH_SECRET,
    callbacks: {
        async jwt({ token, user }) {
            // se llama en el login: agregamos el rol al token
            if (user) {
                token.id = user.id;
                token.role = user.role;
            }
            return token;
        },
        async session({ session, token }) {
            // cada vez que se pide la sesión: propagamos el rol desde el token
            if (token) {
                session.user.id = token.id;
                session.user.role = token.role;
            }
            return session;
        }
    }
};

export default NextAuth(authOptions);
