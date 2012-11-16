#include "frmanadirmedicamento.h"
#include "ui_frmanadirmedicamento.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QXmlStreamReader>
#include <QtSql>
#include "vademecum.h"

FrmAnadirMedicamento::FrmAnadirMedicamento(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmAnadirMedicamento)
{
    ui->setupUi(this);
}

FrmAnadirMedicamento::~FrmAnadirMedicamento()
{
    delete ui;
}

void FrmAnadirMedicamento::on_pushButton_clicked()
{
    vademecum *vConexionVademecum = new vademecum();
    QString cURL = "http://demo.vademecumdata.es/vweb/xml/ws_drug/SearchByName?value="+
            ui->txtBuscar->text().trimmed();
    QString cResultado;
    cResultado = vConexionVademecum->recuperar(cURL);
    ui->textEdit->setText(cResultado);
    delete vConexionVademecum;

}


