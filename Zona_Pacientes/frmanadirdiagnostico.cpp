#include "frmanadirdiagnostico.h"
#include "ui_frmanadirdiagnostico.h"

FrmAnadirDiagnostico::FrmAnadirDiagnostico(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAnadirDiagnostico)
{
    ui->setupUi(this);
    connect(ui->btnBuscar,SIGNAL(clicked()),this,SLOT(BtnBuscar_Clicked()));
    connect(ui->listaDiagnositicos,SIGNAL(clicked(QModelIndex)),this,SLOT(SeleccionLista(QModelIndex)));
}

FrmAnadirDiagnostico::~FrmAnadirDiagnostico()
{
    delete ui;
}

void FrmAnadirDiagnostico::BtnBuscar_Clicked()
{

    Configuracion_global->AbridBDMediTec();
    QSqlQueryModel *modelDiagnostico = new QSqlQueryModel(this);
    if(ui->chkMed_China->isChecked() && !ui->chkMed_Occidental->isChecked())
        modelDiagnostico->setQuery("Select sindrome,id from sindromes where sindrome like '%"+ui->txtBusar->text().trimmed()+
                               "%' and medicina = 'C' order by sindrome",QSqlDatabase::database("db_meditec"));
    if(!ui->chkMed_China->isChecked() && ui->chkMed_Occidental->isChecked())
        modelDiagnostico->setQuery("Select sindrome,id from sindromes where sindrome like '%"+ui->txtBusar->text().trimmed()+
                               "%' and medicina = 'A' order by sindrome",QSqlDatabase::database("db_meditec"));
    if(ui->chkMed_China->isChecked() && ui->chkMed_Occidental->isChecked())
        modelDiagnostico->setQuery("Select sindrome,id from sindromes where sindrome like '%"+ui->txtBusar->text().trimmed()+
                               "%' order by sindrome",QSqlDatabase::database("db_meditec"));
    if(!ui->chkMed_China->isChecked() && !ui->chkMed_Occidental->isChecked())
        QMessageBox::warning(this,tr("Busqueda de Enfermedades /Sindromes"),
                             tr("Al menos debe seleccionar una disciplina médica para poder introducir diagnosticos desde la BD"),
                             tr("Aceptar"));

    ui->listaDiagnositicos->setModel(modelDiagnostico);
    ui->listaDiagnositicos->setColumnHidden(1,true);
    ui->listaDiagnositicos->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    ui->listaDiagnositicos->horizontalHeader()->resizeSection(0,350);
    Configuracion_global->CerrarBDMediTec();


}

void FrmAnadirDiagnostico::SeleccionLista(QModelIndex index)
{
    this->id = ui->listaDiagnositicos->model()->data(ui->listaDiagnositicos->model()->index(index.row(),1),Qt::EditRole).toInt();
}
