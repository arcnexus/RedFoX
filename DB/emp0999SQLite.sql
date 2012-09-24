CREATE TABLE "agenda" (
  "Id" INTEGER PRIMARY KEY AUTOINCREMENT  ,
  "dFecha" numeric  ,
  "cHora" text    ,
  "id_Usuario" int DEFAULT 0,
  "cInicio" text  ,
  "cFin" text  ,
  "cAsunto" text    ,
  "cDescripcionAsunto" text  ,
  "cEstado" text    ,
  "cAvisarTiempo" text    ,
  "cImportancia" text    
   
)  ;


CREATE TABLE "alb_pro" (
  "Id" INTEGER PRIMARY KEY AUTOINCREMENT  ,
  "cAlbaran" text    ,
  "dFecha" numeric  ,
  "id_Proveedor" int DEFAULT 0,
  "cProveedor" text    ,
  "cCifProveedor" text    ,
  "rTotal1" real DEFAULT 0,
  "rTotal2" real DEFAULT 0,
  "rTotal3" real DEFAULT 0,
  "rTotal4" real DEFAULT 0,
  "nIva1" real DEFAULT 0,
  "nIva2" real DEFAULT 0,
  "nIva3" real DEFAULT 0,
  "nIva4" real DEFAULT 0,
  "rIva1" real DEFAULT 0,
  "rIva2" real DEFAULT 0,
  "rIva3" real DEFAULT 0,
  "rIva4" real DEFAULT 0,
  "rBase1" real DEFAULT 0,
  "rBase2" real DEFAULT 0,
  "rBase3" real DEFAULT 0,
  "rBase4" real DEFAULT 0,
  "cFactura" text    ,
  "rBaseTotal" real DEFAULT 0,
  "rIvaTotal" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "tComentario" text  ,
  "nPedido" int DEFAULT 0
   
)  ;



CREATE TABLE "articulos" (
  "Id" INTEGER PRIMARY KEY AUTOINCREMENT  ,
  "cCodigo" text    ,
  "cCodigoBarras" text    ,
  "cCodigoFabricante" text    ,
  "cDescripcion" text    ,
  "cDescripcionReducida" text    ,
  "id_Proveedor" int DEFAULT 0,
  "id_Familia" int DEFAULT 0,
  "cFamilia" text    ,
  "id_Seccion" int DEFAULT 0,
  "cSeccion" text    ,
  "id_Subfamilia" int DEFAULT 0,
  "cSubfamilia" text    ,
  "cTipoIva" text    ,
  "rCoste" real DEFAULT 0,
  "rTarifa1" real DEFAULT 0,
  "rTarifa2" real DEFAULT 0,
  "rTarifa3" real DEFAULT 0,
  "rDto" real DEFAULT 0,
  "nDtoProveedor" real DEFAULT 0,
  "nDtoproveedor2" real DEFAULT 0,
  "nDtoProveedor3" real DEFAULT 0,
  "dUltimaCompra" numeric  ,
  "dUltimaVenta" numeric  ,
  "nMargen1" real DEFAULT 0,
  "nMargen2" real DEFAULT 0,
  "nMargen3" real DEFAULT 0,
  "rPrecioMedio" real DEFAULT 0,
  "rPrecioMedio2" real DEFAULT 0,
  "rPrecioMedio3" real DEFAULT 0,
  "nUnidadesCompradas" int DEFAULT 0,
  "nUnidadesVendidas" int DEFAULT 0,
  "rAcumuladoCompras" real DEFAULT 0,
  "rAcumuladoVentas" real DEFAULT 0,
  "bImagen" blob,
  "tComentario" text  ,
  "nStockMaximo" int DEFAULT 0,
  "nStockMinimo" int DEFAULT 0,
  "nStockReal" int DEFAULT 0,
  "cTipoUnidad" text    ,
  "lControlarStock" int DEFAULT 0,
  "cModelo" text    ,
  "cTalla" text    ,
  "cColor" text    ,
  "cComposicion" text  ,
  "lPvpIncluyeIva" int DEFAULT 0,
  "dFechaPrevistaRecepcion" numeric,
  "nCantidadPendienteRecibir" int DEFAULT 0,
  "nReservados" int DEFAULT 0,
  "lMostrarWeb" int DEFAULT 0,
  "nEtiquetas" int DEFAULT 0,
  "nPaquetes" int DEFAULT 0,
  "cLocalizacion" text 
    
) ;





CREATE TABLE "bancos" (
  "Id" INTEGER PRIMARY KEY AUTOINCREMENT  ,
  "cDescripcion" text    ,
  "cEntidad" text    ,
  "cOficina" text    ,
  "cDc" text    ,
  "cCuenta" text    ,
  "rSaldo" real DEFAULT 0
   
)  ;







