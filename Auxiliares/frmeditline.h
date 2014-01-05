#ifndef FRMEDITLINE_H
#define FRMEDITLINE_H

#include <QDialog>
#include "Globlal_Include.h"
#include "../Almacen/articulo.h"

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
    void set_id_cliente(int id);
    void set_id_tarifa(int id);
    void set_tipo(QString tipo);
    void set_dto_tarifa(float dto);
    void set_id_cab(int id_cabecera);
    void set_tabla(QString t);




private slots:

    void on_txtCodigo_editingFinished();

    void on_txtCantidad_editingFinished();

    void on_txtPVP_editingFinished();

    void on_txtPvp_recomendado_editingFinished();

    void on_txtPorc_dto_editingFinished();

    void on_cboIva_currentIndexChanged(const QString &arg1);

    void on_btnAceptar_clicked();

private:
    Ui::frmEditLine *ui;
    int id,id_cliente,id_tarifa,id_articulo,id_cab;
    QString tipo; // ("C" compras - "V" - Ventas)
    QString tabla,codigo_articulo;
    void cargar_articulo(int id_art, int tarifa, int tipo_dto);
    int dto_tarifa;
    float unidades_regalo;
    Articulo *oArticulo;
    void calcular();

};

#endif // FRMEDITLINE_H
