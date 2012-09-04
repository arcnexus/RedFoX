#include "frmdecision.h"
#include "ui_frmdecision.h"

frmDecision::frmDecision(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmDecision)
{
    ui->setupUi(this);
}

frmDecision::~frmDecision()
{
    delete ui;
}

void frmDecision::setLinea1(QString linea1) {
    ui->linea1->setText(linea1);
}

void frmDecision::setLinea2(QString linea2) {
    ui->linea2->setText(linea2);

}

void frmDecision::setLinea3(QString linea3) {
    ui->linea3->setText(linea3);
}

void frmDecision::setBoton1(QString Boton1) {
    ui->btnAceptar->setText(Boton1);
}

void frmDecision::setBoton2(QString Boton2) {
    ui->btnCancelar->setText(Boton2);
}


void frmDecision::Inicializar(QString titulo, QString linea1, QString linea2, QString linea3, QString Boton1, QString Boton2){

    ui->linea1->setText(linea1);
    ui->linea2->setText(linea2);
    ui->linea3->setText(linea3);
    ui->btnAceptar->setText(Boton1);
    ui->btnCancelar->setText(Boton2);
    this->setWindowTitle(titulo);
}


