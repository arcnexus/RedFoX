#ifndef FRMDECISION_H
#define FRMDECISION_H

#include <QDialog>
#include <QString>

namespace Ui {
class frmDecision;
}

class frmDecision : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmDecision(QWidget *parent = 0);
    ~frmDecision();
    int Salida;
public slots:
    void setLinea1(QString linea1);
    void setLinea2(QString linea2);
    void setLinea3(QString linea3);
    void setBoton1(QString Boton1);
    void setBoton2(QString Boton2);
    void Inicializar(QString titulo,QString linea1,QString linea2, QString linea3, QString Boton1,QString Boton2);

private:
    Ui::frmDecision *ui;
};

#endif // FRMDECISION_H
