    // ===== Cookies Utils =====
    function setCookie(name, value, days) {
        const date = new Date();
        date.setTime(date.getTime() + (days * 86400000));
        const expires = "expires=" + date.toUTCString();
        document.cookie = name + "=" + value + ";" + expires + ";path=/";
    }

    function getCookie(name) {
        const decodedCookie = decodeURIComponent(document.cookie);
        const cookies = decodedCookie.split(';');
        name = name + "=";
        for (let cookie of cookies) {
            cookie = cookie.trim();
            if (cookie.indexOf(name) === 0) {
                return cookie.substring(name.length);
            }
        }
        return "";
    }

    // ===== Función para establecer el enlace activo basado en data-active =====
    function setActiveLink() {
        const activePage = document.body.getAttribute("data-active");
        let linkSelector = "";

        if (activePage === "index") {
            linkSelector = 'a[href="/em/index.html"]';
        } else if (activePage === "datos") {
            linkSelector = 'a[href="/em/datos.html"]';
        } else if (activePage === "about_us") {
            linkSelector = 'a[href="/em/about_us.html"]';
        } else {
            console.warn("⚠️ Data-active no coincide con ningún caso válido:", activePage);
            return;
        }

        document.querySelectorAll(".navbar a").forEach(link => link.classList.remove("active"));

        const link = document.querySelector(linkSelector);
        if (link) {
            link.classList.add("active");
            console.log("✅ Activado:", linkSelector);
        } else {
            console.warn("❌ No se encontró el enlace:", linkSelector);
        }

        observeActiveLink(linkSelector);
    }

    // ===== MutationObserver para mantener la clase active =====
    function observeActiveLink(linkSelector) {
        const targetNode = document.querySelector(".navbar");
        if (!targetNode) return;

        const config = { attributes: true, subtree: true };

        const callback = function (mutationsList) {
            for (const mutation of mutationsList) {
                if (mutation.type === "attributes" && mutation.target.matches(linkSelector)) {
                    if (!mutation.target.classList.contains("active")) {
                        mutation.target.classList.add("active");
                        console.log("♻️ Reaplicando .active a:", linkSelector);
                    }
                }
            }
        };

        const observer = new MutationObserver(callback);
        observer.observe(targetNode, config);
    }

    // ===== Script principal =====
    $(function () {
        // 🔥 Usamos delegación de eventos para el toggler: 
        // Esto permite que, aunque el header se cargue dinámicamente, el evento click sea capturado.
        $(document).on("click", ".theme-toggler", function () {
            document.body.classList.toggle("dark-theme");
            // Alterna la clase "active" en los dos spans dentro del toggle
            $(this).find("span").toggleClass("active");
            const isDark = document.body.classList.contains("dark-theme");
            localStorage.setItem("theme", isDark ? "dark" : "light");
        });

        // Cargamos el header dinámicamente
        $("#header-placeholder").load("frontend/header.html", function (response, status, xhr) {
            if (status === "error") {
                console.error("❌ No se pudo cargar el header:", xhr.status, xhr.statusText);
                return;
            }

            // Establecer enlace activo
            setActiveLink();

            // Aplicar el tema guardado (si existe)
            const savedTheme = localStorage.getItem("theme");
            if (savedTheme === "dark") {
                document.body.classList.add("dark-theme");
                const $themeToggler = $(".theme-toggler");
                if ($themeToggler.length) {
                    // Al togglear, cambiamos el estado de los spans:
                    $themeToggler.find("span").toggleClass("active");
                }
            }

            // Ocultar loader si existe
            document.body.classList.add("loaded");
            const loader = document.getElementById("loader");
            if (loader) loader.style.display = "none";
        });

        // Extra seguridad: reaplicar .active luego de cargar el resto del DOM
        setTimeout(setActiveLink, 500);
        setTimeout(setActiveLink, 1000);

        // === Nodo meteorológico desde cookie (opcional) ===
        const categoriaDropdown = document.getElementById("categoriaDropdown");
        const savedValue = getCookie("categoriaDropdown");
        if (savedValue && categoriaDropdown) {
            categoriaDropdown.value = savedValue;
        } else if (categoriaDropdown) {
            categoriaDropdown.value = "emetec";
        }
        if (categoriaDropdown) {
            categoriaDropdown.dispatchEvent(new Event("change"));
        }

        // === Loader inicial solo la primera vez ===
        const loaderCookie = getCookie("loaderShown");
        if (!loaderCookie) {
            setTimeout(() => {
                document.body.classList.add("loaded");
                const loader = document.getElementById("loader");
                if (loader) loader.style.display = "none";
                setCookie("loaderShown", "true", 0.03125); // 45 min
            }, 700);
        } else {
            document.body.classList.add("loaded");
            const loader = document.getElementById("loader");
            if (loader) loader.style.display = "none";
        }

        // === Magnific Popup (opcional) ===
        $(".image-link").on("click", function () {
            $.magnificPopup.open({
                items: {
                    src: $(this).data("mfp-src")
                },
                type: "image"
            });
            return false;
        });
    });

    window.addEventListener("scroll", () => {
        const header = document.querySelector("header");
        if (window.scrollY > 20) {
        header.classList.add("scrolled");
        } else {
        header.classList.remove("scrolled");
        }
    });

