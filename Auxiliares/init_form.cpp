#include "init_form.h"
#include "ui_init_form.h"

init_form::init_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::init_form)
{
    ui->setupUi(this);
}

init_form::~init_form()
{
    delete ui;
}
