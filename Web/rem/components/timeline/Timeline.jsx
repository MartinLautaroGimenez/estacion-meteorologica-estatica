"use client";

import { useState, useRef, useMemo } from "react";
import "./timeline.css";

export function Timeline({ events = [] }) {
    const [activeEvent, setActiveEvent] = useState(0);
    const scrollRef = useRef(null);
    const markersRef = useRef([]);

    const PX_PER_DAY = 5;
    const extra = 200;

    function parseLocalDate(dateStr) {
        const [year, month, day] = dateStr.split("-").map(Number);
        return new Date(year, month - 1, day);
    }

    // Normalizamos los eventos con fechas en Date
    const eventsWithDates = useMemo(
        () =>
            events.map((e) => ({
                ...e,
                dateObj: parseLocalDate(e.date),
            })),
        [events]
    );

    if (eventsWithDates.length === 0) {
        return <p>No hay eventos para mostrar</p>;
    }

    const minDate = new Date(
        Math.min(...eventsWithDates.map((e) => e.dateObj.getTime()))
    );
    const maxDate = new Date(
        Math.max(...eventsWithDates.map((e) => e.dateObj.getTime()))
    );

    const daysRange = Math.ceil((maxDate - minDate) / (1000 * 60 * 60 * 24));
    const totalWidth = daysRange * PX_PER_DAY + extra;

    const marks = [];
    const current = new Date(minDate.getFullYear(), minDate.getMonth(), 1);
    while (current <= maxDate) {
        const daysFromStart = Math.floor(
            (current - minDate) / (1000 * 60 * 60 * 24)
        );
        marks.push({
            date: new Date(current),
            left: daysFromStart * PX_PER_DAY,
            isYear: current.getMonth() === 0,
        });
        current.setMonth(current.getMonth() + 1);
    }

    const handleEventClick = (index) => {
        setActiveEvent(index);
        scrollToEvent(index);
    };

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

    const handlePrev = () => {
        if (activeEvent > 0) {
            const newIndex = activeEvent - 1;
            setActiveEvent(newIndex);
            scrollToEvent(newIndex);
        }
    };

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
                <div className="timeline-line" style={{ width: totalWidth }} />
                <div
                    className="timeline-scale"
                    style={{
                        width: totalWidth,
                        transform: `translateX(${extra / 2}px)`,
                    }}
                >
                    {marks.map((mark, i) => (
                        <div
                            key={i}
                            className={`timeline-mark ${
                                mark.isYear ? "year" : "month"
                            }`}
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

                <div
                    className="timeline-events"
                    style={{
                        width: totalWidth,
                        transform: `translateX(${extra / 2}px)`,
                    }}
                >
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
                                <div className="timeline-connector" />
                                <div
                                    className={`timeline-card ${
                                        activeEvent === index ? "active" : ""
                                    }`}
                                    onClick={() => handleEventClick(index)}
                                >
                                    <h4>{event.title}</h4>
                                </div>
                            </div>
                        );
                    })}
                </div>
            </div>

            {activeEvent !== null && (
                <div className="timeline-detail">
                    <h3>{eventsWithDates[activeEvent].title}</h3>
                    <span className="timeline-detail-date">
                        {eventsWithDates[
                            activeEvent
                        ].dateObj.toLocaleDateString("es-ES", {
                            year: "numeric",
                            month: "long",
                            day: "numeric",
                        })}
                    </span>
                    <p>{eventsWithDates[activeEvent].description}</p>
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
