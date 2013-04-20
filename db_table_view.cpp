#include "db_table_view.h"
#include "ui_db_table_view.h"



Db_table_View::Db_table_View(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Db_table_View)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    selected_value = "";
    print_menu = new QMenu(this);
    toPaper = new QAction(tr("Imprimir"),this);
    toPDF =new QAction(tr("Exportar a PDF"),this);

    connect(toPaper,SIGNAL(triggered()),this,SLOT(print_clicked()));
    connect(toPDF,SIGNAL(triggered()),this,SLOT(print_clicked()));

    print_menu->addAction(toPaper);
    print_menu->addAction(toPDF);

    ui->btn_print->setMenu(print_menu);
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
    ui->resultado_list->hideColumn(0);
}

void Db_table_View::set_table(QString tabla)
{
    model->setTable(tabla);
    if(!model->select())
        qDebug () << model->lastError();
    ui->resultado_list->hideColumn(0);

    colums = model->columnCount();
}

void Db_table_View::set_table_headers(QStringList headers)
{
    for (int i = 0; i< headers.size();i++)
        model->setHeaderData(i+1, Qt::Horizontal, headers.at(i));
    ui->resultado_list->resizeColumnsToContents();

    this->headers = headers;
}

void Db_table_View::set_relation(int colum, QSqlRelation relation)
{
    model->setRelation(colum,relation);
    model->select();
    ui->resultado_list->hideColumn(0);
}

void Db_table_View::set_filter(QString filter)
{
    model->setFilter(filter);
    model->select();
}

void Db_table_View::set_readOnly(bool state)
{
    if(state)
    {
        ui->resultado_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->btn_add->hide();
        ui->btn_remove->hide();
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
        Configuracion::imprimir(db,file,true,true,this);
    else
        Configuracion::imprimir(db,file,false,true,this);
}

void Db_table_View::resizeEvent(QResizeEvent *)
{
    int w = ui->resultado_list->width();
    int _w = w/colums;

    ui->resultado_list->horizontalHeader()->setUpdatesEnabled(false);

    for (int i = 0; i < ui->resultado_list->model()->columnCount(); i++)
        ui->resultado_list->horizontalHeader()->resizeSection(i, _w);

    ui->resultado_list->horizontalHeader()->setUpdatesEnabled(true);
}
