#include "frmcantidad.h"
#include "ui_frmcantidad.h"

frmcantidad::frmcantidad(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmcantidad)
{
    ui->setupUi(this);
    ui->doubleSpinBox->installEventFilter(this);
    ui->doubleSpinBox->selectAll();
}

frmcantidad::~frmcantidad()
{
    delete ui;
}

float frmcantidad::getCantidad()
{
    return ui->doubleSpinBox->value();
}

void frmcantidad::on_doubleSpinBox_editingFinished()
{
    accept();
}

bool frmcantidad::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() ==Qt::Key_Tab)
            accept();
        return false;
    }
    return false;
}

void frmcantidad::on_pushButton_clicked()
{
    accept();
}
