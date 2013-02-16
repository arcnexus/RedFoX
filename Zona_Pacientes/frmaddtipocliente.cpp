#include "frmaddtipocliente.h"
#include "ui_frmaddtipocliente.h"
//#include "/Auxiliares/Globlal_Include.h"
#include  "QtSql"
#include "QSqlDatabase"

FrmAddTipoCliente::FrmAddTipoCliente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAddTipoCliente)
{
    ui->setupUi(this);
    //----------------------
    // CARGAR LISTA/TABLA
    //----------------------
    QSqlQueryModel *m_familia = new QSqlQueryModel(this);
    m_familia->setQuery("select descripcion from maestro_familia_cliente",QSqlDatabase::database("terra"));
    ui->list_Familia->setModel(m_familia);
}

FrmAddTipoCliente::~FrmAddTipoCliente()
{
    delete ui;
}

void FrmAddTipoCliente::LLenarTablaSubfamilias(QString cFamilia)
{

}
