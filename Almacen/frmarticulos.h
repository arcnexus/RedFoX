#ifndef FRMARTICULOS_H
#define FRMARTICULOS_H

#include "../Auxiliares/Globlal_Include.h"
#include "../Auxiliares/monetarydelegate.h"
//
#include "articulo.h"
namespace Ui {
class FrmArticulos;
}

class FrmArticulos : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmArticulos(QWidget *parent = 0);
    ~FrmArticulos();
    Articulo *oArticulo;
    QSqlQueryModel *modArt;
    QSqlQueryModel *qTarifas;
    QSqlRelationalTableModel * tarifa_model;
    QSqlQueryModel *modelProv;
    QSqlQueryModel *modelTrazabilidad1;
    QSqlQueryModel *modelTrazabilidad2;

private slots:
    void on_botSiguiente_clicked();

    void on_botAnterior_clicked();

    void on_botAnadir_clicked();

    void on_botEditar_clicked();

    void on_botBorrar_clicked();

    void on_botGuardar_clicked();

    void on_botCambiarImagen_clicked();

    void on_botRotarImagen90_clicked();

    void on_botDeshacer_clicked();

    void on_botBuscarArtRapido_clicked();

    void on_TablaBuscarArt_doubleClicked(const QModelIndex &index);

    void on_botBuscarSeccion_clicked();

    void on_botBuscarFamilia_clicked();

    void on_botBuscarSubfamilia_clicked();

    void on_botBuscarSubSubFamilia_clicked();
    void on_btnBuscarProveedor_clicked();
    void on_btnAnadirTarifa_clicked();
    void anadir_proveedor_clicked();
    void editar_proveedor_clicked();
    void borrar_proveedor_clicked();
    void calcular_codigo();
    void trazabilidad2(int id);
    void TablaTrazabilidad_clicked(QModelIndex);
    void GraficaUnidades();
    void GraficaImportes();

public slots:
    void AnadirSeccion();


private:
    Ui::FrmArticulos *ui;
    bool Altas;
    void desbloquearCampos();
    void bloquearCampos();
    void LLenarCampos();
    void CargarCamposEnArticulo();
    void VaciarCampos();
    void ChangeValues_TablaProveedores(int row, int column);

    QListView *lista;
    QDialog *ventana;
    QString *Devolucion;
    QGridLayout *layout;
    bool anadir;
    bool reformateado;
    void rellenar_grafica_proveedores();
    MonetaryDelegate *Delegado;
};

#endif // FRMARTICULOS_H