CREATE TABLE "cab_alb" (
  "Id" INTEGER PRIMARY KEY AUTOINCREMENT  not null ,
  "nAlbaran" int DEFAULT 0,
  "dFecha" numeric  ,
  "cPedidoCli" text    ,
  "id_Cliente" int DEFAULT 0,
  "cCodigoCliente" text    ,
  "cCliente" text    ,
  "CDireccion" text    ,
  "cDireccion2" text    ,
  "cPoblacion" text    ,
  "cProvincia" text    ,
  "cCP" text    ,
  "cPais" text    ,
  "cCif" text    ,
  "lRecargoEquivalencia" int DEFAULT 0,
  "rSubtotal" real DEFAULT 0,
  "rDto" real DEFAULT 0,
  "nDto" real DEFAULT 0,
  "rBase1" real DEFAULT 0,
  "rBase2" real DEFAULT 0,
  "rBase3" real DEFAULT 0,
  "rBase4" real DEFAULT 0,
  "nPorcentajeIva1" real DEFAULT 0,
  "nPorcentajeIva2" real DEFAULT 0,
  "nPorcentajeIva3" real DEFAULT 0,
  "nPorcentajeIva4" real DEFAULT 0,
  "rImporteIva1" real DEFAULT 0,
  "rImporteIva2" real DEFAULT 0,
  "rImporteIva3" real DEFAULT 0,
  "rImporteIva4" real DEFAULT 0,
  "nPorcentajeRecargoEq1" real DEFAULT 0,
  "nPorcentajeRecargoEq2" real DEFAULT 0,
  "nPorcentajeRecargoEq3" real DEFAULT 0,
  "nPorcentajeRecargoEq4" real DEFAULT 0,
  "rImporteRecargoEq1" real DEFAULT 0,
  "rImporteRecargoEq2" real DEFAULT 0,
  "rImporteRecargoEq3" real DEFAULT 0,
  "rImporteRecargoEq4" real DEFAULT 0,
  "rTotal1" real DEFAULT 0,
  "rTotal2" real DEFAULT 0,
  "rTotal3" real DEFAULT 0,
  "rTotal4" real DEFAULT 0,
  "rBaseTotal" real DEFAULT 0,
  "rIvaTotal" real DEFAULT 0,
  "rRecargoEqTotal" real DEFAULT 0,
  "rTotalAlbaran" real DEFAULT 0,
  "lImpreso" int DEFAULT 0,
  "lFacturado" int DEFAULT 0,
  "cFactura" text    ,
  "dFechaFactura" numeric  ,
  "tComentario" text  ,
  "rACuenta" real DEFAULT 0
   
)  ;



CREATE TABLE "cab_fac" (
  "Id" INTEGER PRIMARY KEY AUTOINCREMENT  ,
  "cCodigoCliente" text    ,
  "cFactura" text  ,
  "dFecha" numeric  ,
  "dFechaCobro" numeric  ,
  "iId_Cliente" int DEFAULT 0,
  "cCliente" text    ,
  "cDireccion" text    ,
  "cDireccion2" text    ,
  "cCp" text    ,
  "cPoblacion" text    ,
  "cProvincia" text    ,
  "cPais" text    ,
  "cCif" text    ,
  "lRecargoEquivalencia" int DEFAULT 0,
  "rSubtotal" real DEFAULT 0,
  "nDto" real DEFAULT 0,
  "nDtoPP" real DEFAULT 0,
  "rImporteDescuento" real DEFAULT 0,
  "rImporteDescuentoPP" real DEFAULT 0,
  "rBase" real DEFAULT 0,
  "nIRPF" real DEFAULT 0,
  "rImporteIRPF" real DEFAULT 0,
  "nIva" real DEFAULT 0,
  "rImporteIva" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "lImpresa" int DEFAULT 0,
  "lCobrada" int DEFAULT 0,
  "lContabilizada" int DEFAULT 0,
  "id_FormaPago" int DEFAULT 0,
  "cFormaPago" text    ,
  "tComentario" text  ,
  "rBase1" real DEFAULT 0,
  "rBase2" real DEFAULT 0,
  "rBase3" real DEFAULT 0,
  "rBase4" real DEFAULT 0,
  "nPorcentajeIVA1" real DEFAULT 0,
  "nPorcentajeIVA2" real DEFAULT 0,
  "nPorcentajeIVA3" real DEFAULT 0,
  "nPorcentajeIVA4" real DEFAULT 0,
  "rIVA1" real DEFAULT 0,
  "rIVA2" real DEFAULT 0,
  "rIVA3" real DEFAULT 0,
  "rIVA4" real DEFAULT 0,
  "rTotal1" real DEFAULT 0,
  "rTotal2" real DEFAULT 0,
  "rTotal3" real DEFAULT 0,
  "rTotal4" real DEFAULT 0,
  "nRec1" real DEFAULT 0,
  "nRec2" real DEFAULT 0,
  "nRec3" real DEFAULT 0,
  "nRec4" real DEFAULT 0,
  "rRecargoEq1" real DEFAULT 0,
  "rRecargoEq2" real DEFAULT 0,
  "rRecargoEq3" real DEFAULT 0,
  "rRecargoEq4" real DEFAULT 0,
  "rTotalRecargoEq" real DEFAULT 0,
  "rEntregadoaCuenta" real DEFAULT 0,
  "rImportePendiente" real DEFAULT 0,
  "cCodigoEntidad" text    ,
  "cOficinaEntidad" text    ,
  "cDCCuenta" text    ,
  "cNumeroCuenta" text    ,
  "cPedidoCliente" int DEFAULT 0
   
) ;







