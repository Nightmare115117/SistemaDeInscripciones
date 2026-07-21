import { LiquidGlassCard } from "@/components-custom/GlassCard";
import { AnimatePresence, motion } from "motion/react";
import { Countdown } from "./components-custom/Countdown";
import { useEffect, useState } from "react";
import { LocationMap } from "./components-custom/InteractiveMap";
import { Navbar } from "./components-custom/Navbar";
import "@/css-custom/App.css"
import { SegmentedControl } from "./components-custom/SegmentOption";

function App() {
  const [selectedDay, setSelectedDay] = useState("dia1");
  const [total, setTotal] = useState(0);

  //https://sistemadeinscripciones.onrender.com/

  useEffect(() => {
    fetch('https://sistemadeinscripciones.onrender.com/api/registro/count', {})
    .then((res) => res.json())
    .then((data) => setTotal(data.total))
    .catch((err) => console.error(err));
  }, []);

  const schedule: Record<string, { time: string; activity: string }[]> = {
    dia1: [
      { time: "12:30", activity: "Registro" },
      { time: "14:30", activity: "Inauguración" },
      { time: "15:00", activity: "Competencia" },
      { time: "19:00", activity: "Cena" },
    ],
    dia2: [
      { time: "8:00", activity: "Desayuno" },
      { time: "9:00", activity: "Competencia" },
      { time: "13:00", activity: "Comida" },
      { time: "14:00", activity: "Ultima hora de Competencia" },
      { time: "15:00", activity: "Presentacion y Evaluación"},
      { time: "18:00", activity: "Deliveración del Jurado"},
      { time: "18:30", activity: "Premiación y clausura"},
    ],
  };

  return (
    <main className="relative min-h-screen overflow-hidden">

        <Navbar />

      <section
        id="inicio"
        className="
          min-h-screen
          flex
          items-center
          justify-center
          px-4 sm:px-6
          pt-24 md:pt-32
        "
      >
        <LiquidGlassCard className="p-5 sm:p-8 flex flex-col items-center gap-4 sm:gap-6 text-center">
          <h1 className="text-3xl sm:text-4xl md:text-6xl font-bold text-[#d3d3d3] md:text-black">
            Bienvenido
          </h1>
          <Countdown targetDate={new Date("2026-10-12T12:30:00")} />
          <h2 className="text-sm sm:text-base text-white/70">{47 - total} lugares disponibles</h2>
        </LiquidGlassCard>
      </section>


      <section
        id="itinerario"
        className="
          min-h-screen
          flex
          items-center
          justify-center
          px-4 sm:px-6
        "
      >
        <LiquidGlassCard className="p-5 sm:p-8 w-full max-w-lg flex flex-col items-center" interactive>
          <h2 className="text-2xl sm:text-3xl md:text-4xl font-bold">
            Itinerario
          </h2>

          <SegmentedControl
            options={[
              { id: "dia1", label: "Día 1" },
              { id: "dia2", label: "Día 2" },
            ]}
            value={selectedDay}
            onChange={setSelectedDay}
            className="mt-6 sm:mt-8"
          />

          <div className="w-full min-h-[220px] mt-6">
            <AnimatePresence mode="wait">
              <motion.ul
                key={selectedDay}
                initial={{ opacity: 0, x: 12 }}
                animate={{ opacity: 1, x: 0 }}
                exit={{ opacity: 0, x: -12 }}
                transition={{ duration: 0.25, ease: "easeOut" }}
                className={
                  selectedDay === "dia2"
                    ? "grid grid-cols-1 sm:grid-cols-2 gap-2 sm:gap-3"
                    : "flex flex-col gap-2 sm:gap-3"
                }
              >
                {schedule[selectedDay].map((item) => (
                  <li
                    key={item.time}
                    className="flex items-center gap-3 sm:gap-4 rounded-2xl bg-white/5 px-3 sm:px-4 py-2.5 sm:py-3"
                  >
                    <span className="text-xs sm:text-sm font-semibold text-white/70 w-12 sm:w-14 shrink-0">
                      {item.time}
                    </span>
                    <span className="text-sm sm:text-base text-white">{item.activity}</span>
                  </li>
                ))}
              </motion.ul>
            </AnimatePresence>
          </div>
        </LiquidGlassCard>
      </section>


      <section
        id="ubicacion"
        className="
          min-h-screen
          flex
          items-center
          justify-center
          px-4 sm:px-6
        "
      >
        <LiquidGlassCard className="p-5 sm:p-8 w-full max-w-5xl">
          <div className="flex flex-col md:flex-row gap-5 md:gap-8">
            <div className="flex-1 min-w-0 min-h-[560px]">
              <LocationMap />
            </div>

            <div className="md:w-[360px] flex flex-col items-center justify-center text-center">
              <h3 className="text-lg sm:text-xl font-semibold">Instituto Tecnológico de Saltillo</h3>
              <p className="text-sm sm:text-base text-white/70">Blvd. Venustiano Carranza...</p>
            </div>
          </div>
        </LiquidGlassCard>
      </section>

      <section
        id="requisitos"
        className="
          min-h-screen
          flex
          items-center
          justify-center
          px-4 sm:px-6
          "
          >
        <LiquidGlassCard className="p-5 sm:p-8">
          <h2 className="text-2xl sm:text-3xl md:text-4xl font-bold">
            Requisitos
          </h2>
        </LiquidGlassCard>
      </section>
      

      <section
        id="faq"
        className="
          min-h-screen
          flex
          items-center
          justify-center
          px-4 sm:px-6
        "
      >
        <LiquidGlassCard className="p-5 sm:p-8">
          <h2 className="text-2xl sm:text-3xl md:text-4xl font-bold">
            Preguntas frecuentes
          </h2>
        </LiquidGlassCard>
      </section>
    </main>
  );
}

export default App;
