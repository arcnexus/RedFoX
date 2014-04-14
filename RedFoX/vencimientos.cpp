#include "vencimientos.h"

vencimientos::vencimientos(QObject *parent) :
    QObject(parent)
{
    clear();
}

void vencimientos::recuperar(QStringList condiciones)
{
    QString error;
    QMap <int,QSqlRecord> m = SqlCalls::SelectRecord("formpago",condiciones,Configuracion_global->groupDB,error);
    if(error.isEmpty())
    {
        if(!m.isEmpty())
        {
            QSqlRecord r = m.first();

            this->id = r.value("id").toInt();
            this->codigo = r.value("codigo").toString();
            this->forma_pago = r.value("forma_pago").toString();
            this->codigo_cuenta_contable = r.value("cuenta_cont_pago").toString();
            //this->desc_cuenta_cont = Configuracion_global->Devolver_descripcion_cuenta_contable(this->codigo_cuenta_contable);
            this->dia_pago1 = r.value("dia_pago1").toInt();
            this->dia_pago2 = r.value("dia_pago2").toInt();
            this->dia_pago3 = r.value("dia_pago3").toInt();
            this->dia_pago4 = r.value("dia_pago4").toInt();
            this->dias_entre_plazos = r.value("dias_entre_plazos").toInt();
            this->numero_plazos = r.value("numero_plazos").toInt();
            this->dias_hasta_pago = r.value("dias_hasta_pago").toInt();
            this->al_contado = r.value("al_contado").toBool();
        }
    }
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Formas de Pago"),tr("Ocurrió un error al recuperar: ")+error,
                             tr("Aceptar"));
}

void vencimientos::anadir()
{
    QHash <QString, QVariant> h;
    QString error;
    h["codigo"] = this->codigo;
    h["forma_pago"] = this->forma_pago;
    h["dia_pago1"] = this->dia_pago1;
    h["dia_pago2"] = this->dia_pago2;
    h["dia_pago3"] = this->dia_pago3;
    h["dia_pago4"] = this->dia_pago4;
    h["dias_entre_plazos"] = this->dias_entre_plazos;
    //h["cuenta_cont_pago"] =;
    h["dias_hasta_pago"] = dias_hasta_pago;
    h["al_contado"] = al_contado;

    if(SqlCalls::SqlInsert(h,"formpago",Configuracion_global->groupDB,error) > -1)
        TimedMessageBox::Box(qApp->activeWindow(),tr("Se ha insertado la forma de pago"));
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Formas de Pago"),
                             tr("Ocurrió un error al insertar: ")+error,tr("Aceptar"));
}

void vencimientos::guardar()
{
    QHash <QString, QVariant> h;
    QString error;
    QString condicion =QString("id = %1").arg(this->id);
    h["codigo"] = this->codigo;
    h["forma_pago"] = this->forma_pago;
    h["dia_pago1"] = this->dia_pago1;
    h["dia_pago2"] = this->dia_pago2;
    h["dia_pago3"] = this->dia_pago3;
    h["dia_pago4"] = this->dia_pago4;
    h["dias_entre_plazos"] = this->dias_entre_plazos;
    //h["cuenta_cont_pago"] =;
    h["dias_hasta_pago"] = dias_hasta_pago;
    h["al_contado"] = al_contado;

    if(SqlCalls::SqlUpdate(h,"formpago",Configuracion_global->groupDB,condicion,error))
        TimedMessageBox::Box(qApp->activeWindow(),tr("Se ha actualizado la forma de pago"));
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Formas de Pago"),
                             tr("Ocurrió un error al guardar: ")+error,tr("Aceptar"));
}

void vencimientos::borrar()
{
    QString error;
    if(SqlCalls::SqlDelete("formpago",Configuracion_global->groupDB,QString("id=%1").arg(id),error))
        TimedMessageBox::Box(qApp->activeWindow(),tr("Se ha borrado la forma de pago"));
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Formas de Pago"),tr("Ocurrió un error al borrar: ")+error,tr("Aceptar"));
}

