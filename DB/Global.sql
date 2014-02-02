CREATE DATABASE  IF NOT EXISTS `mayaglobal`;
CREATE TABLE `mayaglobal`.`grupos` (
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
CREATE TABLE `mayaglobal`.`usuarios` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) DEFAULT NULL,
  `contrasena` varchar(45) DEFAULT NULL,
  `cuenta_smtp` varchar(100) DEFAULT NULL,
  `usuario_mail` varchar(100) DEFAULT NULL,
  `password_mail` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `nombre_UNIQUE` (`nombre`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `nivelacceso` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (1,'Sin Acceso');
INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (2,'Lectura parcial');
INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (3,'Lectura total');
INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (4,'Escritura parcial (editar)');
INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (5,'Escritura parcial (añadir)');
INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (6,'Escritural total');
INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (7,'Administrador');