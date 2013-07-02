#include "db_table_view.h"
#include "ui_db_table_view.h"
#include "../Auxiliares/monetarydelegate.h"



Db_table_View::Db_table_View(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Db_table_View)
{
    _readOnly = false;
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    selected_value = "";
    print_menu = new QMenu(this);
    toPaper = new QAction(tr("Imprimir"),this);
    toPDF =new QAction(tr("Exportar a PDF"),this);
    tarifa = 1;


    connect(toPaper,SIGNAL(triggered()),this,SLOT(print_clicked()));
    connect(toPDF,SIGNAL(triggered()),this,SLOT(print_clicked()));

    print_menu->addAction(toPaper);
    print_menu->addAction(toPDF);

    ui->btn_print->setMenu(print_menu);
    ui->txtBuscar->setFocus();
    ui->resultado_list->installEventFilter(this);
}

Db_table_View::~Db_table_View()
{
    delete ui;
}

void Db_table_View::set_db(QString db)
{
    this->db = db;
    model = new QSqlRelationalTableModel(this,QSqlDatabase::database(db));
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    ui->resultado_list->setItemDelegate(new QSqlRelationalDelegate(ui->resultado_list));
    ui->resultado_list->setModel(model);
//    ui->resultado_list->hideColumn(0);
}

void Db_table_View::set_table(QString tabla)
{
    this->tabla = tabla;
    model->setTable(tabla);
    if(!model->select())
        qDebug () << model->lastError();
 //   ui->resultado_list->hideColumn(0);

    colums = model->columnCount();
    //-----------------------
    // Formateo forzado
    //-----------------------
    if (tabla =="vistaArt_tarifa"){
        QString filtro ="tarifa=";
        filtro.append(QString::number(tarifa));
        set_filter(filtro);
        ui->resultado_list->setColumnWidth(0,100);
        ui->resultado_list->setColumnWidth(1,100);
        ui->resultado_list->setColumnWidth(2,100);
        ui->resultado_list->setColumnWidth(3,300);
        ui->resultado_list->setColumnWidth(4,180);
        ui->resultado_list->setColumnWidth(5,50);
        ui->resultado_list->setColumnWidth(6,90);
        ui->resultado_list->setColumnWidth(7,90);
        ui->resultado_list->setColumnHidden(8,true);
        ui->resultado_list->setColumnHidden(9,true);
        ui->resultado_list->setColumnHidden(11,true);
        ui->resultado_list->setColumnWidth(10,50);
        ui->resultado_list->setColumnWidth(12,50);
        ui->resultado_list->setColumnHidden(13,true);

        model->setHeaderData(0,Qt::Horizontal,tr("Código"));
        model->setHeaderData(1,Qt::Horizontal,tr("C.Barras"));
        model->setHeaderData(2,Qt::Horizontal,tr("C.Proveedor"));
        model->setHeaderData(3,Qt::Horizontal,tr("Descripción"));
        model->setHeaderData(4,Qt::Horizontal,tr("Desc. Resumida"));
        model->setHeaderData(5,Qt::Horizontal,tr("IVA"));
        model->setHeaderData(6,Qt::Horizontal,tr("Coste"));
        model->setHeaderData(7,Qt::Horizontal,tr("PVP"));
        model->setHeaderData(10,Qt::Horizontal,tr("St.R"));
        model->setHeaderData(12,Qt::Horizontal,tr("St.F"));
        ui->resultado_list->setItemDelegateForColumn(6,new MonetaryDelegate);
        ui->resultado_list->setItemDelegateForColumn(7, new MonetaryDelegate);

    }
}

void Db_table_View::set_table_headers(QStringList headers)
{
    for (int i = 0; i< headers.size();i++)
        model->setHeaderData(i+1, Qt::Horizontal, headers.at(i));
    ui->resultado_list->resizeColumnsToContents();

//    this->headers = headers;
}

void Db_table_View::set_relation(int colum, QSqlRelation relation)
{
    model->setRelation(colum,relation);
    model->select();
 //   ui->resultado_list->hideColumn(0);
}

void Db_table_View::set_filter(QString filter)
{
    model->setFilter(filter);
    model->select();
}

