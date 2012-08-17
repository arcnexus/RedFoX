#include "articulo.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include "frmdecision.h"

Articulo::Articulo()
{

}

void Articulo::Anadir()
{

}

void Articulo::Recuperar(QString cSQL)
{
    QSqlQuery qryArticulo(QSqlDatabase::database("empresa"));
    qryArticulo.prepare(cSQL);
    if (qryArticulo.exec()) {
           if (qryArticulo.next()){
            this->id = qryArticulo.value(0).toInt();
            this->cCodigo = qryArticulo.value(1).toString();
            this->cCodigoBarras = qryArticulo.value(2).toString();
            this->cCodigoFabricante = qryArticulo.value(3).toString();
            this->cDescripcion = qryArticulo.value(4).toString();

            this->rTarifa1 = qryArticulo.value(17).toDouble();
            } else {
               QMessageBox::critical(NULL,"Búsqueda de artículos", "No se encuentra ningún artículo con este código");
           }
    } else {
        QMessageBox::critical(NULL,"error al leer datos artículo:", qryArticulo.lastError().text());
    }
}

void Articulo::Borrar(int nId)
{
    frmDecision *Decision = new frmDecision();
    QSqlQuery *qryArticulo = new QSqlQuery(QSqlDatabase::database("emp"));
    Decision->Inicializar(QObject::tr("Borrar Artículo"),QObject::tr("¿Desea realmente Borrar este artículo"),QObject::tr("Esta opción no se puede deshacer"),
                          "",QObject::tr("Borrar"),QObject::tr("Cancelar"));
    int elegido = Decision->exec();
   if(elegido == QMessageBox::Yes) {
        qryArticulo->prepare("Delete from articulos where id = :nId");
        qryArticulo->bindValue(":id",nId);
        if(!qryArticulo->exec()){
           QMessageBox::critical(NULL,QObject::tr("Borrar Artíclo"),QObject::tr("Falló el borrado del Artículo"),QObject::tr("&Aceptar"));
        }


     }
   delete Decision;
   delete qryArticulo;

}

void Articulo::Vender(int id, int cantidad, double rPVP)
{
}

void Articulo::Devolucion(int id, int cantidad, double rImporte, QString cMotivo, QString dFechaDevolucion)
{
}



// geters
QString Articulo::getId()
{
}

QString Articulo::getcCodigo()
{
    return this->cCodigo;
}

QString Articulo::getcCodigoBarras()
{
    return this->cCodigoBarras;
}

QString Articulo::getcCodigoFabricante()
{
    return this->cCodigoFabricante;
}

QString Articulo::getcDescripcion()
{
    return this->cDescripcion;
}

QString Articulo::getcDescripcionReducida()
{
    return this->cDescripcionReducida;
}

int Articulo::getid_Proveedor()
{
    return this->id_Proveedor;
}

QString Articulo::getcCodigoFamilia()
{
    return this->cCodigoFamilia;
}

int Articulo::getid_Familia()
{
    return this->id_Familia;
}

QString Articulo::getcSubfamilia()
{
    return this->cSubfamilia;
}

QString Articulo::getcCodigoIva()
{
    return this->cCodigoIva;
}

int Articulo::getid_TipoIva()
{
    return this->id_TipoIva;
}

double Articulo::getrCoste()
{
    return this->rCoste;
}

double Articulo::getrTarifa1()
{
    return this->rTarifa1;
}

double Articulo::getrTarifa2()
{
    return this->rTarifa2;
}

double Articulo::getrTarifa3()
{
    return this->rTarifa3;
}

double Articulo::getrDto()
{
    return this->rDto;
}

double Articulo::getnDtoProveedor()
{
    return this->nDtoProveedor;
}

double Articulo::getnDtoProveedor2()
{
    return this->nDtoProveedor2;
}

double Articulo::getnDtoProveedor3()
{
    return this->nDtoProveedor3;
}

QDate Articulo::getdUltimaCompra()
{
    return this->dUltimaCompra;
}

QDate Articulo::getdUltimaVenta()
{
    return this->dUltimaVenta;
}

double Articulo::getnMargen1()
{
    return this->nMargen1;
}

double Articulo::getnMargen2()
{
    return this->nMargen2;
}

