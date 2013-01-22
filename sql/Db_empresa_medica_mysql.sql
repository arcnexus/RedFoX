CREATE TABLE IF NOT EXISTS `analitica` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `idepisodio` int(11) DEFAULT NULL,
  `analisis` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `tipoanalisis` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `comentarios` text COLLATE utf8_unicode_ci,
  `fechaanalisis` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) 

CREATE TABLE IF NOT EXISTS `analitica2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `idanalitica` int(11) DEFAULT NULL,
  `descripcion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `valor` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `referencia` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `comentarios` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `tipo` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=89 ;

CREATE TABLE IF NOT EXISTS `analiticas` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `idepisodio` int(11) DEFAULT NULL,
  `idpaciente` int(11) DEFAULT NULL,
  `descripcion` text COLLATE utf8_unicode_ci,
  `scaneranalitica` text COLLATE utf8_unicode_ci,
  `acvih` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `acvalproico` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `acidofolico` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `acidourico` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `aclaramientocreatinina` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `albumina` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `alcoholgrdia` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `amilasa` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `ana` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `antichbc` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `antichbe` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `antichbs` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `anticvhc` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `bilirrubinadirecta` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `bilirrubinaindirecta` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `bilirrubinatotal` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `calciocalcemia` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cage` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `carbamacepina` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `ceacarcinoembrionario` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `citologiavaginal` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `cocientecd4cd8` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `colesterolmgdl` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `colesterolhdl` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `colesterolldl` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `consacidofolico` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `consalimentacion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `consantialcohol` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `consanticoncepcionposcoital` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `consantitabaco` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `consejodrogas` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `consejoejercicio` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

CREATE TABLE IF NOT EXISTS `avisos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `motivo` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `avisardesde` datetime DEFAULT NULL,
  `avisarhasta` datetime DEFAULT NULL,
  `textoaviso` text COLLATE utf8_unicode_ci,
  `cerrado` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

CREATE TABLE IF NOT EXISTS `doctores` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `numcolegiado` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
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
  `facturadoEjercicio` decimal(9,2) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=4 ;

CREATE TABLE IF NOT EXISTS `episodios` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `idpaciente` int(11) DEFAULT NULL,
  `cerrado` int(11) DEFAULT NULL,
  `privado` int(11) DEFAULT NULL,
  `doctor` varchar(45) DEFAULT NULL,
  `fecha` date DEFAULT NULL,
  `cie` varchar(90) DEFAULT NULL,
  `descripcion` text,
  `idcie` int(11) DEFAULT NULL,
  `iddoctor` int(11) DEFAULT NULL,
  `historial` text,
  `codigocie` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=14 ;

CREATE TABLE IF NOT EXISTS `histofarma` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `idmedicamento` int(11) DEFAULT NULL,
  `medicamento` varchar(45) DEFAULT NULL,
  `iniciotratamiento` date DEFAULT NULL,
  `indicacionposologia` text,
  `comentarios` text,
  `idepisodio` int(11) DEFAULT NULL,
  `activo` int(11) DEFAULT NULL,
  `codigonacional` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=31 ;

CREATE TABLE IF NOT EXISTS `imagenes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `idepisodio` int(11) DEFAULT NULL,
  `localizacionimagen` text COLLATE utf8_unicode_ci,
  `comentarios` text COLLATE utf8_unicode_ci,
  `evaluada` int(11) DEFAULT NULL,
  `descripcion` varchar(60) COLLATE utf8_unicode_ci DEFAULT NULL,
  `fechaimagen` datetime DEFAULT NULL,
  `tipoimagen` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=15 ;

CREATE TABLE IF NOT EXISTS `interconsulta` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '	',
  `idpaciente` int(11) DEFAULT NULL,
  `idepisodio` int(11) DEFAULT NULL,
  `referidode` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `motivoreferido` text COLLATE utf8_unicode_ci,
  `derivara` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `motivoderivacion` text COLLATE utf8_unicode_ci,
  `peticion` text COLLATE utf8_unicode_ci,
  `respuesta` text COLLATE utf8_unicode_ci,
  `recibido` int(11) DEFAULT NULL,
  `fechapeticion` date DEFAULT NULL,
  `fecharespuesta` date DEFAULT NULL,
  `descripcion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=5 ;

CREATE TABLE IF NOT EXISTS `motivosinterconsulta` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `comentarios` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=3 ;

CREATE TABLE IF NOT EXISTS `pacientes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `idcliente` int(11) DEFAULT NULL,
  `alcohol` int(11) DEFAULT NULL,
  `alergiasConocidas` text,
  `antecedentesFamiliares` text,
  `cirugiasPrevias` text,
  `diastole` decimal(6,2) DEFAULT NULL,
  `enfermedadesConocidas` text,
  `familia` text,
  `fechaAlta` date DEFAULT NULL,
  `filiacion` varchar(45) DEFAULT NULL,
  `habitosDrogas` text,
  `hijos` int(11) DEFAULT NULL,
  `historial` text,
  `idMutua` int(11) DEFAULT NULL,
  `IMC` decimal(9,2) DEFAULT NULL,
  `nacimiento` datetime DEFAULT NULL,
  `nivelEstudios` varchar(45) DEFAULT NULL,
  `numSS` varchar(45) DEFAULT NULL,
  `otrasDrogas` text,
  `perimetroCraneal` decimal(7,2) DEFAULT NULL,
  `peso` decimal(6,2) DEFAULT NULL,
  `profesion` varchar(45) DEFAULT NULL,
  `sexo` char(1) DEFAULT NULL,
  `sistole` decimal(6,2) DEFAULT NULL,
  `tabaco` int(11) DEFAULT NULL,
  `talla` decimal(6,2) DEFAULT NULL,
  `trabaja` int(11) DEFAULT NULL,
  `numhistoria` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=12 ;

CREATE TABLE IF NOT EXISTS `tiposanalitica` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tipoanalisis` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=10 ;

CREATE TABLE IF NOT EXISTS `tiposanalitica2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tipo` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `descripcion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `valoresreferencia` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=6 ;

CREATE TABLE IF NOT EXISTS `tiposimagen` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descripcion` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `comentarios` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=24 ;