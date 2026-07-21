import { useState, useRef, useEffect } from "react";
import { motion } from "motion/react";
import { cn } from "@/lib/utils";

interface SegmentOption {
  id: string;
  label: string;
}

interface SegmentedControlProps {
  options: SegmentOption[];
  value: string;
  onChange: (id: string) => void;
  className?: string;
}

/**
 * Toggle tipo "píldora deslizante" (mismo mecanismo que el navbar):
 * mide la posición real de cada botón con getBoundingClientRect y anima
 * left/width directamente, sin usar layoutId/scale de motion, para
 * evitar el bug de clipping del backdrop-filter en las esquinas.
 */
export function SegmentedControl({
  options,
  value,
  onChange,
  className,
}: SegmentedControlProps) {
  const [pillStyle, setPillStyle] = useState({ left: 0, width: 0 });
  const buttonRefs = useRef<Record<string, HTMLButtonElement | null>>({});
  const containerRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    const el = buttonRefs.current[value];
    const container = containerRef.current;
    if (el && container) {
      const elRect = el.getBoundingClientRect();
      const containerRect = container.getBoundingClientRect();
      setPillStyle({
        left: elRect.left - containerRect.left,
        width: elRect.width,
      });
    }
  }, [value, options]);

  return (
    <div
      ref={containerRef}
      className={cn(
        "relative inline-flex items-center gap-1 rounded-full bg-white/5 p-1",
        className
      )}
    >
      <motion.div
        className="absolute top-1 bottom-1 rounded-full border border-white/20 bg-white/15 backdrop-blur-xl shadow-lg shadow-white/10"
        animate={{ left: pillStyle.left, width: pillStyle.width }}
        transition={{ type: "spring", stiffness: 400, damping: 30 }}
      />
      {options.map((option) => (
        <button
          key={option.id}
          ref={(el) => {
            buttonRefs.current[option.id] = el;
          }}
          onClick={() => onChange(option.id)}
          className={cn(
            "relative z-10 rounded-full px-3 sm:px-5 py-1.5 sm:py-2 text-xs sm:text-sm font-medium transition-colors duration-300",
            value === option.id ? "text-white" : "text-white/60 hover:text-white/80"
          )}
        >
          {option.label}
        </button>
      ))}
    </div>
  );
}