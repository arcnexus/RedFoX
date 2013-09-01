-- phpMyAdmin SQL Dump
-- version 3.5.8.1deb1
-- http://www.phpmyadmin.net
--
-- Servidor: localhost
-- Tiempo de generación: 31-08-2013 a las 16:37:40
-- Versión del servidor: 5.5.32-0ubuntu0.13.04.1
-- Versión de PHP: 5.4.9-4ubuntu2.2

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Base de datos: `empAGUPUNTSL2013conta`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `diario`
--

CREATE TABLE IF NOT EXISTS `diario` (
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
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=18814 ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `plan_general`
--

CREATE TABLE IF NOT EXISTS `plan_general` (
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
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=3715 ;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