CREATE TABLE "cab_pre" (
  "Id" INTEGER PRIMARY KEY AUTOINCREMENT  ,
  "nPresupuesto" int DEFAULT 0,
  "dFecha" numeric  ,
  "dValidoHasta" numeric  ,
  "id_Cliente" int DEFAULT 0,
  "cCodigoCliente" text    ,
  "cCliente" text    ,
  "cCif" text    ,
  "cDireccion" text    ,
  "cDireccion2" text    ,
  "cCP" text    ,
  "cPoblacion" text    ,
  "cProvincia" text    ,
  "cPais" text    ,
  "cTelefono" text    ,
  "cMovil" text    ,
  "cFax" text    ,
  "nDto" real DEFAULT 0,
  "tComentarios" text  ,
  "rImporte" real DEFAULT 0,
  "rSubtotal" real DEFAULT 0,
  "rDescuento" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "lImpreso" int DEFAULT 0,
  "lAprovado" int DEFAULT 0,
  "dFechaAprobacion" numeric  ,
  "rImporteFactura" real DEFAULT 0,
  "rImportePendiente" real DEFAULT 0,
  "nFactura" int DEFAULT 0,
  "nAlbaran" int DEFAULT 0,
  "nPedido" int DEFAULT 0,
  "id_FormaPago" int DEFAULT 0,
  "cCodigoFormaPago" text    ,
  "cDescripcionFormaPago" text    ,
  "tLugarEntrega" text  ,
  "cAtencionde" text    ,
  "rImporte1" real DEFAULT 0,
  "rImporte2" real DEFAULT 0,
  "rImporte3" real DEFAULT 0,
  "rImporte4" real DEFAULT 0,
  "nIva1" real DEFAULT 0,
  "nIva2" real DEFAULT 0,
  "nIva3" real DEFAULT 0,
  "nIva4" real DEFAULT 0,
  "rIva1" real DEFAULT 0,
  "rIva2" real DEFAULT 0,
  "rIva3" real DEFAULT 0,
  "rIva4" real DEFAULT 0,
  "nRecargoEquivalencia1" real DEFAULT 0,
  "nRecargoEquivalencia2" real DEFAULT 0,
  "nRegargoEquivalencia3" real DEFAULT 0,
  "nRecargoEquivalencia4" real DEFAULT 0,
  "rRec1" real DEFAULT 0,
  "rRec2" real DEFAULT 0,
  "rRec3" real DEFAULT 0,
  "rRec4" real DEFAULT 0,
  "rTotal1" real DEFAULT 0,
  "rTotal2" real DEFAULT 0,
  "rTotal3" real DEFAULT 0,
  "rTotal4" real DEFAULT 0
   
)  ;



CREATE TABLE "cab_tpv" (
  "Id" INTEGER PRIMARY KEY AUTOINCREMENT  ,
  "nCaja" int DEFAULT 0,
  "nTicket" int DEFAULT 0,
  "dFecha" numeric  ,
  "cHora" text    ,
  "cUsuario" text    ,
  "cNombreUsuario" text    ,
  "id_Cliente" int DEFAULT 0,
  "cCliente" text    ,
  "cNombreCliente" text    ,
  "lticketEnEpera" int DEFAULT 0,
  "lPendienteCobro" int DEFAULT 0,
  "cFormaPago" text    ,
  "cTipoTarjeta" text    ,
  "lImpreso" int DEFAULT 0,
  "rImporte" real DEFAULT 0,
  "rBase1" real DEFAULT 0,
  "rBase2" real DEFAULT 0,
  "rBase3" real DEFAULT 0,
  "rBase4" real DEFAULT 0,
  "nPorcentajeIva1" real DEFAULT 0,
  "nPorcentajeIva2" real DEFAULT 0,
  "nPorcentajeIva3" real DEFAULT 0,
  "nPorcentajeIva4" real DEFAULT 0,
  "rImporteIva1" real DEFAULT 0,
  "rImporteIva2" real DEFAULT 0,
  "rImporteIva3" real DEFAULT 0,
  "rImporteIva4" real DEFAULT 0,
  "rTotal1" real DEFAULT 0,
  "rTotal2" real DEFAULT 0,
  "rTotal3" real DEFAULT 0,
  "rTotal4" real DEFAULT 0,
  "rSubotal" real DEFAULT 0,
  "rDto" real DEFAULT 0,
  "nPorcentajeRe1" real DEFAULT 0,
  "nPorcentajeRe2" real DEFAULT 0,
  "nPorcentajeRe3" real DEFAULT 0,
  "nPorcentajeRe4" real DEFAULT 0,
  "rImporteRecargoEq1" real DEFAULT 0,
  "rImporteRecargoEq2" real DEFAULT 0,
  "rImporteRecargoEq3" real DEFAULT 0,
  "rImporteRecargoEq4" real DEFAULT 0,
  "rTotalRecargoEq" real DEFAULT 0,
  "rImporteTotalIva" real DEFAULT 0,
  "rCobrado" real DEFAULT 0,
  "rPendienteCobro" real DEFAULT 0,
  "rImporteEfectivo" real DEFAULT 0,
  "rImporteTarjeta" real DEFAULT 0,
  "rImporteCheque" real DEFAULT 0,
  "rImporteCredito" real DEFAULT 0,
  "rImporteDtoRedondeo" real DEFAULT 0,
  "rImporteCambio" real DEFAULT 0,
  "rEntregado" real DEFAULT 0,
  "lPasadoAlbaranFactura" int DEFAULT 0,
  "nNumeroAlbaran" int DEFAULT 0,
  "nNumeroFactura" int DEFAULT 0
   
);




