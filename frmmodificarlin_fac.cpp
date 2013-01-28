#include "frmmodificarlin_fac.h"
#include "ui_frmmodificarlin_fac.h"

#include  "articulo.h"
#include "factura.h"


frmModificarLin_fac::frmModificarLin_fac(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmModificarLin_fac)
{
    ui->setupUi(this);
}

frmModificarLin_fac::~frmModificarLin_fac()
{
    delete ui;
}

void frmModificarLin_fac::ponerCampos(int ID_Lin)
{
    this->nId = ID_Lin;
    QSqlQuery *Mod_linFac = new QSqlQuery(QSqlDatabase::database("empresa"));


    Mod_linFac->prepare("Select * from lin_fac where id = :nID");
    Mod_linFac->bindValue(":nID",this->nId);
    if(Mod_linFac->exec()){
        QSqlRecord record = Mod_linFac->record();
        Mod_linFac->next();
        record = Mod_linFac->record();
        ui->txtCodigo->setText(record.field("cCodigo").value().toString());
        ui->txtCantidad->setText(record.field("nCantidad").value().toString());
        ui->txtDescripcion->setText(record.field("cDescripcion").value().toString());
        ui->txtPVP->setText(Configuracion_global->FormatoNumerico(record.field("rPvp").value().toString()));
        ui->txtPorcDto->setText(record.field("nDto").value().toString());
        ui->txtDTO->setText(Configuracion_global->FormatoNumerico(record.field("rDto").value().toString()));
        ui->txtSubTotal->setText(Configuracion_global->FormatoNumerico(record.field("rSubTotal").value().toString()));
        ui->txtTotal->setText(Configuracion_global->FormatoNumerico(record.field("rTotal").value().toString()));
        ui->txtPorcIVA->setText(Configuracion_global->FormatoNumerico(record.field("nPorcIva").value().toString()));

    }
    delete Mod_linFac;
    ui->txtCodigo->setFocus();
}

void frmModificarLin_fac::CalcularTotal()
{

    // Calculo totales línea
    double impDto,impTot,impSubtotal;
    impSubtotal = (ui->txtCantidad->text().replace(".","").toDouble() * ui->txtPVP->text().replace(".","").toDouble());
    ui->txtSubTotal->setText(Configuracion_global->FormatoNumerico(QString::number(impSubtotal,'f',2)));
    impDto = (ui->txtSubTotal->text().replace(".","").toDouble() * ui->txtPorcDto->text().toDouble())/100 ;
    ui->txtDTO->setText(Configuracion_global->FormatoNumerico(QString::number(impDto,'f',2)));
    impTot = ui->txtSubTotal->text().replace(".","").toDouble() - ui->txtDTO->text().replace(".","").toDouble();
    ui->txtTotal->setText(Configuracion_global->FormatoNumerico(QString::number(impTot,'f',2)));

}

void frmModificarLin_fac::on_txtCodigo_lostFocus()
{
    if (!ui->txtCodigo->text().isEmpty()) {
        if (ui->txtDescripcion->text().isEmpty()) {
            Articulo *oArt =  new Articulo();

            oArt->Recuperar("Select * from articulos where cCodigo = '"+ui->txtCodigo->text()+"'");
            ui->txtCodigo->setText(oArt->getcCodigo());
            ui->txtDescripcion->setText(oArt->getcDescripcion());
            ui->txtPVP->setText(Configuracion_global->FormatoNumerico(QString::number(oArt->getrTarifa1(),'f',2)));
            ui->txtSubTotal->setText(Configuracion_global->FormatoNumerico(QString::number(oArt->getrTarifa1(),'f',2)));
            if (oArt->getrDto() > ui->txtPorcDto->text().toDouble()) {
                ui->txtPorcDto->setText(Configuracion_global->FormatoNumerico(QString::number(oArt->getrDto(),'f',0)));
            }
            ui->txtPorcIVA->setText(QString::number(oArt->getnTipoIva(),'f',0));
            delete oArt;
        }
        CalcularTotal();
    }

}

void frmModificarLin_fac::on_txtCantidad_lostFocus()
{
    CalcularTotal();
}

void frmModificarLin_fac::on_txtPVP_lostFocus()
{
    CalcularTotal();
}

void frmModificarLin_fac::on_txtPorcDto_lostFocus()
{
    CalcularTotal();
}

void frmModificarLin_fac::on_pushButton_clicked()
{
    Factura *oFactura = new Factura();
    oFactura->ModificarLineaFactura(this->nId,ui->txtCodigo->text(),ui->txtCantidad->text().toDouble(),
                                 ui->txtDescripcion->text(),ui->txtPVP->text().replace(".","").toDouble(),
                                 ui->txtSubTotal->text().replace(".","").toDouble(),ui->txtPorcDto->text().toDouble(),
                                 ui->txtDTO->text().replace(".","").toDouble(),ui->txtTotal->text().replace(".","").toDouble(),
                                 ui->txtPorcIVA->text().toDouble());
    delete oFactura;
}
