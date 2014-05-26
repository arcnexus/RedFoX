#ifndef FRMCAJAS_H
#define FRMCAJAS_H

#include <QDialog>
#include "cajas.h"
#include "../mayamodule.h"
#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmCajas;
}

class FrmCajas : public MayaModule
{
    Q_OBJECT

public:
    explicit FrmCajas(QWidget *parent = 0);
    ~FrmCajas();
    module_zone module_zone(){return Mantenimiento;}
    QString module_name(){return "Cajas";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}

    QPushButton* wantShortCut(bool& ok) {ok = false; return 0;}

private slots:
    void on_btnAnadir_clicked();

    void on_btnAnterior_clicked();

    void on_btnSiguiente_clicked();

    void on_btnBuscar_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_btnDeshacer_clicked();

    void on_btnBorrar_clicked();

    void mostrarBusqueda();

    void ocultarBusqueda();

    void on_tablaBusqueda_doubleClicked(const QModelIndex &index);

    void on_lineEditCaja_editingFinished();

private:
    Ui::FrmCajas *ui;
    QAction menuButton;
    QSqlQueryModel *model;

    BarraBusqueda *m_busqueda;
    void setUpBusqueda();
    bool anadiendo;
    cajas *oCajas;
    void filter_table(QString texto, QString orden, QString modo);
    void bloquearCampos(bool estado);
    void recuperarDatos(QString id);
    void llenarCampos();
    void llenarTabla();
};

#endif // FRMCAJAS_H

