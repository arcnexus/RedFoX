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
