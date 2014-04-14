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
    void set_email(QString email);
    void set_ruta(QString ruta);
    void set_preview(bool preview);
    int salida;  // 1- impresora 2- pdf 3- email 4-preview
    QString get_ruta();
    QString get_email();
    int get_option();
    bool to_pdf();
    
private slots:

private:
    Ui::FrmDialogoImprimir *ui;

};

#endif // FRMDIALOGOIMPRIMIR_H
