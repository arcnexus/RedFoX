#ifndef SEARCH_LINEEDIT_H
#define SEARCH_LINEEDIT_H

#include <QWidget>
#include "../db_table_view.h"
namespace Ui {
class Search_LineEdit;
}

class Search_LineEdit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString m_text READ Text WRITE setText USER true)
public:
    explicit Search_LineEdit(QWidget *parent = 0,Db_table_View* table_view=0);
    ~Search_LineEdit();
    void setDb_table_View(Db_table_View* table_view);

public slots:
    QString Text();
    void setText(QString s);
private slots:
    void on_btn_buscar_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    void paintEvent(QPaintEvent *);
    Ui::Search_LineEdit *ui;
    QString db;
    QString tabla;
    Db_table_View* table_view;
    QString m_text;
};

#endif // SEARCH_LINEEDIT_H
