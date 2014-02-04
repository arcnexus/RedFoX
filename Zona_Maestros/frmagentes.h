#ifndef FRMAGENTES_H
#define FRMAGENTES_H

#include <QDialog>
#include "../mayamodule.h"

#include "../Auxiliares/Globlal_Include.h"
#include <QDataWidgetMapper>

namespace Ui {
class frmAgentes;
}

class frmAgentes : public MayaModule
{
    Q_OBJECT

public:
    explicit frmAgentes(QWidget *parent = 0);
    ~frmAgentes();

    module_zone module_zone(){return Ventas;}
    QString module_name(){return "Agentes";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    QPushButton* wantShortCut(bool& ok){ok = false; return 0;}

    void init();
    void init_querys();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);
    void on_btnAnadir_clicked();

    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

    void on_btnBuscar_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_btnDeshacer_clicked();

    void on_btnListados_clicked();

    void on_btnBorrar_clicked();

    void bloquear_campos(bool state);
    void on_tabla_busqueda_doubleClicked(const QModelIndex &index);

    void mapperIndexChanged(int i);
    void on_btn_addComision_clicked();

    void on_btn_removeComision_clicked();

    void on_tabwidget_currentChanged(int index);

    void on_lista_tarifas_clicked(const QModelIndex &index);

    void on_cboEmpresas_currentIndexChanged(int index);

private:
    Ui::frmAgentes *ui;
    QAction menuButton;
    QDataWidgetMapper mapper;

    QSqlTableModel * model_busqueda;
    QSqlTableModel * model_comisiones;
    QSqlTableModel * model_codTarifa;
    QSqlQueryModel * model_empresas;

    BarraBusqueda* m_busqueda;
    void setUpBusqueda();
    bool editando;
};

#endif // FRMAGENTES_H
