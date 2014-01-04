#ifndef FRMEDITLINE_H
#define FRMEDITLINE_H

#include <QDialog>
#include "Globlal_Include.h"

namespace Ui {
class frmEditLine;
}

class frmEditLine : public QDialog
{
    Q_OBJECT

public:
    explicit frmEditLine(QWidget *parent = 0);
    ~frmEditLine();
    int get_id();
    void set_linea(int id, QString fichero);
    void set_tipo(QString tipo);


private slots:

    void on_txtCodigo_editingFinished();

private:
    Ui::frmEditLine *ui;
    int id;
    QString tipo; // ("C" compras - "V" - Ventas)
    void cargar_articulo(int id, int tarifa, int tipo_dto);

};

#endif // FRMEDITLINE_H
