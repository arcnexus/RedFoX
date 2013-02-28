CREATE TABLE empresas (
    "id" INTEGER,
    "codigo" TEXT,
    "nombre" TEXT,
    "digitosfactura" INTEGER,
    "serie" INTEGER,
    "RutaBDSqLite" TEXT,
    "host" TEXT,
    "user" TEXT,
    "contrasena" TEXT,
    "nombreBD" TEXT,
    "puerto" TEXT,
    "driverBD" TEXT,
    "Direccion" TEXT,
    "cp" TEXT,
    "poblacion" TEXT,
    "provincia" TEXT,
    "pais" TEXT,
    "telefono1" TEXT,
    "telefono2" TEXT,
    "fax" TEXT,
    "mail" TEXT,
    "web" TEXT,
    "cif" TEXT,
    "inscripcion" TEXT,
    "comentarioalbaran" TEXT,
    "comentariofactura" TEXT,
    "comentarioticket" TEXT,
    "ejercicio" INTEGER)
    
    
    
    -- Describe USUARIOS
CREATE TABLE "usuarios" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT,
    "nombre" TEXT,
    "contrasena" TEXT,
    "nivelacceso" INTEGER
)

