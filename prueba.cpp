#include "prueba.h"
#include "ui_prueba.h"

Prueba::Prueba(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Prueba)
{
    ui->setupUi(this);
}

Prueba::~Prueba()
{
    delete ui;
}
