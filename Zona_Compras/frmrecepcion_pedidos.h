#ifndef FRMRECEPCION_PEDidOS_H
#define FRMRECEPCION_PEDidOS_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../mayamodule.h"
namespace Ui {
class Frmrecepcion_pedidos;
}

class Frmrecepcion_pedidos : public MayaModule
{
    Q_OBJECT
    
public:
    explicit Frmrecepcion_pedidos(QWidget *parent = 0);
    ~Frmrecepcion_pedidos();
    bool paso;
    module_zone module_zone(){return Compras;}
    QString module_name(){return "Recepcion de pedidos";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
private slots:
    void on_btnBuscar_clicked();

    void on_chkForzarCierre_clicked();

    void on_tablaPedidos_doubleClicked(const QModelIndex &index);

    void validarcantidad(int, int);

    void abrir_albaran();

    void on_btnFactura_clicked();

    void on_BtnCancelar_clicked();

    void on_btncancelar_alb_clicked();

    void on_btnBuscaProv_clicked();

    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
private:
    Ui::Frmrecepcion_pedidos *ui;
    bool albaran;
    int id_albaran;
    bool factura;
    int id_factura;
    int id_pedido;
    QAction menuButton;
    QPushButton* push;
    QSqlQueryModel *modelPedidos;

    QMap<int, QSqlRecord> _lineas;
signals:

};

#endif // FRMRECEPCION_PEDidOS_H
