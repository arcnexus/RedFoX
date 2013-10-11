#include "frmcambiarusuario.h"
#include "ui_frmcambiarusuario.h"


frmCambiarUsuario::frmCambiarUsuario(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmCambiarUsuario)
{
    ui->setupUi(this);
    QSqlQueryModel *users = new QSqlQueryModel(this);
    users->setQuery("select nombre from usuarios",Configuracion_global->groupDB);
    ui->cboUsers->setModel(users);

}

frmCambiarUsuario::~frmCambiarUsuario()
{
    delete ui;
}
