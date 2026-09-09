-- Correcciones de esquema para el backend actual
-- Ejecutar sobre la base de datos del proyecto.

DO $$
BEGIN
  -- Quitar UNIQUE de id_pais si existiera.
  IF EXISTS (
    SELECT 1
    FROM pg_constraint
    WHERE conname = 'alumnos_id_pais_key'
  ) THEN
    ALTER TABLE public.alumnos DROP CONSTRAINT alumnos_id_pais_key;
  END IF;
END $$;

ALTER TABLE public.alumnos
  ADD COLUMN IF NOT EXISTS edad smallint,
  ADD COLUMN IF NOT EXISTS acepta_codigo_conducta_mlh boolean NOT NULL DEFAULT false,
  ADD COLUMN IF NOT EXISTS acepta_compartir_datos_mlh boolean NOT NULL DEFAULT false,
  ADD COLUMN IF NOT EXISTS acepta_correos_mlh boolean NOT NULL DEFAULT false,
  ADD COLUMN IF NOT EXISTS correo_verificado boolean NOT NULL DEFAULT false;

ALTER TABLE public.alumnos
  ALTER COLUMN id_pais DROP NOT NULL;

CREATE TABLE IF NOT EXISTS public.email_verification_tokens (
  id bigint GENERATED ALWAYS AS IDENTITY NOT NULL,
  equipo_id bigint NOT NULL,
  email character varying NOT NULL,
  token character varying NOT NULL UNIQUE,
  usado boolean NOT NULL DEFAULT false,
  creado_en timestamp with time zone NOT NULL DEFAULT now(),
  expira_en timestamp with time zone NOT NULL DEFAULT (now() + interval '24 hours'),
  CONSTRAINT email_verification_tokens_pkey PRIMARY KEY (id),
  CONSTRAINT email_verification_tokens_equipo_id_fkey
    FOREIGN KEY (equipo_id) REFERENCES public.equipo(idequipo)
);

CREATE TABLE IF NOT EXISTS public.correo_configuracion (
  id smallint NOT NULL DEFAULT 1 CHECK (id = 1),
  configurado boolean NOT NULL DEFAULT false,
  habilitado boolean NOT NULL DEFAULT false,
  remitente character varying NOT NULL DEFAULT ''::character varying,
  CONSTRAINT correo_configuracion_pkey PRIMARY KEY (id)
);

INSERT INTO public.evento_configuracion (id, cupo, min_integrantes, max_integrantes, registro_abierto, updated_at)
VALUES (1, 60, 3, 5, true, NOW())
ON CONFLICT (id) DO UPDATE
SET cupo = EXCLUDED.cupo,
    min_integrantes = EXCLUDED.min_integrantes,
    max_integrantes = EXCLUDED.max_integrantes,
    registro_abierto = EXCLUDED.registro_abierto,
    updated_at = NOW();

INSERT INTO public.correo_configuracion (id, configurado, habilitado, remitente)
VALUES (1, false, false, '')
ON CONFLICT (id) DO NOTHING;

INSERT INTO public.paises (nombre, codigo_iso2, codigo_iso3)
VALUES
  ('México', 'MX', 'MEX'),
  ('Estados Unidos', 'US', 'USA'),
  ('España', 'ES', 'ESP'),
  ('Colombia', 'CO', 'COL'),
  ('Argentina', 'AR', 'ARG')
ON CONFLICT (codigo_iso2) DO NOTHING;

INSERT INTO public.nivel_estudio (nivel)
VALUES
  ('Secundaria'),
  ('Preparatoria / Bachillerato'),
  ('Licenciatura'),
  ('Maestría'),
  ('Doctorado'),
  ('Otro')
ON CONFLICT DO NOTHING;

INSERT INTO public.universidades (nombre_uni)
VALUES
  ('Universidad Nacional Autónoma de México'),
  ('Tecnológico de Monterrey'),
  ('Universidad de Guadalajara'),
  ('Universidad Anáhuac'),
  ('Otra')
ON CONFLICT DO NOTHING;
