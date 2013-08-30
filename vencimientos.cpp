#include "vencimientos.h"

vencimientos::vencimientos(QObject *parent) :
    QObject(parent)
{
}

void vencimientos::calcular_vencimiento(QDate fecha, int id_cliente,int id_ticket,int id_factura, QString documento,
                                        int tipo,QString compra_venta,double importe)
{
    Configuracion_global->groupDB.transaction();
    QMap <int,QSqlRecord> m;
    QString condiciones  = QString("id=%1").arg(id_cliente);
    QString error,error2,entidad,oficina,dc,cuenta;
    QDate vencimiento;
    bool t_error;
    m = SqlCalls::SelectRecord("clientes",condiciones,Configuracion_global->groupDB,error);
    int id_forma_pago;
    if(error.isEmpty())
    {
        QMapIterator<int,QSqlRecord> cli(m);
        while(cli.hasNext())
        {
            cli.next();
            id_forma_pago = cli.value().value("id_forma_pago").toInt();
            entidad = cli.value().value("entidad_bancaria").toString();
            oficina = cli.value().value("oficina_bancaria").toString();
            dc = cli.value().value("dc").toString();
            cuenta = cli.value().value("cuenta_corriente").toString();
        }


        QMap <int,QSqlRecord> f;
        condiciones  = QString("id=%1").arg(id_forma_pago);
        f= SqlCalls::SelectRecord("formpago",condiciones,Configuracion_global->groupDB,error);
        if(error2.isEmpty())
        {
            QMapIterator <int,QSqlRecord> fp(f);
            int dia_pago1,dia_pago2,dia_pago3,dia_pago4,dias_entre_plazos,numero_plazos;
            QString cuenta_cont_pago;
            while(fp.hasNext())
            {
                fp.next();
                dia_pago1 = fp.value().value("dia_pago1").toInt();
                dia_pago2 = fp.value().value("dia_pago2").toInt();
                dia_pago3 = fp.value().value("dia_pago3").toInt();
                dia_pago4 = fp.value().value("dia_pago4").toInt();
                dias_entre_plazos = fp.value().value("dias_entre_plazos").toInt();

                cuenta_cont_pago = fp.value().value("cuenta_cont_pago").toString();
                numero_plazos = fp.value().value("numero_plazos").toInt();

                //----------------------
                // Calculo vencimiento
                //----------------------
                int plazo2;
                for(int plazo = 1; plazo <=numero_plazos;plazo ++)
                {
                    plazo2 = plazo;
                    vencimiento = fecha.addDays(dias_entre_plazos * plazo);
                    int dia,mes,ano;
                    dia = vencimiento.day();
                    mes = vencimiento.month();
                    ano = vencimiento.year();
                    if(dia < dia_pago1)
                        dia = dia_pago1;
                    else if(dia < dia_pago2)
                        dia = dia_pago2;
                    else if(dia < dia_pago3)
                        dia = dia_pago3;
                    else if(dia < dia_pago4)
                        dia = dia_pago4;
                    else if(dia > dia_pago1 && dia > dia_pago2 &&dia > dia_pago3 && dia > dia_pago4)
                    {
                        dia = dia_pago1;
                        if(dia == 0)
                            dia = QDate().currentDate().day();
                        if (mes <=11)
                            mes++;
                        else
                        {
                            mes = 1;
                            ano++;
                        }


                    }
                    vencimiento = QDate(ano,mes,dia);
                   //---------------------
                   // insertar vencimiento
                   //---------------------
                   QHash <QString,QVariant> h;
                   h["fecha"] =fecha;
                   h["vencimiento"] = vencimiento;
                   h["documento"]= documento;
                   if(id_ticket > -1)
                       h["id_ticket"] =id_ticket;
                   if(id_factura > -1)
                       h["id_factura"] = id_factura;

                   h["tipo"] = tipo;
                   h["importe"] =importe;
                   h["pendiente_cobro"] = importe;
                   h["entidad"] = entidad;
                   h["oficina"] = oficina;
                   h["dc"] = dc;
                   h["cuenta"] = cuenta;

                   int new_id = SqlCalls::SqlInsert(h,"clientes_deuda",Configuracion_global->groupDB,error);

                   if(new_id ==-1)
                   {
                       t_error = true;
                       QMessageBox::warning(qApp->activeWindow(),tr("Gestión de vencimientos"),
                                            tr("Ocurrió un error:")+error,tr("Aceptar"));
                   }

                }
                if(!t_error)
                {
                    Configuracion_global->groupDB.commit();
                    QString mensaje;
                    if (plazo2 ==1)
                        mensaje = tr("Se ha creado el vencimiento");
                    else if(plazo2 >1)
                        mensaje = tr("Se han creado los vencimientos");
                    t = new TimedMessageBox(qApp->activeWindow(),mensaje);
                 } else
                {
                    Configuracion_global->groupDB.rollback();
                    QMessageBox::warning(qApp->activeWindow(),tr("Vencimientos"),tr("No se ha podido realizar la transacción"),
                                         tr("Aceptar"));
                }
            }
        }
    }
}
