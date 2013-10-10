#ifndef FRMARTICULOS_H
#define FRMARTICULOS_H

#include "../Auxiliares/Globlal_Include.h"
#include "../Auxiliares/monetarydelegate.h"
//
#include "articulo.h"
#include "../mayamodule.h"
namespace Ui {
class FrmArticulos;
}

class FrmArticulos : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmArticulos(QWidget *parent = 0, bool closeBtn = false);
    ~FrmArticulos();
    Articulo *oArticulo;
    QSqlQueryModel *modArt;
    QSqlQueryModel *qTarifas;
    QSqlRelationalTableModel * tarifa_model;
    QSqlQueryModel *modelProv;
    QSqlQueryModel *modelTrazabilidad1;
    QSqlQueryModel *modelTrazabilidad2;
    QSqlQueryModel * model_ofertas;

    module_zone module_zone(){return Almacen;}
    QString module_name(){return "Articulos";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}    

    QPushButton* wantShortCut(bool& ok){ok = true; return shortCut;}
private slots:
    void on_botSiguiente_clicked();

    void on_botAnterior_clicked();

    void on_botAnadir_clicked();

    void on_botEditar_clicked();

    void on_botBorrar_clicked();

    void on_botGuardar_clicked();

    void CambiarImagen_clicked(QLabel *label ,QString campo);

    void on_botDeshacer_clicked();

    void on_botBuscarSeccion_clicked();

    void on_botBuscarFamilia_clicked();

    void on_botBuscarSubfamilia_clicked();

    void on_botBuscarSubSubFamilia_clicked();
    void on_btnBuscarProveedor_clicked();
    void on_btnAnadirTarifa_clicked();

    void btnEditarTarifa_clicked();
    void btnBorrarTarifa_clicked();

    void anadir_proveedor_clicked();
    void editar_proveedor_clicked();
    void borrar_proveedor_clicked();
    void asignar_proveedor_principal_clicked();
    void calcular_codigo();
    void trazabilidad2(int id);
    void TablaTrazabilidad_clicked(QModelIndex);
    void graficar(QString Tipo);
    void GraficaUnidades();
    void GraficaImportes();
    void GraficaUnidades_comparativa();
    void GraficaImportes_comparativa();
    void MostrarGrafica_comparativa(bool);
    void LLenarGraficas();
    void LLenarGrafica_comparativa(int);
    void LlenarTablas();
    void SeleccionarPestana(int);
    void listados();

    void on_checkBox_toggled(bool checked);
    void ani_end();
    void toggleChecbox();
    void on_chkmostrarvalores_comparativa_toggled(bool checked);
    void ani_comparativas_end();
    void togglechkmostrarvalores_comparativa();
    void on_botCambiarImagen_clicked();

    void on_botCambiarImagen_2_clicked();

    void on_botCambiarImagen_3_clicked();

    void on_botCambiarImagen_4_clicked();
    void actualizar();

    void on_txtcoste_editingFinished();

    void on_btnBuscar_clicked();

    void on_cboOrden_busquedas_currentIndexChanged(const QString &arg1);

    void on_txtBuscar_textEdited(const QString &arg1);

    void on_tabla_clicked(const QModelIndex &index);

    void on_tabla_doubleClicked(const QModelIndex &index);

    void on_btnLimpiar_clicked();

    void on_cboOrden_currentIndexChanged(const QString &arg1);

    void on_cboModo_currentIndexChanged(const QString &arg1);

    void on_btnExcepciones_2_clicked();

    void on_btnExcepciones_clicked();

    void on_btnKit_2_clicked();

    void on_btnAnadir_oferta_clicked();

    void on_btnguardar_oferta_clicked();

    void on_chkArticulo_promocionado_toggled(bool checked);

    void on_tabla_ofertas_clicked(const QModelIndex &index);

    void on_btnDeshacer_oferta_clicked();
    

    
    void on_chkOferta_32_toggled(bool checked);

    void on_chkOferta_dto_toggled(bool checked);

    void on_chkOferta_pvp_toggled(bool checked);

    void on_chkOferta_web_toggled(bool checked);

    void on_btnExcepciones_3_clicked();


    void on_btnKit_clicked();

    void on_btnAnadir_clicked();

    void on_btnBorrarKit_clicked();

public slots:
    void AnadirSeccion();


private:
    Ui::FrmArticulos *ui;
    bool eventFilter(QObject *target, QEvent *event);
    bool Altas;
    void bloquearCampos(bool state);
    void LLenarCampos();
    void CargarCamposEnArticulo();
    void VaciarCampos();
    void ChangeValues_TablaProveedores(int row, int column);
    void formato_tabla();
    void filter_table();

    QListView *lista;
    QDialog *ventana;
    QString *Devolucion;
    QGridLayout *layout;
    bool anadir;
    bool reformateado;
    void rellenar_grafica_proveedores();
    MonetaryDelegate *Delegado;
    QAction menuButton;
    QPushButton* shortCut;
    QSqlQueryModel *m;
    bool nueva_oferta;
    int id_oferta = 0;
};

#endif // FRMARTICULOS_H
