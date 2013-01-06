#ifndef FRMVERANALITICA_H
#define FRMVERANALITICA_H

#include <QDialog>

namespace Ui {
class FrmVerAnalitica;
}

class FrmVerAnalitica : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmVerAnalitica(QWidget *parent = 0);
    ~FrmVerAnalitica();
    
private:
    Ui::FrmVerAnalitica *ui;
private slots:
    void llenarTabla();
    void capturaId(int);
    void capturaPaciente(QString);
    void cargarDatos();
};

#endif // FRMVERANALITICA_H
