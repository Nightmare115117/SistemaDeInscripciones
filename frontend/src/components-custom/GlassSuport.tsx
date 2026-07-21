/**
 * Detecta si el navegador es Chromium (Chrome, Edge, Brave, Opera) —
 * el único motor que hoy interpreta `backdrop-filter: url(#id) ...`
 * como referencia real a un filtro SVG y aplica la distorsión.
 *
 * Firefox y Safari aceptan la sintaxis sin marcar error (por eso probar
 * con `el.style.backdropFilter = '...'` no es confiable: el navegador
 * puede "aceptar" el string sin realmente aplicar el efecto), así que
 * en vez de feature-detection usamos detección de motor:
 *
 * 1. `navigator.userAgentData.brands` (API moderna, expuesta solo por
 *    navegadores Chromium) — método preferido, no depende de parsear texto.
 * 2. Si no existe esa API (Firefox y Safari no la implementan), caemos
 *    a revisar el user-agent string clásico.
 */
export function isChromiumBrowser(): boolean {
  if (typeof navigator === "undefined") return false; // SSR safety

  // Chrome, Edge, Brave, Opera exponen esta API; Firefox y Safari no.
  const uaData = (navigator as Navigator & {
    userAgentData?: { brands: { brand: string }[] };
  }).userAgentData;

  if (uaData?.brands) {
    return uaData.brands.some((b) =>
      /Chrom(?:e|ium)|Google Chrome|Microsoft Edge|Brave|Opera/i.test(b.brand)
    );
  }

  // Fallback: user-agent string clásico (Safari y Firefox no exponen
  // userAgentData, así que siempre caen aquí).
  const ua = navigator.userAgent;
  const isFirefox = /Firefox/i.test(ua);
  const isSafari = /Safari/i.test(ua) && !/Chrome|Chromium|CriOS|CrMo|OPR|Edg|Brave/i.test(ua);
  const isChromium = /Chrome|Chromium|CriOS|CrMo|OPR|Edg|Brave/i.test(ua);

  return isChromium && !isFirefox && !isSafari;
}

/**
 * Aplica un atributo data-glass="svg" | "fallback" en <html>, para que el CSS
 * pueda dar estilos distintos a cada caso sin necesitar JS en cada componente.
 */
export function applyGlassSupportAttribute(): void {
  const supported = isChromiumBrowser();
  document.documentElement.dataset.glass = supported ? "svg" : "fallback";
}