void vencimientos::clear()
{
    this->codigo.clear();
    this->forma_pago.clear();
    this->codigo_cuenta_contable.clear();
    this->dia_pago1 = 0;
    this->dia_pago2 = 0;
    this->dia_pago3 = 0;
    this->dia_pago4 = 0;
    this->dias_entre_plazos =0;
    this->numero_plazos = 0;
    this->dias_hasta_pago = false;
}

bool vencimientos::calcular_vencimiento(QDate fecha,int id_forma_pago, int id_cliente,int id_ticket,int id_factura, QString documento,
                                        int tipo,bool is_venta,double importe)
{
    QString error,entidad,oficina,dc,cuenta;
    QDate vencimiento;
    bool t_error = false;

    QMap <int,QSqlRecord> m = SqlCalls::SelectRecord("clientes",QString("id=%1").arg(id_cliente),Configuracion_global->groupDB,error);

    if(error.isEmpty())
    {
        if(!m.isEmpty())
        {
            QSqlRecord r = m.first();

            entidad       = r.value("entidad_bancaria").toString();
            oficina       = r.value("oficina_bancaria").toString();
            dc            = r.value("dc").toString();
            cuenta        = r.value("cuenta_corriente").toString();

            QMap <int,QSqlRecord> f = SqlCalls::SelectRecord("formpago",QString("id=%1").arg(id_forma_pago),Configuracion_global->groupDB,error);
            if(error.isEmpty())
            {
                if(!f.isEmpty())
                {
                    QSqlRecord f_r = f.first();
                    int cli_dia_pago1 = SqlCalls::SelectOneField("clientes","dia_pago1",QString("id=%1").arg(id_cliente),Configuracion_global->groupDB,error).toInt();
                    int cli_dia_pago2 = SqlCalls::SelectOneField("clientes","dia_pago2",QString("id=%1").arg(id_cliente),Configuracion_global->groupDB,error).toInt();

                    int _dia_pago1 = f_r.value("dia_pago1").toInt();
                    int _dia_pago2 = f_r.value("dia_pago2").toInt();
                    int _dia_pago3 = f_r.value("dia_pago3").toInt();
                    int _dia_pago4 = f_r.value("dia_pago4").toInt();
                    int _dias_entre_plazos = f_r.value("dias_entre_plazos").toInt();
                    int _numero_plazos = f_r.value("numero_plazos").toInt();
                    bool _contado = f_r.value("al_contado").toBool();
                    int _dias_hast = f_r.value("dias_hasta_pago").toInt();

                    double importe_plazo = importe/_numero_plazos;
                    QDate today = QDate::currentDate();
                    if(_contado)
                    {
                        int new_id;
                        if(is_venta)
                        {
                            // insertar vencimiento cliente
                            QHash <QString,QVariant> h;
                            h["fecha"] = today;
                            h["vencimiento"] = today;
                            h["documento"]= documento;

                            if(id_ticket > -1)
                                h["id_ticket"] = id_ticket;
                            if(id_factura > -1)
                                h["id_factura"] = id_factura;

                            h["tipo"] = tipo;
                            h["importe"] =importe;
                            h["pendiente_cobro"] = importe;
                            h["entidad"] = entidad;
                            h["oficina"] = oficina;
                            h["dc"] = dc;
                            h["cuenta"] = cuenta;
                            h["id_cliente"] = id_cliente;
                            h["id_empresa"] = Configuracion_global->idEmpresa;
                            new_id = SqlCalls::SqlInsert(h,"clientes_deuda",Configuracion_global->groupDB,error);
                        }
                        else
                        {
                            // insertar vencimiento proveedor
                            QHash <QString,QVariant> h;
                            //`asiento_numero`,
                            h["documento"] = documento;
                            h["fecha_deuda"]= today;
                            //`id_asiento`
                            h["id_documento"] =documento;
                            h["id_proveedor"]= id_cliente;
                            h["importe_deuda"] = importe;
                            h["pendiente"] = importe;
                            h["vencimiento"] = vencimiento;
                            h["id_empresa"]=Configuracion_global->idEmpresa;
                            new_id = SqlCalls::SqlInsert(h,"deudas_proveedores",Configuracion_global->groupDB,error);
                        }
                        if(new_id ==-1)
                        {
                            t_error = true;
                            QMessageBox::warning(qApp->activeWindow(),tr("Gestión de vencimientos"),
                                                 tr("Ocurrió un error:")+error,tr("Aceptar"));
                        }
                        if(!t_error)
                        {
                            TimedMessageBox::Box(qApp->activeWindow(),tr("Se ha creado el vencimiento"));
                            return true;
                        }
                        else
                        {
                            QMessageBox::warning(qApp->activeWindow(),tr("Vencimientos"),tr("No se ha podido realizar la transacción"),
                                                 tr("Aceptar"));
                        }
                    }
                    else
                    {
                        QDate first = today.addDays(_dias_hast);
                        int first_day = first.day();
                        int target_first_day = _dia_pago1;

                        if(target_first_day < first_day)
                            target_first_day = _dia_pago2;
                        if(target_first_day < first_day)
                            target_first_day = _dia_pago3;
                        if(target_first_day < first_day)
                            target_first_day = _dia_pago4;
                        if(target_first_day < first_day)
                            target_first_day = _dia_pago1;

                        QDate first_date;
                        first_date.setDate(first.year(),first.month(),target_first_day);
                        if(target_first_day < first_day)
                        {
                            first_date = first.addMonths(1);
                            first_date.setDate(first_date.year(),first_date.month(),target_first_day);
                        }

                        if(cli_dia_pago1!=0)
                        {
                            int day = target_first_day;
                            if(cli_dia_pago1 > day)
                                target_first_day = cli_dia_pago1;
                            else if(cli_dia_pago2 > day)
                                target_first_day = cli_dia_pago2;
                            else
                                target_first_day = cli_dia_pago1;
                            first_date.setDate(first_date.year(),first_date.month(),target_first_day);
                        }

                        if(is_venta)
                        {
                            //------------------------------
                            // insertar vencimiento cliente
                            //------------------------------
                            QHash <QString,QVariant> h;
                            h["fecha"] =fecha;
                            h["vencimiento"] = first_date;
                            h["documento"]= documento;

                            if(id_ticket > -1)
                                h["id_ticket"] = id_ticket;
                            if(id_factura > -1)
                                h["id_factura"] = id_factura;

                            h["tipo"] = tipo;
                            h["importe"] =importe_plazo;
                            h["pendiente_cobro"] = importe_plazo;
                            h["entidad"] = entidad;
                            h["oficina"] = oficina;
                            h["dc"] = dc;
                            h["cuenta"] = cuenta;
                            h["id_cliente"] = id_cliente;
                            h["id_empresa"] = Configuracion_global->idEmpresa;
                            if(SqlCalls::SqlInsert(h,"clientes_deuda",Configuracion_global->groupDB,error)==-1)
                            {
                                QMessageBox::warning(qApp->activeWindow(),tr("Gestión de vencimientos"),
                                                     tr("Ocurrió un error:")+error,tr("Aceptar"));
                                return false;
                            }
                        }
                        else
                        {
                            //--------------------------------
                            // insertar vencimiento proveedor
                            //--------------------------------
                            QHash <QString,QVariant> h;
                            //`asiento_numero`,
                            h["documento"] = documento;
                            h["fecha_deuda"]= first_date;
                            //`id_asiento`
                            h["id_documento"] =documento;
                            h["id_proveedor"]= id_cliente;
                            h["importe_deuda"] = importe_plazo;
                            h["pendiente"] = importe_plazo;
                            h["vencimiento"] = vencimiento;
                            h["id_empresa"] = Configuracion_global->idEmpresa;
                            if(SqlCalls::SqlInsert(h,"deudas_proveedores",Configuracion_global->groupDB,error) ==-1)
                            {
                                QMessageBox::warning(qApp->activeWindow(),tr("Gestión de vencimientos"),
                                                     tr("Ocurrió un error:")+error,tr("Aceptar"));
                                return false;
                            }
                        }


                        for(int plazo = 1; plazo < _numero_plazos;++plazo)
                        {
                            vencimiento = first_date.addDays(_dias_entre_plazos * plazo);

                            int first_day = vencimiento.day();
                            int target_first_day = _dia_pago1;

                            if(target_first_day < first_day)
                                target_first_day = _dia_pago2;
                            if(target_first_day < first_day)
                                target_first_day = _dia_pago3;
                            if(target_first_day < first_day)
                                target_first_day = _dia_pago4;
                            if(target_first_day < first_day)
                                target_first_day = _dia_pago1;

                            vencimiento.setDate(vencimiento.year(),vencimiento.month(),target_first_day);
                            if(target_first_day < first_day)
                            {
                                vencimiento = vencimiento.addMonths(1);
                                vencimiento.setDate(vencimiento.year(),vencimiento.month(),target_first_day);
                            }

                            if(cli_dia_pago1!=0)
                            {
                                int day = target_first_day;
                                if(cli_dia_pago1 > day)
                                    target_first_day = cli_dia_pago1;
                                else if(cli_dia_pago2 > day)
                                    target_first_day = cli_dia_pago2;
                                else
                                    target_first_day = cli_dia_pago1;
                                vencimiento.setDate(vencimiento.year(),vencimiento.month(),target_first_day);
                            }

                            int new_id;
                            if(is_venta)
                            {
                                //------------------------------
                                // insertar vencimiento cliente
                                //------------------------------
                                QHash <QString,QVariant> h;
                                h["fecha"] =fecha;
                                h["vencimiento"] = vencimiento;
                                h["documento"]= documento;

                                if(id_ticket > -1)
                                    h["id_ticket"] = id_ticket;
                                if(id_factura > -1)
                                    h["id_factura"] = id_factura;

                                h["tipo"] = tipo;
                                h["importe"] =importe_plazo;
                                h["pendiente_cobro"] = importe_plazo;
                                h["entidad"] = entidad;
                                h["oficina"] = oficina;
                                h["dc"] = dc;
                                h["cuenta"] = cuenta;
                                h["id_cliente"] = id_cliente;
                                h["id_empresa"] = Configuracion_global->idEmpresa;
                                new_id = SqlCalls::SqlInsert(h,"clientes_deuda",Configuracion_global->groupDB,error);
                            }
                            else
                            {
                                //--------------------------------
                                // insertar vencimiento proveedor
                                //--------------------------------
                                QHash <QString,QVariant> h;
                                //`asiento_numero`,
                                h["documento"] = documento;
                                h["fecha_deuda"]= fecha;
                                //`id_asiento`
                                h["id_documento"] =documento;
                                h["id_proveedor"]= id_cliente;
                                h["importe_deuda"] = importe_plazo;
                                h["pendiente"] = importe_plazo;
                                h["vencimiento"] = vencimiento;
                                h["id_empresa"] = Configuracion_global->idEmpresa;
                                new_id = SqlCalls::SqlInsert(h,"deudas_proveedores",Configuracion_global->groupDB,error);
                            }
                            if(new_id ==-1)
                            {
                                QMessageBox::warning(qApp->activeWindow(),tr("Gestión de vencimientos"),
                                                     tr("Ocurrió un error:")+error,tr("Aceptar"));
                                return false;
                            }
                        }
                    return true;
                    }
                }
            }
        }
    }
    return false;
}
