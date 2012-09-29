#ifndef FRMPRESUPUESTOSCLI_H
#define FRMPRESUPUESTOSCLI_H

#include <QDialog>

namespace Ui {
class FrmPresupuestosCli;
}

class FrmPresupuestosCli : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmPresupuestosCli(QWidget *parent = 0);
    ~FrmPresupuestosCli();
    
private slots:
    void on_chklEnviado_stateChanged(int arg1);

private:
    Ui::FrmPresupuestosCli *ui;
};

#endif // FRMPRESUPUESTOSCLI_H
