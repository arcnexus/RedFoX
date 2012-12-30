#include "frmanalitica.h"
#include "ui_frmanalitica.h"
#include <QSql>
#include<QSqlQueryModel>
#include "frmanalitica2.h"

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
    connect(this,SIGNAL(pasartipo(QString)),frmanalitica2,SLOT(cargarDatos(QString)));
    emit pasartipo(ui->cboTipoAnalitica->currentText());
    frmanalitica2->show();

}
