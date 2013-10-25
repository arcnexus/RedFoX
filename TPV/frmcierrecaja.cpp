#include "frmcierrecaja.h"
#include "ui_frmcierrecaja.h"

FrmCierreCaja::FrmCierreCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmCierreCaja)
{
    ui->setupUi(this);
    QSqlQueryModel *users = new QSqlQueryModel(this);
    users->setQuery("select nombre from usuarios",Configuracion_global->groupDB);
    ui->cboUsuarioCierre->setModel(users);

}

FrmCierreCaja::~FrmCierreCaja()
{
    delete ui;
}
