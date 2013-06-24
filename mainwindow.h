#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSplashScreen>

#include "archivosgeneralesext.h"

#include "../Auxiliares/Globlal_Include.h"
#include "../Auxiliares/init_form.h"
#include "../Auxiliares/toolbarbutton.h"
#include "db_table_view.h"

#include "Zona_Pacientes/frmClientes.h"
#include "Zona_Pacientes/clinicaext.h"

#include "../Almacen/frmarticulos.h"
#include "../Almacen/frmtipostarifa.h"
#include "../Almacen/divisionalmacenext.h"

#include "../Zona_Compras/frmproveedores.h"
#include "../Zona_Compras/frmpedidosproveedor.h"
#include "../Zona_Compras/frmalbaranproveedor.h"
#include "../Zona_Compras/frmfacturasproveedor.h"
#include "../Zona_Compras/frmorden_pedido_producto.h"
#include "../Zona_Compras/frmrecepcion_pedidos.h"
#include "../Zona_Compras/frmpedidosproveedor.h"


#include "../Zona_Ventas/frmgestioncobros.h"
#include "Zona_Ventas/frmfacturaralabaranes.h"
#include "Zona_Ventas/frmalbaran.h"
#include "Zona_Ventas/frmpedidos.h"
#include "Zona_Ventas/frmpresupuestoscli.h"
#include "Zona_Ventas/frmfacturas.h"
#include "TPV/frmcajaminuta.h"


#include "openrptLibs/reportmodule.h"
#include "Agenda/agendaform.h"
#include "Agenda/permisosagendaform.h"

#include "../Zona_Contabilidad/frmentrada_apuntes.h"

#include "../Zona_Administrador/frmempresas.h"
#include "../Zona_Administrador/frmconfigmaya.h"
#include "../Zona_Administrador/frmusuarios.h"
#include "../Zona_Administrador/block_Maya_form.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString usuario;
    QString empresa;

    QString user;
    QString pass;
    void showInfo();

    void updateDivisas();
private slots:
    void actualizar_divisas(float valor_divisa, QString divisaDest);

    void block_main();
    void unblock_main();
    void on_btn_bloquear_clicked();

    void handle_toolBar();

    void on_comboBox_currentIndexChanged(int index);
    void handle_permisosAgenda();
private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *e);
    void blockMe(bool state);

    void loadVentasModules(QSplashScreen *splash);
    void loadComprasModules(QSplashScreen *splash);
    void loadAlmacenModules(QSplashScreen *splash);
    void loadMantenModules(QSplashScreen *splash);
    void loadUtilsModules(QSplashScreen *splash);
    void loadAminModules(QSplashScreen *splash);
    void loadContaModules(QSplashScreen *splash);
    void loadSecMedModules(QSplashScreen *splash);

    //Barra de mantenimiento
    void crear_barraMantenimiento();
    void crear_barraVentas();
    void crear_barraAlmacen();
    void crear_barraCompras();
    void crear_barraUtils();
    void crear_barraAdmin();
    void crear_barraContabilidad();
    void crear_barraClinica();

    init_form * MayaForm;
    bool on_edit;
    QVector<MayaModule*> _mantenModules;
    QVector<MayaModule*> _ventasModules;
    QVector<MayaModule*> _comprasModules;
    QVector<MayaModule*> _almacenModules;
    QVector<MayaModule*> _utilsModules;
    QVector<MayaModule*> _adminModules;
    QVector<MayaModule*> _contaModules;
    QVector<MayaModule*> _clinicaModules;

    QVector<ModuleExtension* > _almacenExtensions;
    QVector<ModuleExtension* > _mantenExtensions;
    QVector<ModuleExtension* > _clinicaExtensions;
};

#endif // MAINWINDOW_H
