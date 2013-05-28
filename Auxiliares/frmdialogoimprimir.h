#ifndef FRMDIALOGOIMPRIMIR_H
#define FRMDIALOGOIMPRIMIR_H

#include <QDialog>

namespace Ui {
class FrmDialogoImprimir;
}

class FrmDialogoImprimir : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmDialogoImprimir(QWidget *parent = 0);
    ~FrmDialogoImprimir();
    
private:
    Ui::FrmDialogoImprimir *ui;
};

#endif // FRMDIALOGOIMPRIMIR_H
