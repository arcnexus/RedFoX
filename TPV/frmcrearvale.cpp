#include "frmcrearvale.h"
#include "ui_frmcrearvale.h"
#include "../Auxiliares/Globlal_Include.h"
#include "../Zona_Pacientes/vale.h"
#include "../Busquedas/db_consulta_view.h"

frmCrearVale::frmCrearVale(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmCrearVale)
{
    ui->setupUi(this);
    ui->dateFechaCreacion->setDate(QDate::currentDate());
    ui->dateVencimiento->setDate(QDate::currentDate().addDays(Configuracion_global->caducidad_vales));
}

frmCrearVale::~frmCrearVale()
{
    delete ui;
}

void frmCrearVale::setImporte(double importe)
{
    if(importe < 0)
        importe = - importe;

    ui->txtImporte->setText(Configuracion_global->toFormatoMoneda(QString::number(importe,'f',
                                                                                  Configuracion_global->decimales_campos_totales)));
}

void frmCrearVale::setId_ticket(int id_ticket)
{
    this->id_ticket == id_ticket;
}

void frmCrearVale::on_txtImporte_editingFinished()
{
    blockSignals(true);
    ui->txtImporte->setText(Configuracion_global->toFormatoMoneda(ui->txtImporte->text()));
    blockSignals(false);
}

void frmCrearVale::on_txtcodigo_editingFinished()
{
    ui->txtcodigo->blockSignals(true);
    QMap <int,QSqlRecord> cli;
    QString error;
    cli = SqlCalls::SelectRecord("clientes",QString("codigo_cliente='%1'").arg(ui->txtcodigo->text().trimmed()),Configuracion_global->groupDB,error);
    QMapIterator <int,QSqlRecord> i(cli);
    if(cli.count() > 0 && cli.count()<2){
        while(i.hasNext())
        {
            i.next();
            ui->txtnombre_fiscal->setText(i.value().value("nombre_fiscal").toString());
        }
    }else
    {
        if(cli.count()>1)
            QMessageBox::warning(this,tr("Vales"),tr("Atención se ha localizado más de un cliente con el mismo código:%1"
                                                 "repase la base de datos").arg(ui->txtcodigo->text()),tr("Aceptar"));
        else
        {
            if(!ui->txtcodigo->text().isEmpty())
            {
                QMessageBox::warning(this,tr("Vales"),tr("No se ha encontrado ningún cliente con ese código:\n%1").arg(
                                         ui->txtcodigo->text()),tr("Aceptar"));
                ui->txtcodigo->clear();
            }
        }
    }
    ui->txtcodigo->blockSignals(false);

}

void frmCrearVale::on_btnCrear_clicked()
{
    QString error;
    int id_cliente = SqlCalls::SelectOneField("clientes","id",QString("codigo_cliente = '%1'").arg(ui->txtcodigo->text()),
                                                               Configuracion_global->groupDB,error).toInt();

    vale oVale;
    int numvale = oVale.crearvale(id_cliente,ui->txtnombre_fiscal->text(), ui->dateFechaCreacion->date(),ui->dateVencimiento->date(),
                                  Configuracion_global->MonedatoDouble(ui->txtImporte->text()),this->id_ticket);
    if(numvale > 0)
    {
        if(QMessageBox::question(this,tr("Gestión de vales"),tr("¿Imprimir copia del vale para el cliente?"),
                                 tr("No"),tr("Sí"))==QMessageBox::Accepted)

        {
            // TODO - Imprimir vale
        }

    }
    accept();
}

void frmCrearVale::on_pushButton_3_clicked()
{
    db_consulta_view consulta;
    QStringList campos;
    QString error;
    campos << "nombre_fiscal" << "codigo_cliente" <<"cif_nif" <<"telefono1"  << "movil";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Artículos");
    consulta.set_db("Maya");
    consulta.set_SQL("select id, nombre_fiscal, codigo_cliente,cif_nif,telefono1,movil from clientes");
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Nombre Fiscal") <<tr("código") << tr("cif/nif/nie") << tr("Teléfono ") << tr("móvil");
    tamanos <<"0" << "300"  <<"100" << "100" << "120" <<"120";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Clientes");
    if(consulta.exec())
    {
        int id_cliente = consulta.get_id();

        QString nombre,codigo;
        QMap<int, QSqlRecord> cli;
        cli = SqlCalls::SelectRecord("clientes",QString("id=%1").arg(id_cliente),Configuracion_global->groupDB,error);
        nombre = cli.value(id_cliente).value("nombre_fiscal").toString();
        codigo = cli.value(id_cliente).value("codigo_cliente").toString();
        ui->txtcodigo->setText(codigo);
        ui->txtnombre_fiscal->setText(nombre);


    }
}
