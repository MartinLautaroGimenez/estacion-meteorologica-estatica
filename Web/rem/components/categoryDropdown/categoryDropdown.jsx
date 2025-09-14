import { NODES } from "@/models";
import { descargarDatosEnXLSX } from "@/lib";
import { setCookie } from "@/lib";
import "./categoryDropdown.css"

export const CategoryDropdown = ({set, estadoNodo}) => {
    const handleChange = (e) => {
        const selectedValue = e.target.value;
        set(selectedValue);
        setCookie("categoriaDropdown", selectedValue, 30);
    };
    return (
        <div className="zarazap">
            <h1>Datos</h1>
            <h3>Seleccione el nodo meteorológico: </h3>
            <select
                id="categoriaDropdown"
                className="dropdown"
                style={{ marginRight: "0px" }}
                defaultValue=""
                onChange={handleChange}
            >
                <option value="" disabled>
                    Seleccione nodo meteorológico
                </option>
                {Object.keys(NODES).map((key) => (
                    <option value={key} key={key}>
                        {NODES[key][0]}
                    </option>
                ))}
            </select>
            <div
                className="cs"
                style={{ cursor: "pointer" }}
                onClick={descargarDatosEnXLSX}
            >
                <span className="material-symbols-sharp">download</span>
                <h3>Descargar Excel</h3>
            </div>
            <div
                id="estadoNodo"
                className={estadoNodo? 'card-estado-nodo compact online' : 'card-estado-nodo compact offline'}
                style={{ marginTop: "10px" }}
            >
                {estadoNodo?
                    <>
                        <i className="fa-solid fa-circle-xmark"></i>
                        <span className="estado-texto">Nodo fuera de línea</span>
                    </>
                    :
                    <>
                        <i className="fa-solid fa-circle-check"></i>
                        <span className="estado-texto">Nodo en línea</span>
                    </>
                }
            </div>
        </div>
    )
}