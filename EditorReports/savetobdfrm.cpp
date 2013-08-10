#include "savetobdfrm.h"
#include "ui_savetobdfrm.h"
#include <QSqlTableModel>
#include "../Auxiliares/Globlal_Include.h"
SaveToBDFrm::SaveToBDFrm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveToBDFrm)
{
    grad = 0;
    ui->setupUi(this);
    QSqlTableModel* model = new QSqlTableModel(this,Configuracion_global->empresaDB);
    model->setTable("report");
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(3,true);

    QStringList headers;
    headers << "" << tr("Nombre") << tr("Descripcion")<< "" << tr("Grado");
    for (int i = 0; i< headers.size();i++)
        model->setHeaderData(i, Qt::Horizontal, headers.at(i));
    ui->tableView->resizeColumnsToContents();

    connect(ui->tableView->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(currentRowChanged(QModelIndex,QModelIndex)));
    connect(ui->btnAcept,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

SaveToBDFrm::~SaveToBDFrm()
{
    delete ui;
}

void SaveToBDFrm::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    int r = current.row();
    ui->txtNombre->setText(ui->tableView->model()->index(r,1).data().toString());
    ui->txtDesc->setText(ui->tableView->model()->index(r,2).data().toString());
}


void SaveToBDFrm::on_txtNombre_textChanged(const QString &arg1)
{
    nombre = arg1;
}

void SaveToBDFrm::on_txtDesc_textChanged(const QString &arg1)
{
    desc = arg1;
}

void SaveToBDFrm::on_spinGrd_valueChanged(int arg1)
{
    grad = arg1;
}
