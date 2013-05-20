
#include "pedidoproveedor.h"

PedidoProveedor::PedidoProveedor(QObject *parent) :
    QObject(parent)
{

}

int PedidoProveedor::nuevo_pedido_proveedor()
{

    QSqlQuery queryPedido(QSqlDatabase::database("empresa"));
    queryPedido.prepare("INSERT INTO ped_pro (`dFecha`,`nEjercicio`) VALUES(:dfecha,:ejercicio);");
    queryPedido.bindValue(":dfecha",QDate::currentDate());
    queryPedido.bindValue(":ejercicio",Configuracion_global->cEjercicio.toInt());
    if(!queryPedido.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("ATENCIÓN:"),
                             tr("No se puede insertar un nuevo pedido: %1").arg(queryPedido.lastError().text()),
                             tr("aceptar"));

    } else
        return queryPedido.lastInsertId().toInt();
}

void PedidoProveedor::guardar()
{
    //-------------------------------------------------------------
    // Si se trata de un nuevo pedido se le asigna numero de pedido
    //-------------------------------------------------------------
    QString cSql;
    int nPedido =0;
    QSqlQuery queryPedido(QSqlDatabase::database("empresa"));

    if(this->nPedido == 0)
    {

        if(queryPedido.exec("select nPedido from ped_pro order by nPedido desc limit 0,1"))
        {
            queryPedido.next();
            nPedido = queryPedido.record().value("nPedido").toInt();
            nPedido++;
            this->nPedido = nPedido;
        }

    }

    queryPedido.prepare("UPDATE ped_pro SET nPedido =  :nPedido,"
    "nEjercicio =:cSerie,"
    "dFecha =:dFecha,"
    "dRecepcion =:dRecepcion,"
    "Id_Proveedor =:Id_Proveedor,"
    "cCodigoProveedor =:cCodigoProveedor,"
    "cProveedor =:cProveedor"
    "cDireccion1 =:cDireccion1,"
    "cDireccion2 =:cDireccion2,"
    "cCP =:cCP,"
    "cPoblacion =:cPoblacion,"
    "cProvincia =:cProvincia,"
    "idpais =:idpais,"
    "cCifNif =:cCifNif,"
    "rBaseTotal =:rBaseTotal,"
    "rSubotal =:rSubotal,"
    "rDto =:rDto,"
     "rRecTotal =:rRecTotal,"
     "rTotal =:rTotal,"
     "lEnviado =:lEnviado,"
     "lRecibido =:lRecibido,"
     "lRecibidoCompleto =:lRecibidoCompleto,"
     "lGeneroPendiente =:lGeneroPendiente,"
     "lTraspasado =:lTraspasado,"
     "nPedidoCliente =:nPedidoCliente,"
     "Id_FormaPago =:Id_FormaPago,"
     "tComentario =:tComentario,"
     "dFechaEntrega =:dFechaEntrega,"
     "cDireccion1Entrega =:cDireccion1Entrega,"
     "cDireccion2Entrega =:cDireccion2Entrega,"
     "cCPEntrega =:cCPEntrega,"
     "cPoblacionEntrega =:cPoblacionEntrega,"
     "cProvinciaEntrega =:cProvinciaEntrega,"
     "id_paisEntrega =:id_paisEntrega,"
     "cNombreCliente =:cNombreCliente,"
     "cHorarioActivo =:cHorarioActivo,"
     "rBase1 =:rBase1,"
     "rBase2 =:rBase2,"
     "rBase3 =:rBase3,"
     "rBase4 =:rBase4,"
     "niva1 =:niva1,"
     "niva2 =:niva2,"
     "niva3 =:niva3,"
     "niva4 =:niva4,"
     "riva1 =:riva1,"
     "riva2 =:riva2,"
     "riva3 =:riva3,"
     "riva4 =:riva4,"
     "nrec1 =:nrec1,"
     "nrec2 =:nrec2,"
     "nrec3 =:nrec3,"
     "nrec4 =:nrec4,"
     "rrec1 =:rrec1,"
     "rrec2 =:rrec2,"
     "rrec3 =:rrec3,"
     "rrec4 =:rrec4,"
     "rtotal1 =:rtotal1,"
     "rtotal2 =:rtotal2,"
     "rtotal3 =:rtotal3,"
     "rtotal4 =:rtotal4 "
    " WHERE id = :id;");

    queryPedido.bindValue(":cSerie",this->nEjercicio);
    queryPedido.bindValue(":dFecha",this->dFecha);
    queryPedido.bindValue(":dRecepcion",this->dRecepcion);
    queryPedido.bindValue(":Id_Proveedor",this->Id_Proveedor);
    queryPedido.bindValue(":cCodigoProveedor",this->cCodigoProveedor);
    queryPedido.bindValue(":cProveedor",this->cProveedor);
    queryPedido.bindValue(":cDireccion1",this->cDireccion1);
    queryPedido.bindValue(":cDireccion2",this->cDireccion2);
    queryPedido.bindValue(":cCP",this->cCP);
    queryPedido.bindValue(":cPoblacion",this->cPoblacion);
    queryPedido.bindValue(":cProvincia",this->cProvincia);
    queryPedido.bindValue(":idpais",this->idpais);
    queryPedido.bindValue(":cCifNif",this->cCifNif);
    queryPedido.bindValue(":rBaseTotal",this->rBaseTotal);
    queryPedido.bindValue(":rSubotal",this->rSubotal);
    queryPedido.bindValue(":rDto",this->rDto);
    queryPedido.bindValue(":rRecTotal",this->rRecTotal);
    queryPedido.bindValue(":rTotal",this->rTotal);
    queryPedido.bindValue(":lEnviado",this->lEnviado);
    queryPedido.bindValue(":lRecibido",this->lRecibido);
    queryPedido.bindValue(":lRecibidoCompleto",this->lRecibidoCompleto);
    queryPedido.bindValue(":lGeneroPendiente",this->lGeneroPendiente);
    queryPedido.bindValue(":lTraspasado",this->lTraspasado);
    queryPedido.bindValue(":nPedidoCliente",this->nPedidoCliente);
    queryPedido.bindValue(":Id_FormaPago",this->Id_FormaPago);
    queryPedido.bindValue(":tComentario",this->tComentario);
    queryPedido.bindValue(":dFechaEntrega",this->dFechaEntrega);
    queryPedido.bindValue(":cDireccion1Entrega",this->cDireccion1Entrega);
    queryPedido.bindValue(":cDireccion2Entrega",this->cDireccion2Entrega);
    queryPedido.bindValue(":cCPEntrega",this->cCPEntrega);
    queryPedido.bindValue(":cPoblacionEntrega",this->cPoblacionEntrega);
    queryPedido.bindValue(":cProvinciaEntrega",this->cProvinciaEntrega);
    queryPedido.bindValue(":id_paisEntrega",this->id_paisEntrega);
    queryPedido.bindValue(":cNombreCliente",this->cNombreCliente);
    queryPedido.bindValue(":cHorarioActivo",this->cHorarioActivo);
    queryPedido.bindValue(":rBase1",this->rBase1);
    queryPedido.bindValue(":rBase2",this->rBase2);
    queryPedido.bindValue(":rBase3",this->rBase3);
    queryPedido.bindValue(":rBase4",this->rBase4);
    queryPedido.bindValue(":niva1",this->niva1);
    queryPedido.bindValue(":niva2",this->niva2);
    queryPedido.bindValue(":niva3",this->niva3);
    queryPedido.bindValue(":niva4",this->niva4);
    queryPedido.bindValue(":riva1",this->riva1);
    queryPedido.bindValue(":riva2",this->riva2);
    queryPedido.bindValue(":riva3",this->riva3);
    queryPedido.bindValue(":riva4",this->riva4);
    queryPedido.bindValue(":nrec1",this->nrec1);
    queryPedido.bindValue(":nrec2",this->nrec2);
    queryPedido.bindValue(":nrec3",this->nrec3);
    queryPedido.bindValue(":nrec4",this->nrec4);
    queryPedido.bindValue(":rrec1",this->rrec1);
    queryPedido.bindValue(":rrec2",this->rrec2);
    queryPedido.bindValue(":rrec3",this->rrec3);
    queryPedido.bindValue(":rrec4",this->rrec4);
    queryPedido.bindValue(":rtotal1",this->rtotal1);
    queryPedido.bindValue(":rtotal2",this->rtotal2);
    queryPedido.bindValue(":rtotal3",this->rtotal3);
    queryPedido.bindValue(":rtotal4",this->rtotal4);
    queryPedido.bindValue(":id",this->id);

    if(!queryPedido.exec())
        QMessageBox::warning(qApp->activeWindow(),tr("ATENCIÓN:"),
                             tr("Ha ocurrido un error al guardar el pedido: %1").arg(queryPedido.lastError().text()),
                             tr("Aceptar"));

    qDebug() << queryPedido.lastQuery();

}

