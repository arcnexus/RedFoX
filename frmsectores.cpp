#include "frmsectores.h"
#include "ui_frmsectores.h"

FrmSectores::FrmSectores(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmSectores)
{
    ui->setupUi(this);
}

FrmSectores::~FrmSectores()
{
    delete ui;
}
