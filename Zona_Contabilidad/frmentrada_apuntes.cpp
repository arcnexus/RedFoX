#include "frmentrada_apuntes.h"
#include "ui_frmentrada_apuntes.h"

FrmEntrada_apuntes::FrmEntrada_apuntes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmEntrada_apuntes)
{
    ui->setupUi(this);
}

FrmEntrada_apuntes::~FrmEntrada_apuntes()
{
    delete ui;
}
