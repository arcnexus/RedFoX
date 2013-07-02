#include "frmcuadro_cuentas.h"
#include "ui_frmcuadro_cuentas.h"
#include "../Auxiliares/monetarydelegate.h"


frmCuadro_cuentas::frmCuadro_cuentas(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmCuadro_cuentas),
    toolButton(tr("Cuentas\nP.G.C."),":/Icons/PNG/cuentas.png",this),
    menuButton(QIcon(":/Icons/PNG/cuentas.png"),tr("Cuentas P.G.C."),this),
    push(new QPushButton(QIcon(":/Icons/PNG/cuentas.png"),"",this))
{
    ui->setupUi(this);
    cuentas = new Cuentas_contables(this);
    llenar_tabla();
    ui->blinkink->setVisible(false);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión del P.G.C"));


}

frmCuadro_cuentas::~frmCuadro_cuentas()
{
    delete ui;
}

void frmCuadro_cuentas::on_btnAnadir_clicked()
{
    cuentas->clear();
    llenar_campos();
    ui->txtcodigo_cuenta->setFocus();
    activar_controles(true);
    this->anadir = true;


}

void frmCuadro_cuentas::llenar_campos()
{
    ui->rad_activo->setChecked(cuentas->activo);
    ui->txtcodigo_balance->setText(cuentas->codigo_balance);
    ui->txtcodigo_cuenta->setText(cuentas->codigo_cta);
    ui->txtdescripcion_cuenta->setText(cuentas->descripcion);
    ui->txtDesglose_balance->setText(cuentas->desglose_balance);
    ui->txtsaldo_cta->setText(Configuracion_global->toFormatoMoneda(QString::number(cuentas->saldo)));
}

void frmCuadro_cuentas::llenar_objeto()
{
    cuentas->activo = ui->rad_activo->isChecked();
    cuentas->codigo_balance = ui->txtcodigo_balance->text();
    cuentas->codigo_cta = ui->txtcodigo_cuenta->text();
    cuentas->descripcion = ui->txtdescripcion_cuenta->text();
    cuentas->desglose_balance = ui->txtDesglose_balance->text();
    cuentas->saldo = ui->txtsaldo_cta->text().replace(",",".").toDouble();
}

void frmCuadro_cuentas::activar_controles(bool state)
{
    ui->txtcodigo_balance->setEnabled(state);
    ui->txtcodigo_cuenta->setEnabled(state);
    ui->txtcuadro_buscar->setEnabled(!state);
    ui->txtdescripcion_cuenta->setEnabled(state);
    ui->txtDesglose_balance->setEnabled(state);
    ui->txtsaldo_cta->setEnabled(state);
    ui->rad_activo->setEnabled(state);
    ui->rad_pasivo->setEnabled(state);
    ui->btnAnadir->setEnabled(!state);
    ui->btnBorrar->setEnabled(!state);
    ui->btnBuscar->setEnabled(!state);
    ui->btndeshacer->setEnabled(state);
    ui->btnEditar->setEnabled(!state);
    ui->btnguardar->setEnabled(state);
    ui->btnCodigoBalance->setEnabled(state);
}

void frmCuadro_cuentas::on_btnguardar_clicked()
{
    if(ui->txtcodigo_balance->text().isEmpty() || ui->txtcodigo_cuenta->text().isEmpty() || ui->txtdescripcion_cuenta->text().isEmpty() ||
            ui->txtDesglose_balance->text().isEmpty())
    {
        ui->blinkink->setVisible(true);
        ui->blinkink->setblink(true);
    } else
    {
        activar_controles(false);
        ui->blinkink->setVisible(false);
        ui->blinkink->setblink(false);
        llenar_objeto();
        if (this->anadir)
        {
            this->anadir = false;
            cuentas->anadir_cuenta();
        } else
        {
            cuentas->guardar_cuenta();
        }
        llenar_tabla();
    }
}

void frmCuadro_cuentas::llenar_tabla()
{
    QSqlQueryModel *model_cuentas = new QSqlQueryModel(this);
    model_cuentas->setQuery("select id, codigo_cta, descripcion, saldo from plan_general",QSqlDatabase::database("dbconta"));
    ui->tabla_cuentas->setModel(model_cuentas);
    ui->tabla_cuentas->setColumnHidden(0,true);
    ui->tabla_cuentas->setColumnWidth(1,100);
    ui->tabla_cuentas->setColumnWidth(2,160);
    ui->tabla_cuentas->setColumnWidth(3,100);
    ui->tabla_cuentas->setItemDelegateForColumn(3,new MonetaryDelegate);
    QStringList cabecera;
    cabecera << tr("id") <<tr("Código Cta.") <<tr("Descripción") <<tr("Saldo");
    for (int pos=1;pos <cabecera.size();pos++)
    {
        model_cuentas->setHeaderData(pos,Qt::Horizontal,cabecera.at(pos));
    }
}