void PedidoProveedor::recuperar(int id)
{
    QSqlQuery *queryPedido = new QSqlQuery(QSqlDatabase::database("empresa"));
    queryPedido->prepare("select * from ped_pro where id = :id");
    queryPedido->bindValue(":id",id);
    if(queryPedido->exec())
    {
        cargar(queryPedido,0);
  }else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Atención"),
                             tr("Se produjo un error al recuperar los datos: %1").arg(queryPedido->lastError().text()),
                             tr("Aceptar"));
    }
    delete queryPedido;

}

void PedidoProveedor::recuperar(QString cadenaSQL)
{
    QSqlQuery *queryPedido = new QSqlQuery(QSqlDatabase::database("empresa"));

    if(queryPedido->exec(cadenaSQL))
    {
        cargar(queryPedido,0);
    }else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Atención"),
                             tr("Se produjo un error al recuperar los datos: %1").arg(queryPedido->lastError().text()),
                             tr("Aceptar"));
    }
    delete queryPedido;
}

void PedidoProveedor::recuperar(QString cadenaSQL, int accion)
{
    QSqlQuery *queryPedido = new QSqlQuery(QSqlDatabase::database("empresa"));

    if(queryPedido->exec(cadenaSQL))
    {
        cargar(queryPedido,accion);
    } else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Atención"),
                             tr("Se produjo un error al recuperar los datos: %1").arg(queryPedido->lastError().text()),
                             tr("Aceptar"));
    }
    delete queryPedido;
}

