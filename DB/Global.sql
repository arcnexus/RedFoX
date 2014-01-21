CREATE DATABASE  IF NOT EXISTS `mayaglobal`;
CREATE TABLE `grupos` (
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
CREATE TABLE `usuarios` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) DEFAULT NULL,
  `contrasena` text,
  `nivel` int(11) DEFAULT NULL,
  `categoria` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
