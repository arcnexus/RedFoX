#include "frmaddtipocliente.h"
#include "ui_frmaddtipocliente.h"
#include "Auxiliares/Globlal_Include.h"
#include  "QtSql"
#include "QSqlDatabase"
#include <QMessageBox>

void FrmAddTipoCliente::updateListas()
{
    ui->list_tipo->blockSignals(true);
    ui->list_subTipo->blockSignals(true);

    ui->list_tipo->clear();

    model_tipos->select();
    model_SubTipos->select();
    model_cliente->select();

    for(auto i=0;i < model_tipos->rowCount();i++)
    {
        QListWidgetItem* item = new QListWidgetItem(model_tipos->record(i).value("nombre").toString());

        int id_tipo = model_tipos->record(i).value("id").toInt();
        bool sel = false;
        auto sub_count = 0;

        for(auto c = 0; c<model_SubTipos->rowCount(); c++)
        {
            if(model_SubTipos->record(c).value("id_tipocliente").toInt() == id_tipo)
                sub_count++;
        }

        auto sub_selected = 0;
        for(auto z = 0; z<model_cliente->rowCount();z++)
        {
            if(model_cliente->record(z).value("id_tipo_cliente").toInt() == id_tipo)
            {
                sel = true;
                sub_selected++;
            }
        }
        bool full = sub_count == sub_selected;
        Qt::CheckState state = sel ?  Qt::PartiallyChecked : Qt::Unchecked;
        if(full)
            state = Qt::Checked;
        item->setCheckState(state);
        item->setData(Qt::UserRole + 1,id_tipo);
        item->setToolTip(model_tipos->record(i).value("desc").toString());
        ui->list_tipo->addItem(item);
    }

    ui->list_tipo->blockSignals(false);
    ui->list_subTipo->blockSignals(false);
}

FrmAddTipoCliente::FrmAddTipoCliente(QWidget *parent, int id_cliente) :
    QDialog(parent),
    ui(new Ui::FrmAddTipoCliente)
{
    this->id_cliente = id_cliente;
    ui->setupUi(this);

    model_tipos = new QSqlTableModel(this,Configuracion_global->groupDB);
    model_SubTipos = new QSqlTableModel(this,Configuracion_global->groupDB);
    model_cliente = new QSqlTableModel(this,Configuracion_global->groupDB);

    model_tipos   ->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_SubTipos->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_cliente->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model_tipos   ->setTable("tipocliente_def");
    model_SubTipos->setTable("tiposubcliente_def");
    model_cliente->setTable("tipocliente");

    model_cliente->setFilter(QString("id_cliente = %1").arg(id_cliente));

    ui->list_tipo   ->setModelColumn(1);
    ui->list_subTipo->setModelColumn(2);

    updateListas();
    connect(ui->list_tipo,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(itemTipoChanged(QListWidgetItem*)));
    connect(ui->list_subTipo,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(itemSubTipoChanged(QListWidgetItem*)));
}

FrmAddTipoCliente::~FrmAddTipoCliente()
{
    delete ui;
}

void FrmAddTipoCliente::on_btnAceptar_clicked()
{
    this->close();
}

void FrmAddTipoCliente::on_list_tipo_clicked(const QModelIndex &index)
{
    int target_id_tipo = model_tipos->record(index.row()).value("id").toInt();

    ui->list_subTipo->clear();
    for(auto i=0;i < model_SubTipos->rowCount();i++)
    {
        int id_tipo = model_SubTipos->record(i).value("id_tipocliente").toInt();
        if(id_tipo == target_id_tipo)
        {
            QListWidgetItem* item = new QListWidgetItem(model_SubTipos->record(i).value("nombre").toString());
            int id_sub = model_SubTipos->record(i).value("id").toInt();
            bool sel = false;
            int id_row = 0;
            for(auto z = 0; z<model_cliente->rowCount();z++)
            {
                if(model_cliente->record(z).value("id_subtipo_cliente").toInt() == id_sub)
                {
                    sel = true;
                    id_row = model_cliente->record(z).value("id").toInt();
                    break;
                }
            }
            item->setCheckState(sel ? Qt::Checked : Qt::Unchecked);
            item->setToolTip(model_SubTipos->record(i).value("desc").toString());
            item->setData(Qt::UserRole +1 , target_id_tipo);
            item->setData(Qt::UserRole +2 , id_sub);
            item->setData(Qt::UserRole +3 , id_row);
            ui->list_subTipo->addItem(item);
        }
    }
}

void FrmAddTipoCliente::itemTipoChanged(QListWidgetItem * item)
{
    if(item->checkState() == Qt::PartiallyChecked)
        return;
    int id_tipo = item->data(Qt::UserRole+1).toInt();

    bool used = item->checkState() == Qt::Checked;
    QString error;
    if(used)
    {
        QList<int> needed;
        for(auto i= 0; i< model_SubTipos->rowCount();i++)
        {
            if(model_SubTipos->record(i).value("id_tipocliente").toInt() == id_tipo)
            {
                int id_subtipo = model_SubTipos->record(i).value("id").toInt();
                bool already = false;
                for(auto z = 0; z < model_cliente->rowCount(); z++)
                {
                    if(model_cliente->record(z).value("id_subtipo_cliente").toInt() == id_subtipo)
                    {
                        already = true;
                        break;
                    }
                }
                if(!already)
                    needed.append(id_subtipo);
            }
        }
        QHash<QString,QVariant> _data;
        _data["id_cliente"] = id_cliente;
        _data["id_tipo_cliente"] = id_tipo;

        QListIterator<int> _it(needed);
        while(_it.hasNext())
        {
            _data["id_subtipo_cliente"] = _it.next();
            if(!SqlCalls::SqlInsert(_data,"tipocliente",Configuracion_global->groupDB,error))
                QMessageBox::critical(this,tr("Error al asignar tipo cliente"),error);
        }
        updateListas();
    }
    else
    {
        if(SqlCalls::SqlDelete("tipocliente",Configuracion_global->groupDB,QString("id_tipo_cliente = %1").arg(id_tipo),error))
            updateListas();
        else
            QMessageBox::critical(this,tr("Error al asignar tipo cliente"),error);
    }
}

void FrmAddTipoCliente::itemSubTipoChanged(QListWidgetItem *item)
{
    if(item->checkState() == Qt::PartiallyChecked)
        return;

    bool used = item->checkState() == Qt::Checked;
    QString error;
    if(used)
    {
        QHash<QString,QVariant> _data;
        _data["id_cliente"] = id_cliente;
        _data["id_tipo_cliente"] = item->data(Qt::UserRole +1);
        _data["id_subtipo_cliente"] = item->data(Qt::UserRole +2);

        if(SqlCalls::SqlInsert(_data,"tipocliente",Configuracion_global->groupDB,error))
            updateListas();
        else
            QMessageBox::critical(this,tr("Error al asignar tipo cliente"),error);
    }
    else
    {
        if(SqlCalls::SqlDelete("tipocliente",Configuracion_global->groupDB,QString("id = %1").arg(item->data(Qt::UserRole +3).toInt()),error))
            updateListas();
        else
            QMessageBox::critical(this,tr("Error al asignar tipo cliente"),error);
    }
}
