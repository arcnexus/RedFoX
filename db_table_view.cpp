#include "db_table_view.h"
#include "ui_db_table_view.h"

Db_table_View::Db_table_View(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Db_table_View)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    selected_value = "";
}

Db_table_View::~Db_table_View()
{
    delete ui;
}

void Db_table_View::set_db(QString db)
{
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
}

void Db_table_View::set_table_headers(QStringList headers)
{
    for (int i = 0; i< headers.size();i++)
        model->setHeaderData(i+1, Qt::Horizontal, headers.at(i));
    ui->resultado_list->resizeColumnsToContents();
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
    //TODO read-only
}

void Db_table_View::set_selection(QString column_name)
{
    selection_column = column_name;
}

void Db_table_View::set_columnHide(int column)
{
    ui->resultado_list->setColumnHidden(column, true);
    ui->resultado_list->resizeColumnsToContents();
}

void Db_table_View::set_noInsertDeleteRows()
{
    ui->btn_add->hide();
    ui->btn_remove->hide();
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
    if(model->isDirty())
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
