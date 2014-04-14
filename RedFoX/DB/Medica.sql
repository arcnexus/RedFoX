CREATE TABLE `@medica@`.`analitica` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `analisis` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `tipo_analisis` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `comentarios` text COLLATE utf8_unicode_ci,
  `fecha_analisis` datetime DEFAULT NULL,
  `id_paciente` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@medica@`.`analitica2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `valor` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `referencia` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `comentarios` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `tipo` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@medica@`.`doctores` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `num_colegiado` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `telefono` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `especialidad1` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `especialidad2` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `direccion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cp` varchar(5) COLLATE utf8_unicode_ci DEFAULT NULL,
  `poblacion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `provincia` varchar(25) COLLATE utf8_unicode_ci DEFAULT NULL,
  `pais` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `movil` varchar(25) COLLATE utf8_unicode_ci DEFAULT NULL,
  `pendiente` decimal(9,2) DEFAULT NULL,
  `facturado` decimal(9,2) DEFAULT NULL,
  `facturado_ejercicio` decimal(9,2) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@medica@`.`episodios` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `cerrado` int(11) DEFAULT NULL,
  `privado` int(11) DEFAULT NULL,
  `fecha` date DEFAULT NULL,
  `descripcion` text,
  `cie` varchar(45) DEFAULT NULL,
  `historial` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@medica@`.`histofarma` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_medicamento` int(11) DEFAULT NULL,
  `medicamento` varchar(45) DEFAULT NULL,
  `inicio_tratamiento` date DEFAULT NULL,
  `indicacion_posologia` text,
  `comentarios` text,
  `activo` int(11) DEFAULT NULL,
  `codigo_nacional` varchar(45) DEFAULT NULL,
  `id_paciente` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `@medica@`.`imagenes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `imagen` blob,
  `comentarios` text COLLATE utf8_unicode_ci,
  `evaluada` int(11) DEFAULT NULL,
  `descripcion` varchar(60) COLLATE utf8_unicode_ci DEFAULT NULL,
  `fecha_imagen` datetime DEFAULT NULL,
  `id_paciente` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@medica@`.`interconsulta` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '	',
  `id_paciente` int(11) DEFAULT NULL,
  `referido_de` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `motivo_referido` text COLLATE utf8_unicode_ci,
  `derivar_a` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `motivo_derivacion` text COLLATE utf8_unicode_ci,
  `peticion` text COLLATE utf8_unicode_ci,
  `respuesta` text COLLATE utf8_unicode_ci,
  `recibido` int(11) DEFAULT NULL,
  `fecha_peticion` date DEFAULT NULL,
  `fecha_respuesta` date DEFAULT NULL,
  `descripcion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE SCHEMA IF NOT EXISTS `@medica@` DEFAULT CHARACTER SET utf8 COLLATE utf8_unicode_ci ;
CREATE TABLE `@medica@`.`motivosinterconsulta` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `comentarios` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@medica@`.`nivelestudios` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nivel_estudios` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@medica@`.`pacientes` (
  `id_cliente` int(11) DEFAULT NULL,
  `alcohol` int(11) DEFAULT NULL,
  `alergias_conocidas` text,
  `antecedentes_familiares` text,
  `cirugias_previas` text,
  `diastole` decimal(6,2) DEFAULT NULL,
  `enfermedades_conocidas` text,
  `familia` text,
  `fecha_alta` date DEFAULT NULL,
  `filiacion` varchar(45) DEFAULT NULL,
  `habitos_drogas` text,
  `hijos` int(11) DEFAULT NULL,
  `historial` text,
  `id_mutua` int(11) DEFAULT NULL,
  `imc` decimal(9,2) DEFAULT NULL,
  `nacimiento` datetime DEFAULT NULL,
  `num_ss` varchar(45) DEFAULT NULL,
  `otras_drogas` text,
  `perimetro_craneal` decimal(7,2) DEFAULT NULL,
  `peso` decimal(6,2) DEFAULT NULL,
  `profesion` varchar(45) DEFAULT NULL,
  `sexo` char(1) DEFAULT NULL,
  `sistole` decimal(6,2) DEFAULT NULL,
  `tabaco` int(11) DEFAULT NULL,
  `talla` decimal(6,2) DEFAULT NULL,
  `trabaja` int(11) DEFAULT NULL,
  `num_historia` varchar(45) DEFAULT NULL,
  `id` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`),
  KEY `fk_pacientes__clientes_id_idx` (`id_cliente`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@medica@`.`sindromes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_sindrome` int(11) DEFAULT NULL,
  `sindrome` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@medica@`.`tiposanalitica` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tipo_analisis` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@medica@`.`tiposanalitica2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tipo` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `descripcion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `valores_referencia` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@medica@`.`tiposimagen` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `comentarios` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
CREATE TABLE `@medica@`.`visitas` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `fecha_hora` datetime DEFAULT NULL,
  `medico` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `exploracion` text COLLATE utf8_unicode_ci,
  `tratamiento` text CHARACTER SET utf8,
  `lengua` text COLLATE utf8_unicode_ci,
  `pulso` text COLLATE utf8_unicode_ci,
  `diagnostico` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