void Db_table_View::set_readOnly(bool state)
{
    _readOnly = state;
    if(state)
    {
        ui->resultado_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->btn_add->hide();
        ui->btn_remove->hide();
        ui->btn_save->hide();
        ui->btn_print->hide();
    }
}

void Db_table_View::set_selection(QString column_name)
{
    selection_column = column_name;
}

void Db_table_View::set_columnHide(int column)
{
    ui->resultado_list->setColumnHidden(column, true);
    ui->resultado_list->resizeColumnsToContents();

    hide_headers.append(column);
    colums--;
}

void Db_table_View::set_noInsertDeleteRows()
{
    ui->btn_add->hide();
    ui->btn_remove->hide();
}

void Db_table_View::set_printFile(QString file)
{
    ui->btn_print->setEnabled(true);
    this->file = file;
}

void Db_table_View::set_tarifa(int tarifa)
{
    this->tarifa = tarifa;
}

void Db_table_View::on_btn_add_clicked()
{
    model->insertRow(model->rowCount());
}

void Db_table_View::on_btn_remove_clicked()
{
    model->removeRow(ui->resultado_list->currentIndex().row());
}

void Db_table_View::on_btn_save_clicked()
{
    model->submitAll();
}

void Db_table_View::on_btn_aceptar_clicked()
{
    model->submitAll();
    if(model->rowCount()>0)
    {
        QModelIndexList indexes = ui->resultado_list->selectionModel()->selection().indexes();
        if (!indexes.isEmpty())
        {
            QModelIndex index = indexes.at(0);
            int row = index.row();
            QSqlRecord r = model->record(row);
            selected_value = r.value(selection_column).toString();
        }
    }
    Db_table_View::done(QDialog::Accepted);
}

void Db_table_View::on_resultado_list_clicked(const QModelIndex &index)
{
    int row = index.row();
    QSqlRecord r = model->record(row);
    selected_value = r.value(selection_column).toString();
}

void Db_table_View::on_btn_cancel_clicked()
{
    Db_table_View::done(QDialog::Rejected);
}

void Db_table_View::print_clicked()
{
    if(sender() == toPDF)
        Configuracion::imprimir(true,true,this);
    else
        Configuracion::imprimir(false,true,this);
}

void Db_table_View::resizeEvent(QResizeEvent *)
{
//    int w = ui->resultado_list->width();
//    int _w = w/colums;

//    ui->resultado_list->horizontalHeader()->setUpdatesEnabled(false);

//    for (int i = 0; i < ui->resultado_list->model()->columnCount(); i++)
//        ui->resultado_list->horizontalHeader()->resizeSection(i, _w);

//    ui->resultado_list->horizontalHeader()->setUpdatesEnabled(true);
}

void Db_table_View::on_txtBuscar_textChanged(const QString &arg1)
{
    if (this->tabla =="vistaArt_tarifa" )
    {
        QString filtro = "descripcion like '%";
        filtro.append(arg1);
        filtro.append("%' and tarifa =");
        filtro.append(QString::number(tarifa));
        this->set_filter(filtro);
    }
}

void Db_table_View::on_txtBuscar_editingFinished()
{
    ui->resultado_list->setFocus();
    ui->resultado_list->selectRow(ui->resultado_list->currentIndex().row());
}

bool Db_table_View::eventFilter(QObject *target, QEvent *event)
{
    Q_UNUSED(target)
    if (event->type() == QEvent::KeyPress)
    {
        ui->resultado_list->blockSignals(true);
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ((keyEvent->key() == Qt::Key_Tab || keyEvent->key() == Qt::Key_Return) && _readOnly)
        {
            int row = ui->resultado_list->currentIndex().row();
            selected_value = ui->resultado_list->model()->data(ui->resultado_list->model()->index(row,0)).toString();
            ui->btn_aceptar->setFocus();
        }
        ui->resultado_list->blockSignals(false);
    }
    return false;
}


void Db_table_View::on_resultado_list_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QSqlRecord r = model->record(row);
    selected_value = r.value(selection_column).toString();
    if(_readOnly)
        this->accept();
}
