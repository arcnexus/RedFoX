#include "frmmtcbase.h"
#include "ui_frmmtcbase.h"

FrmMTCbase::FrmMTCbase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmMTCbase)
{
    ui->setupUi(this);

}

FrmMTCbase::~FrmMTCbase()
{
    delete ui;
}

void FrmMTCbase::cargarSindromes(int id)
{
    QSqlQueryModel *modelSindromes = new QSqlQueryModel(this);
    modelSindromes->setQuery("select id,sindrome from sindromes where id_visita = "+QString::number(id),
                             QSqlDatabase::database("dbmedica"));

    ui->tabla_Enf_sindromes->setModel(modelSindromes);
    ui->tabla_Enf_sindromes->setColumnWidth(0,0);
    ui->tabla_Enf_sindromes->setColumnWidth(1,250);
}
