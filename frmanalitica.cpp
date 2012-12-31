#include "frmanalitica.h"
#include "ui_frmanalitica.h"
#include <QSql>
#include<QSqlQueryModel>
#include "frmanalitica2.h"
#include <QMessageBox>

FrmAnalitica::FrmAnalitica(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAnalitica)
{
    ui->setupUi(this);
    /*----------------------------------------
     * CONEXIONES
     *--------------------------------------*/
    connect(ui->btnCerrar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->btnAnadirCampos,SIGNAL(clicked()),this,SLOT(AnadirCamposAnalitica()));
    /* -----------------------------------
     * RELLENO COMBO
     *------------------------------------*/
    QSqlQueryModel *qTipos = new QSqlQueryModel(this);
    qTipos->setQuery("select tipoanalisis from tiposanalitica",QSqlDatabase::database("dbmedica"));
    ui->cboTipoAnalitica->setModel(qTipos);
}

FrmAnalitica::~FrmAnalitica()
{
    delete ui;
}

void FrmAnalitica::AnadirCamposAnalitica()
{
    Frmanalitica2 *frmanalitica2 = new Frmanalitica2(this);
    connect(this,SIGNAL(pasartipo(QString,int)),frmanalitica2,SLOT(cargarDatos(QString,int)));
    emit pasartipo(ui->cboTipoAnalitica->currentText(),idanalitica);
    if(frmanalitica2->exec() == QDialog::Accepted)
        QMessageBox::information(this,"prova","Acceptat","OK");
    else
        QMessageBox::information(this,"prova","Cancelled","OK");
}