CREATE TABLE "cajaabierta" (
"id" integer primary key  autoincrement,
  "dFechaAbertura" numeric  ,
  "cHoraAbertura" text    ,
  "nCaja" int DEFAULT 0,
  "id_UsuarioAbertura" int DEFAULT 0,
  "cUsuarioAbertura" text    ,
  "id_UsuarioCierre" int DEFAULT 0,
  "cUsuarioCierre" text    ,
  "rImporteAbertura" real DEFAULT 0,
  "rImporteCierreAnterior" real DEFAULT 0,
  "lCajaCerrada" int DEFAULT 0,
  "rMoneda" real DEFAULT 0,
  "rCheque" real DEFAULT 0,
  "rTarjeta" real DEFAULT 0,
  "rCredito" real DEFAULT 0,
  "rEntradasExtras" real DEFAULT 0,
  "rSalidasExtras" real DEFAULT 0,
  "rImportePendiente" real DEFAULT 0,
  "rVales" real DEFAULT 0,
  "rImporteCierre" real DEFAULT 0,
  "rValorRealCaja" real DEFAULT 0,
  "rDescuadre" real DEFAULT 0,
  "rImporteACuenta" real DEFAULT 0,
  "rImporteCambios" real DEFAULT 0,
  "rValesEmitidos" real DEFAULT 0
   
)  ;






CREATE TABLE "cierrecaja" (
"id" integer primary key  autoincrement,
  "dFechaCierre" numeric  ,
  "rDeudas" real DEFAULT 0,
  "rEntregasACuenta" real DEFAULT 0,
  "rValesEmitidos" real DEFAULT 0,
  "lCajaCerrada" int DEFAULT 0
   
)  ;





CREATE TABLE "clientes" (
  "Id" INTEGER PRIMARY KEY AUTOINCREMENT  ,
  "cCodigoCliente" text    ,
  "cApellido1" text    ,
  "cApellido2" text    ,
  "cNombre" text    ,
  "cNombreFiscal" text    ,
  "cNombreComercial" text    ,
  "cPersonaContacto" text    ,
  "cCifNif" text    ,
  "cDireccion1" text    ,
  "cDireccion2" text    ,
  "cCP" text    ,
  "cPoblacion" text    ,
  "cProvincia" text    ,
  "cPais" text    ,
  "cTelefono1" text    ,
  "cTelefono2" text    ,
  "cFax" text    ,
  "cMovil" text    ,
  "cEmail" text    ,
  "cWeb" text    ,
  "cDireccionFactura1" text    ,
  "cDireccionFactura2" text    ,
  "cCPFactura" text    ,
  "cPoblacionFactura" text    ,
  "cProvinciaFactura" text    ,
  "cPaisFactura" text    ,
  "cDireccionAlmacen" text    ,
  "cDireccionAlmacen2" text    ,
  "cCPAlmacen" text    ,
  "cPoblacionAlmacen" text    ,
  "cProvinciaAlmacen" text    ,
  "cPaisAlmacen" text    ,
  "dFechaAlta" numeric  ,
  "dFechaUltimaCompra" numeric  ,
  "rAcumuladoVentas" real DEFAULT 0,
  "rVentasEjercicio" real DEFAULT 0,
  "rRiesgoMaximo" real DEFAULT 0,
  "rDeudaActual" real DEFAULT 0,
  "tComentarios" text  ,
  "lBloqueado" int DEFAULT 0,
  "tComentarioBloqueo" text  ,
  "nPorcDtoCliente" real DEFAULT 0,
  "lRecargoEquivalencia" int DEFAULT 0,
  "cCuentaContable" text    ,
  "cCuentaIvaRepercutido" text    ,
  "cCuentaDeudas" text    ,
  "cCuentaCobros" text    ,
  "cFormaPago" text    ,
  "nDiapago1" int DEFAULT 0,
  "nDiaPago2" int DEFAULT 0,
  "nTarifaCliente" int DEFAULT 0,
  "rImporteACuenta" real DEFAULT 0,
  "rVales" real DEFAULT 0,
  "cCentidadBancaria" text    ,
  "cOficinaBancaria" text    ,
  "cDC" text    ,
  "cCuentaCorriente" text    ,
  "dFechaNacimiento" numeric  ,
  "rImportePendiente" real DEFAULT 0,
  "cTipoCliente" text  ,
  "cAccesoWeb" text    ,
  "cPasswordWeb" text ,
  "nIRPF" int DEFAULT 0
  ) ;
  
  
  


CREATE TABLE "clientes_deuda" (
"id" integer primary key  autoincrement ,
  "Id_Cliente" int DEFAULT 0,
  "dFecha" numeric  ,
  "dVencimiento" numeric  ,
  "cDocumento" int DEFAULT 0,
  "Id_Ticket" int DEFAULT 0,
  "Id_Factura" int DEFAULT 0,
  "nTipo" int DEFAULT 0,
  "rImporte" real DEFAULT 0,
  "rPagado" real DEFAULT 0,
  "rPendienteCobro" real DEFAULT 0,
  "cEntidad" text    ,
  "cOficina" text    ,
  "cDC" text    ,
  "cCuenta" text    
   
)  ;
  
  
  CREATE TABLE "clientes_entregas" (
"id" integer primary key  autoincrement,
  "Id_Cliente" int DEFAULT 0,
  "dFecha" numeric  ,
  "rImporte" real DEFAULT 0,
  "nDocumento" int DEFAULT 0,
  "cTipo" text    
   
)  ;




