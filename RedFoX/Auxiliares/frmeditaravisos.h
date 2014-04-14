#ifndef FRMEDITARAVISOS_H
#define FRMEDITARAVISOS_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmEditarAvisos;
}

class FrmEditarAvisos : public QDialog
{
    Q_OBJECT

public:
    explicit FrmEditarAvisos(QWidget *parent = 0);
    ~FrmEditarAvisos();
    void setValores(int id);

private slots:
    void on_btnAceptar_clicked();

    void on_btnCerrar_clicked();

private:
    Ui::FrmEditarAvisos *ui;
    int id;
};

#endif // FRMEDITARAVISOS_H
