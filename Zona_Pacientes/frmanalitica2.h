#ifndef FRMANALITICA2_H
#define FRMANALITICA2_H

#include <QDialog>

namespace Ui {
class Frmanalitica2;
}

class Frmanalitica2 : public QDialog
{
    Q_OBJECT
    
public:
    explicit Frmanalitica2(QWidget *parent = 0);
    ~Frmanalitica2();
    
private:
    Ui::Frmanalitica2 *ui;
    int idAnalitica;
    QString cTipo;
public slots:


private slots:
    void cargarDatos(QString cTipo,int idAnalitica);
    void aceptar();

signals:
};

#endif // FRMANALITICA2_H