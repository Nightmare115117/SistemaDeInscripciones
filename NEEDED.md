                                                              # NEEDED.md

Lo que el frontend ya no puede resolver por su cuenta. Cada punto se comprobó
contra `https://api.road2tech.com` y contra el código de
[SistemaDeInscripciones](https://github.com/Nightmare115117/SistemaDeInscripciones)
el 2026-09-08; se citan archivo y línea del backend para que sea directo.

El frontend quedó conectado a todo lo que la API expone hoy (login, sesión,
registro, equipos, agenda, staff, patrocinadores, evento y ejes). Nada de esta
lista bloquea el sitio: son huecos del backend que el frontend no puede tapar
sin inventarse datos.

---

## 🔴 Bloqueantes para abrir el registro

### 1. `GET /api/equipos` devuelve el correo y el teléfono cifrados

El alta cifra los datos antes de guardarlos:

```cpp
// src/Registro/Services/RegistroService.cpp:84
alumno.setCorreo(AES::encrypt(alumno.getCorreo()));
alumno.setNumeroTel(AES::encrypt(alumno.getNumeroTel()));
```

pero la lectura los devuelve tal cual salen de la base, sin descifrar:

```cpp
// src/Equipo/Repositories/AlumnoRepository.cpp:106
alumno.setCorreo(fila["correo"].as<string>());
alumno.setNumeroTel(fila["numerotel"].as<string>());
```

**Efecto:** el panel de equipos muestra cadenas AES en lugar de correos y
teléfonos, y el buscador por correo no encuentra nada. **Se necesita**
`AES::decrypt` en `findByEquipoId` (y en cualquier otra lectura de alumnos).

### 2. El comité no tiene forma de avisar que aceptó un equipo

`PATCH /api/equipos/<id>` solo cambia el estado en la base:

```cpp
// src/Equipo/Services/EquipoService.cpp:45
return repo.updateReview(id, estado, revisadoEn, notas);
```

No se manda ningún correo. La landing y el formulario prometen lo contrario
("el comité revisa cada registro y confirma por correo al líder"), así que hoy
esa promesa no se cumple.

**Se necesita:** que `updateReview` dispare el correo al líder al pasar a
`aceptado` o `rechazado`, usando `EmailService::sendEmail`.

### 3. `POST /api/registro` no valida cupo, apertura ni tamaño de equipo

`insertRegistroCompleto` solo comprueba que haya al menos un integrante y que
cada uno traiga contacto de emergencia (`src/Registro/Services/RegistroService.cpp:70-73`).
No consulta `evento.cupo`, ni `registroAbierto`, ni `minIntegrantes` /
`maxIntegrantes`, ni correos repetidos.

**Efecto:** con el registro cerrado o el cupo lleno la API sigue aceptando
equipos. El frontend valida el tamaño y bloquea el envío, pero eso es
cortesía de interfaz: cualquiera puede llamar la API directamente.

**Se necesita**, antes de insertar:

| Situación | Respuesta esperada |
| --- | --- |
| `registroAbierto = false` | `403 {"detail":"El registro está cerrado."}` |
| equipos aceptados ≥ `cupo` | `409 {"detail":"El cupo de N equipos está lleno."}` |
| correo ya registrado | `409 {"detail":"Ese correo ya está registrado."}` |
| integrantes fuera de `[min, max]` | `422` con el detalle |

Hoy todo error sale como `422` porque el `catch` del controller es único
(`src/Registro/Controllers/RegistroController.h:120`). El frontend ya distingue
por código: pinta el `detail` tal cual, así que basta con devolver el correcto.

---

## 🟡 Datos que el formulario captura y el backend tira

`RegistroController::registroCompleto` solo lee `nombre`, `correo`, `telefono`,
`alergias`, `emergencia` y `aceptaReglamento`
(`src/Registro/Controllers/RegistroController.h:76-110`). Todo lo demás se
descarta en silencio.

| Campo que manda el frontend | Qué pasa hoy | Nota |
| --- | --- | --- |
| `apellido` | Se pierde | La tabla **ya tiene** `apellidopaterno` y `apellidomaterno` (`AlumnoRepository.cpp:129`), solo no se llenan. El frontend manda el nombre completo en `nombre` para no perderlo. |
| `edad` | Se pierde | No hay columna. MLH la exige. |
| `nivelEstudio` | Se pierde | Existe el módulo `NivelEstudio` y su tabla, pero `alumnos` no guarda la FK. |
| `paisResidencia` | Se pierde | Existe el módulo `Pais`, misma situación. |
| `escuela` / `institucion` | Se pierde | `alumno.setIdUniversidad(-1)` fijo (`RegistroController.h:104`). El frontend lo manda como `institucion` porque el validador lo exige, pero nunca se guarda. |
| `aceptaCodigoConductaMLH` | Se pierde | |
| `aceptaCompartirDatosMLH` | Se pierde | |
| `aceptaCorreosMLH` | Se pierde | |
| `liderId` | Se ignora | El líder siempre es el primer integrante insertado (`RegistroService.cpp:87`). Coincide con lo que manda el frontend solo por casualidad del orden. |

**Por qué importa:** los siete primeros son el mínimo que MLH exige a un *MLH
Member Event*. Si no se guardan, el evento no puede acreditarse. Las tres
casillas MLH hoy solo se validan en el navegador.

**Efecto secundario ya visible:** como `iduniversidad` queda en `-1`,
`GET /api/equipos` devuelve `integrantes[].institucion = "-1"`. El panel
descarta cualquier valor puramente numérico (`src/lib/admin/store.ts`,
`escuelaLegible`) para no enseñar un `-1` como si fuera una escuela.

---

## 🟡 Verificación de correo: no existe

La tarea pedía integrarla. **No hay nada que integrar**: el backend no tiene
ningún endpoint de verificación. Lo único relacionado es `POST /test/email`
(`src/main.cpp:196`), que manda un correo suelto y no está ligado a ningún
registro ni genera token.

Contrato mínimo que el frontend puede consumir en cuanto exista:

```
POST /api/registro
  → además de crear el equipo, manda al líder un correo con un enlace
    https://roadtotech.mx/registro/verificar?token=<token opaco, un solo uso>

POST /api/verificar-correo   { "token": "..." }
  200 { "correo": "lider@correo.com", "equipo": "Los Debuggers" }
  410 { "detail": "El enlace expiró. Pide uno nuevo." }
  404 { "detail": "El enlace no es válido." }

POST /api/verificar-correo/reenviar   { "correo": "lider@correo.com" }
  202 (siempre 202, aunque el correo no exista, para no filtrar quién está registrado)
```

Y en `GET /api/equipos`, un `correoVerificado: boolean` por integrante para que
el comité no acepte equipos con correos que nadie confirmó.

Cuando esos endpoints existan aviso y monto la pantalla `/registro/verificar`;
son ~40 líneas en `src/lib/publico.ts` más una página.

---

## 🟡 `react-email` y los correos

`react-email-starter/` trae las plantillas de ejemplo del starter oficial (Notion,
Plaid, Stripe, Vercel), ninguna del evento. Dos cosas que hay que saber antes de
usarlo:

1. **El frontend no puede mandar correos.** El sitio se compila estático
   (`output: "export"`): no hay API routes ni servidor propio. Quien manda es el
   backend, con Resend.
2. **El backend solo manda texto plano.** El JSON que arma para Resend no
   incluye `html`:

   ```cpp
   // src/Email/EmailService.cpp:78
   contenido["from"] = RESEND_FROM_EMAIL;
   contenido["to"] = to;
   contenido["subject"] = subject;
   contenido["text"] = body;   // ← falta "html"
   ```

**Se necesita**, para que las plantillas sirvan de algo:

- Añadir `contenido["html"] = html;` en `EmailService::sendEmail` (o una firma
  con `html` opcional). Sin eso, cualquier plantilla de react-email llega como
  un churro de etiquetas o no llega.
- Del lado de react-email: `pnpm --dir react-email-starter exec email export`
  genera el HTML; ese HTML es el que el backend incrusta, sustituyendo las
  variables (nombre del equipo, del líder, enlace de verificación).

Las plantillas que hacen falta: **confirmación de solicitud recibida**,
**equipo aceptado**, **equipo rechazado** y, cuando exista, **verificación de
correo**. `react-email-starter` está fuera de `tsconfig.json` a propósito: es un
paquete aparte con sus propias dependencias, sin instalar.

---

## 🟢 Menores

- **Usuario administrador.** No hay ninguno documentado ni sembrado. El panel ya
  no tiene credenciales locales: sin un admin en la base, `/admin` no deja
  entrar a nadie. Hace falta crear uno (bcrypt + correo cifrado con AES + HMAC)
  y pasar las credenciales al comité.
- **Estado del servicio de correo.** El panel tiene el bloque listo
  (`DashboardView`, tipo `EstadoCorreo`) pero no hay endpoint. Bastaría con
  `GET /api/correo/estado → { configurado, habilitado, remitente }`. Mientras
  tanto el panel no afirma nada.
- **Agenda y patrocinadores vacíos.** `GET /api/agenda` y
  `GET /api/patrocinadores` devuelven `[]`. La landing se queda con el contenido
  compilado de `src/data/event.ts`, que es la degradación correcta. En cuanto se
  siembren desde el panel, la landing los toma.
- **Solo hay un eje en la base.** `GET /api/evento` devuelve únicamente
  `{"id":"1","code":"01","title":"Salud"}`. Los tres ejes compilados (Agua,
  Medio ambiente, Educación) no están. El selector de `/registro` ya usa la
  lista viva, así que hoy solo ofrece "Salud": hay que darlos de alta desde
  `/admin/configuracion`.
- **CORS abierto.** `src/main.cpp:80` permite `origin("*")`. Antes de producción
  conviene restringirlo al dominio real del sitio.
- **El README del backend está desactualizado.** Documenta rutas que no existen
  (`/api/registro` con otro prefijo, `/api/staff` como protegida en GET) y omite
  `/api/problematica`, `/api/universidades`, `/api/alumno` y
  `/api/contactos-emergencia`. La fuente buena es `src/main.cpp` y
  `src/Middleware/AuthMiddleware.cpp`. El contrato que consume este frontend
  está en `API.md`.
- **`enviadoEn` se ignora.** El backend pone su propia marca de tiempo. Es lo
  correcto; se anota solo para que no se busque el campo.
