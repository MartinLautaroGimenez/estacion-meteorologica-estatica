"use client";

import { useState, useRef } from "react";
import "./timeline.css";
import { TIMELINE_EVENTS } from "@/config/timeLineEvents";

export function Timeline() {
    const [activeEvent, setActiveEvent] = useState(0); // inicial en el primero
    const scrollRef = useRef(null);
    const markersRef = useRef([]);

    const PX_PER_DAY = 1;
    const extra = 200;

    function parseLocalDate(dateStr) {
        const [year, month, day] = dateStr.split("-").map(Number);
        return new Date(year, month - 1, day); // 👈 evita UTC, crea fecha local
    }
    const eventsWithDates = TIMELINE_EVENTS.map((e) => ({
        ...e,
        dateObj: parseLocalDate(e.date),
    }));


    const minDate = new Date(
        Math.min(...eventsWithDates.map((e) => e.dateObj.getTime()))
    );
    const maxDate = new Date(
        Math.max(...eventsWithDates.map((e) => e.dateObj.getTime()))
    );

    const daysRange = Math.ceil(
        (maxDate - minDate) / (1000 * 60 * 60 * 24)
    );
    const totalWidth = daysRange * PX_PER_DAY + extra;

    const marks = [];
    const current = new Date(minDate.getFullYear(), minDate.getMonth(), 1);
    while (current <= maxDate) {
        const daysFromStart = Math.floor((current - minDate) / (1000 * 60 * 60 * 24));
        marks.push({
            date: new Date(current),
            left: daysFromStart * PX_PER_DAY,
            isYear: current.getMonth() === 0, // si es enero
        });
        current.setMonth(current.getMonth() + 1);
    }

    // Manejar click en evento
    const handleEventClick = (index) => {
        setActiveEvent(index);
        scrollToEvent(index);
    };

    // Auto-scroll a un evento
    const scrollToEvent = (index) => {
        const marker = markersRef.current[index];
        const container = scrollRef.current;

        if (marker && container) {
            const markerRect = marker.getBoundingClientRect();
            const containerRect = container.getBoundingClientRect();

            const offset =
                markerRect.left -
                containerRect.left -
                containerRect.width / 2 +
                markerRect.width / 2;

            container.scrollBy({
                left: offset,
                behavior: "smooth",
            });
        }
    };

    // Ir al anterior
    const handlePrev = () => {
        if (activeEvent > 0) {
            const newIndex = activeEvent - 1;
            setActiveEvent(newIndex);
            scrollToEvent(newIndex);
        }
    };

    // Ir al siguiente
    const handleNext = () => {
        if (activeEvent < eventsWithDates.length - 1) {
            const newIndex = activeEvent + 1;
            setActiveEvent(newIndex);
            scrollToEvent(newIndex);
        }
    };

    return (
        <section className="timeline">
            <div className="timeline-scroll" ref={scrollRef}>
                {/* Línea */}
                <div
                    className="timeline-line"
                    style={{
                        width: totalWidth,
                    }}
                />

                {/* Marcas de tiempo */}
                <div className="timeline-scale" style={{
                    width: totalWidth,
                    transform: `translateX(${extra / 2}px)`,
                }}>
                    {marks.map((mark, i) => (
                        <div
                            key={i}
                            className={`timeline-mark ${mark.isYear ? "year" : "month"}`}
                            style={{ left: `${mark.left}px` }}
                        >
                            <span>
                                {mark.isYear
                                    ? mark.date.getFullYear()
                                    : mark.date.toLocaleDateString("es-ES", {
                                        month: "short",
                                    })}
                            </span>
                        </div>
                    ))}
                </div>

                {/* Eventos */}
                <div className="timeline-events" style={{
                    width: totalWidth,
                    transform: `translateX(${extra / 2}px)`,
                }}>
                    {eventsWithDates.map((event, index) => {
                        const daysFromStart = Math.floor(
                            (event.dateObj - minDate) / (1000 * 60 * 60 * 24)
                        );
                        const left = daysFromStart * PX_PER_DAY;

                        return (
                            <div
                                key={index}
                                className="timeline-event"
                                style={{ left: `${left}px` }}
                            >
                                {/* Línea que conecta */}
                                <div className="timeline-connector" />

                                {/* Tarjeta con título */}
                                <div
                                    className={`timeline-card ${activeEvent === index ? "active" : ""}`}
                                    onClick={() => handleEventClick(index)}
                                >
                                    <h4>{event.title}</h4>
                                </div>
                            </div>
                        );
                    })}
                </div>
            </div>

            {/* Panel de detalle */}
            {activeEvent !== null && (
                <div className="timeline-detail">
                    <h3>{eventsWithDates[activeEvent].title}</h3>
                    <span className="timeline-detail-date">
                        {eventsWithDates[activeEvent].dateObj.toLocaleDateString("es-ES", {
                            year: 'numeric',
                            month: 'long',
                            day: 'numeric',
                        })}
                    </span>
                    <p>{eventsWithDates[activeEvent].description}</p>

                    {/* Botones de navegación */}
                    <div className="timeline-nav">
                        <button
                            onClick={handlePrev}
                            disabled={activeEvent === 0}
                            className="timeline-arrow"
                        >
                            ←
                        </button>
                        <button
                            onClick={handleNext}
                            disabled={activeEvent === eventsWithDates.length - 1}
                            className="timeline-arrow"
                        >
                            →
                        </button>
                    </div>
                </div>
            )}
        </section>
    );
}
