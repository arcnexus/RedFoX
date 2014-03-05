CREATE DATABASE  IF NOT EXISTS `redfoxglobal`;
CREATE TABLE `redfoxglobal`.`grupos` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) DEFAULT NULL,
  `bd_name` varchar(45) DEFAULT NULL,
  `bd_driver` varchar(45) DEFAULT 'QMYSQL',
  `bd_ruta` varchar(45) DEFAULT NULL,
  `bd_host` varchar(45) DEFAULT 'localhost',
  `bd_user` varchar(45) DEFAULT 'root',
  `bd_pass` varchar(45) DEFAULT NULL,
  `bd_port` varchar(45) DEFAULT '3306',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `redfoxglobal`.`usuarios` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) DEFAULT NULL,
  `nombre_completo` varchar(45) DEFAULT NULL,
  `contrasena` varchar(45) DEFAULT NULL,
  `cuenta_smtp` varchar(100) DEFAULT NULL,
  `usuario_mail` varchar(100) DEFAULT NULL,
  `password_mail` varchar(100) DEFAULT NULL,
  `port_mail` int(11) DEFAULT NULL,
  `super_user` tinyint(4) DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `nombre_UNIQUE` (`nombre`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `redfoxglobal`.`nivelacceso` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `redfoxglobal`.`seguimiento` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `usuario` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,
  `fecha` datetime DEFAULT NULL,
  `accion` varchar(200) COLLATE utf8_unicode_ci DEFAULT NULL,
  `comentarios` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
INSERT INTO `redfoxglobal`.`nivelacceso` (`id`,`nombre`) VALUES (1,'Sin Acceso');
INSERT INTO `redfoxglobal`.`nivelacceso` (`id`,`nombre`) VALUES (2,'Lectura parcial');
INSERT INTO `redfoxglobal`.`nivelacceso` (`id`,`nombre`) VALUES (3,'Lectura total');
INSERT INTO `redfoxglobal`.`nivelacceso` (`id`,`nombre`) VALUES (4,'Escritura parcial (editar)');
INSERT INTO `redfoxglobal`.`nivelacceso` (`id`,`nombre`) VALUES (5,'Escritura parcial (añadir)');
INSERT INTO `redfoxglobal`.`nivelacceso` (`id`,`nombre`) VALUES (6,'Escritural total');
INSERT INTO `redfoxglobal`.`nivelacceso` (`id`,`nombre`) VALUES (7,'Administrador');
