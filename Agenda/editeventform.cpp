#include "editeventform.h"
#include "ui_editeventform.h"

EditEventForm::EditEventForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditEventForm)
{
    ui->setupUi(this);
}

EditEventForm::~EditEventForm()
{
    delete ui;
}