CREATE TABLE "conceptostallas" (
"id" integer primary key  autoincrement,
  "cConcepto" text    
)  ;



  
  CREATE TABLE "dependient" (
"id" integer primary key  autoincrement ,
  "cCodigo" text    ,
  "cNombre" text    ,
  "cDireccion" text    ,
  "cCP" text    ,
  "cPoblacion" text    ,
  "cProvincia" text    ,
  "dFechaNacimiento" numeric  ,
  "cTelefono1" text    ,
  "cTelefono2" text    ,
  "cMovil" text    ,
  "cNombrePariente" text    ,
  "cPassword" text    ,
  "rCosteHora" real DEFAULT 0,
  "rCosteHoraExtra" real DEFAULT 0,
  "dFechaAlta" numeric  ,
  "dFinContrato" numeric  ,
  "cCategoria" text   
   
)  ;



CREATE TABLE "devoluciones" (
"id" integer primary key  autoincrement,
  "nTicket" int DEFAULT 0,
  "dFechaDevolucion" numeric  ,
  "cAtendidoPor" text    ,
  "id_Cliente" int DEFAULT 0,
  "cCliente" text    ,
  "tComentarios" text  ,
  "cCodArticulo" text    ,
  "cDescArticulo" text    
   
)  ;




CREATE TABLE "fac_pro" (
"id" integer primary key  autoincrement,
  "cFactura" text    ,
  "dFecha" numeric  ,
  "dRecepcion" numeric  ,
  "cPedido" text  ,
  "Id_Proveedor" int DEFAULT 0,
  "cProveedor" text    ,
  "cCifProveedor" text    ,
  "lRetencionIRPF" int DEFAULT 0,
  "lRecargoEquivalencia" int DEFAULT 0,
  "rBase1" real DEFAULT 0,
  "rBase2" real DEFAULT 0,
  "rBase3" real DEFAULT 0,
  "rBase4" real DEFAULT 0,
  "nPorcIva1" real DEFAULT 0,
  "nPorcIva2" real DEFAULT 0,
  "nPorcIva3" real DEFAULT 0,
  "nPorcIva4" real DEFAULT 0,
  "rIVA1" real DEFAULT 0,
  "rIVA2" real DEFAULT 0,
  "rIVA3" real DEFAULT 0,
  "rIVA4" real DEFAULT 0,
  "nREC1" real DEFAULT 0,
  "nREC2" real DEFAULT 0,
  "nREC3" real DEFAULT 0,
  "nREC4" real DEFAULT 0,
  "rREC1" real DEFAULT 0,
  "rREC2" real DEFAULT 0,
  "rREC3" real DEFAULT 0,
  "rREC4" real DEFAULT 0,
  "rTotal1" real DEFAULT 0,
  "rTotal2" real DEFAULT 0,
  "rTotal3" real DEFAULT 0,
  "rTotal4" real DEFAULT 0,
  "rTotalBase" real DEFAULT 0,
  "rTotalIVA" real DEFAULT 0,
  "rTotalRetencion" real DEFAULT 0,
  "rTotalRecargoEq" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "cFormaPago" text    ,
  "nDiaPago1" numeric  ,
  "nDiaPago2" numeric  ,
  "nDiaPago3" numeric  ,
  "nDiaPago4" numeric  ,
  "rImportePagadoTotal" real DEFAULT 0,
   "Id_TipoGasto" int DEFAULT 0,
  "cTipoGasto" text    ,
  "tComentario" text  ,
  "lPagado" int DEFAULT 0,
  "rImporteDeudaPendiente" real DEFAULT 0,
  "cCAlbaran" text    
   
);






CREATE TABLE "familias" (
"id" integer primary key  autoincrement,
  "cCodigo" text    ,
  "cFamilia" text    ,
  "Id_Seccion" int DEFAULT 0
   
)  ;





CREATE TABLE "FormPago" (
"id" integer primary key  autoincrement,
  "cCodigo" text    ,
  "cFormapago" text    ,
  "nDiaPago1" int DEFAULT 0,
  "nDiaPago2" int DEFAULT 0,
  "nDiaPago3" int DEFAULT 0,
  "nDiaPago4" int DEFAULT 0,
  "nDias1" int DEFAULT 0,
  "nDias2" int DEFAULT 0,
  "nDias3" int DEFAULT 0,
  "nDias4" int DEFAULT 0
   
)  ;




CREATE TABLE "histoart" (
"id" integer primary key  autoincrement,
  "Id_Articulo" int DEFAULT 0,
  "cCodigo" text    ,
  "dFecha" numeric  ,
  "nCantidad" real DEFAULT 0,
  "rImporte" real DEFAULT 0  ,
  "cNombreUsuario" text    
   
)  ;




CREATE TABLE "histovales" (
"id" integer primary key  autoincrement,
  "nVale" int DEFAULT 0,
  "dFecha" numeric  ,
  "dVencimientoVale" numeric  ,
  "cDependiente" text    ,
  "rImporte" real DEFAULT 0,
  "Id_Ticket" int DEFAULT 0,
  "Id_Cliente" int DEFAULT 0,
  "cCliente" text    
   
)  ;





CREATE TABLE "lin_alb" (
"id" integer primary key  autoincrement,
  "Id_Cab" int DEFAULT 0,
  "cCodigo" text    ,
  "nCantidad" real DEFAULT 0,
  "cDescripcion" text  ,
  "rPvp" real DEFAULT 0,
  "rSubTotal" real DEFAULT 0,
  "nDto" real DEFAULT 0,
  "rDto" real DEFAULT 0,
  "nPorcIva" real DEFAULT 0,
  "nTotal" real DEFAULT 0
   
)  ;




