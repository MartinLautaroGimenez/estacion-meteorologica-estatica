"use client"

import { useState, useRef, useMemo } from "react"
import "./timeline.css"

export function Timeline({ events = [] }) {
    const [activeEvent, setActiveEvent] = useState(0)
    const scrollRef = useRef(null)
    const markersRef = useRef([])

    const PX_PER_DAY = 5
    const extra = 200

    function parseLocalDate(dateStr) {
        const date = new Date(dateStr)
        return new Date(date.getFullYear(), date.getMonth(), date.getDate())
    }

    const eventsWithDates = useMemo(
        () =>
            events.map((e) => ({
                ...e,
                dateObj: parseLocalDate(e.fecha),
            })),
        [events],
    )

    if (eventsWithDates.length === 0) {
        return (
            <div className="timeline-empty">
                <p>No hay eventos para mostrar</p>
            </div>
        )
    }

    const minDate = new Date(Math.min(...eventsWithDates.map((e) => e.dateObj.getTime())))
    const maxDate = new Date(Math.max(...eventsWithDates.map((e) => e.dateObj.getTime())))

    const daysRange = Math.ceil((maxDate.getTime() - minDate.getTime()) / (1000 * 60 * 60 * 24))
    const totalWidth = daysRange * PX_PER_DAY + extra

    const marks = []
    const current = new Date(minDate.getFullYear(), minDate.getMonth(), 1)
    while (current <= maxDate) {
        const daysFromStart = Math.floor((current.getTime() - minDate.getTime()) / (1000 * 60 * 60 * 24))
        marks.push({
            date: new Date(current),
            left: daysFromStart * PX_PER_DAY,
            isYear: current.getMonth() === 0,
        })
        current.setMonth(current.getMonth() + 1)
    }

    const handleEventClick = (index) => {
        setActiveEvent(index)
        scrollToEvent(index)
    }

    const scrollToEvent = (index) => {
        const marker = markersRef.current[index]
        const container = scrollRef.current
        if (marker && container) {
            const markerRect = marker.getBoundingClientRect()
            const containerRect = container.getBoundingClientRect()
            const offset = markerRect.left - containerRect.left - containerRect.width / 2 + markerRect.width / 2
            container.scrollBy({
                left: offset,
                behavior: "smooth",
            })
        }
    }

    const handlePrev = () => {
        if (activeEvent > 0) {
            const newIndex = activeEvent - 1
            setActiveEvent(newIndex)
            scrollToEvent(newIndex)
        }
    }

    const handleNext = () => {
        if (activeEvent < eventsWithDates.length - 1) {
            const newIndex = activeEvent + 1
            setActiveEvent(newIndex)
            scrollToEvent(newIndex)
        }
    }

    return (
        <div className="timeline-container">
            <div className="timeline-details">
                <div className="timeline-detail-single">
                    {eventsWithDates[activeEvent] && (
                        <div className="timeline-detail-card active">
                            {eventsWithDates[activeEvent].imagen && (
                                <div className="timeline-detail-image">
                                    <img
                                        src={eventsWithDates[activeEvent].imagen || "/placeholder.svg"}
                                        alt={eventsWithDates[activeEvent].titulo}
                                    />
                                </div>
                            )}
                            <div className="timeline-detail-content">
                                <span className="timeline-detail-date">
                                    {eventsWithDates[activeEvent].dateObj.toLocaleDateString("es-ES", {
                                        day: "numeric",
                                        month: "long",
                                        year: "numeric",
                                    })}
                                </span>
                                <h4>{eventsWithDates[activeEvent].titulo}</h4>
                                <p>{eventsWithDates[activeEvent].descripcion}</p>
                            </div>
                        </div>
                    )}
                </div>
            </div>

            <div className="timeline-scroll" ref={scrollRef}>
                <button
                    onClick={handlePrev}
                    disabled={activeEvent === 0}
                    className="timeline-nav-btn left"
                    aria-label="Evento anterior"
                >
                    <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                        <polyline points="15 18 9 12 15 6"></polyline>
                    </svg>
                </button>

                <button
                    onClick={handleNext}
                    disabled={activeEvent === eventsWithDates.length - 1}
                    className="timeline-nav-btn right"
                    aria-label="Evento siguiente"
                >
                    <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                        <polyline points="9 18 15 12 9 6"></polyline>
                    </svg>
                </button>

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
                            className={`timeline-mark ${mark.isYear ? "year" : "month"}`}
                            style={{ left: `${mark.left}px` }}
                        >
                            <div className="timeline-mark-line" />
                            <span className="timeline-mark-label">
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
                        const daysFromStart = Math.floor((event.dateObj.getTime() - minDate.getTime()) / (1000 * 60 * 60 * 24))
                        const left = daysFromStart * PX_PER_DAY

                        return (
                            <div
                                key={index}
                                ref={(el) => (markersRef.current[index] = el)}
                                className="timeline-event"
                                style={{ left: `${left}px` }}
                            >
                                <div className="timeline-connector" />
                                <div
                                    className={`timeline-label ${activeEvent === index ? "active" : ""}`}
                                    onClick={() => handleEventClick(index)}
                                >
                                    {event.imagen && (
                                        <div className="timeline-label-image">
                                            <img src={event.imagen || "/placeholder.svg"} alt={event.titulo} />
                                        </div>
                                    )}
                                    <div className="timeline-label-content">
                                        <span className="timeline-label-date">
                                            {event.dateObj.toLocaleDateString("es-ES", {
                                                day: "numeric",
                                                month: "short",
                                                year: "numeric",
                                            })}
                                        </span>
                                        <h4 className="timeline-label-titulo">{event.titulo}</h4>
                                    </div>
                                </div>
                            </div>
                        )
                    })}
                </div>
            </div>
        </div>
    )
}
