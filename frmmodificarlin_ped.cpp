#include "frmmodificarlin_ped.h"
#include "ui_frmmodificarlin_ped.h"
#include "pedidos.h"
#include "articulo.h"
#include "configuracion.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

FrmModificarLin_ped::FrmModificarLin_ped(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmModificarLin_ped)
{
    ui->setupUi(this);
}

FrmModificarLin_ped::~FrmModificarLin_ped()
{
    delete ui;
}

void FrmModificarLin_ped::on_pushButton_clicked()
{
    Pedidos *oPedido = new Pedidos();
    oPedido->ModificarLineaPedido(this->nId,ui->txtCodigo->text(),ui->txtCantidad->text().toDouble(),
                                 ui->txtDescripcion->text(),ui->txtPVP->text().replace(".","").toDouble(),
                                 ui->txtSubTotal->text().replace(".","").toDouble(),ui->txtPorcDto->text().toDouble(),
                                 ui->txtDTO->text().replace(".","").toDouble(),ui->txtTotal->text().replace(".","").toDouble(),
                                 ui->txtPorcIVA->text().toDouble());

    delete oPedido;
}

void FrmModificarLin_ped::on_txtCodigo_editingFinished()
{
    if (!ui->txtCodigo->text().isEmpty()) {
        if (ui->txtDescripcion->text().isEmpty()) {
            Articulo *oArt =  new Articulo();
            Configuracion *o_configuracion = new Configuracion();
            oArt->Recuperar("Select * from articulos where cCodigo = '"+ui->txtCodigo->text()+"'");
            ui->txtCodigo->setText(oArt->getcCodigo());
            ui->txtDescripcion->setText(oArt->getcDescripcion());
            ui->txtPVP->setText(o_configuracion->FormatoNumerico(QString::number(oArt->getrTarifa1(),'f',2)));
            ui->txtSubTotal->setText(o_configuracion->FormatoNumerico(QString::number(oArt->getrTarifa1(),'f',2)));
            if (oArt->getrDto() > ui->txtPorcDto->text().toDouble()) {
                ui->txtPorcDto->setText(o_configuracion->FormatoNumerico(QString::number(oArt->getrDto(),'f',0)));
            }
            ui->txtPorcIVA->setText(QString::number(oArt->getnTipoIva(),'f',0));
            delete oArt;
            delete o_configuracion;
        }
        CalcularTotal();
    }
}

void FrmModificarLin_ped::PonerCampos(int ID_Lin)
{
    this->nId = ID_Lin;
    QSqlQuery *Mod_linPed = new QSqlQuery(QSqlDatabase::database("empresa"));
    Configuracion *o_config = new Configuracion();
    o_config->CargarDatos();
    Mod_linPed->prepare("Select * from lin_ped where id = :nID");
    Mod_linPed->bindValue(":nID",this->nId);
    if(Mod_linPed->exec()){
        QSqlRecord record = Mod_linPed->record();
        Mod_linPed->next();
        record = Mod_linPed->record();
        ui->txtCodigo->setText(record.field("cCodigo").value().toString());
        ui->txtCantidad->setText(record.field("nCantidad").value().toString());
        ui->txtDescripcion->setText(record.field("cDescripcion").value().toString());
        ui->txtPVP->setText(o_config->FormatoNumerico(record.field("rPvp").value().toString()));
        ui->txtPorcDto->setText(record.field("nDto").value().toString());
        ui->txtDTO->setText(o_config->FormatoNumerico(record.field("rDto").value().toString()));
        ui->txtSubTotal->setText(o_config->FormatoNumerico(record.field("rSubTotal").value().toString()));
        ui->txtTotal->setText(o_config->FormatoNumerico(record.field("rTotal").value().toString()));
        ui->txtPorcIVA->setText(o_config->FormatoNumerico(record.field("nPorcIva").value().toString()));
    }
}
void FrmModificarLin_ped::CalcularTotal()
{
    Configuracion *o_config = new Configuracion();
    // Calculo totales línea
    double impDto,impTot,impSubtotal;
    impSubtotal = (ui->txtCantidad->text().replace(".","").toDouble() * ui->txtPVP->text().replace(".","").toDouble());
    ui->txtSubTotal->setText(o_config->FormatoNumerico(QString::number(impSubtotal,'f',2)));
    impDto = (ui->txtSubTotal->text().replace(".","").toDouble() * ui->txtPorcDto->text().toDouble())/100 ;
    ui->txtDTO->setText(o_config->FormatoNumerico(QString::number(impDto,'f',2)));
    impTot = ui->txtSubTotal->text().replace(".","").toDouble() - ui->txtDTO->text().replace(".","").toDouble();
    ui->txtTotal->setText(o_config->FormatoNumerico(QString::number(impTot,'f',2)));
    delete o_config;
}


void FrmModificarLin_ped::on_txtCantidad_editingFinished()
{
     CalcularTotal();
}

void FrmModificarLin_ped::on_txtPVP_editingFinished()
{
     CalcularTotal();
}

void FrmModificarLin_ped::on_txtPorcDto_editingFinished()
{
     CalcularTotal();
}
