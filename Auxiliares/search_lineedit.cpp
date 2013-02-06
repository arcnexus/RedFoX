#include "search_lineedit.h"
#include "ui_search_lineedit.h"

Search_LineEdit::Search_LineEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search_LineEdit)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}

Search_LineEdit::~Search_LineEdit()
{
    delete ui;
}

QString Search_LineEdit::Text()
{
    return m_text;
}

void Search_LineEdit::setText(QString s)
{
    m_text = s;
    ui->lineEdit->setText(s);
    ui->lineEdit->setFocus();
}

void Search_LineEdit::on_btn_buscar_clicked()
{
    Db_table_View searcher(this);
    searcher.set_db("empresa");
    searcher.set_table("articulos");
    searcher.set_selection("cCodigo");
    searcher.setWindowTitle(tr("Articulos"));

    QStringList headers;
    headers << tr("Codigo")<< "1" << "2" << tr("Descripción");
    searcher.set_table_headers(headers);

    searcher.set_columnHide(0);
    searcher.set_columnHide(2);
    searcher.set_columnHide(3);

    for(int i = 5; i<50;i++)
        searcher.set_columnHide(i);
    if(searcher.exec() == QDialog::Accepted)
                ui->lineEdit->setText(searcher.selected_value);
    /*
    if(table_view)
        table_view->setModal(true);
        if(table_view->exec() == QDialog::Accepted)
            if (ui)
                if(ui->lineEdit)
                    ui->lineEdit->setText(table_view->selected_value);*/
}

void Search_LineEdit::paintEvent(QPaintEvent *)
{
    /*
    if(ui->lineEdit->hasFocus() || ui->btn_buscar->hasFocus())
        ui->btn_buscar->show();
    else
        ui->btn_buscar->hide();*/
}

void Search_LineEdit::on_lineEdit_textChanged(const QString &arg1)
{
    m_text = arg1;
}
