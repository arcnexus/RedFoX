#include "frmgestioncobros2.h"
#include "ui_frmgestioncobros2.h"
#include "../vencimientos.h"
#include "../Zona_Contabilidad/apuntes.h"
#include"../Zona_Pacientes/cliente.h"

FrmGestionCobros2::FrmGestionCobros2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmGestionCobros2)
{
    ui->setupUi(this);
    //-------------------
    // Conexiones
    //-------------------

    //-------------------
    // Entidades
    //-------------------
    QSqlQueryModel *model_bancos = new QSqlQueryModel(this);
    model_bancos->setQuery("select descripcion from bancos", Configuracion_global->groupDB);
    ui->cboEntidad->setModel(model_bancos);


}

FrmGestionCobros2::~FrmGestionCobros2()
{
    delete ui;
}

void FrmGestionCobros2::on_btnAceptar_clicked()
{
    valores val;
    val.efectivo = Configuracion_global->MonedatoDouble(ui->txtEfectivo->text());
    val.cheque  = Configuracion_global->MonedatoDouble(ui->txtCheque->text());
    val.internet = Configuracion_global->MonedatoDouble(ui->txtInternet->text());
    val.tarjeta = Configuracion_global->MonedatoDouble(ui->txtTarjeta->text());
    val.transferencia = Configuracion_global->MonedatoDouble(ui->txtTransferencia->text());
    val.vale = Configuracion_global->MonedatoDouble(ui->txtVale->text());
    val.domiciliacion = Configuracion_global->MonedatoDouble(ui->txtDomiciliacion->text());

    // Actualizamos tabla clientes_deuda    
    QHash <QString,QVariant> e;
    e["importe_efectivo"] = val.efectivo;
    e["importe_cheque"] = val.cheque;
    e["importe_domiciliacion"] = val.domiciliacion;
    e["importe_internet"] = val.internet;
    e["importe_tarjeta"] = val.tarjeta;
    e["importe_transferencia"] = val.transferencia;
    e["importe_vale"] = val.vale;
    e["pagado"] = Configuracion_global->MonedatoDouble(ui->txtEntrega->text());
    e["pendiente_cobro"] = Configuracion_global->MonedatoDouble(ui->txtPendiente->text());

    QString error;
    bool updated = SqlCalls::SqlUpdate(e,"clientes_deuda",Configuracion_global->groupDB,QString("id=%1 and id_empresa=%2").arg(this->id_deuda).arg(Configuracion_global->idEmpresa),error);
    if(!updated)
    {
        QMessageBox::warning(this,tr("Gestión de cobros"),tr("Ocurrió un error al actualizar los datos: %1").arg(error));
    }
    else
    {
        if(val.efectivo >0 || val.tarjeta >0 || val.cheque >0 || val.vale >0)
        {
            /* TODO Reactivar y revias con TPV!!
            // Guardar/mostrar en cierre de caja            
            if(QMessageBox::question(this,tr("Gestión de cobros y pagos"),
                                     tr("¿Desea que los cobros efectivo, tarjeta, cheque o vale aparezcan reflejados en el cierre de caja?"
                                        "\nEsto es importante si el dinero o resguardo de cobro se guarda en el cajón portamonedas"),
                                     tr("No"),tr("Sí"))==QMessageBox::Accepted)
            {
                QHash <QString, QVariant> h;
                QString error;
                QString cliente,documento,fecha;
                QMap <int,QSqlRecord> c;
                c = SqlCalls::SelectRecord("clientes_deuda",QString("id=%1 and id_empresa=%2").arg(this->id).arg(Configuracion_global->idEmpresa),Configuracion_global->empresaDB,error);
                cliente = SqlCalls::SelectOneField("clientes","nombre_fiscal",QString("id=%1").arg(c.value(this->id).value(
                                                                                                       "id_cliente").toInt()),
                                                   Configuracion_global->groupDB,error).toString();
                documento = c.value(this->id).value("documento").toString();
                fecha = c.value(this->id).value("fecha").toDate().toString("dd/MM/yyyy");

                // efectivo
                h["concepto"] = tr("Cobro efectivo deuda pendiente de : %1 documento : %2 y de fecha: %3").arg(cliente,documento,fecha);
                h["id_usuario"] = Configuracion_global->id_usuario_activo;
                h["importe"] = val.efectivo;
                h["fecha"]= QDate::currentDate().toString("yyyyMMdd");
                h["hora"] = QTime::currentTime().toString("hh:mm");
                h["entrada"]  = 3;
                int new_id = SqlCalls::SqlInsert(h,"e_s_caja",Configuracion_global->empresaDB,error);
                if(new_id <0)
                    QMessageBox::warning(this,tr("Gestión de cobros"),tr("se produjo un error al insertar: %1").arg(error));
                // tarjetas
                h["concepto"] = tr("Cobro tarjeta deuda pendiente de : %1 documento : %2 y de fecha: %3").arg(cliente,documento,fecha);
                h["id_usuario"] = Configuracion_global->id_usuario_activo;
                h["importe"] = val.efectivo;
                h["fecha"]= QDate::currentDate().toString("yyyyMMdd");
                h["hora"] = QTime::currentTime().toString("hh:mm");
                h["entrada"]  = 4;
                new_id = SqlCalls::SqlInsert(h,"e_s_caja",Configuracion_global->empresaDB,error);
                if(new_id <0)
                    QMessageBox::warning(this,tr("Gestión de cobros"),tr("se produjo un error al insertar: %1").arg(error));
                // Cheques
                h["concepto"] = tr("Cobro cheque deuda pendiente de : %1 documento : %2 y de fecha: %3").arg(cliente,documento,fecha);
                h["id_usuario"] = Configuracion_global->id_usuario_activo;
                h["importe"] = val.efectivo;
                h["fecha"]= QDate::currentDate().toString("yyyyMMdd");
                h["hora"] = QTime::currentTime().toString("hh:mm");
                h["entrada"]  = 5;
                new_id = SqlCalls::SqlInsert(h,"e_s_caja",Configuracion_global->empresaDB,error);
                if(new_id <0)
                    QMessageBox::warning(this,tr("Gestión de cobros"),tr("se produjo un error al insertar: %1").arg(error));
                // vales R
                h["concepto"] = tr("Cobro con vale deuda pendiente de : %1 documento : %2 y de fecha: %3").arg(cliente,documento,fecha);
                h["id_usuario"] = Configuracion_global->id_usuario_activo;
                h["importe"] = val.efectivo;
                h["fecha"]= QDate::currentDate().toString("yyyyMMdd");
                h["hora"] = QTime::currentTime().toString("hh:mm");
                h["entrada"]  = 6;
                new_id = SqlCalls::SqlInsert(h,"e_s_caja",Configuracion_global->empresaDB,error);
                if(new_id <0)
                    QMessageBox::warning(this,tr("Gestión de cobros"),tr("se produjo un error al insertar: %1").arg(error));
            }
            */
        }

        if(val.internet >0 || val.transferencia >0 || val.domiciliacion >0)
        {
            //-----------------------
            // recupero datos cuenta
            //-----------------------
            QHash <QString, QVariant > deuda;
            QMap<int,QSqlRecord> bancos;
            bancos = SqlCalls::SelectRecord("bancos",QString("descripcion = '%1'").arg(ui->cboEntidad->currentText()),
                                            Configuracion_global->groupDB,error);
            if(error.isEmpty())
            {
                QMapIterator <int, QSqlRecord> i(bancos);
                while (i.hasNext())
                {
                    i.next();
                    deuda["entidad"] = i.value().value("entidad").toString();
                    this->entidad =  i.value().value("entidad").toString();
                    deuda["oficina"] = i.value().value("oficina").toString();
                    this->oficina  = i.value().value("oficina").toString();
                    deuda["dc"] = i.value().value("dc").toString();
                    this->dc = i.value().value("dc").toString();
                    deuda["cuenta"] = i.value().value("cuenta").toString();
                    this->cuenta  = i.value().value("cuenta").toString();

                }

            } else
            {
                QMessageBox::warning(this,tr("Ocurrió un error al localizar la cuenta :%1").arg(error),tr("Aceptar"));
            }
            bool updated = SqlCalls::SqlUpdate(deuda,"clientes_deuda",Configuracion_global->groupDB,QString("id=%1 id_empresa=%2").arg(this->id_deuda).arg(Configuracion_global->idEmpresa),error);
            if(!updated)
                QMessageBox::warning(this,tr("Gestión de cobros"),tr("Ocurrió un error al actualizar los datos de la deuda:%1").arg(error),
                                     tr("Aceptar"));
        }

        // historial de deudas        
        QHash <QString, QVariant> historial;
        historial["id_cab"] = this->id_deuda;
        historial["fecha_movimiento"] = QDate::currentDate().toString("yyyyMMdd");
        historial["importe_anterior"] = importe;
        historial["pagado"] = Configuracion_global->MonedatoDouble(ui->txtEntrega->text());
        historial["pendiente_cobro"] = Configuracion_global->MonedatoDouble(ui->txtPendiente->text());
        historial["entidad"] = this->entidad;
        this->entidad.clear();
        historial["oficina"] = this->oficina;
        this->oficina.clear();
        historial["dc"] = this->dc;
        this->dc.clear();
        historial["cuenta"] = this->cuenta;
        this->cuenta.clear();
        if(Configuracion_global->MonedatoDouble(ui->txtCambio->text())>0)
            historial["importe_efectivo"] = importe ; // total pendiente
        else
            historial["importe_efectivo"] = Configuracion_global->MonedatoDouble(ui->txtEfectivo->text());
        historial["importe_tarjeta"] = Configuracion_global->MonedatoDouble(ui->txtTarjeta->text());
        historial["importe_cheque"] = Configuracion_global->MonedatoDouble(ui->txtCheque->text());
        historial["importe_transferencia"] =Configuracion_global->MonedatoDouble(ui->txtTransferencia->text());
        historial["importe_internet"] = Configuracion_global->MonedatoDouble(ui->txtInternet->text());
        historial["importe_vale"] = Configuracion_global->MonedatoDouble(ui->txtVale->text());
        historial["importe_cambio"] =Configuracion_global->MonedatoDouble(ui->txtCambio->text());
        historial["id_empresa"] = Configuracion_global->idEmpresa;
        int new_id = SqlCalls::SqlInsert(historial,"histo_clientes_deuda",Configuracion_global->groupDB,error);
        if(new_id == -1)
            QMessageBox::warning(this,tr("Gestión de deudas"),
                                 tr("Ocurrió un error al insertar en historial: %1").arg(error));


        //fin historial

    }
    if(e.value("pendiente_cobro").toDouble() == 0){


        QHash <QString,QVariant> c;
        c["cobrado"] = true;
        bool updated = true;
        QString clausula;
        if(this->id_factura > -1)
        {
            //QString("select id,fecha,vencimiento,importe,pagado,pendiente_cobro from clientes_deuda where id_factura = %1 and id_empresa = %2;"
            QStringList cl;
            cl << QString("id_factura = %1 and id_empresa = %2").arg(id_factura).arg(Configuracion_global->idEmpresa);
            cl << QString("pendiente_cobro > 0");
            if(SqlCalls::SelectRecord("clientes_deuda",cl,Configuracion_global->groupDB,error).isEmpty())
            {
                clausula = QString("id = %1").arg(this->id_factura);
                updated =SqlCalls::SqlUpdate(c,"cab_fac",Configuracion_global->empresaDB,clausula,error);
            }
        }
        else if(id_ticket > -1)
        {
            clausula = QString("id = %1").arg(this->id_ticket);
            updated =SqlCalls::SqlUpdate(c,"cab_tpv",Configuracion_global->empresaDB,clausula,error);
        }

        if(!updated)
            QMessageBox::warning(this,tr("Gestión de cobros"),tr("Ocurrió un error al actualizar: %1").arg(error),tr("Aceptar"));
    }

    if(Configuracion_global->contabilidad)

    {
        // TODO - Asiento contable cobro
        //----------------------------
        // Asiento cobro
        //----------------------------
//        apuntes oApunte;
//        Cliente oCliente;
//        int apunte;
//        bool ok = true;
//        bool nuevo;
//        int pos = 1;
//        oCliente.Recuperar("select * from clientes where id = " + QString::number(this->id_cliente));

//        oApunte.cuenta_d = oCliente.codigo_cliente;
//        oApunte.cuenta_relacion = oCliente.codigo_cliente;
//        oApunte.cuenta_h = Configuracion_global->cuenta_venta_mercaderias;
//        oApunte.descripcion_d = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_d);
//        oApunte.id_cuenta_d = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_d);
//        oApunte.descripcion_h = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_h);
//        oApunte.id_cuenta_h = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_h);
//        oApunte.comentario_d = tr("Nuestra factura num: ")+this->factura;
//        oApunte.comentario_h = tr("Ventas mercaderias factura num: ")+this->factura;
//        oApunte.fecha_asiento = this->fecha;
//        oApunte.id_documento = this->id;
//        oApunte.importe_d = this->total;
//        oApunte.importe_h = this->base;
//        oApunte.pos_en_asiento = pos;
//        oApunte.asiento = oApunte.nuevo_numero_apunte();
//        apunte = oApunte.asiento;
//        nuevo = oApunte.nuevalinea();
    }
    accept();
}
double FrmGestionCobros2::getImporte() const
{
    return importe;
}

