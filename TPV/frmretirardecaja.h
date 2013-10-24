#ifndef FRMRETIRARDECAJA_H
#define FRMRETIRARDECAJA_H

#include <QDialog>

namespace Ui {
class FrmRetirardeCaja;
}

class FrmRetirardeCaja : public QDialog
{
    Q_OBJECT

public:
    explicit FrmRetirardeCaja(QWidget *parent = 0);
    ~FrmRetirardeCaja();

private:
    Ui::FrmRetirardeCaja *ui;
};

#endif // FRMRETIRARDECAJA_H
