#ifndef PRUEBAS_H
#define PRUEBAS_H

#include <QDialog>
#include <QSqlQuery>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSqlRecord>

namespace Ui {
class Pruebas;
}

class Pruebas : public QDialog
{
    Q_OBJECT
    
public:
    explicit Pruebas(QWidget *parent = 0);
    ~Pruebas();
    
private:
    Ui::Pruebas *ui;

    QSqlDatabase db;
    QPushButton *boton;
    QStringList cabecera;
    QSqlQuery *resultado;
    QDoubleSpinBox *qnumero;
    QLineEdit *qtexto;
    QSqlRecord *rec;
};

#endif // PRUEBAS_H
