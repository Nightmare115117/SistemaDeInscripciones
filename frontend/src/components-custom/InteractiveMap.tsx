import { useEffect, useRef, useState } from "react";

export function LocationMap() {
  const [visible, setVisible] = useState(false);
  const ref = useRef<HTMLDivElement | null>(null);

  useEffect(() => {
    const element = ref.current;
    if (!element) return;

    const observer = new IntersectionObserver(
      ([entry]) => {
        if (entry.isIntersecting) {
          setVisible(true);
          observer.disconnect();
        }
      },
      { rootMargin: "200px" }
    );

    observer.observe(element);
    return () => observer.disconnect();
  }, []);

  return (
    <div ref={ref} className="min-h-[560px] overflow-hidden rounded-[40px] bg-white/5">
      {visible ? (
        <iframe
          src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d3602.8956428427405!2d-100.9954106235086!3d25.44175222174982!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x86886d59eb5bc4d3%3A0x60771ba46d37358c!2sTecnol%C3%B3gico%20Nacional%20de%20M%C3%A9xico%20Campus%20Saltillo!5e0!3m2!1ses-419!2smx!4v1784044953519!5m2!1ses-419!2smx"
          width="100%"
          style={{ border: 0, minHeight: 560 }}
          allowFullScreen
          loading="lazy"
          referrerPolicy="no-referrer-when-downgrade"
          className="h-full w-full rounded-[40px]"
        />
      ) : (
        <div className="h-full w-full rounded-[40px] bg-zinc-950/70" />
      )}
    </div>
  );
}

