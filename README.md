# SistemaRegistroHackaton — Backend

*[English version below](#english-version)*

API REST en C++ para el registro de equipos, alumnos y problemáticas de un hackathon. Construida con [Crow](https://crowcpp.org/) como microframework web y PostgreSQL como base de datos, usando [libpqxx](https://github.com/jtv/libpqxx) para la conexión.

## Stack

- **C++20**
- **[Crow](https://github.com/CrowCpp/Crow)** — microframework HTTP (descargado automáticamente vía `FetchContent`)
- **PostgreSQL** + **libpqxx** — acceso a base de datos
- **CMake** (3.14+) — sistema de build
- **Docker** — imagen lista para despliegue (Ubuntu 24.04)

## Arquitectura

El proyecto sigue una arquitectura por capas, organizada por módulo de dominio:

```
src/
├── Generics/              # Clases base genéricas (Controller, Service, Repository, Entity)
├── DBConfig/               # Carga de configuración y conexión a la base de datos
├── Registro/                # Registros de asistencia/participación
├── Equipo/                   # Equipos, alumnos y contactos de emergencia
├── Universidad/            # Universidades participantes
├── Problematica/           # Problemáticas/retos del hackathon
└── main.cpp                  # Punto de entrada, registro de rutas
```

Cada módulo de dominio sigue el mismo patrón:

```
Modulo/
├── Models/         # Entidad (hereda de Generics/Entity)
├── Repositories/   # Acceso a datos (consultas SQL con pqxx)
├── Services/       # Lógica de negocio
└── Controllers/    # Rutas HTTP y serialización JSON
```

Las clases genéricas en `Generics/` (`Controller`, `Service`, `Repository`, `Entity`) implementan el CRUD estándar una sola vez mediante templates, y cada módulo las especializa para su entidad correspondiente.

## Requisitos

- Compilador con soporte C++20 (GCC 10+ o Clang 12+)
- CMake 3.14+
- PostgreSQL (servidor accesible, local o remoto)
- Librerías de desarrollo:

```bash
# Debian/Ubuntu
sudo apt-get install build-essential cmake git pkg-config \
    libasio-dev libboost-all-dev libssl-dev libpq-dev libpqxx-dev

# Arch Linux
sudo pacman -S base-devel cmake git pkgconf asio boost libpqxx postgresql-libs
```

> Crow se descarga automáticamente durante la configuración de CMake (`FetchContent`), no requiere instalación manual.

## Configuración

El proyecto lee la cadena de conexión a la base de datos desde la variable de entorno `DATABASE_URL`, ya sea exportada en el sistema o definida en un archivo `.env` en la raíz del proyecto:

```bash
# .env
DATABASE_URL=postgresql://usuario:contraseña@host:puerto/nombre_bd
```

> `.env` está incluido en `.gitignore` — nunca subas credenciales al repositorio.

## Compilación

```bash
mkdir -p build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
make -j$(nproc)
```

El binario resultante es `build/mi_api`.

## Ejecución

```bash
./build/mi_api
```

El servidor levanta en el puerto **8080**.

## Docker

También puedes construir y correr el proyecto en un contenedor (usa Ubuntu 24.04 y compila `libpqxx` desde código fuente):

```bash
docker build -t sistema-registro-backend .
docker run -p 8080:8080 --env-file .env sistema-registro-backend
```

## Endpoints

Cada módulo expone el mismo conjunto de rutas CRUD sobre su `basePath`:

| Método | Ruta                    | Descripción            |
|--------|-------------------------|-------------------------|
| GET    | `/basePath`              | Listar todos los registros |
| GET    | `/basePath/<id>`         | Obtener un registro por ID |
| POST   | `/basePath/insert`       | Crear un nuevo registro     |
| PUT    | `/basePath/update/<id>`  | Actualizar un registro      |
| DELETE | `/basePath/remove/<id>`  | Eliminar un registro        |

Módulos disponibles y su `basePath`:

| Módulo                    | Base path                        |
|---------------------------|-----------------------------------|
| Registros                 | `/api/registros`                  |
| Equipos                   | `/api/equipos`                    |
| Universidades             | `/api/universidades`              |
| Problemáticas             | `/api/problematica`               |
| Contactos de emergencia   | `/api/contactos-emergencia`       |

Rutas adicionales:

| Método | Ruta                  | Descripción                          |
|--------|------------------------|----------------------------------------|
| GET    | `/api/registro/count`  | Total de registros                      |
| GET    | `/api/test-db`         | Verifica la conexión a la base de datos |

## CORS

El middleware CORS está habilitado globalmente para todos los orígenes (`*`) y los métodos `GET`, `POST`, `PUT`, `DELETE`, útil en desarrollo. Ajusta el origen en `main.cpp` antes de desplegar a producción.

### Modelo de la base de datos

![Modelo de la base de datos](assets/DatabaseModel.png)

## Repositorios relacionados

- 🎨 Frontend: https://github.com/Nightmare115117/SistemaDeInscripciones-Frontend.git

---

## English version

REST API in C++ for managing teams, students, and challenge tracks ("problemáticas") for a hackathon. Built with [Crow](https://crowcpp.org/) as the web microframework and PostgreSQL as the database, using [libpqxx](https://github.com/jtv/libpqxx) for the connection.

### Stack

- **C++20**
- **[Crow](https://github.com/CrowCpp/Crow)** — HTTP microframework (fetched automatically via `FetchContent`)
- **PostgreSQL** + **libpqxx** — database access
- **CMake** (3.14+) — build system
- **Docker** — deployment-ready image (Ubuntu 24.04)

### Architecture

The project follows a layered architecture, organized by domain module:

```
src/
├── Generics/              # Generic base classes (Controller, Service, Repository, Entity)
├── DBConfig/               # Configuration loading and database connection
├── Registro/                # Attendance/participation records
├── Equipo/                   # Teams, students, and emergency contacts
├── Universidad/            # Participating universities
├── Problematica/           # Hackathon challenge tracks
└── main.cpp                  # Entry point, route registration
```

Each domain module follows the same pattern:

```
Modulo/
├── Models/         # Entity (extends Generics/Entity)
├── Repositories/   # Data access (SQL queries via pqxx)
├── Services/       # Business logic
└── Controllers/    # HTTP routes and JSON serialization
```

The generic classes in `Generics/` (`Controller`, `Service`, `Repository`, `Entity`) implement the standard CRUD once via templates, and each module specializes them for its corresponding entity.

### Requirements

- C++20-capable compiler (GCC 10+ or Clang 12+)
- CMake 3.14+
- PostgreSQL (local or remote server)
- Development libraries:

```bash
# Debian/Ubuntu
sudo apt-get install build-essential cmake git pkg-config \
    libasio-dev libboost-all-dev libssl-dev libpq-dev libpqxx-dev

# Arch Linux
sudo pacman -S base-devel cmake git pkgconf asio boost libpqxx postgresql-libs
```

> Crow is downloaded automatically during CMake configuration (`FetchContent`) — no manual install needed.

### Configuration

The project reads the database connection string from the `DATABASE_URL` environment variable, either exported system-wide or defined in a `.env` file at the project root:

```bash
# .env
DATABASE_URL=postgresql://user:password@host:port/database_name
```

> `.env` is listed in `.gitignore` — never commit credentials to the repository.

### Build

```bash
mkdir -p build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
make -j$(nproc)
```

The resulting binary is `build/mi_api`.

### Run

```bash
./build/mi_api
```

The server listens on port **8080**.

### Docker

You can also build and run the project in a container (uses Ubuntu 24.04 and compiles `libpqxx` from source):

```bash
docker build -t sistema-registro-backend .
docker run -p 8080:8080 --env-file .env sistema-registro-backend
```

### Endpoints

Each module exposes the same set of CRUD routes under its `basePath`:

| Method | Route                    | Description               |
|--------|---------------------------|----------------------------|
| GET    | `/basePath`                | List all records            |
| GET    | `/basePath/<id>`           | Get a record by ID          |
| POST   | `/basePath/insert`         | Create a new record          |
| PUT    | `/basePath/update/<id>`    | Update a record               |
| DELETE | `/basePath/remove/<id>`    | Delete a record                |

Available modules and their `basePath`:

| Module                    | Base path                        |
|---------------------------|-----------------------------------|
| Records                   | `/api/registros`                  |
| Teams                     | `/api/equipos`                    |
| Universities              | `/api/universidades`              |
| Challenge tracks          | `/api/problematica`               |
| Emergency contacts        | `/api/contactos-emergencia`       |

Additional routes:

| Method | Route                 | Description                        |
|--------|-------------------------|--------------------------------------|
| GET    | `/api/registro/count`   | Total number of records                |
| GET    | `/api/test-db`          | Verifies the database connection        |

### CORS

The CORS middleware is enabled globally for all origins (`*`) and the `GET`, `POST`, `PUT`, `DELETE` methods, useful for development. Adjust the origin in `main.cpp` before deploying to production.

### Database Model

![DatabaseModel](assets/DatabaseModel.png)

## Related Repositories

- 🎨 Frontend: https://github.com/Nightmare115117/SistemaDeInscripciones-Frontend.git