"use client"

import { useState } from "react";
import "./popUpImage.css"; // Importamos los estilos

export function PopupImage({ image, description, title }) {
    const [isOpen, setIsOpen] = useState(false);

    return (
        <div className="popup-container">
            {/* Miniatura */}
            <img
                src={image}
                alt={description}
                className="popup-thumbnail"
                onClick={() => setIsOpen(true)}
            />

            {/* Popup */}
            {isOpen && (
                <div className="popup-overlay" onClick={() => setIsOpen(false)}>
                    <div
                        className="popup-content"
                        onClick={(e) => e.stopPropagation()} // Evitar cerrar al clickear dentro
                    >
                        <img src={image} alt={description} className="popup-image" />
                        <div>
                            <h3 className="popup-title">{title}</h3>
                            <p className="popup-description">{description}</p>
                        </div>
                    </div>
                </div>
            )}
        </div>
    );
}
