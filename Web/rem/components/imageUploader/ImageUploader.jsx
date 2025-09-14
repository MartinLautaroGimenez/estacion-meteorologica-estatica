"use client";

import { useState } from "react";
import "./imageUploader.css";

export function ImageUploader({ onFileSelect, defaultImage }) {
    const [preview, setPreview] = useState(defaultImage || null);

    const handleFileChange = (e) => {
        const file = e.target.files[0];
        if (!file) return;

        // Validar tipo
        if (!file.type.startsWith("image/")) {
            alert("Solo se permiten imágenes");
            return;
        }

        // Previsualizar
        const reader = new FileReader();
        reader.onloadend = () => {
            setPreview(reader.result);
        };
        reader.readAsDataURL(file);

        // Pasar archivo al padre (form)
        onFileSelect(file);
    };

    return (
        <div className="image-uploader">
            <label className="upload-label">
                <input type="file" accept="image/*" onChange={handleFileChange} hidden/>
                <span className="material-symbols-sharp">upload</span>
                <span>Subir imagen</span>
            </label>

            {preview && (
                <div className="image-preview">
                    <img src={preview} alt="Vista previa" />
                </div>
            )}
        </div>
    );
}
