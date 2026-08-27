-- Fixes for the existing Supabase schema.
-- Apply this file after esquema.sql using the postgres role.
-- It preserves existing data.

begin;

-- The C++ registration flow creates the team before its leader and uses -1
-- for optional relationships. These columns must accept NULL temporarily.
alter table public.equipo
    alter column idlider drop not null;

alter table public.alumnos
    alter column idcontacto drop not null,
    alter column iduniversidad drop not null;

-- Normalize the -1 sentinel before foreign keys are checked.
create or replace function public.normalizar_ids_opcionales()
returns trigger
language plpgsql
set search_path = public
as $$
begin
    if tg_table_name = 'equipo' then
        if new.iduniversidad = -1 then new.iduniversidad := null; end if;
        if new.idlider = -1 then new.idlider := null; end if;
    elsif tg_table_name = 'alumnos' then
        if new.idcontacto = -1 then new.idcontacto := null; end if;
        if new.iduniversidad = -1 then new.iduniversidad := null; end if;
    end if;
    return new;
end;
$$;

drop trigger if exists equipo_normalizar_ids on public.equipo;
create trigger equipo_normalizar_ids
before insert or update on public.equipo
for each row execute function public.normalizar_ids_opcionales();

drop trigger if exists alumno_normalizar_ids on public.alumnos;
create trigger alumno_normalizar_ids
before insert or update on public.alumnos
for each row execute function public.normalizar_ids_opcionales();

-- Keep the fields used by the API available even if an older deployment
-- created the tables without them.
alter table public.alumnos
    add column if not exists institucion varchar(200) not null default '';

alter table public.equipo
    add column if not exists estado varchar(20) not null default 'pendiente',
    add column if not exists registrado_en timestamptz not null default now(),
    add column if not exists revisado_en timestamptz,
    add column if not exists notas text;

alter table public.problematica
    add column if not exists codigo varchar(20);

-- Validate allowed values without failing if the constraint already exists.
do $$
begin
    if not exists (
        select 1 from pg_constraint
        where conrelid = 'public.equipo'::regclass
          and conname = 'equipo_estado_ck'
    ) then
        alter table public.equipo add constraint equipo_estado_ck
            check (estado in ('pendiente', 'aceptado', 'rechazado'));
    end if;
end;
$$;

create index if not exists equipo_estado_idx on public.equipo (estado);
create index if not exists alumnos_idequipo_idx on public.alumnos (idequipo);
create index if not exists registro_idequipo_idx on public.registro (idequipo);
create unique index if not exists problematica_codigo_uidx
    on public.problematica (codigo) where codigo is not null;

-- The current dump already contains these relationships. Add only missing ones.
do $$
begin
    if not exists (select 1 from pg_constraint where conname = 'equipo_universidad_fk') then
        alter table public.equipo add constraint equipo_universidad_fk
            foreign key (iduniversidad) references public.universidades(iduniversidad);
    end if;
    if not exists (select 1 from pg_constraint where conname = 'equipo_problematica_fk') then
        alter table public.equipo add constraint equipo_problematica_fk
            foreign key (idproblematica) references public.problematica(idproblematica);
    end if;
    if not exists (select 1 from pg_constraint where conname = 'alumno_equipo_fk') then
        alter table public.alumnos add constraint alumno_equipo_fk
            foreign key (idequipo) references public.equipo(idequipo) on delete cascade;
    end if;
    if not exists (select 1 from pg_constraint where conname = 'alumno_contacto_fk') then
        alter table public.alumnos add constraint alumno_contacto_fk
            foreign key (idcontacto) references public.contactoemergencia(idcontacto) on delete set null;
    end if;
    if not exists (select 1 from pg_constraint where conname = 'alumno_universidad_fk') then
        alter table public.alumnos add constraint alumno_universidad_fk
            foreign key (iduniversidad) references public.universidades(iduniversidad) on delete set null;
    end if;
    if not exists (select 1 from pg_constraint where conname = 'registro_equipo_fk') then
        alter table public.registro add constraint registro_equipo_fk
            foreign key (idequipo) references public.equipo(idequipo) on delete cascade;
    end if;
end;
$$;

-- Ensure the contract tables exist and contain every API field.
create table if not exists public.evento_configuracion (
    id smallint primary key default 1 check (id = 1),
    cupo integer not null default 60 check (cupo > 0),
    min_integrantes smallint not null default 3 check (min_integrantes > 0),
    max_integrantes smallint not null default 5 check (max_integrantes >= min_integrantes),
    registro_abierto boolean not null default true,
    updated_at timestamptz not null default now()
);
alter table public.evento_configuracion
    add column if not exists cupo integer not null default 60,
    add column if not exists min_integrantes smallint not null default 3,
    add column if not exists max_integrantes smallint not null default 5,
    add column if not exists registro_abierto boolean not null default true,
    add column if not exists updated_at timestamptz not null default now();
insert into public.evento_configuracion (id) values (1) on conflict (id) do nothing;

create table if not exists public.correo_configuracion (
    id smallint primary key default 1 check (id = 1),
    configurado boolean not null default false,
    habilitado boolean not null default false,
    remitente varchar(320) not null default '',
    updated_at timestamptz not null default now()
);
alter table public.correo_configuracion
    add column if not exists configurado boolean not null default false,
    add column if not exists habilitado boolean not null default false,
    add column if not exists remitente varchar(320) not null default '',
    add column if not exists updated_at timestamptz not null default now();
insert into public.correo_configuracion (id) values (1) on conflict (id) do nothing;

commit;

-- Backend changes still required outside SQL:
-- 1. AlumnoRepository::findAll needs a comma after idcontacto.
-- 2. AlumnoRepository::update needs THEN instead of THE and no comma before WHERE.
-- 3. AlumnoRepository::insert must include institucion.
-- 4. EquipoRepository::update must pass all five parameters used by $1..$5.
-- 5. Registration must use one database transaction to avoid partial inserts.
