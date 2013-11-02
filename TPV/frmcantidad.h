#ifndef FRMCANTIDAD_H
#define FRMCANTIDAD_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class frmcantidad;
}

class frmcantidad : public QDialog
{
    Q_OBJECT

public:
    explicit frmcantidad(QWidget *parent = 0);
    ~frmcantidad();
    float getCantidad();
private slots:
    void on_doubleSpinBox_editingFinished();

private:
    Ui::frmcantidad *ui;
    bool eventFilter(QObject *target, QEvent *event);
};

#endif // FRMCANTIDAD_H
