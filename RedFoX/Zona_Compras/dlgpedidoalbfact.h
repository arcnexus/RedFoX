#ifndef DLGPEDIDOALBFACT_H
#define DLGPEDIDOALBFACT_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
namespace Ui {
class DlgPedidoAlbFact;
}

class DlgPedidoAlbFact : public QDialog
{
    Q_OBJECT    

public:
    DlgPedidoAlbFact(QString label ,QString prov, QWidget *parent = 0);
    ~DlgPedidoAlbFact();

    enum tipo { Albaran = 0 , Factura };

    QMap<int, QSqlRecord> getLineas() const;
    void setLineas(const QMap<int, QSqlRecord> &value);
    void set_proveedor(int id);
    void set_tipo(tipo t);

private slots:
    void on_btnAccept_clicked();
    void calcular(double);
    void validarcantidad(int row, int column);
    void on_chkrecargo_equivalencia_toggled(bool checked);
    void cbo_IndexChanged(int /*index*/);
private:
    Ui::DlgPedidoAlbFact *ui;
    QMap<int, QSqlRecord> lineas;
    QSqlRecord r_prov;
    tipo _tipo;

    bool crear_documento();
    bool crear_lineas(int id_cab);
    QHash<QString,QVariant> _datosComunes();
};

#endif // DLGPEDIDOALBFACT_H
