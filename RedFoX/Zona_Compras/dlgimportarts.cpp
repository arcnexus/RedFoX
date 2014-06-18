#include "dlgimportarts.h"
#include "ui_dlgimportarts.h"

DlgImportArts::DlgImportArts(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgImportArts)
{
    ui->setupUi(this);
}

DlgImportArts::~DlgImportArts()
{
    delete ui;
}