CREATE TABLE "lin_alb_pro" (
"id" integer primary key  autoincrement,
  "id_cab" int DEFAULT 0,
  "cCodigo" text,
  "cDescripcion" text,
  "rCoste" real DEFAULT 0,
  "nDto" real DEFAULT 0,
  "rPvp1" real DEFAULT 0,
  "rPvp2" real DEFAULT 0,
  "rPvp3" real DEFAULT 0,
  "nIva" real DEFAULT 0,
  "nCantidad" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "rDto" real DEFAULT 0,
  "rSUBTOTAL" real DEFAULT 0
)  ;

CREATE TABLE "lin_fac_pro" (
"id" integer primary key  autoincrement,
  "Id_Cab" int DEFAULT 0,
  "cCodigo" text    ,
  "cDescripcion" text    ,
  "nCantidad" real DEFAULT 0,
  "rCosteBruto" real DEFAULT 0,
  "nPorcDto" real DEFAULT 0,
  "rDto" real DEFAULT 0,
  "rCoste" real DEFAULT 0,
  "nPorcIva" real DEFAULT 0,
  "rIva" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "nPedido" int DEFAULT 0
   
)  ;



CREATE TABLE "lin_fac" (
"id" integer primary key  autoincrement,
  "id_Cab" int DEFAULT 0,
  "cCodigo" text    ,
  "nCantidad" real DEFAULT 0,
  "cDescripcion" text  ,
  "rPvp" real DEFAULT 0,
  "nDto" real DEFAULT 0,
  "rDto" real DEFAULT 0,
  "rSubTotal" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "nPorcIva" real DEFAULT 0
   
)  ;

CREATE TABLE "lin_ped_pro" (
"id" integer primary key  autoincrement,
  "Id_Cab" int DEFAULT 0,
  "Id_Articulo" int DEFAULT 0,
  "cCodigo" text    ,
  "cDescripcion" text  ,
  "nCantidad" real DEFAULT 0,
  "rCosteBruto" real DEFAULT 0,
  "rSubtotal" real DEFAULT 0,
  "nPorcDto" real DEFAULT 0,
  "nDto" real DEFAULT 0,
  "nPorcIva" real DEFAULT 0,
  "rIVA" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "rTarifa1" real DEFAULT 0,
  "nMargen1" real DEFAULT 0,
  "rTarifa2" real DEFAULT 0,
  "nMargen2" real DEFAULT 0,
  "rTarifa3" real DEFAULT 0,
  "nMargen3" real DEFAULT 0,
  "nCantidadRecibida" real DEFAULT 0,
  "lRecibido" int DEFAULT 0,
  "lTraspasado" int DEFAULT 0,
  "cTalla" text    ,
  "cColor" text    ,
  "cCodigoInterno" text    ,
  "nEtiquetas" int DEFAULT 0,
  "lOK" int DEFAULT 0
   
)  ;




CREATE TABLE "lin_pre" (
"id" integer primary key  autoincrement,
  "Id_Cab" int DEFAULT 0,
  "cCodigo" text    ,
  "cDescripcion" text  ,
  "nCantidad" real DEFAULT 0,
  "rPvp" real DEFAULT 0,
  "rSubotal" real DEFAULT 0,
  "nPorcDto" real DEFAULT 0,
  "rDto" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "nPorcIVA" real DEFAULT 0
   
)  ;






CREATE TABLE "lin_res" (
"id" integer primary key  autoincrement,
  "Id_Cab" int DEFAULT 0,
  "cCodigo" text    ,
  "cDescripcion" text    ,
  "nCantidad" int DEFAULT 0,
  "rImporte" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "Id_Cliente" int DEFAULT 0,
  "cCliente" text    ,
  "dFechaReserva" numeric  ,
  "dReservadoHasta" numeric  ,
  "nDto" real DEFAULT 0
   
)  ;



CREATE TABLE "lin_tpv" (
  "Id" INTEGER PRIMARY KEY AUTOINCREMENT  ,
  "id_Cab" int DEFAULT 0,
  "id_Articulo" int DEFAULT 0,
  "cCodigo" text    ,
  "cDescripcion" text    ,
  "rPvp" real DEFAULT 0,
  "nCantidad" real DEFAULT 0,
  "rImporte" real DEFAULT 0,
  "nPorcentajeIva" real DEFAULT 0,
  "nPorcDto" real DEFAULT 0,
  "nTotal" real DEFAULT 0,
  "cTipo" text    ,
  "rSubTotal" real DEFAULT 0,
  "dFechaLinea" numeric  
   
) ;





CREATE TABLE "ObligacionesTrib" (
"id" integer primary key  autoincrement,
  "cObligacion" text    ,
  "dFecha" numeric  ,
  "cModeloDocumento" text    ,
  "cPeriodicidad" text    
   
)  ;





