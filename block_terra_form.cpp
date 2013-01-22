#include "block_terra_form.h"
#include "ui_block_terra_form.h"

block_terra_form::block_terra_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::block_terra_form)
{
    ui->setupUi(this);
}

block_terra_form::~block_terra_form()
{
    delete ui;
}

void block_terra_form::set_user(QString user, QString pass)
{
    this->user = user;
    this->pass = pass;
}

void block_terra_form::on_pushButton_clicked()
{
    if(ui->txt_user->text() == user)
        if(ui->txt_pass->text() == pass)
            this->hide();
}
void block_terra_form::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
}

void block_terra_form::closeEvent(QCloseEvent * e)
{
    e->ignore();
}
