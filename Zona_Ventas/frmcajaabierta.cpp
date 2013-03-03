#include "frmcajaabierta.h"
#include "ui_frmcajaabierta.h"

Frmcajaabierta::Frmcajaabierta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Frmcajaabierta)
{
    ui->setupUi(this);
}

Frmcajaabierta::~Frmcajaabierta()
{
    delete ui;
}