CREATE TABLE "ped_cli" (
"id" integer primary key  autoincrement,
  "cPedido" text  ,
  "dFecha" numeric  ,
  "dFechaLimiteEntrega" numeric  ,
  "Id_Cliente" int DEFAULT 0,
  "cCodigoCliente" text    ,
  "cCliente" text    ,
  "cDireccion1" text    ,
  "cDireccion2" text    ,
  "cCP" text    ,
  "cPoblacion" text    ,
  "cProvincia" text    ,
  "cPais" text    ,
  "cCifNif" text    ,
  "id_FormaPago" int    ,
  "nPorcDto" real DEFAULT 0,
  "rBase" real DEFAULT 0,
  "rSubotal" real DEFAULT 0,
  "rDto" real DEFAULT 0,
  "rIVA" real DEFAULT 0,
  "rRecargoEqTotal" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "lEnviado" int DEFAULT 0,
  "lCompleto" int DEFAULT 0,
  "lPendiente" int DEFAULT 0,
  "tComentario" text  ,
  "rBase1" real DEFAULT 0,
  "rBase2" real DEFAULT 0,
  "rBase3" real DEFAULT 0,
  "rBase4" real DEFAULT 0,
  "nPorcIVA1" real DEFAULT 0,
  "nPorcIVA2" real DEFAULT 0,
  "nPorcIVA3" real DEFAULT 0,
  "nPorcIVA4" real DEFAULT 0,
  "rIVA1" real DEFAULT 0,
  "rIVA2" real DEFAULT 0,
  "rIVA3" real DEFAULT 0,
  "rIVA4" real DEFAULT 0,
  "rTotal1" real DEFAULT 0,
  "rTotal2" real DEFAULT 0,
  "rTotal3" real DEFAULT 0,
  "rTotal4" real DEFAULT 0,
  "nREC1" real DEFAULT 0,
  "nREC2" real DEFAULT 0,
  "nREC3" real DEFAULT 0,
  "nREC4" real DEFAULT 0,
  "rREC1" real DEFAULT 0,
  "rREC2" real DEFAULT 0,
  "rREC3" real DEFAULT 0,
  "rREC4" real DEFAULT 0,
  "lTraspasado" int DEFAULT 0,
  "lTraspasado2" int DEFAULT 0,
  "cDireccionEntrega1" text    ,
  "cDireccionEntrega2" text    ,
  "cCPEntrega" text    ,
  "cPoblacionEntrega" text    ,
  "cProvinciaEntrega" text    ,
  "cPaisEntrega" text    
   
)  ;






CREATE TABLE "ped_pro" (
"id" integer primary key  autoincrement,
  "nPedido" int DEFAULT 0,
  "cSerie" text    ,
  "dFecha" numeric  ,
  "dRecepcion" numeric  ,
  "Id_Proveedor" int DEFAULT 0,
  "cCodigoProveedor" text    ,
  "cProveedor" text    ,
  "cDireccion1" text    ,
  "cDireccion2" text    ,
  "cCP" text    ,
  "cPoblacion" text    ,
  "cProvincia" text    ,
  "cPais" text    ,
  "cCifNif" text    ,
  "rBase" real DEFAULT 0,
  "rSubotal" real DEFAULT 0,
  "rDto" real DEFAULT 0,
  "nIVA" real DEFAULT 0,
  "rRecTotal" real DEFAULT 0,
  "rTotal" real DEFAULT 0,
  "lEnviado" int DEFAULT 0,
  "lRecibido" int DEFAULT 0,
  "lRecibidoCompleto" int DEFAULT 0,
  "lGeneroPendiente" int DEFAULT 0,
  "rBase1" real DEFAULT 0,
  "rBase2" real DEFAULT 0,
  "rBase3" real DEFAULT 0,
  "rBase4" real DEFAULT 0,
  "nPorcIva1" real DEFAULT 0,
  "nPorcIva2" real DEFAULT 0,
  "nPorcIva3" real DEFAULT 0,
  "nPorcIva4" real DEFAULT 0,
  "rIVA1" real DEFAULT 0,
  "rIVA2" real DEFAULT 0,
  "rIVA3" real DEFAULT 0,
  "rIVA4" real DEFAULT 0,
  "rTotal1" real DEFAULT 0,
  "rTotal2" real DEFAULT 0,
  "rTotal3" real DEFAULT 0,
  "rTotal4" real DEFAULT 0,
  "nMargenREC1" real DEFAULT 0,
  "nMargenREC2" real DEFAULT 0,
  "nMargenREC3" real DEFAULT 0,
  "nMargenREC4" real DEFAULT 0,
  "rREC1" real DEFAULT 0,
  "rREC2" real DEFAULT 0,
  "rREC3" real DEFAULT 0,
  "rREC4" real DEFAULT 0,
  "lTraspasado" int DEFAULT 0,
  "nPedidoCliente" int DEFAULT 0,
  "Id_FormaPago" int DEFAULT 0,
  "cCodigoFormaPago" text    ,
  "cFormaPago" text    ,
  "dVencimiento1" numeric  ,
  "dVencimiento2" numeric  ,
  "dVencimiento3" numeric  ,
  "dVencimiento4" numeric  ,
  "lPagado1" int DEFAULT 0,
  "lPagado2" int DEFAULT 0,
  "lPagado3" int DEFAULT 0,
  "lPagado4" int DEFAULT 0,
  "tComentario" text  ,
  "dFechaEntrega" numeric  ,
  "cDireccion1Entrega" text    ,
  "cDireccion2Entrega" text    ,
  "cCPEntrega" text    ,
  "cPoblacionEntrega" text    ,
  "cProvinciaEntrega" text    ,
  "cPaisEntrega" text    ,
  "cNombreCliente" text    ,
  "cHorarioActivo" text  
   
)  ;