void FrmGestionCobros2::setImporte(double value,int id_)
{
    importe = value;
    this->id_deuda = id_;
    ui->txtImporte_pendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(importe,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtPendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(importe,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtEfectivo->setFocus();
}


void FrmGestionCobros2::on_txtEfectivo_editingFinished()
{
    blockSignals(true);
    ui->txtEfectivo->setText(Configuracion_global->toFormatoMoneda(ui->txtEfectivo->text()));
    calcular();
    blockSignals(false);
}

void FrmGestionCobros2::on_txtTransferencia_editingFinished()
{
    blockSignals(true);
    ui->txtTransferencia->setText(Configuracion_global->toFormatoMoneda(ui->txtTransferencia->text()));
    calcular();
    blockSignals(false);
}

void FrmGestionCobros2::on_txtTarjeta_editingFinished()
{
    blockSignals(true);
    ui->txtTarjeta->setText(Configuracion_global->toFormatoMoneda(ui->txtTarjeta->text()));
    calcular();
    blockSignals(false);
}

void FrmGestionCobros2::on_txtCheque_editingFinished()
{
    blockSignals(true);
    ui->txtCheque->setText(Configuracion_global->toFormatoMoneda(ui->txtCheque->text()));
    calcular();
    blockSignals(false);
}

void FrmGestionCobros2::on_txtInternet_editingFinished()
{
    blockSignals(true);
    ui->txtInternet->setText(Configuracion_global->toFormatoMoneda(ui->txtInternet->text()));
    calcular();
    blockSignals(false);
}

void FrmGestionCobros2::calcular()
{
    double pendiente,efectivo,transferencia,tarjeta,cheque,internet;
    double domiciliacion,nuevo_pendiente,cambio,pagado;

    pendiente = Configuracion_global->MonedatoDouble(ui->txtImporte_pendiente->text());
    efectivo = Configuracion_global->MonedatoDouble(ui->txtEfectivo->text());
    transferencia = Configuracion_global->MonedatoDouble(ui->txtTransferencia->text());
    domiciliacion = Configuracion_global->MonedatoDouble(ui->txtDomiciliacion->text());
    tarjeta = Configuracion_global->MonedatoDouble(ui->txtTarjeta->text());
    cheque = Configuracion_global->MonedatoDouble(ui->txtCheque->text());
    internet = Configuracion_global->MonedatoDouble(ui->txtInternet->text());

    pagado = efectivo + transferencia + tarjeta + cheque + internet + domiciliacion;
    ui->txtEntrega->setText(Configuracion_global->toFormatoMoneda(QString::number(pagado,'f',2)));
    nuevo_pendiente = pendiente - pagado;
    if(nuevo_pendiente >0)
    {
        ui->txtPendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(nuevo_pendiente,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
        ui->txtCambio->setText("0,00");
    }
    else{
        nuevo_pendiente = -nuevo_pendiente;
        ui->txtCambio->setText(Configuracion_global->toFormatoMoneda(QString::number(nuevo_pendiente,'f',
                                                                                     Configuracion_global->decimales_campos_totales)));
        ui->txtPendiente->setText("0,00");
    }

}
int FrmGestionCobros2::getId_ticket() const
{
    return id_ticket;
}

void FrmGestionCobros2::setId_ticket(int value)
{
    id_ticket = value;
}

int FrmGestionCobros2::getId_factura() const
{
    return id_factura;
}

void FrmGestionCobros2::setId_factura(int value)
{
    id_factura = value;
}




void FrmGestionCobros2::on_txtDomiciliacion_editingFinished()
{
    ui->txtDomiciliacion->blockSignals(true);
    ui->txtDomiciliacion->setText(Configuracion_global->toFormatoMoneda(ui->txtDomiciliacion->text()));
    calcular();
    ui->txtDomiciliacion->blockSignals(false);

}