void PedidoProveedor::clear()
{
    this->id = 0;
    this->nPedido = 0;
    this->nEjercicio = 0;
    this->dFecha = QDate::currentDate();
    this->dRecepcion = QDate::currentDate();
    this->Id_Proveedor = 0;
    this->cCodigoProveedor = "";
    this->cProveedor = "";
    this->cDireccion1 = "";
    this->cDireccion2 = "";
    this->cCP = "";
    this->cPoblacion = "";
    this->cProvincia = "";
    this->idpais = 0;
    this->cCifNif = "";
    this->rBaseTotal = 0;
    this->rSubotal = 0;
    this->rDto = 0;
    this->nIVA = 0;
    this->rRecTotal = 0;
    this->rTotal = 0;
    this->lEnviado = false;
    this->lRecibido = false;
    this->lRecibidoCompleto = false;
    this->lGeneroPendiente = false;
    this->lTraspasado = false;
    this->nPedidoCliente = 0;
    this->Id_FormaPago = 0;
    this->tComentario = "";
    this->dFechaEntrega = QDate::currentDate();
    this->cDireccion1Entrega = "";
    this->cDireccion2Entrega = "";
    this->cCPEntrega = "";
    this->cPoblacionEntrega = "";
    this->cProvinciaEntrega = "";
    this->id_paisEntrega = 0;
    this->cNombreCliente = "";
    this->cHorarioActivo = "";

    this->rBase1 = 0;
    this->rBase2 = 0;
    this->rBase3 = 0;
    this->rBase4 = 0;
    this->niva1 = 0;
    this->niva2 = 0;
    this->niva3 = 0;
    this->niva4 = 0;
    this->riva1 = 0;
    this->riva2 = 0;
    this->riva3 = 0;
    this->riva4 = 0;
    this->rdto1 = 0;
    this->rdto2 = 0;
    this->rdto3 = 0;
    this->rdto4 = 0;
    this->nrec1 = 0;
    this->nrec2 = 0;
    this->nrec3 = 0;
    this->nrec4 = 0;
    this->rrec1 = 0;
    this->rrec2 = 0;
    this->rrec3 = 0;
    this->rrec4 = 0;
    this->rtotal1 = 0;
    this->rtotal2 = 0;
    this->rtotal3 = 0;
    this->rtotal4 = 0;
}