CREATE TABLE "proveedores" (
  "Id" INTEGER PRIMARY KEY AUTOINCREMENT,
  "cCodigo" int DEFAULT 0,
  "cProveedor" text    ,
  "cCif" text    ,
  "cDireccion1" text    ,
  "cDireccion2" text    ,
  "cCP" text    ,
  "cPoblacion" text    ,
  "cProvincia" text    ,
  "cPais" text    ,
  "cTelefono1" text    ,
  "cTelefono2" text    ,
  "cTelefono3" text    ,
  "cFax" text    ,
  "cMovil" text    ,
  "cEMail" text    ,
  "cWeb" text    ,
  "cPersonaContacto" text    ,
  "nDiaCobro" int DEFAULT 0,
  "cDireccionAlmacen" text    ,
  "cCPAlmacen" text    ,
  "cPoblacionAlmacen" text    ,
  "cProvinciaAlmacen" text    ,
  "cPaisAlmacen" text    ,
  "cTelefonoAlmacen" text    ,
  "cFaxAlmacen" text    ,
  "cCodigoFormaPago" text    ,
  "dFechaUltimaCompra" numeric  ,
  "rAcumuladoCompras" real DEFAULT 0,
  "cEntidadBancariaProveedor" text    ,
  "cOficinaBancariaProveedor" text    ,
  "cDCProveedor" text    ,
  "cCCProveedor" text    ,
  "cEntidadPagoProveedor" text    ,
  "cOficinaPagoProveedor" text    ,
  "cDCPagoProveedor" text    ,
  "cCuentaPagoProveedor" text    ,
  "cCuentaIvaSoportado" text    ,
  "rRetencionIRPF" real DEFAULT 0,
  "nTipoRetencion" int DEFAULT 0,
  "cCuentaAplicacion" text    ,
  "cCuentaPagos" text    ,
  "tComentarios" text  ,
  "nDto" real DEFAULT 0,
  "dFechaAlta" numeric  ,
  "rDeudaMaxima" real DEFAULT 0,
  "rDeudaActual" real DEFAULT 0,
  "lRecargoEquivalencia" int DEFAULT 0,
  "tTextoparaPedidos" text,  
  "rEntregadoaCuenta" real DEFAULT 0
   
) ;





   
   
   
   CREATE TABLE "recibos" (
  "id" integer primary key  autoincrement,
  "dFecha" numeric  ,
  "dFechaCobro" numeric  ,
  "cDescripcionFecha1" text    ,
  "cDescripcionFecha2" text    ,
  "cLocalidad" text    ,
  "nRecibo" int DEFAULT 0,
  "Id_Cliente" int DEFAULT 0,
  "cCliente" text    ,
  "cDireccion1" text    ,
  "cDireccion2" text    ,
  "cCp" text    ,
  "cPoblacion" text    ,
  "cProvincia" text    ,
  "cPais" text    ,
  "cConcepto" text    ,
  "rImporte" real DEFAULT 0,
  "cImporte" text   ,
  "cDescripcionEntidad" text    ,
  "cDescripcionOficina" text    ,
  "cEntidad" text    ,
  "cOficina" text    ,
  "cDC" text    ,
  "cCuenta" text    ,
  "lCobrado" int DEFAULT 0
   
)  ;


CREATE TABLE "reservas" (
"id" integer primary key  autoincrement,
  "nReserva" int DEFAULT 0,
  "dFecha" numeric  ,
  "id_cliente" int DEFAULT 0,
  "cCliente" text    ,
  "cDireccion" text    ,
  "cDireccion2" text    ,
  "cCP" text    ,
  "cPoblacion" text    ,
  "cProvincia" text    ,
  "cPais" text    ,
  "cTelefono1" text    ,
  "cTelefono2" text    ,
  "dFechaLimite" numeric  ,
  "rImporte" real DEFAULT 0,
  "rPendiente" real DEFAULT 0,
  "rEntregado" real DEFAULT 0
   
)  ;




CREATE TABLE "Secciones" (
"id" integer primary key  autoincrement,
  "cSeccion" text    
   
)  ;




CREATE TABLE "Sectores" (
"id" integer primary key  autoincrement,
  "cSector" text    
   
)  ;


CREATE TABLE "subfamilias" (
"id" integer primary key  autoincrement,
  "cSubfamilia" text    ,
  "Id_Familia" int DEFAULT 0
   
)  ;




CREATE TABLE "tallas" (
"id" integer primary key  autoincrement,
  "Talla" text    ,
  "Id_ConceptoTalla" int DEFAULT 0
   
)  ;





CREATE TABLE "tareas" (
"id" integer primary key  autoincrement,
  "cTarea" text    ,
  "cAvisarA" text    ,
  "nDiasFecha" int DEFAULT 0,
  "lCobros" int DEFAULT 0,
  "lPagos" int DEFAULT 0,
  "lObligacionesTributarias" int DEFAULT 0
   
)  ;





CREATE TABLE "tiposgasto" (
"id" integer primary key  autoincrement,
  "cDescripcionTipoGasto" text    ,
  "cComentario" text  
   
)  ;


CREATE TABLE "tiposiva" (
"id" integer primary key  autoincrement,
  "cTipo" text    ,
  "cDescripcionTipoIVA" text    ,
  "nIVA" real DEFAULT 0,
  "nRegargoEquivalencia" real DEFAULT 0
   
)  ;


CREATE TABLE "vales" (
"id" integer primary key  autoincrement,
  "nNumero" int DEFAULT 0,
  "cCodigo" text    ,
  "dFecha" numeric  ,
  "dVto" numeric  ,
  "cDependient" text    ,
  "rImporte" real DEFAULT 0,
  "IdTicket" int DEFAULT 0,
  "Id_Cliente" int DEFAULT 0,
  "cCliente" text    ,
  "lPagado" int DEFAULT 0
   
)  ;









































