export function DataCard({ icon, title, value, className, onClick, unit = '', style = {}}) {
    return (
        <div className={className} onClick={onClick} style={{ cursor: "pointer"}}>
            <span className="material-symbols-sharp">{icon}</span>
            <h3>{title}</h3>
            <h2 style={{ ...style }}>{value!=="undefined"? (value + " " + unit) : "No disponible"}</h2>
        </div>
    );
}