void PedidoProveedor::cargar(QSqlQuery *queryPedido, int accion)
{
    if(queryPedido->next()){
        this->id = queryPedido->record().value("id").toInt();
        this->nPedido = queryPedido->record().value("nPedido").toInt();
        this->nEjercicio = queryPedido->record().value("nEjercicio").toInt();
        this->dFecha = queryPedido->record().value("dFecha").toDate();
        this->dRecepcion = queryPedido->record().value("dRecepcion").toDate();
        this->Id_Proveedor = queryPedido->record().value("Id_Proveedor").toInt();
        this->cCodigoProveedor = queryPedido->record().value("cCodigoProveedor").toString();
        this->cProveedor = queryPedido->record().value("cProveedor").toString();
        this->cDireccion1 = queryPedido->record().value("cDireccion1").toString();
        this->cDireccion2 = queryPedido->record().value("cDireccion2").toString();
        this->cCP = queryPedido->record().value("cCP").toString();
        this->cPoblacion = queryPedido->record().value("cPoblacion").toString();
        this->cProvincia = queryPedido->record().value("cProvincia").toString();
        this->idpais = queryPedido->record().value("idpais").toInt();
        this->cCifNif = queryPedido->record().value("cCifNif").toString();
        this->rBaseTotal = queryPedido->record().value("rBase").toDouble();
        this->rSubotal = queryPedido->record().value("rSubtotal").toDouble();
        this->rDto = queryPedido->record().value("rDto").toDouble();
        this->nIVA = queryPedido->record().value("nIVA").toInt();
        this->rRecTotal = queryPedido->record().value("rRecTotal").toDouble();
        this->rTotal = queryPedido->record().value("rTotal").toDouble();
        this->lEnviado = queryPedido->record().value("lEnviado").toBool();
        this->lRecibido = queryPedido->record().value("lRecibido").toBool();
        this->lRecibidoCompleto = queryPedido->record().value("lRecibidoCompleto").toBool();
        this->lGeneroPendiente = queryPedido->record().value("lGeneroPendiente").toBool();
        this->lTraspasado = queryPedido->record().value("lTraspasado").toBool();
        this->nPedidoCliente = queryPedido->record().value("nPedidoCliente").toInt();
        this->Id_FormaPago = queryPedido->record().value("Id_FormaPago").toInt();
        this->tComentario = queryPedido->record().value("tComentario").toString();
        this->dFechaEntrega = queryPedido->record().value("dFechaEntrega").toDate();
        this->cDireccion1Entrega = queryPedido->record().value("cDireccion1Entrega").toString();
        this->cDireccion2Entrega = queryPedido->record().value("cDireccion2Entrega").toString();
        this->cCPEntrega = queryPedido->record().value("cCPEntrega").toString();
        this->cPoblacionEntrega = queryPedido->record().value("cPoblacionEntrega").toString();
        this->cProvinciaEntrega = queryPedido->record().value("cProvinciaEntrega").toString();
        this->id_paisEntrega = queryPedido->record().value("id_paisEntrega").toInt();
        this->cNombreCliente = queryPedido->record().value("cNombreCliente").toString();
        this->cHorarioActivo = queryPedido->record().value("cHorarioActivo").toString();
        this->rBase1 = queryPedido->record().value("rBase1").toDouble();
        this->rBase2 = queryPedido->record().value("rBase2").toDouble();
        this->rBase3 = queryPedido->record().value("rBase3").toDouble();
        this->rBase4 = queryPedido->record().value("rBase4").toDouble();
        this->niva1 = queryPedido->record().value("niva1").toInt();
        this->niva2 = queryPedido->record().value("niva2").toInt();
        this->niva3 = queryPedido->record().value("niva3").toInt();
        this->niva4 = queryPedido->record().value("niva4").toInt();
        this->riva1 = queryPedido->record().value("riva1").toDouble();
        this->riva2 = queryPedido->record().value("riva2").toDouble();
        this->riva3 = queryPedido->record().value("riva3").toDouble();
        this->riva4 = queryPedido->record().value("riva4").toDouble();
        this->rdto1 = queryPedido->record().value("rdto1").toDouble();
        this->rdto2 = queryPedido->record().value("rdto2").toDouble();
        this->rdto3 = queryPedido->record().value("rdto3").toDouble();
        this->rdto4 = queryPedido->record().value("rdto4").toDouble();
        this->nrec1 = queryPedido->record().value("nrec1").toInt();
        this->nrec2 = queryPedido->record().value("nrec2").toInt();
        this->nrec3 = queryPedido->record().value("nrec3").toInt();
        this->nrec4 = queryPedido->record().value("nrec4").toInt();
        this->rrec1 = queryPedido->record().value("rrec1").toDouble();
        this->rrec2 = queryPedido->record().value("rrec2").toDouble();
        this->rrec3 = queryPedido->record().value("rrec3").toDouble();
        this->rrec4 = queryPedido->record().value("rrec4").toDouble();
        this->rtotal1 = queryPedido->record().value("rtotal1").toDouble();
        this->rtotal2 = queryPedido->record().value("rtotal2").toDouble();
        this->rtotal3 = queryPedido->record().value("rtotal3").toDouble();
        this->rtotal4 = queryPedido->record().value("rtotal4").toDouble();
    } else
    {
        switch (accion) {
        case 0:
            QMessageBox::warning(qApp->activeWindow(),tr("Atención"),tr("No se ha encontrado el pedido."),tr("Aceptar"));
            break;
        case 1:
            QMessageBox::warning(qApp->activeWindow(),tr("Atención"),tr("Se ha llegado al final del fichero."),tr("Aceptar"));
            break;
        case 2:
            QMessageBox::warning(qApp->activeWindow(),tr("Atención"),tr("Se ha llegado al inicio del fichero."),tr("Aceptar"));
            break;

        }

    }
}