double Articulo::getnMargen3()
{
    return this->nMargen3;
}

double Articulo::getrPrecioMedio()
{
    return this->rPrecioMedio;
}

int Articulo::getnUnidadesCompradas()
{
    return this->nUnidadesCompradas;
}

double Articulo::getrPrecioMedio2()
{
    return this->rPrecioMedio2;
}

int Articulo::getnUnidadesVendidas()
{
    return this->nUnidadesVendidas;
}

double Articulo::getrPrecioMedio3()
{
    return this->rPrecioMedio3;
}

double Articulo::getrAcumuladoCompras()
{
    return this->rAcumuladoCompras;
}

double Articulo::getrAcumuladoVentas()
{
    return this->rAcumuladoVentas;
}

QString Articulo::gettComentario()
{
    return this->tComentario;
}

int Articulo::getnStockMaximo()
{
    return this->nStockMaximo;
}

int Articulo::getnStockMinimo()
{
    return this->nStockMinimo;
}

int Articulo::getnStockReal()
{
    return this->nStockReal;
}

QString Articulo::getcTipoUnidad()
{
    return this->cTipoUnidad;
}

int Articulo::getlControlarStock()
{
    return this->lControlarStock;
}

QString Articulo::getcModelo()
{
    return this->cModelo;
}

QString Articulo::getcTalla()
{
    return this->cTalla;
}

QString Articulo::getcColor()
{
    return this->cColor;
}

QString Articulo::getcComposicion()
{
    return this->cComposicion;
}

int Articulo::getlPvpIncluyeIva()
{
    return this->lPvpIncluyeIva;
}

int Articulo::getlPendienteRecibir()
{
    return this->lPendienteRecibir;
}

int Articulo::getnCantidadPendienteRecibir()
{
    return this->nCantidadPendienteRecibir;
}

int Articulo::getnReservados()
{
    return this->nReservados;
}

int Articulo::getlMostrarWeb()
{
    return this->lMostrarWeb;
}

int Articulo::getnEtiquetas()
{
    return this->nEtiquetas;
}

int Articulo::getnPaquetes()
{
    return this->nPaquetes;
}

QString Articulo::getcLocalizacion()
{
    return this->cLocalizacion;
}

// setters
void Articulo::setId(int nId)
{
    this->id = nId;
}

void Articulo::setcCodigo(QString cCodigo)
{
    this->cCodigo = cCodigo;
}

void Articulo::setcCodigoBarras(QString cCodigoBarras)
{
    this->cCodigoBarras = cCodigoBarras;
}

void Articulo::setcCodigoFabricante(QString cCodigoFabricante)
{
    this->cCodigoFabricante = cCodigoFabricante;
}

void Articulo::setcDescripcion(QString cDescripcion)
{
    this->cDescripcion = cDescripcion;
}

void Articulo::setcDescripcionReducida(QString cDescripcionReducida)
{
    this->cDescripcionReducida = cDescripcionReducida;
}

void Articulo::setid_Proveedor(int id_Proveedor)
{
    this->id_Proveedor = id_Proveedor;
}

void Articulo::setcCodigoFamilia(QString cCodigoFamilia)
{
    this->cCodigoFamilia = cCodigoFamilia;
}

void Articulo::setid_Familia(int id_Familia)
{
    this->id_Familia = id_Familia;
}

void Articulo::setcSubfamilia(QString cSubfamilia)
{
    this->cSubfamilia = cSubfamilia;
}

void Articulo::setcCodigoIva(QString cCodigoIva)
{
    this->cCodigoIva = this->cCodigoIva;
}

void Articulo::setid_TipoIva(int id_TipoIva)
{
    this->id_TipoIva = id_TipoIva;
}

void Articulo::setrCoste(double rCoste)
{
    this->rCoste = rCoste;
}

void Articulo::setrTarifa1(double rTarifa1)
{
    this->rTarifa1 = rTarifa1;
}

void Articulo::setrTarifa2(double rTarifa2)
{
    this->rTarifa2 = rTarifa2;
}

void Articulo::setrTarifa3(double rTarifa3)
{
    this->rTarifa3 = rTarifa3;
}

void Articulo::setrDto(double rDto)
{
    this->rDto = rDto;
}

