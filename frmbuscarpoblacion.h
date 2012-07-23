#ifndef FRMBUSCARPOBLACION_H
#define FRMBUSCARPOBLACION_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QMessageBox>

namespace Ui {
class FrmBuscarPoblacion;
}

class FrmBuscarPoblacion : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmBuscarPoblacion(QWidget *parent = 0);
    ~FrmBuscarPoblacion();
    int DevolverID();

public slots:
    void  setcPoblacion(QString cPoblacion,int nPoblacion);

    
private:
    Ui::FrmBuscarPoblacion *ui;
    QString cPoblacion;
    QString cSelect;
    QSqlQueryModel * ModelPoblaciones;
    QHeaderView *Cabecera;
    QSqlDatabase dbPobla;

};

#endif // FRMBUSCARPOBLACION_H
