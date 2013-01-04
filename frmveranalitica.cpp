#include "frmveranalitica.h"
#include "ui_frmveranalitica.h"

FrmVerAnalitica::FrmVerAnalitica(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmVerAnalitica)
{
    ui->setupUi(this);
    /*---------------------------------------
     *Conexiones
     *-------------------------------------*/
    connect(ui->btnCerrar,SIGNAL(clicked()),this,SLOT(close()));
}

FrmVerAnalitica::~FrmVerAnalitica()
{
    delete ui;
}