void Articulo::setnDtoProveedor(double nDtoProveedor)
{
    this->nDtoProveedor = nDtoProveedor;
}

void Articulo::setnDtoProveedor2(double nDtoProveedor2)
{
    this->nDtoProveedor2 = nDtoProveedor2;
}

void Articulo::setnDtoProveedor3(double nDtoProveedor3)
{
    this->nDtoProveedor3 = nDtoProveedor3;
}

void Articulo::setdUltimaCompra(QDate dUltimaCompra)
{
    this->dUltimaCompra = dUltimaCompra;
}

void Articulo::setdUltimaVenta(QDate dUltimaVenta)
{
    this->dUltimaVenta = dUltimaVenta;
}

void Articulo::setnMargen1(double nMargen1)
{
    this->nMargen1 = nMargen1;
}

void Articulo::setnMargen2(double nMargen2)
{
    this->nMargen2 = nMargen2;
}

void Articulo::setnMargen3(double nMargen3)
{
    this->nMargen3 = nMargen3;
}

void Articulo::setrPrecioMedio(double rPrecioMedio)
{
    this->rPrecioMedio = rPrecioMedio;
}

void Articulo::setnUnidadesCompradas(int nUnidadesCompradas)
{
    this->nUnidadesCompradas = nUnidadesCompradas;
}

void Articulo::setrPrecioMedio2(double rPrecioMedio2)
{
    this->rPrecioMedio = rPrecioMedio2;
}

void Articulo::setnUnidadesVendidas(int nUnidadesVendidas)
{
    this->nUnidadesVendidas = nUnidadesVendidas;
}

void Articulo::setrPrecioMedio3(double rPrecioMedio3)
{
    this->rPrecioMedio3 = rPrecioMedio3;
}

void Articulo::setrAcumuladoCompras(double rAcumuladoCompras)
{
    this->rAcumuladoCompras = rAcumuladoCompras;
}

void Articulo::setrAcumuladoVentas(double rAcumuladoVentas)
{
    this->rAcumuladoVentas = rAcumuladoVentas;
}

void Articulo::settComentario(QString tComentario)
{
    this->tComentario = tComentario;
}

void Articulo::setnStockMaximo(int nStockMaximo)
{
    this->nStockMaximo = nStockMaximo;
}

void Articulo::setnStockMinimo(int nStockMinimo)
{
    this->nStockMinimo = nStockMinimo;
}

void Articulo::setnStockReal(int nStockReal)
{
    this->nStockReal = nStockReal;
}

void Articulo::setcTipoUnidad(QString cTipoUnidad)
{
    this->cTipoUnidad = cTipoUnidad;
}

void Articulo::setlControlarStock(int lControlarStock)
{
    this->lControlarStock = lControlarStock;
}

void Articulo::setcModelo(QString cModelo)
{
    this->cModelo = cModelo;
}

void Articulo::setcTalla(QString cTalla)
{
    this->cTalla = cTalla;
}

void Articulo::setcColor(QString cColor)
{
    this->cColor = cColor;
}

void Articulo::setcComposicion(QString cComposicion)
{
    this->cComposicion = cComposicion;
}

void Articulo::setlPvpIncluyeIva(int lPvpIncluyeIva)
{
    this->lPvpIncluyeIva = lPvpIncluyeIva;
}

void Articulo::setlPendienteRecibir(int lPendienteRecibir)
{
    this->lPendienteRecibir = lPendienteRecibir;
}

void Articulo::setnCantidadPendienteRecibir(int nCantidadPendienteRecibir)
{
    this->nCantidadPendienteRecibir = nCantidadPendienteRecibir;
}

void Articulo::setnReservados(int nReservados)
{
    this->nReservados = nReservados;
}

void Articulo::setlMostrarWeb(int lMostrarWeb)
{
    this->lMostrarWeb = this->lMostrarWeb;
}

void Articulo::setnEtiquetas(int nEtiquetas)
{
    this->nEtiquetas = nEtiquetas;
}

void Articulo::setnPaquetes(int nPaquetes)
{
    this->nPaquetes = nPaquetes;
}

void Articulo::setcLocalizacion(QString cLocalizacion)
{
    this->cLocalizacion = cLocalizacion;
}












