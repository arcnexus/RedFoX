#include "frmmodificarlin_precli.h"
#include "ui_frmmodificarlin_precli.h"
#include "presupuesto.h"
#include "articulo.h"



FrmModificarLin_precli::FrmModificarLin_precli(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmModificarLin_precli)
{
    ui->setupUi(this);
}

FrmModificarLin_precli::~FrmModificarLin_precli()
{
    delete ui;
}

void FrmModificarLin_precli::on_pushButton_clicked()
{
    Presupuesto *oPresup = new Presupuesto();
    oPresup->ModificarLineaPresupuesto(this->nId,ui->txtCodigo->text(),ui->txtCantidad->text().toDouble(),
                                 ui->txtDescripcion->text(),ui->txtPVP->text().replace(".","").toDouble(),
                                 ui->txtSubTotal->text().replace(".","").toDouble(),ui->txtPorcDto->text().toDouble(),
                                 ui->txtDTO->text().replace(".","").toDouble(),ui->txtTotal->text().replace(".","").toDouble(),
                                 ui->txtPorcIVA->text().toDouble());

    delete oPresup;
}

void FrmModificarLin_precli::on_txtCodigo_editingFinished()
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

void FrmModificarLin_precli::PonerCampos(int ID_Lin)
{
    this->nId = ID_Lin;
    QSqlQuery *Mod_lin_pre = new QSqlQuery(QSqlDatabase::database("empresa"));

    Configuracion_global->CargarDatos();
    Mod_lin_pre->prepare("Select * from lin_pre where id = :nID");
    Mod_lin_pre->bindValue(":nID",this->nId);
    if(Mod_lin_pre->exec()){
        QSqlRecord record = Mod_lin_pre->record();
        Mod_lin_pre->next();
        record = Mod_lin_pre->record();
        ui->txtCodigo->setText(record.field("cCodigo").value().toString());
        ui->txtCantidad->setText(record.field("nCantidad").value().toString());
        ui->txtDescripcion->setText(record.field("cDescripcion").value().toString());
        ui->txtPVP->setText(Configuracion_global->FormatoNumerico(record.field("rPvp").value().toString()));
        ui->txtPorcDto->setText(record.field("nPorcDto").value().toString());
        ui->txtDTO->setText(Configuracion_global->FormatoNumerico(record.field("rDto").value().toString()));
        ui->txtSubTotal->setText(Configuracion_global->FormatoNumerico(record.field("rSubTotal").value().toString()));
        ui->txtTotal->setText(Configuracion_global->FormatoNumerico(record.field("rTotal").value().toString()));
        ui->txtPorcIVA->setText(Configuracion_global->FormatoNumerico(record.field("nPorcIva").value().toString()));
    }
    delete Mod_lin_pre;
}

void FrmModificarLin_precli::CalcularTotal()
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

void FrmModificarLin_precli::on_txtCantidad_editingFinished()
{
    CalcularTotal();
}

void FrmModificarLin_precli::on_txtPVP_editingFinished()
{
    CalcularTotal();
}

void FrmModificarLin_precli::on_txtPorcDto_editingFinished()
{
    CalcularTotal();
}
