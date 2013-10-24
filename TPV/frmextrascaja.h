#ifndef FRMEXTRASCAJA_H
#define FRMEXTRASCAJA_H

#include <QDialog>

namespace Ui {
class frmExtrasCaja;
}

class frmExtrasCaja : public QDialog
{
    Q_OBJECT

public:
    explicit frmExtrasCaja(QWidget *parent = 0);
    ~frmExtrasCaja();

private:
    Ui::frmExtrasCaja *ui;
};

#endif // FRMEXTRASCAJA_H
