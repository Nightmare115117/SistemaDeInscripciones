import { motion, useMotionValue, useTransform, AnimatePresence } from "motion/react";
import { useEffect, useState } from "react";
import { cn } from "@/lib/utils";

export function LiquidGlassCard({
  className,
  children,
  interactive = false,
  style,
  variant = "card",
}: {
  className?: string;
  children: React.ReactNode;
  interactive?: boolean;
  style?: React.CSSProperties;
  variant?: "card" | "nav";
}) {
  const mouseX = useMotionValue(0);
  const mouseY = useMotionValue(0);
  const [isHovering, setIsHovering] = useState(false);
  const [allowHover, setAllowHover] = useState(false);

  useEffect(() => {
    const media = window.matchMedia("(hover: hover) and (pointer: fine)");
    setAllowHover(media.matches);

    const handleChange = () => setAllowHover(media.matches);
    media.addEventListener("change", handleChange);

    return () => media.removeEventListener("change", handleChange);
  }, []);

  const isInteractive = interactive && allowHover;

  function handleMouseMove(e: React.MouseEvent<HTMLDivElement>) {
    if (!isInteractive) return;
    const rect = e.currentTarget.getBoundingClientRect();
    mouseX.set(e.clientX - rect.left);
    mouseY.set(e.clientY - rect.top);
  }

  const highlight = useTransform(
    [mouseX, mouseY],
    ([x, y]) => `radial-gradient(circle 180px at ${x}px ${y}px, rgba(255,255,255,0.25), transparent 70%)`
  );

  return (
    <motion.div
      onMouseMove={handleMouseMove}
      onMouseEnter={() => isInteractive && setIsHovering(true)}
      onMouseLeave={() => isInteractive && setIsHovering(false)}
      whileHover={isInteractive ? { scale: 1.01 } : undefined}
      transition={isInteractive ? { type: "spring", stiffness: 300, damping: 25 } : undefined}
      className={cn(
        variant === "nav" ? "glass-nav" : "glass-liquid",
        "relative isolate overflow-hidden rounded-[40px] p-6",
        className
      )}
      style={style}
    >
      {interactive && (
        <AnimatePresence>
          {isHovering && (
            <motion.div
              initial={{ opacity: 0 }}
              animate={{ opacity: 1 }}
              exit={{ opacity: 0 }}
              transition={{ duration: 0.2 }}
              className="pointer-events-none absolute inset-0"
              style={{ background: highlight }}
            />
          )}
        </AnimatePresence>
      )}
      <div className="relative z-10">{children}</div>
    </motion.div>
  );
}