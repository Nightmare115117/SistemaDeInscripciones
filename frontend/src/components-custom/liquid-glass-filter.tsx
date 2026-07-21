import { useEffect, useState } from "react";

export function LiquidGlassFilter() {
  const [enabled, setEnabled] = useState(false);

  useEffect(() => {
    const media = window.matchMedia("(hover: hover) and (pointer: fine)");
    setEnabled(media.matches);

    const handleChange = () => setEnabled(media.matches);
    media.addEventListener("change", handleChange);
    return () => media.removeEventListener("change", handleChange);
  }, []);

  if (!enabled) return null;

  return (
    <svg width="0" height="0" style={{ position: "absolute" }} aria-hidden="true">
      <filter id="lg-filter" x="0" y="0" width="100%" height="100%" colorInterpolationFilters="sRGB">
        <feImage
          x="0"
          y="0"
          width="420"
          height="280"
          href="data:image/svg+xml;utf8,%3Csvg%20xmlns%3D%22http%3A%2F%2Fwww.w3.org%2F2000%2Fsvg%22%20width%3D%22420%22%20height%3D%22280%22%20viewBox%3D%220%200%20420%20280%22%3E%3Cdefs%3E%3ClinearGradient%20id%3D%22Y%22%20x1%3D%220%22%20x2%3D%220%22%20y1%3D%223%25%22%20y2%3D%2297%25%22%3E%3Cstop%20offset%3D%220%25%22%20stop-color%3D%22%230F0%22%2F%3E%3Cstop%20offset%3D%22100%25%22%20stop-color%3D%22%23000%22%2F%3E%3C%2FlinearGradient%3E%3ClinearGradient%20id%3D%22X%22%20x1%3D%222%25%22%20x2%3D%2298%25%22%20y1%3D%220%22%20y2%3D%220%22%3E%3Cstop%20offset%3D%220%25%22%20stop-color%3D%22%23F00%22%2F%3E%3Cstop%20offset%3D%22100%25%22%20stop-color%3D%22%23000%22%2F%3E%3C%2FlinearGradient%3E%3C%2Fdefs%3E%3Crect%20width%3D%22420%22%20height%3D%22280%22%20fill%3D%22%23808080%22%2F%3E%3Cg%20filter%3D%22blur(2px)%22%3E%3Crect%20width%3D%22420%22%20height%3D%22280%22%20fill%3D%22%23000080%22%2F%3E%3Crect%20width%3D%22420%22%20height%3D%22280%22%20fill%3D%22url(%23Y)%22%20style%3D%22mix-blend-mode%3Ascreen%22%2F%3E%3Crect%20width%3D%22420%22%20height%3D%22280%22%20fill%3D%22url(%23X)%22%20style%3D%22mix-blend-mode%3Ascreen%22%2F%3E%3Crect%20x%3D%2214%22%20y%3D%2214%22%20width%3D%22392%22%20height%3D%22252%22%20rx%3D%2226%22%20ry%3D%2226%22%20fill%3D%22%23808080%22%20filter%3D%22blur(14px)%22%2F%3E%3C%2Fg%3E%3C%2Fsvg%3E"
          result="displacementMap"
        />
        <feDisplacementMap in="SourceGraphic" in2="displacementMap" scale="35" xChannelSelector="R" yChannelSelector="G" />
        <feColorMatrix
          type="matrix"
          result="displacedR"
          values="1 0 0 0 0  0 0 0 0 0  0 0 0 0 0  0 0 0 1 0"
        />
        <feDisplacementMap in="SourceGraphic" in2="displacementMap" scale="34" xChannelSelector="R" yChannelSelector="G" />
        <feColorMatrix
          type="matrix"
          result="displacedG"
          values="0 0 0 0 0  0 1 0 0 0  0 0 0 0 0  0 0 0 1 0"
        />
        <feDisplacementMap in="SourceGraphic" in2="displacementMap" scale="33" xChannelSelector="R" yChannelSelector="G" />
        <feColorMatrix
          type="matrix"
          result="displacedB"
          values="0 0 0 0 0  0 0 0 0 0  0 0 1 0 0  0 0 0 1 0"
        />
        <feBlend in="displacedR" in2="displacedG" mode="screen" result="rg" />
        <feBlend in="rg" in2="displacedB" mode="screen" />
      </filter>
    </svg>
  );
}