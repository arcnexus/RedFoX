#include "frmanadirdiagnostico.h"
#include "ui_frmanadirdiagnostico.h"

FrmAnadirDiagnostico::FrmAnadirDiagnostico(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAnadirDiagnostico)
{
    ui->setupUi(this);
    connect(ui->btnBuscar,SIGNAL(clicked()),this,SLOT(BtnBuscar_Clicked()));
}

FrmAnadirDiagnostico::~FrmAnadirDiagnostico()
{
    delete ui;
}

void FrmAnadirDiagnostico::BtnBuscar_Clicked()
{
    QSqlQueryModel *modelDiagnostico = new QSqlQueryModel(this);
    modelDiagnostico->setQuery("Select sindrome,id from sindromes where sindrome like '%"+ui->txtBusar->text().trimmed()+
                               "%' order by sindrome",QSqlDatabase::database("dbinfomed"));
    ui->listaDiagnositicos->setModel(modelDiagnostico);

}
