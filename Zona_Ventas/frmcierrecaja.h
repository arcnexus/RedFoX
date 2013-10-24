#ifndef FRMCIERRECAJA_H
#define FRMCIERRECAJA_H

#include <QDialog>

namespace Ui {
class FrmCierreCaja;
}

class FrmCierreCaja : public QDialog
{
    Q_OBJECT

public:
    explicit FrmCierreCaja(QWidget *parent = 0);
    ~FrmCierreCaja();

private:
    Ui::FrmCierreCaja *ui;
};

#endif // FRMCIERRECAJA_H
