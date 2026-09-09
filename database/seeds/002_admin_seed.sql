-- Seed mínimo para el usuario administrador.
-- Reemplaza los valores de ejemplo por los hashes reales generados por el backend.
-- El backend usa bcrypt para contrasena, AES para correo_aes y HMAC para correo_hmac.

INSERT INTO public.admin (nombre, correo_aes, contrasena, correo_hmac)
VALUES (
  'Administrador',
  'REEMPLAZAR_POR_AES_ENCRYPT(admin@dominio.com)',
  'REEMPLAZAR_POR_BCRYPT_HASH',
  'REEMPLAZAR_POR_HMAC_DEL_CORREO'
)
ON CONFLICT DO NOTHING;

-- Ejemplo de cómo se debería generar el usuario desde el backend:
-- correo_aes = AES::encrypt("admin@dominio.com")
-- contrasena = PasswordHasher::hash("admin123")
-- correo_hmac = HMAC::generate("admin@dominio.com")
