#include "frmtipocliente.h"
#include "ui_frmtipocliente.h"

frmTipoCliente::frmTipoCliente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmTipoCliente)
{
    ui->setupUi(this);

    model_tipos    = new QSqlTableModel(this,Configuracion_global->groupDB);
    model_Subtipos = new QSqlTableModel(this,Configuracion_global->groupDB);

    model_tipos   ->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_Subtipos->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model_tipos   ->setTable("tipocliente_def");
    model_Subtipos->setTable("tiposubcliente_def");

    model_tipos->select();

    ui->lista_tipos   ->setModel(model_tipos);
    ui->lista_subtipos->setModel(model_Subtipos);

    ui->lista_tipos   ->setModelColumn(1);
    ui->lista_subtipos->setModelColumn(2);

    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(close()));
}

frmTipoCliente::~frmTipoCliente()
{
    delete ui;
}

void frmTipoCliente::on_lista_tipos_clicked(const QModelIndex &index)
{
    model_Subtipos->setFilter(QString("id_tipocliente = %1").arg(model_tipos->record(index.row()).value("id").toInt()));
    model_Subtipos->select();

    ui->txtNombre->setText(model_tipos->record(index.row()).value("nombre").toString());
    ui->txtDesc->setText(model_tipos->record(index.row()).value("desc").toString());

    edit_tipo = true;
}

void frmTipoCliente::on_lista_subtipos_clicked(const QModelIndex &index)
{
    ui->txtNombre->setText(model_Subtipos->record(index.row()).value("nombre").toString());
    ui->txtDesc->setText(model_Subtipos->record(index.row()).value("desc").toString());

    edit_tipo = false;
}

void frmTipoCliente::on_btnAddTipo_clicked()
{
    if(ui->txtNombre->text().isEmpty() || ui->txtDesc->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Campos vacios"),tr("Por favor, rellene los datos antes de añadir"));
        return;
    }

    int row = model_tipos->rowCount();
    if(model_tipos->insertRow(row))
    {
        model_tipos->setData(model_tipos->index(row,1),ui->txtNombre->text());
        model_tipos->setData(model_tipos->index(row,2),ui->txtDesc->text());
        if(model_tipos->submitAll())
            TimedMessageBox * t = new TimedMessageBox(this,tr("Tipo cliente insertado con éxito"));
        else
            QMessageBox::critical(this,tr("Error al añadir tipo cliente"),model_tipos->lastError().text());
    }
    else
        QMessageBox::critical(this,tr("Error al añadir tipo cliente"),model_tipos->lastError().text());
}

void frmTipoCliente::on_btnAddSubTipo_clicked()
{
    if(ui->txtNombre->text().isEmpty() || ui->txtDesc->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Campos vacios"),tr("Por favor, rellene los datos antes de añadir"));
        return;
    }
    if(!ui->lista_tipos->currentIndex().isValid())
    {
        QMessageBox::warning(this,tr("Seleccione tipo"),tr("Por favor, selleccione el tipo de cliente\nal que desea añadir un subtipo."));
        return;
    }
    int id_tipo = model_tipos->record(ui->lista_tipos->currentIndex().row()).value("id").toInt();

    int row = model_Subtipos->rowCount();
    if(model_Subtipos->insertRow(row))
    {
        model_Subtipos->setData(model_Subtipos->index(row,1),id_tipo);
        model_Subtipos->setData(model_Subtipos->index(row,2),ui->txtNombre->text());
        model_Subtipos->setData(model_Subtipos->index(row,3),ui->txtDesc->text());
        if(model_Subtipos->submitAll())
            TimedMessageBox * t = new TimedMessageBox(this,tr("Subtipo cliente insertado con éxito"));
        else
            QMessageBox::critical(this,tr("Error al añadir subtipo cliente"),model_Subtipos->lastError().text());
    }
    else
        QMessageBox::critical(this,tr("Error al añadir subtipo cliente"),model_Subtipos->lastError().text());
}

void frmTipoCliente::on_btnEditarTipo_clicked()
{
    if(!ui->lista_tipos->currentIndex().isValid())
    {
        QMessageBox::warning(this,tr("Seleccione tipo"),tr("Por favor, selleccione el tipo de cliente\nque desea editar."));
        return;
    }
    int row = ui->lista_tipos->currentIndex().row();

    model_tipos->setData(model_tipos->index(row,1),ui->txtNombre->text());
    model_tipos->setData(model_tipos->index(row,2),ui->txtDesc->text());
    if(model_tipos->submitAll())
        TimedMessageBox * t = new TimedMessageBox(this,tr("Tipo cliente actualizado con éxito"));
    else
        QMessageBox::critical(this,tr("Error al actualizar tipo cliente"),model_tipos->lastError().text());
}

void frmTipoCliente::on_btnBorrarTipo_clicked()
{
    if(!ui->lista_tipos->currentIndex().isValid())
    {
        QMessageBox::warning(this,tr("Seleccione tipo"),tr("Por favor, selleccione el tipo de cliente\nque desea borrar."));
        return;
    }

    int row = ui->lista_tipos->currentIndex().row();
    int id_tipo = model_tipos->record(row).value("id").toInt();
    if(model_tipos->removeRow(row))
    {
        if(model_tipos->submitAll())
        {
            QString error;
            if(SqlCalls::SqlDelete("tiposubcliente_def",Configuracion_global->groupDB,QString("id_tipocliente = %1").arg(id_tipo),error))
            {
                model_tipos->select();
                model_Subtipos->select();
                TimedMessageBox * t = new TimedMessageBox(this,tr("Tipo cliente borrado con éxito"));
            }
            else
                QMessageBox::critical(this,tr("Error al borrar tipo cliente: subtipos"),error);
        }
        else
            QMessageBox::critical(this,tr("Error al borrar tipo cliente"),model_tipos->lastError().text());
    }
    else
        QMessageBox::critical(this,tr("Error al borrar tipo cliente"),model_tipos->lastError().text());
}

void frmTipoCliente::on_btnEditarSubTipo_clicked()
{
    if(!ui->lista_subtipos->currentIndex().isValid())
    {
        QMessageBox::warning(this,tr("Seleccione subtipo"),tr("Por favor, selleccione el subtipo de cliente\nque desea editar."));
        return;
    }
    int row = ui->lista_subtipos->currentIndex().row();

    model_Subtipos->setData(model_Subtipos->index(row,3),ui->txtNombre->text());
    model_Subtipos->setData(model_Subtipos->index(row,3),ui->txtDesc->text());
    if(model_Subtipos->submitAll())
        TimedMessageBox * t = new TimedMessageBox(this,tr("Tipo cliente actualizado con éxito"));
    else
        QMessageBox::critical(this,tr("Error al actualizar tipo cliente"),model_Subtipos->lastError().text());
}

void frmTipoCliente::on_btnBorrarSubTipo_clicked()
{
    if(!ui->lista_subtipos->currentIndex().isValid())
    {
        QMessageBox::warning(this,tr("Seleccione tipo"),tr("Por favor, selleccione el subtipo de cliente\nque desea borrar."));
        return;
    }

    int row = ui->lista_subtipos->currentIndex().row();
    if(model_Subtipos->removeRow(row))
    {
        if(model_Subtipos->submitAll())
        {
            model_Subtipos->select();
            TimedMessageBox * t = new TimedMessageBox(this,tr("Tipo cliente borrado con éxito"));
        }
        else
            QMessageBox::critical(this,tr("Error al borrar tipo cliente"),model_Subtipos->lastError().text());
    }
    else
        QMessageBox::critical(this,tr("Error al borrar tipo cliente"),model_Subtipos->lastError().text());
}
