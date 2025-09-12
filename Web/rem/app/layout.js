"use client";

import "./globals.css";
import { Header, Footer } from "@/components";
import Script from "next/script";

export default function RootLayout({ children }) {
  return (
    <html lang="en">
      <head>
        {/* Scripts y CSS globales de Magnific Popup */}
        <link
          rel="stylesheet"
          href="https://cdnjs.cloudflare.com/ajax/libs/magnific-popup.js/1.1.0/magnific-popup.min.css"
        />

        <Script src="https://code.jquery.com/jquery-3.6.0.min.js" strategy="beforeInteractive" />
        <Script
          src="https://cdnjs.cloudflare.com/ajax/libs/magnific-popup.js/1.1.0/jquery.magnific-popup.min.js"
          strategy="afterInteractive"
        />

        {/* No se ve aquí el favicon ya que Next.js lo carga solo mediante el nombre en /public */}

        {/* Google Fonts - Material Symbols Sharp */}
        <link href="https://fonts.googleapis.com/icon?family=Material+Symbols+Sharp" rel="stylesheet" />
        {/* jQuery CDN */}
        <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
      </head>
      <body>
        <Header />
        <div className="content">
          <main>
            {children}
          </main>
          <Footer />
        </div>
      </body>
    </html>
  );
}
