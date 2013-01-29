#ifndef FRMVERANALITICA_H
#define FRMVERANALITICA_H

#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmVerAnalitica;
}

class FrmVerAnalitica : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmVerAnalitica(QWidget *parent = 0);
    ~FrmVerAnalitica();
public slots:
    void capturaId(int nId);
    void capturaPaciente(QString);
private:
    Ui::FrmVerAnalitica *ui;
    int nID;
    int nEdited;
private slots:
    void llenarTabla(int nID);
    void cargarDatos(int nID);
    void cargarDatos();
    void editarDatos();
    void guardarDatos();
    void guardarDatosItems(QTableWidgetItem*);
    void Deshacer();
    void addLineas();
    void BorrarLineas();
signals:
    void pasarID(int);
    void pasarPaciente(QString);
    void pasarFecha(QDate);
    void pasarAnalitica(QString);
};

#endif // FRMVERANALITICA_H
