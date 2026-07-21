import { useState, useRef, useEffect } from "react";
import { motion } from "motion/react";
import { Menu } from "lucide-react";
import { Button } from "@/components/ui/button";
import {
  Sheet,
  SheetContent,
  SheetHeader,
  SheetTitle,
  SheetTrigger,
} from "@/components/ui/sheet";
import { LiquidGlassCard } from "@/components-custom/GlassCard";

export function Navbar() {
  const navLink =
    "relative z-10 rounded-full px-4 py-2 flex items-center justify-center text-white/80 transition-colors duration-300 hover:text-white";

  const [active, setActive] = useState("inicio");
  const [pillStyle, setPillStyle] = useState({ left: 0, width: 0 });
  const [menuOpen, setMenuOpen] = useState(false);
  const linkRefs = useRef<Record<string, HTMLAnchorElement | null>>({});
  const containerRef = useRef<HTMLDivElement>(null);

  const links = [
    { id: "inicio", label: "Inicio" },
    { id: "itinerario", label: "Itinerario" },
    { id: "ubicacion", label: "Ubicación" },
    { id: "requisitos", label: "Requisitos" },
    { id: "faq", label: "FAQ" },
  ];

  useEffect(() => {
    const sections = links
      .map((link) => document.getElementById(link.id))
      .filter((el): el is HTMLElement => el !== null);

    if (sections.length === 0) return;

    const observer = new IntersectionObserver(
      (entries) => {
        const visible = entries
          .filter((entry) => entry.isIntersecting)
          .sort((a, b) => b.intersectionRatio - a.intersectionRatio);

        if (visible.length > 0) {
          setActive(visible[0].target.id);
        }
      },
      {
        rootMargin: "-40% 0px -40% 0px",
        threshold: [0, 0.25, 0.5, 0.75, 1],
      }
    );

    sections.forEach((section) => observer.observe(section));

    return () => observer.disconnect();
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  useEffect(() => {
    const el = linkRefs.current[active];
    const container = containerRef.current;
    if (el && container) {
      const elRect = el.getBoundingClientRect();
      const containerRect = container.getBoundingClientRect();
      setPillStyle({
        left: elRect.left - containerRect.left,
        width: elRect.width,
      });
    }
  }, [active]);

  return (
    <>
      {/* ============ DESKTOP (md y más grande) ============ */}
      {/* Navbar centrado con píldora deslizante */}
      <motion.header
        initial={{ y: -30, opacity: 0 }}
        animate={{ y: 0, opacity: 1 }}
        transition={{ duration: 0.4 }}
        className="fixed top-4 left-1/2 -translate-x-1/2 z-50 hidden w-full max-w-3xl px-6 md:block"
      >
        <LiquidGlassCard variant="nav" className="px-6 py-3" >
          <nav className="flex items-center justify-between">
            <div className="flex items-center gap-3">
              <img
                src="/blanco_sin_fondo.png"
                className="h-10 w-10 scale-150 object-contain"
                alt="Logo"
              />
              <h1 className="text-xl font-bold text-white">Hackathon</h1>
            </div>

            <div
              ref={containerRef}
              className="relative flex items-center gap-1 rounded-full bg-white/5 p-1"
            >
              <motion.div
                className="absolute top-1 bottom-1 rounded-full border border-white/20 bg-white/10 backdrop-blur-xl shadow-lg shadow-white/10"
                animate={{ left: pillStyle.left, width: pillStyle.width }}
                transition={{ type: "spring", stiffness: 400, damping: 30 }}
              />
              {links.map((link) => (
                <a
                  key={link.id}
                  ref={(el) => {
                    linkRefs.current[link.id] = el;
                  }}
                  href={`#${link.id}`}
                  onClick={() => setActive(link.id)}
                  className={navLink}
                >
                  <span
                    className={`transition-colors ${
                      active === link.id ? "text-white" : "text-white/80"
                    }`}
                  >
                    {link.label}
                  </span>
                </a>
              ))}
            </div>
          </nav>
        </LiquidGlassCard>
      </motion.header>

      {/* Botones fijos en la esquina, solo en desktop */}
      <motion.div
        initial={{ y: -30, opacity: 0 }}
        animate={{ y: 0, opacity: 1 }}
        transition={{ duration: 0.4, delay: 0.1 }}
        className="fixed top-4 right-6 z-50 hidden md:flex items-center gap-3"
      >
        <Button
          variant="ghost"
          className="glass-liquid !rounded-full px-6 py-5 text-white hover:text-white hover:bg-white/10"
        >
          Registrarme
        </Button>

        <Button
          variant="ghost"
          className="!rounded-full px-4 text-white/70 hover:text-white hover:bg-transparent underline underline-offset-4"
        >
          Reglamento
        </Button>
      </motion.div>

      {/* ============ MOBILE (sólo móvil) ============ */}
      {/* Barra compacta: logo + botón "Registrarme" + hamburguesa que abre el menú */}
      <header className="fixed bottom-3 left-1/2 z-50 w-[min(92vw,420px)] -translate-x-1/2 md:hidden">
        <LiquidGlassCard variant="nav" className="px-4 py-2.5">
          <div className="flex items-center justify-between gap-2">
            <div className="flex items-center gap-2">
              <img
                src="/blanco_sin_fondo.png"
                className="h-7 w-7 scale-150 object-contain"
                alt="Logo"
              />
              <h1 className="text-sm font-bold text-white">Hackathon</h1> 
            </div>

            <div className="flex items-center gap-1.5">
              <Button
                size="sm"
                variant="ghost"
                className="glass-liquid !rounded-full px-4 text-xs text-white hover:text-white hover:bg-white/10"
              >
                Registro
              </Button>

              <Sheet open={menuOpen} onOpenChange={setMenuOpen}>
                <SheetTrigger asChild>
                  <Button
                    size="icon"
                    variant="ghost"
                    className="!rounded-full text-white hover:bg-white/10"
                    aria-label="Abrir menú"
                  >
                    <Menu className="h-5 w-5" />
                  </Button>
                </SheetTrigger>
                <SheetContent
                  side="right"
                  className="bg-zinc-900 p-0"
                >
                  <motion.div
                    initial={{ opacity: 0, x: 40 }}
                    animate={{ opacity: 1, x: 0 }}
                    transition={{ duration: 0.25, ease: "easeOut" }}
                    className="flex h-full flex-col"
                  >
                    <SheetHeader className="border-b border-white/10 px-6 py-5">
                      <SheetTitle className="text-white">Hackathon</SheetTitle>
                    </SheetHeader>

                    <motion.nav
                      className="flex flex-col gap-1 px-4 py-4"
                      variants={{
                        hidden: {},
                        visible: {
                          transition: {
                            staggerChildren: 0.05,
                            delayChildren: 0.05,
                          },
                        },
                      }}
                      initial="hidden"
                      animate="visible"
                    >
                      {links.map((link) => (
                        <motion.a
                          key={link.id}
                          href={`#${link.id}`}
                          onClick={() => {
                            setActive(link.id);
                            setMenuOpen(false);
                          }}
                          variants={{
                            hidden: { opacity: 0, x: 20 },
                            visible: { opacity: 1, x: 0 },
                          }}
                          transition={{ duration: 0.2 }}
                          className={`rounded-xl px-4 py-3 text-base transition-colors ${
                            active === link.id
                              ? "bg-white/10 text-white"
                              : "text-white/70 hover:bg-white/5 hover:text-white"
                          }`}
                        >
                          {link.label}
                        </motion.a>
                      ))}
                    </motion.nav>

                    <motion.div
                      initial={{ opacity: 0, y: 20 }}
                      animate={{ opacity: 1, y: 0 }}
                      transition={{ delay: 0.25, duration: 0.25 }}
                      className="mt-auto flex flex-col gap-2 border-t border-white/10 px-4 py-4"
                    >
                      <Button
                        variant="ghost"
                        className="glass-liquid !rounded-full text-white hover:text-white hover:bg-white/10"
                        onClick={() => setMenuOpen(false)}
                      >
                        Registrarme
                      </Button>

                      <Button
                        variant="ghost"
                        className="!rounded-full text-white/70 hover:text-white hover:bg-transparent underline underline-offset-4"
                        onClick={() => setMenuOpen(false)}
                      >
                        Reglamento
                      </Button>
                    </motion.div>
                  </motion.div>
                </SheetContent>
              </Sheet>
            </div>
          </div>
        </LiquidGlassCard>
      </header>
    </>
  );
}