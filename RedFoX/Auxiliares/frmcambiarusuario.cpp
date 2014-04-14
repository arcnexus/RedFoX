#include "frmcambiarusuario.h"
#include "ui_frmcambiarusuario.h"


frmCambiarUsuario::frmCambiarUsuario(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmCambiarUsuario)
{
    ui->setupUi(this);
    ui->cboUsers->setModel(Configuracion_global->usuarios_model);
}

frmCambiarUsuario::~frmCambiarUsuario()
{
    delete ui;
}
