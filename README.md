# SistemaRegistro — Backend

> 🚧 **This project is currently under active development. Features and documentation may change.**
>
> 🚧 **Este proyecto se encuentra actualmente en desarrollo activo. Las características, funcionalidades y la documentación pueden cambiar.**

*[English version below](#english-version)*

API REST en C++ para la gestión de registros, equipos, alumnos, universidades y problemáticas de un hackathon. Construida con [Crow](https://crowcpp.org/) como microframework web y PostgreSQL como base de datos, utilizando `libpqxx` para la conexión.

El backend utiliza una arquitectura por capas y cuenta con autenticación de administradores mediante **JWT**, almacenamiento seguro de credenciales mediante **bcrypt**, y protección de datos mediante **AES-256-GCM** y **HMAC**.

## Stack

* **C++20**
* **[Crow](https://github.com/CrowCpp/Crow)** — microframework HTTP
* **PostgreSQL** — base de datos
* **[libpqxx](https://github.com/jtv/libpqxx)** — acceso a PostgreSQL
* **CMake** — sistema de build
* **OpenSSL** — operaciones criptográficas
* **jwt-cpp** — generación y validación de JSON Web Tokens
* **bcrypt** — hash y verificación de contraseñas
* **Docker** — contenedor para despliegue
* **GitHub Actions** — automatización del build y publicación de imágenes
* **GitHub Container Registry (GHCR)** — almacenamiento de imágenes Docker

## Arquitectura

El proyecto sigue una arquitectura por capas, organizada por módulo de dominio:

```text
src/
├── Generics/               # Clases base genéricas
│   ├── Controller/
│   ├── Service/
│   ├── Repository/
│   └── Entity/
│
├── DBConfig/               # Configuración de la base de datos
├── Security/               # Criptografía, HMAC y hashing
├── Utils/                  # Utilidades, incluyendo JWT
├── Middleware/             # Middleware de autenticación
│
├── Admin/                  # Administración y autenticación
├── Registro/               # Registros de asistencia/participación
├── Equipo/                 # Equipos
├── Universidad/            # Universidades participantes
└── Problematica/           # Problemáticas/retos del hackathon
```

Cada módulo de dominio sigue el patrón:

```text
Modulo/
├── Models/         # Entidades
├── Repositories/   # Acceso a datos y consultas SQL
├── Services/       # Lógica de negocio
└── Controllers/    # Rutas HTTP y serialización JSON
```

Las clases genéricas de `Generics/` implementan las operaciones CRUD comunes mediante templates, permitiendo reutilizar la misma estructura entre los distintos módulos.

### Flujo de una petición protegida

```text
HTTP Request
     │
     ▼
AuthMiddleware
     │
     ├── Authorization: Bearer <JWT>
     │
     ▼
JWT::validarToken()
     │
     ├── Firma
     ├── Expiración
     └── Claims
     │
     ▼
Controller
     │
     ▼
Service
     │
     ▼
Repository
     │
     ▼
PostgreSQL
```

## Seguridad

El módulo de administración utiliza diferentes mecanismos para proteger las credenciales y la autenticación.

### JWT

El inicio de sesión genera un JWT firmado mediante **HMAC-SHA256**.

El token contiene información como:

* `adminId`
* `username`
* `role`
* fecha de emisión
* fecha de expiración
* issuer

Los endpoints protegidos reciben el token mediante el header HTTP:

```http
Authorization: Bearer <JWT>
```

La validación del token se realiza mediante `AuthMiddleware` antes de llegar al controller.

### Contraseñas

Las contraseñas de los administradores no se almacenan directamente en la base de datos.

Se utiliza **bcrypt** para generar y verificar los hashes.

```text
Contraseña
    │
    ▼
bcrypt
    │
    ▼
Hash almacenado
```

### Correo electrónico

Los correos de los administradores utilizan dos mecanismos diferentes:

* **AES-256-GCM** para cifrar el correo almacenado.
* **HMAC** para realizar búsquedas determinísticas sin almacenar el correo en texto plano.

Esto permite buscar administradores por correo mediante su HMAC sin depender de almacenar el valor original directamente.

### Secretos

Las claves utilizadas por los mecanismos de seguridad se cargan mediante variables de entorno.

Entre ellas:

```text
DATABASE_URL
JWT_SECRET
AES_KEY
HMAC_KEY
```

Las credenciales y secretos no deben almacenarse directamente en el código fuente.

## Requisitos

* Compilador con soporte C++20
* CMake 3.14+
* PostgreSQL
* OpenSSL
* libpqxx
* Docker (opcional)

### Debian / Ubuntu

```bash
sudo apt-get install build-essential cmake git pkg-config \
    libasio-dev libboost-all-dev libssl-dev libpq-dev libpqxx-dev
```

### Arch Linux

```bash
sudo pacman -S base-devel cmake git pkgconf asio boost \
    openssl libpqxx postgresql-libs
```

> Crow se descarga automáticamente durante la configuración de CMake mediante `FetchContent`.

## Configuración

El proyecto utiliza variables de entorno para la configuración.

Ejemplo de `.env`:

```bash
DATABASE_URL=postgresql://usuario:contraseña@host:puerto/nombre_bd
JWT_SECRET=una_clave_secreta_segura
AES_KEY=una_clave_aes_segura
HMAC_KEY=una_clave_hmac_segura
```

> `.env` está incluido en `.gitignore`. **Nunca subas credenciales o claves secretas al repositorio.**

## Compilación

```bash
mkdir -p build
cd build

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cmake --build . -j$(nproc)
```

El binario resultante es:

```text
build/mi_api
```

## Ejecución

```bash
./build/mi_api
```

El servidor escucha en el puerto **8080**.

## Docker

El proyecto incluye una imagen basada en **Ubuntu 24.04**.

La imagen compila `libpqxx` desde código fuente para utilizar la versión requerida por el proyecto.

```bash
docker build -t sistema-registro-backend .
```

Para ejecutar el contenedor:

```bash
docker run -p 8080:8080 --env-file .env sistema-registro-backend
```

## CI/CD

El proyecto utiliza **GitHub Actions** para construir y publicar automáticamente la imagen Docker.

El flujo realiza:

```text
Push a main
    │
    ▼
GitHub Actions
    │
    ├── Checkout
    ├── Configuración de Docker Buildx
    ├── Build de la imagen
    ├── Cache de Docker/BuildKit
    └── Push
         │
         ▼
GitHub Container Registry
```

La imagen se publica en GHCR con el tag:

```text
ghcr.io/<owner>/mi-api:latest
```

El cache de BuildKit permite reutilizar capas que no hayan cambiado, incluyendo la compilación de dependencias como `libpqxx`.

## Endpoints

### CRUD

Cada módulo expone las siguientes operaciones:

| Método | Ruta                    | Descripción                |
| ------ | ----------------------- | -------------------------- |
| GET    | `/basePath`             | Listar todos los registros |
| GET    | `/basePath/<id>`        | Obtener un registro por ID |
| POST   | `/basePath/insert`      | Crear un nuevo registro    |
| PUT    | `/basePath/update/<id>` | Actualizar un registro     |
| DELETE | `/basePath/remove/<id>` | Eliminar un registro       |

### Módulos

| Módulo                  | Base path                   |
| ----------------------- | --------------------------- |
| Registros               | `/api/registros`            |
| Equipos                 | `/api/equipos`              |
| Universidades           | `/api/universidades`        |
| Problemáticas           | `/api/problematica`         |
| Contactos de emergencia | `/api/contactos-emergencia` |

### Administración

| Método         | Ruta               | Descripción                       | Autenticación |
| -------------- | ------------------ | --------------------------------- | ------------- |
| PUT            | `/api/admin/login` | Inicio de sesión de administrador | No            |
| GET/PUT/DELETE | `/api/admin/...`   | Operaciones administrativas       | JWT           |

> Los endpoints administrativos protegidos requieren un JWT mediante el header `Authorization`.

Ejemplo:

```http
Authorization: Bearer <JWT>
```

### Rutas adicionales

| Método | Ruta                  | Descripción                             |
| ------ | --------------------- | --------------------------------------- |
| GET    | `/api/registro/count` | Total de registros                      |
| GET    | `/api/test-db`        | Verifica la conexión a la base de datos |

## CORS

El middleware CORS está habilitado globalmente para todos los orígenes (`*`) y los métodos HTTP utilizados por la API.

Esta configuración es útil durante el desarrollo.

> ⚠️ Antes de desplegar a producción, se recomienda restringir los orígenes permitidos al dominio real del frontend.

## Modelo de la base de datos

![Modelo de la base de datos](./assets/DBModel.png)

## Repositorios relacionados

* 🎨 [Repositorio del Frontend](https://github.com/Nightmare115117/SistemaDeInscripciones-Frontend)

## Hoja de ruta

### ✅ Completado

* [x] Arquitectura CRUD genérica
* [x] Arquitectura por capas
* [x] Integración con PostgreSQL
* [x] API REST
* [x] Soporte para Docker
* [x] Configuración mediante variables de entorno
* [x] Soporte para CORS
* [x] Endpoints CRUD genéricos
* [x] Módulo de administración
* [x] Autenticación mediante JWT
* [x] Middleware de autenticación
* [x] Hashing de contraseñas mediante bcrypt
* [x] Cifrado AES-256-GCM
* [x] HMAC para búsqueda de correos
* [x] Pipeline de build y publicación mediante GitHub Actions
* [x] Publicación de imágenes en GitHub Container Registry
* [x] Cache de BuildKit para dependencias de Docker

## Licencia

Este proyecto está licenciado bajo la **Licencia PolyForm No Comercial 1.0.0**.

El uso comercial, incluyendo la venta, redistribución como producto comercial o integración en servicios de pago, requiere autorización previa del titular de los derechos de autor.

Para más información, consulta el archivo [LICENSE](./LICENSE).

---

# English Version

REST API in C++ for managing registrations, teams, students, universities, and hackathon challenge tracks. Built with [Crow](https://crowcpp.org/) as the HTTP microframework and PostgreSQL as the database, using `libpqxx` for database access.

The backend follows a layered architecture and includes administrator authentication using **JWT**, password hashing using **bcrypt**, and data protection using **AES-256-GCM** and **HMAC**.

## Stack

* **C++20**
* **[Crow](https://github.com/CrowCpp/Crow)** — HTTP microframework
* **PostgreSQL** — database
* **[libpqxx](https://github.com/jtv/libpqxx)** — PostgreSQL access
* **CMake** — build system
* **OpenSSL** — cryptographic operations
* **jwt-cpp** — JSON Web Token generation and validation
* **bcrypt** — password hashing and verification
* **Docker** — containerized deployment
* **GitHub Actions** — automated builds and image publishing
* **GitHub Container Registry (GHCR)** — Docker image registry

## Architecture

The project follows a layered architecture organized by domain module:

```text
src/
├── Generics/               # Generic base classes
│   ├── Controller/
│   ├── Service/
│   ├── Repository/
│   └── Entity/
│
├── DBConfig/               # Database configuration
├── Security/               # Cryptography, HMAC and password hashing
├── Utils/                  # Utilities, including JWT
├── Middleware/             # Authentication middleware
│
├── Admin/                  # Administration and authentication
├── Registro/               # Attendance/participation records
├── Equipo/                 # Teams
├── Universidad/             # Participating universities
└── Problematica/           # Hackathon challenge tracks
```

Each domain module follows:

```text
Module/
├── Models/         # Entities
├── Repositories/   # Database access and SQL queries
├── Services/       # Business logic
└── Controllers/    # HTTP routes and JSON serialization
```

## Security

### JWT

Administrator login generates an **HMAC-SHA256 signed JWT**.

The token contains:

* `adminId`
* `username`
* `role`
* issued-at timestamp
* expiration timestamp
* issuer

Protected endpoints receive the token through:

```http
Authorization: Bearer <JWT>
```

The token is validated by `AuthMiddleware` before the request reaches the controller.

### Passwords

Administrator passwords are never stored directly in the database.

**bcrypt** is used for password hashing and verification.

### Email protection

Administrator email addresses use two different mechanisms:

* **AES-256-GCM** for encrypted storage.
* **HMAC** for deterministic lookups without storing the plaintext email.

### Secrets

Security keys and configuration values are loaded through environment variables:

```text
DATABASE_URL
JWT_SECRET
AES_KEY
HMAC_KEY
```

Secrets must never be committed to the repository.

## Requirements

* C++20-capable compiler
* CMake 3.14+
* PostgreSQL
* OpenSSL
* libpqxx
* Docker (optional)

### Debian / Ubuntu

```bash
sudo apt-get install build-essential cmake git pkg-config \
    libasio-dev libboost-all-dev libssl-dev libpq-dev libpqxx-dev
```

### Arch Linux

```bash
sudo pacman -S base-devel cmake git pkgconf asio boost \
    openssl libpqxx postgresql-libs
```

> Crow is downloaded automatically during CMake configuration through `FetchContent`.

## Configuration

Example `.env`:

```bash
DATABASE_URL=postgresql://user:password@host:port/database_name
JWT_SECRET=your_secure_jwt_secret
AES_KEY=your_secure_aes_key
HMAC_KEY=your_secure_hmac_key
```

> `.env` is included in `.gitignore`. **Never commit credentials or secret keys.**

## Build

```bash
mkdir -p build
cd build

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cmake --build . -j$(nproc)
```

The resulting binary is:

```text
build/mi_api
```

## Run

```bash
./build/mi_api
```

The server listens on port **8080**.

## Docker

The project includes an **Ubuntu 24.04** based Docker image.

The image builds `libpqxx` from source to use the required version.

```bash
docker build -t sistema-registro-backend .
```

Run the container with:

```bash
docker run -p 8080:8080 --env-file .env sistema-registro-backend
```

## CI/CD

GitHub Actions automatically builds and publishes the Docker image.

The workflow performs:

```text
Push to main
    │
    ▼
GitHub Actions
    │
    ├── Checkout
    ├── Docker Buildx setup
    ├── Image build
    ├── Docker/BuildKit cache
    └── Push
         │
         ▼
GitHub Container Registry
```

The image is published as:

```text
ghcr.io/<owner>/mi-api:latest
```

BuildKit caching allows unchanged Docker layers to be reused between builds, including dependency compilation such as `libpqxx`.

## Endpoints

### CRUD

Each module exposes:

| Method | Route                   | Description         |
| ------ | ----------------------- | ------------------- |
| GET    | `/basePath`             | List all records    |
| GET    | `/basePath/<id>`        | Get a record by ID  |
| POST   | `/basePath/insert`      | Create a new record |
| PUT    | `/basePath/update/<id>` | Update a record     |
| DELETE | `/basePath/remove/<id>` | Delete a record     |

### Modules

| Module             | Base path                   |
| ------------------ | --------------------------- |
| Records            | `/api/registros`            |
| Teams              | `/api/equipos`              |
| Universities       | `/api/universidades`        |
| Challenge tracks   | `/api/problematica`         |
| Emergency contacts | `/api/contactos-emergencia` |

### Administration

| Method         | Route              | Description               | Authentication |
| -------------- | ------------------ | ------------------------- | -------------- |
| PUT            | `/api/admin/login` | Administrator login       | No             |
| GET/PUT/DELETE | `/api/admin/...`   | Administrative operations | JWT            |

Protected administrative endpoints require:

```http
Authorization: Bearer <JWT>
```

### Additional routes

| Method | Route                 | Description                    |
| ------ | --------------------- | ------------------------------ |
| GET    | `/api/registro/count` | Total number of records        |
| GET    | `/api/test-db`        | Verifies database connectivity |

## CORS

CORS is currently enabled globally for all origins (`*`) and the HTTP methods used by the API.

This configuration is intended for development.

> ⚠️ Before production deployment, restrict allowed origins to the actual frontend domain.

## Database Model

![Database model](./assets/DBModel.png)

## Related Repositories

* 🎨 [Frontend Repository](https://github.com/Nightmare115117/SistemaDeInscripciones-Frontend)

## Roadmap

### ✅ Completed

* [x] Generic CRUD architecture
* [x] Layered architecture
* [x] PostgreSQL integration
* [x] REST API
* [x] Docker support
* [x] Environment-based configuration
* [x] CORS support
* [x] Generic CRUD endpoints
* [x] Administrator module
* [x] JWT authentication
* [x] Authentication middleware
* [x] bcrypt password hashing
* [x] AES-256-GCM encryption
* [x] HMAC email lookup
* [x] GitHub Actions build and publishing pipeline
* [x] GitHub Container Registry publishing
* [x] BuildKit dependency caching

## License

This project is licensed under the **PolyForm Noncommercial License 1.0.0**.

Commercial use, including selling, redistribution as a commercial product, or integration into paid services, requires prior permission from the copyright holder.

For more information, see the [LICENSE](./LICENSE) file.
