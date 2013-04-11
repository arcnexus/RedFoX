#include "Zona_Administrador/block_Maya_form.h"
#include "ui_block_Maya_form.h"

block_Maya_form::block_Maya_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::block_Maya_form)
{
    ui->setupUi(this);
}

block_Maya_form::~block_Maya_form()
{
    delete ui;
}

void block_Maya_form::set_user(QString user, QString pass)
{
    this->user = user;
    this->pass = pass;
}

void block_Maya_form::on_pushButton_clicked()
{
    if(ui->txt_user->text() == user)
        if(ui->txt_pass->text() == pass)
            this->hide();
}
void block_Maya_form::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
}

void block_Maya_form::closeEvent(QCloseEvent * e)
{
    e->ignore();
}
