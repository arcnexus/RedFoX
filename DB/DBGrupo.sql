CREATE SCHEMA IF NOT EXISTS `@grupo@` DEFAULT CHARACTER SET utf8 ;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`accesousuarios` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_user` INT(11) NULL DEFAULT NULL ,
  `id_modulo` INT(11) NULL DEFAULT NULL ,
  `id_nivel_acceso` INT(11) NULL DEFAULT '0' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`agenda` (
  `Id` INT(11) NOT NULL AUTO_INCREMENT ,
  `fecha` DATE NULL DEFAULT NULL ,
  `hora` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_usuario` INT(11) NULL DEFAULT '0' ,
  `inicio` DATETIME NULL DEFAULT NULL ,
  `fin` DATETIME NULL DEFAULT NULL ,
  `asunto` VARCHAR(40) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion_asunto` LONGTEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `estado` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `avisar_tiempo` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `importancia` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `color` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_especialidad` INT(11) NULL DEFAULT NULL ,
  `id_departamento` INT(11) NULL DEFAULT NULL ,
  `is_medica` TINYINT(1) NULL DEFAULT NULL ,
  `id_Cliente` INT(11) NULL DEFAULT NULL ,
  `is_cita` TINYINT(1) NULL DEFAULT NULL ,
  `is_privado` TINYINT(1) NULL DEFAULT '0' ,
  PRIMARY KEY (`Id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`articulos` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `codigo` VARCHAR(200) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `codigo_barras` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `codigo_fabricante` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion_reducida` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_proveedor` INT(11) NULL DEFAULT NULL ,
  `id_familia` INT(11) NULL DEFAULT NULL ,
  `id_seccion` INT(11) NULL DEFAULT NULL ,
  `id_subfamilia` INT(11) NULL DEFAULT NULL ,
  `tipo_iva` FLOAT NULL DEFAULT '0' ,
  `coste` DOUBLE NULL DEFAULT '0' ,
  `porc_dto` FLOAT NULL DEFAULT '0' ,
  `dto_proveedor` DOUBLE NULL DEFAULT '0' ,
  `fecha_ultima_compra` DATE NULL DEFAULT NULL ,
  `fecha_ultima_venta` DATE NULL DEFAULT NULL ,
  `unidades_compradas` INT(11) NULL DEFAULT '0' ,
  `unidades_vendidas` INT(11) NULL DEFAULT '0' ,
  `importe_acumulado_compras` DOUBLE NULL DEFAULT '0' ,
  `importe_acumulado_ventas` DOUBLE NULL DEFAULT '0' COMMENT '	' ,
  `imagen` BLOB NULL DEFAULT NULL ,
  `comentario` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `stock_maximo` INT(11) NULL DEFAULT '0' ,
  `stock_minimo` INT(11) NULL DEFAULT '0' ,
  `stock_real` INT(11) NULL DEFAULT '0' ,
  `tipo_unidad` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `controlar_stock` INT(11) NULL DEFAULT '0' ,
  `pvp_incluye_iva` INT(11) NULL DEFAULT '0' ,
  `fecha_prevista_recepcion` DATE NULL DEFAULT NULL ,
  `cantidad_pendiente_recibir` INT(11) NULL DEFAULT '0' ,
  `unidades_reservadas` INT(11) NULL DEFAULT '0' ,
  `mostrar_web` TINYINT(1) NULL DEFAULT '0' ,
  `etiquetas` INT(11) NULL DEFAULT '0' ,
  `paquetes` INT(11) NULL DEFAULT '0' ,
  `localizacion_en_almacen` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_tipos_iva` INT(11) NULL DEFAULT NULL ,
  `id_subsub_familia` INT(11) NULL DEFAULT NULL ,
  `id_grupo_art` INT(11) NULL DEFAULT NULL ,
  `id_web` INT(11) NULL DEFAULT '0' ,
  `stock_fisico_almacen` INT(11) NULL DEFAULT NULL ,
  `articulo_promocionado` TINYINT(1) NULL DEFAULT NULL ,
  `descripcion_promocion` VARCHAR(45) NULL DEFAULT NULL ,
  `tipo_oferta` INT(1) NULL DEFAULT NULL ,
  `por_cada` INT(11) NULL DEFAULT NULL ,
  `regalo_de` INT(11) NULL DEFAULT NULL ,
  `porc_dto_web` DOUBLE NULL DEFAULT NULL ,
  `oferta_pvp_fijo` DOUBLE NULL DEFAULT NULL ,
  `comentario_oferta` TEXT NULL DEFAULT NULL ,
  `bImagen2` BLOB NULL DEFAULT NULL ,
  `bImagen3` BLOB NULL DEFAULT NULL ,
  `bImagen4` BLOB NULL DEFAULT NULL ,
  `margen` DOUBLE NULL DEFAULT NULL ,
  `margen_min` DOUBLE NULL DEFAULT NULL ,
  `coste_real` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `cCodigo_UNIQUE` (`codigo` ASC) ,
  INDEX `fk_articulos_1_idx` (`id_tipos_iva` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`articulos_prov_frec` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_articulo` INT(11) NULL DEFAULT NULL ,
  `id_proveedor` INT(11) NULL DEFAULT NULL ,
  `pvd` DOUBLE NULL DEFAULT NULL ,
  `oferta` VARCHAR(45) NULL DEFAULT NULL ,
  `codigo` VARCHAR(20) NULL DEFAULT NULL ,
  `descoferta` VARCHAR(45) NULL DEFAULT NULL ,
  `pvd_real` DOUBLE NULL DEFAULT NULL ,
  `id_divisa` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_articulos_prov_frec_1_idx` (`id_articulo` ASC) ,
  INDEX `fk_articulos_prov_frec_2_idx` (`id_proveedor` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`avisos` (
  `id` INT(11) NOT NULL ,
  `id_tipoaviso` INT(11) NULL DEFAULT NULL ,
  `id_usuario` INT(11) NULL DEFAULT NULL ,
  `aviso` TEXT NULL DEFAULT NULL ,
  `fecha_hora_aviso` DATETIME NULL DEFAULT NULL ,
  `completado` TINYINT(1) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_avisos_1_idx` (`id_tipoaviso` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`bancos` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `descripcion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `entidad` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `ofinicina` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dc` VARCHAR(2) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cuenta` VARCHAR(10) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL COMMENT '\n' ,
  `saldo` DECIMAL(9,2) NULL DEFAULT '0.00' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`calles` (
  `nombre_calle` VARCHAR(60) NULL DEFAULT NULL ,
  `inp_inf` VARCHAR(60) NULL DEFAULT NULL ,
  `inp_sup` VARCHAR(60) NULL DEFAULT NULL ,
  `par_inf` VARCHAR(60) NULL DEFAULT NULL ,
  `par_sup` VARCHAR(60) NULL DEFAULT NULL ,
  `cp` VARCHAR(60) NULL DEFAULT NULL ,
  `poblacion` VARCHAR(60) NULL DEFAULT NULL ,
  `provincia` VARCHAR(60) NULL DEFAULT NULL ,
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`cliente_direcciones` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `descripcion` VARCHAR(45) NULL DEFAULT NULL ,
  `direccion1` VARCHAR(45) NULL DEFAULT NULL ,
  `direccion2` VARCHAR(45) NULL DEFAULT NULL ,
  `cp` VARCHAR(8) NULL DEFAULT NULL ,
  `poblacion` VARCHAR(45) NULL DEFAULT NULL ,
  `provincia` VARCHAR(45) NULL DEFAULT NULL ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `id_cliente` INT(11) NULL DEFAULT NULL ,
  `direccion_envio` TINYINT(1) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_table1_1_idx` (`id_pais` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`clientes` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `codigo_cliente` VARCHAR(25) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `apellido1` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `apellido2` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `nombre` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `nombre_fiscal` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `nombre_comercial` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `persona_contacto` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cif_nif` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion1` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion2` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp` VARCHAR(12) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `telefono1` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `telefono2` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fax` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `movil` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `email` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `web` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha_alta` DATE NULL DEFAULT NULL ,
  `fecha_ultima_compra` DATE NULL DEFAULT NULL ,
  `acumulado_ventas` DOUBLE NULL DEFAULT '0' ,
  `ventas_ejercicio` DOUBLE NULL DEFAULT '0' ,
  `riesgo_maximo` DOUBLE NULL DEFAULT '0' ,
  `deuda_actual` DOUBLE NULL DEFAULT '0' ,
  `comentarios` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `bloqueado` DOUBLE NULL DEFAULT '0' ,
  `comentario_bloqueo` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `porc_dto_cliente` FLOAT NULL DEFAULT '0' ,
  `recargo_equivalencia` TINYINT(1) NULL DEFAULT '0' ,
  `cuenta_contable` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cuenta_iva_repercutido` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cuenta_deudas` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cuenta_cobros` VARCHAR(30) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_forma_pago` INT(11) NULL DEFAULT NULL ,
  `dia_pago1` INT(11) NULL DEFAULT '0' ,
  `dia_pago2` INT(11) NULL DEFAULT '0' ,
  `tarifa_cliente` INT(11) NULL DEFAULT '0' ,
  `importe_a_cuenta` DOUBLE NULL DEFAULT '0' ,
  `vales` DOUBLE NULL DEFAULT '0' ,
  `entidad_bancaria` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `oficina_bancaria` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dc` VARCHAR(2) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cuenta_corriente` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha_nacimiento` DATE NULL DEFAULT NULL ,
  `importe_pendiente` DOUBLE NULL DEFAULT '0' ,
  `id_tipo_cliente` INT(11) NULL DEFAULT NULL ,
  `acceso_web` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `password_web` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `irpf` INT(11) NULL DEFAULT '0' ,
  `id_empresa` INT(11) NULL DEFAULT NULL ,
  `id_idioma_documentos` INT(11) NULL DEFAULT NULL ,
  `cif_vies` VARCHAR(35) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_web` INT(11) NULL DEFAULT NULL ,
  `id_monedas` INT(11) NULL DEFAULT NULL ,
  `observaciones` VARCHAR(255) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `visa_distancia1` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `visa_distancia2` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `visa1_caduca_mes` INT(2) NULL ,
  `visa2_caduca_mes` INT(2) NULL ,
  `visa1_caduca_ano` INT(4) NULL ,
  `visa2_caduca_ano` INT(4) NULL ,
  `visa1_cod_valid` INT(3) NULL ,
  `visa2_cod_valid` INT(3) NULL ,
  `id_agente` INT(11) NULL ,
  `id_transportista` INT(11) NULL ,
  `grupo_iva` VARCHAR(45) NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_clientes_2_idx` (`id_idioma_documentos` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`clientes_deuda` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `fecha` DATE NULL DEFAULT NULL ,
  `vencimiento` DATE NULL DEFAULT NULL ,
  `documento` INT(11) NULL DEFAULT '0' ,
  `id_ticket` INT(11) NULL DEFAULT '0' ,
  `id_factura` INT(11) NULL DEFAULT '0' ,
  `tipo` INT(11) NULL DEFAULT '0' ,
  `importe` DOUBLE NULL DEFAULT '0' ,
  `pagado` DOUBLE NULL DEFAULT '0' ,
  `pendiente_cobro` DOUBLE NULL DEFAULT '0' ,
  `entidad` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `oficina` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dc` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cuenta` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `importe_efectivo` DOUBLE NULL DEFAULT NULL ,
  `importe_tarjeta` DOUBLE NULL DEFAULT NULL ,
  `importe_cheque` DOUBLE NULL DEFAULT NULL ,
  `importe_transferencia` DOUBLE NULL DEFAULT NULL ,
  `importe_internet` DOUBLE NULL DEFAULT NULL ,
  `id_cliente` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_clientes_deuda_2_idx` (`id_ticket` ASC) ,
  INDEX `fk_clientes_deuda_3_idx` (`id_factura` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`clientes_entregas` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_cliente` INT(11) NULL DEFAULT '0' ,
  `fecha_entrega` DATE NULL DEFAULT NULL ,
  `importe` DOUBLE NULL DEFAULT '0' ,
  `concepto` INT(11) NULL DEFAULT '0' ,
  `disponible` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_clientes_entregas_1_idx` (`id_cliente` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`clientes_intervalo_horario` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `dia_semana` VARCHAR(45) NULL DEFAULT NULL ,
  `horario_manana_inicio` TIME NULL DEFAULT NULL ,
  `horario_manana_fin` TIME NULL DEFAULT NULL ,
  `horario_tarde_inicio` TIME NULL DEFAULT NULL ,
  `horario_tarde_fin` TIME NULL DEFAULT NULL ,
  `id_cliente` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`agentes` (
  `id` INT NOT NULL AUTO_INCREMENT ,
  `nombre` VARCHAR(45) NULL ,
  `codigo` VARCHAR(15) NULL ,
  `dni` VARCHAR(15) NULL ,
  `telefono` VARCHAR(15) NULL ,
  `movill` VARCHAR(15) NULL ,
  `email` VARCHAR(150) NULL ,
  `comentarios` TEXT NULL ,
  `facturado` DOUBLE NULL ,
  `pendiente` DOUBLE NULL ,
  PRIMARY KEY (`id`) )
  ENGINE = InnoDB
  DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`transportista` (
  `id` INT NOT NULL AUTO_INCREMENT ,
  `codigo` VARCHAR(15) NULL ,
  `transportista` VARCHAR(70) NULL ,
  `cif` VARCHAR(13) NULL ,
  `direccion1` VARCHAR(45) NULL ,
  `direccion2` VARCHAR(45) NULL ,
  `cp` VARCHAR(10) NULL ,
  `poblacion` VARCHAR(45) NULL ,
  `provincia` VARCHAR(45) NULL ,
  `pais` VARCHAR(45) NULL ,
  `telefono1` VARCHAR(20) NULL ,
  `telefono2` VARCHAR(20) NULL ,
  `fax` VARCHAR(20) NULL ,
  `movil` VARCHAR(20) NULL ,
  `email` VARCHAR(100) NULL ,
  `web` VARCHAR(100) NULL ,
  PRIMARY KEY (`id`) )
  ENGINE = InnoDB
  DEFAULT CHARACTER SET = utf8;


CREATE  TABLE IF NOT EXISTS `@grupo@`.`codigotarifa` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `descripcion` VARCHAR(45) NULL DEFAULT NULL ,
  `codigo_tarifa` VARCHAR(20) NULL DEFAULT NULL ,
  `dto` DOUBLE NULL DEFAULT NULL ,
  `desc_promo` VARCHAR(45) NULL DEFAULT NULL ,
  `cant_promo` INT(11) NULL DEFAULT NULL ,
  `cant_base` INT(11) NULL DEFAULT NULL ,
  `importe_dto` DOUBLE NULL DEFAULT NULL ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `id_monedas` INT(11) NULL DEFAULT NULL ,
  `margen` DOUBLE NULL DEFAULT NULL ,
  `margen_min` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `descripcion_UNIQUE` (`descripcion` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`departamento` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `departamento` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`deudas_proveedores` (
  `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '	' ,
  `id_documento` INT(11) NULL DEFAULT NULL ,
  `documento` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fecha_deuda` DATE NULL DEFAULT NULL ,
  `vencimiento` DATE NULL DEFAULT NULL ,
  `importe_deuda` DOUBLE NULL DEFAULT NULL ,
  `pago_por` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `numero_tarjeta_cuenta` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_proveedor` INT(11) NULL DEFAULT NULL ,
  `pendiente` DOUBLE NULL DEFAULT NULL ,
  `id_asiento` INT(11) NULL DEFAULT NULL ,
  `asiento_numero` INT(11) NULL DEFAULT NULL ,
  `pagado` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`empresas` ( `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `codigo` VARCHAR(10) NULL DEFAULT NULL , 
  `nombre` VARCHAR(75) NULL DEFAULT NULL ,
  `digitos_factura` INT(11) NULL DEFAULT NULL , 
  `serie` INT(11) NULL DEFAULT NULL , 
  `ruta_bd_sqlite` TEXT NULL DEFAULT NULL , 
  `ruta_db_conta` TEXT NULL DEFAULT NULL , 
  `ruta_bd_medica_sqlite` TEXT NULL DEFAULT NULL , 
  `nombre_bd` VARCHAR(50) NULL DEFAULT NULL , 
  `nombre_db_conta` VARCHAR(100) NULL DEFAULT NULL , 
  `nombre_bd_medica` VARCHAR(45) NULL DEFAULT NULL , 
  `direccion` VARCHAR(150) NULL DEFAULT NULL , 
  `cp` VARCHAR(8) NULL DEFAULT NULL , 
  `poblacion` VARCHAR(60) NULL DEFAULT NULL , 
  `provincia` VARCHAR(50) NULL DEFAULT NULL , 
  `pais` VARCHAR(40) NULL DEFAULT NULL , 
  `telefono1` VARCHAR(20) NULL DEFAULT NULL , 
  `telefono2` VARCHAR(20) NULL DEFAULT NULL , 
  `fax` VARCHAR(20) NULL DEFAULT NULL , 
  `mail` TEXT NULL DEFAULT NULL , 
  `web` TEXT NULL DEFAULT NULL , 
  `cif` VARCHAR(25) NULL DEFAULT NULL , 
  `inscripcion` TEXT NULL DEFAULT NULL , 
  `comentario_albaran` TEXT NULL DEFAULT NULL , 
  `comentario_factura` TEXT NULL DEFAULT NULL , 
  `comentario_ticket` TEXT NULL DEFAULT NULL , 
  `ejercicio` INT(11) NULL DEFAULT NULL , 
  `usar_irpf` INT(11) NULL DEFAULT NULL , 
  `codigo_cuenta_clientes` VARCHAR(25) NULL DEFAULT NULL , 
  `codigo_cuenta_proveedores` VARCHAR(25) NULL DEFAULT NULL , 
  `codigo_cuenta_acreedores` VARCHAR(25) NULL DEFAULT NULL , 
  `digitos_cuenta` INT(11) NULL DEFAULT NULL , 
  `clave1` TEXT NULL DEFAULT NULL , 
  `clave2` TEXT NULL DEFAULT NULL , 
  `medica` INT(11) NULL DEFAULT NULL , 
  `internacional` INT(11) NULL DEFAULT NULL , 
  `auto_codigo` TINYINT(1) NULL DEFAULT NULL , 
  `tamano_codigo` INT(11) NULL DEFAULT NULL , 
  `cuenta_cobros` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_pagos` VARCHAR(45) NULL DEFAULT NULL , 
  `id_divisa` INT(11) NULL DEFAULT NULL , 
  `enlace_web` TINYINT(1) NULL DEFAULT NULL , 
  `contabilidad` TINYINT(1) NULL DEFAULT NULL , 
  `consultas` INT(11) NULL DEFAULT NULL , 
  `primer_dia_laborable` VARCHAR(45) NULL DEFAULT NULL , 
  `ultimo_dia_laborable` VARCHAR(45) NULL DEFAULT NULL , 
  `horario_primer_dia` VARCHAR(45) NULL DEFAULT NULL , 
  `horario_dia_normal` VARCHAR(45) NULL DEFAULT NULL , 
  `horario_ultimo_dia` VARCHAR(45) NULL DEFAULT NULL , 
  `ticket_factura` TINYINT(1) NULL DEFAULT NULL , 
  `margen` DOUBLE NULL DEFAULT NULL , 
  `margen_minimo` DOUBLE NULL DEFAULT NULL , 
  `seguimiento` TINYINT(1) NULL DEFAULT NULL , 
  `id_tarifa_predeterminada` INT(11) NULL DEFAULT NULL , 
  `actualizar_divisas` TINYINT(1) NULL DEFAULT NULL , 
  `cuenta_ventas_mercaderias` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_ventas_servicios` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_soportado1` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_soportado2` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_soportado3` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_soportado4` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_repercutido1` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_repercutido2` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_repercutido3` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_repercutido4` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_repercutido1_re` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_repercutido2_re` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_repercutido3_re` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_repercutido4_re` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_soportado1_re` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_soportado2_re` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_soportado3_re` VARCHAR(45) NULL DEFAULT NULL , 
  `cuenta_iva_soportado4_re` VARCHAR(45) NULL DEFAULT NULL , 
  PRIMARY KEY (`id`) )
 ENGINE = InnoDB AUTO_INCREMENT = 1 
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`familias` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `codigo` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `familia` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_seccion` INT(11) NULL DEFAULT '0' ,
  `image` BLOB NULL DEFAULT NULL ,
  `type` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`formpago` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `codigo` VARCHAR(8) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `forma_pago` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dia_pago1` INT(11) NULL DEFAULT '0' ,
  `dia_pago2` INT(11) NULL DEFAULT '0' ,
  `dia_pago3` INT(11) NULL DEFAULT '0' ,
  `dia_pago4` INT(11) NULL DEFAULT '0' ,
  `dias1` INT(11) NULL DEFAULT '0' ,
  `dias2` INT(11) NULL DEFAULT '0' ,
  `dias3` INT(11) NULL DEFAULT '0' ,
  `dias4` INT(11) NULL DEFAULT '0' ,
  `cuenta_cont_pago` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `numero_plazos` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`grupos_gasto` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `descripcion` VARCHAR(100) NULL DEFAULT NULL ,
  `enero` DOUBLE NULL DEFAULT NULL ,
  `febrero` DOUBLE NULL DEFAULT NULL ,
  `marzo` DOUBLE NULL DEFAULT NULL ,
  `abril` DOUBLE NULL DEFAULT NULL ,
  `mayo` DOUBLE NULL DEFAULT NULL ,
  `junio` DOUBLE NULL DEFAULT NULL ,
  `julio` DOUBLE NULL DEFAULT NULL ,
  `agosto` DOUBLE NULL DEFAULT NULL ,
  `septiembre` DOUBLE NULL DEFAULT NULL ,
  `octubre` DOUBLE NULL DEFAULT NULL ,
  `noviembre` DOUBLE NULL DEFAULT NULL ,
  `diciembre` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`gruposart` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_subsubfamilia` INT(11) NULL DEFAULT NULL ,
  `grupo_art` VARCHAR(45) NULL DEFAULT NULL ,
  `image` BLOB NULL DEFAULT NULL ,
  `type` VARCHAR(4) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_gruposart_1_idx` (`id_subsubfamilia` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`idiomas` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `idioma` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`maestro_familia_cliente` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `descripcion` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`maestro_subfamilia_cliente` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `descripcion` VARCHAR(45) NULL DEFAULT NULL ,
  `id_maestro_familia_cliente` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_maestro_subfamilia_cliente_1_idx` (`id_maestro_familia_cliente` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`modulos` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `module_zone` INT(11) NULL DEFAULT NULL ,
  `module_name` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`monedas` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `moneda` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `nombre_corto` VARCHAR(10) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `simbolo` VARCHAR(5) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cambio` DECIMAL(9,4) NULL DEFAULT NULL ,
  `fecha_cambio` DATE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `nombreCorto_UNIQUE` (`nombre_corto` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`nivelacceso` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `nombre` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`paises` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `pais` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_monedas` INT(11) NULL DEFAULT NULL ,
  `country_code` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `countryCode_UNIQUE` (`country_code` ASC) ,
  UNIQUE INDEX `pais_UNIQUE` (`pais` ASC) ,
  INDEX `fk_paises_1_monedas_id_idx` (`id_monedas` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`permisos_agenda` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_usuario_editor` INT(11) NULL DEFAULT NULL ,
  `id_usuario_agenda` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`personascontactocliente` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `nombre` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `telefono1` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `telefono2` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `telefono3` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `movil` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `movil2` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_cliente` INT(11) NULL DEFAULT NULL ,
  `desc_telefono1` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_telefono2` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_telefono3` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_movil1` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_movil2` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cargo_empresa` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `email` VARCHAR(150) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `id_UNIQUE` (`id` ASC) ,
  INDEX `fk_Personascontactocliente_1_idx` (`id_cliente` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`personascontactoproveedor` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_proveedor` INT(11) NULL DEFAULT NULL ,
  `nombre` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `telefono1` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `telefono2` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `telefono3` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `movil1` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `movil2` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_telefono1` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_telefono2` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_telefono3` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_movil1` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `desc_movil2` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cargo_empresa` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `email` VARCHAR(150) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`poblaciones` (
  `cod_pais` INT(11) NULL DEFAULT NULL ,
  `poblacion` VARCHAR(60) NULL DEFAULT NULL ,
  `cp` VARCHAR(60) NULL DEFAULT NULL ,
  `cod_pro` VARCHAR(60) NULL DEFAULT NULL ,
  `provincia` VARCHAR(60) NULL DEFAULT NULL ,
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`proveedor_a_cuenta` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `importe` DOUBLE NULL DEFAULT NULL ,
  `fecha_entrega` DATE NULL DEFAULT NULL ,
  `concepto` VARCHAR(45) NULL DEFAULT NULL ,
  `id_proveedor` INT(11) NULL DEFAULT NULL ,
  `disponible` DOUBLE NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_proveedor_a_cuenta_1_idx` (`id_proveedor` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`proveedores` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `codigo` INT(11) NULL DEFAULT '0' ,
  `proveedor` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cif` VARCHAR(15) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion1` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `direccion2` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp` VARCHAR(12) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `telefono1` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `telefono2` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `telefono3` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fax` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `movil` VARCHAR(20) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `email` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `web` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `persona_contacto` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dia_cobro` INT(11) NULL DEFAULT '0' ,
  `direccion_almacen` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cp_almacen` VARCHAR(12) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `poblacion_almacen` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `provincia_almacen` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_pais_almacen` INT(11) NULL DEFAULT NULL ,
  `telefono_almacen` VARCHAR(15) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `fax_almacen` VARCHAR(15) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_forma_pago` INT(11) NULL DEFAULT NULL ,
  `fecha_ultima_compra` DATE NULL DEFAULT NULL ,
  `importe_acumulado_compras` DOUBLE NULL DEFAULT '0' ,
  `entidad_bancaria_proveedor` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `oficina_bancaria_proveedor` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dc_proveedor` VARCHAR(2) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `cc_proveedor` VARCHAR(10) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `entidad_pago_proveedor` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `oficina_pago_proveedor` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dc_pago_proveedor` VARCHAR(2) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `retencion_irpf` DOUBLE NULL DEFAULT '0' ,
  `tipo_retencion` INT(11) NULL DEFAULT '0' ,
  `cuenta_aplicacion` VARCHAR(100) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `comentarios` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `dto` FLOAT NULL DEFAULT '0' ,
  `fecha_alta` DATE NULL DEFAULT NULL ,
  `deuda_maxima` DOUBLE NULL DEFAULT '0' ,
  `deuda_actual` DOUBLE NULL DEFAULT '0' ,
  `recargo_equivalencia` INT(11) NULL DEFAULT '0' ,
  `texto_para_pedidos` TEXT CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `entregado_a_cuenta` DOUBLE NULL DEFAULT '0' ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `id_monedas` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`report` (
  `report_id` INT(11) NOT NULL AUTO_INCREMENT ,
  `report_name` TEXT NOT NULL ,
  `report_descrip` TEXT NULL DEFAULT NULL ,
  `report_source` TEXT NOT NULL ,
  `report_grade` INT(11) NOT NULL ,
  PRIMARY KEY (`report_id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`secciones` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `seccion` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_web` INT(11) NULL DEFAULT NULL ,
  `image` BLOB NULL DEFAULT NULL ,
  `type` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `codigo` VARCHAR(3) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`subfamilias` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `sub_familia` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `id_familia` INT(11) NULL DEFAULT '0' ,
  `image` BLOB NULL DEFAULT NULL ,
  `type` VARCHAR(4) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `codigo` VARCHAR(3) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_subfamilias_subfamilias_idx` (`id_familia` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`subsubfamilias` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_subfamilia` INT(11) NULL DEFAULT NULL ,
  `subsub_familia` VARCHAR(45) NULL DEFAULT NULL ,
  `im` BLOB NULL DEFAULT NULL ,
  `type` VARCHAR(4) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_subsubfamilias_subf_id_idx` (`id_subfamilia` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`tarifas` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_articulo` INT(11) NULL DEFAULT NULL ,
  `id_pais` INT(11) NULL DEFAULT NULL ,
  `id_monedas` INT(11) NULL DEFAULT NULL ,
  `margen` DECIMAL(9,2) NULL DEFAULT NULL ,
  `margen_minimo` DECIMAL(9,2) NULL DEFAULT NULL ,
  `pvp` DECIMAL(9,2) NULL DEFAULT NULL ,
  `id_codigo_tarifa` INT(11) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_tarifas_1_articulos_id_idx` (`id_articulo` ASC) ,
  INDEX `fk_tarifas_1_paises_id_idx` (`id_pais` ASC) ,
  INDEX `fk_tarifas_1_moneda_id_idx` (`id_monedas` ASC) ,
  INDEX `fk_tarifas_1_idx` (`id_codigo_tarifa` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`tipocliente` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `tipo_cliente` VARCHAR(45) NULL DEFAULT NULL ,
  `id_cliente` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`tiposaviso` (
  `id` INT(11) NOT NULL ,
  `tipoaviso` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`tiposiva` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `nombre_interno` VARCHAR(45) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NOT NULL ,
  `tipo` VARCHAR(25) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `descripcion_tipo_iva` VARCHAR(50) CHARACTER SET 'utf8' COLLATE 'utf8_unicode_ci' NULL DEFAULT NULL ,
  `iva` DECIMAL(9,2) NULL DEFAULT '0.00' ,
  `recargo_equivalencia` DECIMAL(9,2) NULL DEFAULT '0.00' ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`trazabilidad1` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_articulo` INT(11) NULL DEFAULT NULL ,
  `lote` VARCHAR(45) NULL DEFAULT NULL ,
  `cantidad` INT(11) NULL DEFAULT NULL ,
  `stock_lote` INT(11) NULL DEFAULT NULL ,
  `stock_real` INT(11) NULL DEFAULT NULL ,
  `fecha_compra` DATE NULL DEFAULT NULL ,
  `fecha_caducidad` DATE NULL DEFAULT NULL ,
  `id_fac_pro` INT(11) NULL DEFAULT NULL ,
  `fac_pro` VARCHAR(45) NULL DEFAULT NULL ,
  `proveedor` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_trazabilidad1_1_idx` (`id_articulo` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`trazabilidad2` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_trazabilidad1` INT(11) NULL DEFAULT NULL ,
  `id_documento_venta` INT(11) NULL DEFAULT NULL ,
  `documento_venta` VARCHAR(45) NULL DEFAULT NULL ,
  `id_ticket` INT(11) NULL DEFAULT NULL ,
  `numero_ticket` INT(11) NULL DEFAULT NULL ,
  `unidades_vendidas` VARCHAR(45) NULL DEFAULT NULL ,
  `fecha_venta` DATE NULL DEFAULT NULL ,
  `codigo_cuenta_cliente` VARCHAR(25) NULL DEFAULT NULL ,
  `cliente` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_trazabilidad2_1_idx` (`id_trazabilidad1` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE  TABLE IF NOT EXISTS `@grupo@`.`usuarios` (
  `id` INT(11) NOT NULL AUTO_INCREMENT ,
  `nombre` VARCHAR(45) NULL DEFAULT NULL ,
  `contrasena` VARCHAR(45) NULL DEFAULT NULL ,
  `nivel_acceso` INT(4) NULL DEFAULT NULL ,
  `categoria` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `nombre_UNIQUE` (`nombre` ASC) )
ENGINE = InnoDB
AUTO_INCREMENT = 1
DEFAULT CHARACTER SET = utf8;

CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`proveedores_frecuentes` AS select `@grupo@`.`articulos_prov_frec`.`id_articulo` AS `id_art`,`@grupo@`.`proveedores`.`codigo` AS `cod_pro`,`@grupo@`.`proveedores`.`proveedor` AS `proveedor`,`@grupo@`.`proveedores`.`id` AS `id_prov`,`@grupo@`.`articulos_prov_frec`.`pvd` AS `pvd`,`@grupo@`.`articulos_prov_frec`.`oferta` AS `oferta`,`@grupo@`.`articulos_prov_frec`.`codigo` AS `codigo`,`@grupo@`.`articulos_prov_frec`.`descoferta` AS `descoferta`,`@grupo@`.`articulos_prov_frec`.`pvd_real` AS `pvd_real`,`@grupo@`.`monedas`.`moneda` AS `moneda`,`@grupo@`.`articulos_prov_frec`.`id` AS `id` from ((`@grupo@`.`articulos_prov_frec` join `@grupo@`.`proveedores` on((`@grupo@`.`articulos_prov_frec`.`id_proveedor` = `@grupo@`.`proveedores`.`id`))) left join `@grupo@`.`monedas` on((`@grupo@`.`articulos_prov_frec`.`id_divisa` = `@grupo@`.`monedas`.`id`)));

CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`viewtarifa` AS select `@grupo@`.`tarifas`.`id` AS `id`,`@grupo@`.`paises`.`pais` AS `pais`,`@grupo@`.`monedas`.`moneda` AS `moneda`,`@grupo@`.`monedas`.`simbolo` AS `simbolo`,`@grupo@`.`codigotarifa`.`codigo_tarifa` AS `codigo_tarifa`,`@grupo@`.`codigotarifa`.`descripcion` AS `descripcion`,`@grupo@`.`tarifas`.`margen` AS `margen`,`@grupo@`.`tarifas`.`margen_minimo` AS `margen_minimo`,`@grupo@`.`tarifas`.`pvp` AS `pvp`,`@grupo@`.`tarifas`.`id_articulo` AS `id_Articulo` from (((`@grupo@`.`tarifas` join `@grupo@`.`monedas` on((`@grupo@`.`tarifas`.`id_monedas` = `@grupo@`.`monedas`.`id`))) join `@grupo@`.`paises` on((`@grupo@`.`tarifas`.`id_pais` = `@grupo@`.`paises`.`id`))) join `@grupo@`.`codigotarifa` on((`@grupo@`.`tarifas`.`id_codigo_tarifa` = `@grupo@`.`codigotarifa`.`id`)));

CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`viewtrazabilidad1` AS select `@grupo@`.`trazabilidad1`.`id` AS `id`,`@grupo@`.`trazabilidad1`.`lote` AS `lote`,`@grupo@`.`trazabilidad1`.`cantidad` AS `cantidad`,`@grupo@`.`trazabilidad1`.`stock_lote` AS `stock_lote`,`@grupo@`.`trazabilidad1`.`stock_real` AS `stock_real`,`@grupo@`.`trazabilidad1`.`fac_pro` AS `fac_pro`,`@grupo@`.`trazabilidad1`.`id_articulo` AS `id_articulo`,`@grupo@`.`trazabilidad1`.`fecha_compra` AS `fecha_compra`,`@grupo@`.`trazabilidad1`.`fecha_caducidad` AS `fecha_caducidad`,`@grupo@`.`trazabilidad1`.`proveedor` AS `proveedor`,`@grupo@`.`articulos`.`descripcion` AS `descripcion`,`@grupo@`.`articulos`.`codigo` AS `codigo` from (`@grupo@`.`trazabilidad1` join `@grupo@`.`articulos` on((`@grupo@`.`trazabilidad1`.`id_articulo` = `@grupo@`.`articulos`.`id`)));

CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`vistaart_tarifa` AS select `@grupo@`.`articulos`.`codigo` AS `codigo`,`@grupo@`.`articulos`.`codigo_barras` AS `codigo_barras`,`@grupo@`.`articulos`.`codigo_fabricante` AS `codigo_fabricante`,`@grupo@`.`articulos`.`descripcion` AS `descripcion`,`@grupo@`.`articulos`.`descripcion_reducida` AS `descripcion_reducida`,`@grupo@`.`articulos`.`tipo_iva` AS `tipo_iva`,`@grupo@`.`articulos`.`coste` AS `coste`,`@grupo@`.`tarifas`.`pvp` AS `pvp`,`@grupo@`.`articulos`.`stock_maximo` AS `stock_maximo`,`@grupo@`.`articulos`.`stock_minimo` AS `stock_minimo`,`@grupo@`.`articulos`.`stock_real` AS `stock_real`,`@grupo@`.`articulos`.`controlar_stock` AS `controlar_stock`,`@grupo@`.`articulos`.`stock_fisico_almacen` AS `stock_fisico_almacen`,`@grupo@`.`tarifas`.`id_codigo_tarifa` AS `tarifa` from ((`@grupo@`.`articulos` left join `@grupo@`.`proveedores` on((`@grupo@`.`articulos`.`id_proveedor` = `@grupo@`.`proveedores`.`id`))) join `@grupo@`.`tarifas` on((`@grupo@`.`articulos`.`id` = `@grupo@`.`tarifas`.`id_articulo`)));

CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`vistaarticulos` AS select `@grupo@`.`articulos`.`codigo` AS `codigo`,`@grupo@`.`articulos`.`codigo_barras` AS `codigo_barras`,`@grupo@`.`articulos`.`codigo_fabricante` AS `codigo_fabricante`,`@grupo@`.`articulos`.`descripcion` AS `descripcion`,`@grupo@`.`proveedores`.`proveedor` AS `proveedor`,`@grupo@`.`articulos`.`tipo_iva` AS `tipo_iva`,`@grupo@`.`articulos`.`coste` AS `coste`,`@grupo@`.`articulos`.`fecha_ultima_compra` AS `fecha_ultima_compra`,`@grupo@`.`articulos`.`fecha_ultima_venta` AS `fecha_ultima_venta`,`@grupo@`.`articulos`.`unidades_compradas` AS `unidades_compradas`,`@grupo@`.`articulos`.`unidades_vendidas` AS `unidades_vendidas`,`@grupo@`.`articulos`.`stock_maximo` AS `stock_maximo`,`@grupo@`.`articulos`.`stock_minimo` AS `stock_minimo`,`@grupo@`.`articulos`.`stock_real` AS `stock_real`,`@grupo@`.`articulos`.`controlar_stock` AS `controlar_stock`,`@grupo@`.`articulos`.`stock_fisico_almacen` AS `stock_fisico_almacen` from (`@grupo@`.`articulos` left join `@grupo@`.`proveedores` on((`@grupo@`.`articulos`.`id_proveedor` = `@grupo@`.`proveedores`.`id`)));

CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `@grupo@`.`vistaempresa` AS select concat_ws(' ',`@grupo@`.`empresas`.`nombre`,`@grupo@`.`empresas`.`ejercicio`) AS `empresa`,`@grupo@`.`empresas`.`id` AS `id` from `@grupo@`.`empresas`;
