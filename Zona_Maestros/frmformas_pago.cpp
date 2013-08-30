#include "frmformas_pago.h"
#include "ui_frmformas_pago.h"
#include "vencimientos.h"

FrmFormas_pago::FrmFormas_pago(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmFormas_pago),
  toolButton(tr("Formas de\n Pago"),":/Icons/PNG/fpagos.png",this),
  menuButton(QIcon(":/Icons/PNG/fpagos.png"),tr("Formas de Pago"),this),
  push(new QPushButton(QIcon(":/Icons/PNG/transport.png"),"",this))

{
    ui->setupUi(this);
    m=  new QSqlQueryModel(this);
    m->setQuery("select id, codigo, forma_pago, dia_pago1, dia_pago2, dia_pago3, dia_pago4, dias_entre_plazos, "
                "numero_plazos, cuenta_cont_pago  from formpago order by codigo",
                Configuracion_global->groupDB);
    ui->tabla_buscar->setModel(m);
    QStringList orden,modo;
    orden <<  tr("Código") << tr("Descripción");
    modo << tr("Z-A") <<tr("A-Z");
    ui->cboOrden->addItems(orden);
    ui->cboModo->addItems(modo);
    ui->stackedWidget->setCurrentIndex(0);
    ui->txtBuscar->setFocus();

}

FrmFormas_pago::~FrmFormas_pago()
{
    delete ui;
}
