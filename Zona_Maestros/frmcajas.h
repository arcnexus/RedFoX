#ifndef FRMCAJAS_H
#define FRMCAJAS_H

#include <QWidget>

namespace Ui {
class FrmCajas;
}

class FrmCajas : public QWidget
{
    Q_OBJECT

public:
    explicit FrmCajas(QWidget *parent = 0);
    ~FrmCajas();

private:
    Ui::FrmCajas *ui;
};

#endif // FRMCAJAS_H
