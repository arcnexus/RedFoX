CREATE SCHEMA IF NOT EXISTS `@conta@` DEFAULT CHARACTER SET utf8 COLLATE utf8_unicode_ci ;
CREATE TABLE `@conta@`.`diario` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_documento` int(11) DEFAULT NULL,
  `cuenta_d` varchar(45) DEFAULT NULL,
  `descripcion_d` varchar(100) DEFAULT NULL,
  `cuenta_h` varchar(45) DEFAULT NULL,
  `descripcion_h` varchar(100) DEFAULT NULL,
  `importe_d` double DEFAULT NULL,
  `importe_h` double DEFAULT NULL,
  `asiento` int(11) DEFAULT NULL,
  `id_cuenta_d` int(11) DEFAULT NULL,
  `id_cuenta_h` int(11) DEFAULT NULL,
  `fecha_asiento` date DEFAULT NULL,
  `pos_en_asiento` int(11) DEFAULT NULL,
  `comentario_d` text,
  `comentario_h` text,
  `cuenta_relacion` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
CREATE TABLE `@conta@`.`plan_general` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `codigo_cta` varchar(20) DEFAULT NULL,
  `descripcion` varchar(120) DEFAULT NULL,
  `activo` tinyint(1) DEFAULT '1',
  `saldo` double DEFAULT '0',
  `cif_nif` varchar(20) DEFAULT NULL,
  `saldo_debe` double DEFAULT NULL,
  `saldo_haber` double DEFAULT NULL,
  `direccion` varchar(45) DEFAULT NULL,
  `cp` varchar(10) DEFAULT NULL,
  `poblacion` varchar(45) DEFAULT NULL,
  `provincia` varchar(45) DEFAULT NULL,
  `pais` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
