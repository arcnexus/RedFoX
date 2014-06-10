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

    enum tipoDoc { Presupuesto = 0, Pedido, Albaran , Factura};
    void setTipoDoc(tipoDoc doc);

    int get_id();
    void set_linea(int id, QString fichero);
    void set_id_cliente(int id);
    void set_id_tarifa(int id);
    void set_venta(bool is_venta);
    void set_id_cab(int id_cabecera);
    void set_tabla(QString t);
    void set_editando();
    void set_acumula(bool acum);
    void set_reserva(bool reserv);

    void init();
    bool getAdd_pendientes() const;
    void setAdd_pendientes(bool value);

    bool getUse_re() const;
    void setUse_re(bool value);

    int getId_prov() const;
    void setId_prov(int value);

private slots:

    void on_txtCodigo_editingFinished();

    void on_txtCantidad_editingFinished();

    void on_txtPVP_editingFinished();

    void on_txtPvp_recomendado_editingFinished();

    void on_txtPorc_dto_editingFinished();

    void on_cboIva_currentIndexChanged(const QString &arg1);

    void on_btnAceptar_clicked();

    void on_btnAnadir_mas_nueva_clicked();

private:
    Ui::frmEditLine *ui;
    int id,id_cliente,id_tarifa,id_articulo,id_cab;
    int id_prov;

    QString tabla,codigo_articulo;
    void cargar_articulo(int id_art, int tarifa);
    void vaciar_campos();
    int id_lote;
    float unidades_regalo;

    bool eventFilter(QObject *obj, QEvent *event);
    QHash <QString,QVariant> anterior;
    void calcular();

    bool editando;
    bool is_venta;
    bool realiza_acumulados;
    bool reserva_unidades ;
    bool add_pendientes;

    void buscar_art();

    QHash <QString, QVariant> get_datos();
    void _insert_nueva_linea();
    bool use_re;

    tipoDoc _tipoDoc;

    void do_stocks(double cantidad);
signals:
    void refrescar_lineas();
};

#endif // FRMEDITLINE_H
