#ifndef FRMANALITICA_H
#define FRMANALITICA_H

#include <QDialog>

namespace Ui {
class FrmAnalitica;
}

class FrmAnalitica : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAnalitica(QWidget *parent = 0);
    ~FrmAnalitica();
    
private:
    Ui::FrmAnalitica *ui;
private slots:
    void AnadirCamposAnalitica();

signals:
    void pasartipo(QString);
};

#endif // FRMANALITICA_H
