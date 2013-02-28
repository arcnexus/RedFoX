#ifndef DB_TABLE_VIEW_H
#define DB_TABLE_VIEW_H

#include "Auxiliares/Globlal_Include.h"
namespace Ui {
class Db_table_View;
}

class Db_table_View : public QDialog
{
    Q_OBJECT
    
public:
    explicit Db_table_View(QWidget *parent = 0);
    ~Db_table_View();
    void set_db(QString db);
    void set_table(QString tabla);
    void set_table_headers(QStringList headers);
    void set_relation(int colum, QSqlRelation relation);
    void set_filter(QString filter);
    void set_readOnly(bool state);
    void set_selection(QString column_name);
    void set_columnHide(int column);
    void set_noInsertDeleteRows();
    void set_printFile(QString file);
    QString selected_value;
private slots:
    void on_btn_add_clicked();

    void on_btn_remove_clicked();

    void on_btn_save_clicked();

    void on_btn_aceptar_clicked();

    void on_resultado_list_clicked(const QModelIndex &index);

    void on_btn_cancel_clicked();

    void print_clicked();

private:
    Ui::Db_table_View *ui;
    QString db;
    QSqlRelationalTableModel* model;
    QString selection_column;
    QString file;

    QMenu * print_menu;
    QAction* toPaper;
    QAction* toPDF;

    QStringList headers;
    QVector<int> hide_headers;
};

#endif // DB_TABLE_VIEW_H
