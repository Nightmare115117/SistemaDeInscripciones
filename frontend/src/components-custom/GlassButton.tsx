import { Button } from "@/components/ui/button";
import { cn } from "@/lib/utils";

export function GlassButton({
  className,
  ...props
}: React.ComponentProps<typeof Button>) {
  return (
    <Button
      className={cn(
        "rounded-full border border-white/20 bg-white/10 backdrop-blur-xl text-white hover:bg-white/20 shadow-lg",
        className
      )}
      {...props}
    />
  );
}