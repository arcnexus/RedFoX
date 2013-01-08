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
    int nID;
private slots:
    void llenarTabla(int nID);
    void capturaId(int nId);
    void capturaPaciente(QString);
    void cargarDatos(int nID);
    void editarDatos();
    void guardarDatos();
    void guardarDatosItems(int,int);
};

#endif // FRMVERANALITICA_H
