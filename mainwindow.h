#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSplashScreen>

#include "archivosgeneralesext.h"

#include "Zona_Administrador/frmconfigmaya.h"
#include "Zona_Administrador/frmempresas.h"
#include "Zona_Administrador/frmusuarios.h"
#include "../Zona_Administrador/block_Maya_form.h"

#include "../Auxiliares/Globlal_Include.h"
#include "../Auxiliares/init_form.h"

#include "db_table_view.h"

#include "Zona_Pacientes/frmClientes.h"
#include "Zona_Pacientes/clinicaext.h"

#include "../Almacen/frmarticulos.h"
#include "../Almacen/frmtipostarifa.h"
#include "../Almacen/divisionalmacenext.h"
#include "../Almacen/frminventario.h"

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
#include "TPV/frmtpv.h"
#include "../Zona_Maestros/frmformas_pago.h"


//#include "openrptLibs/reportmodule.h"
#include "EditorReports/repdesignmodule.h"
#include "Agenda/agendaform.h"
#include "Agenda/permisosagendaform.h"

#include "../Zona_Contabilidad/frmentrada_apuntes.h"
#include "../Zona_Contabilidad/frmcuadro_cuentas.h"

#include "../Zona_Maestros/frmtransportistas.h"

#include "barraavisos.h"
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
protected:
    bool eventFilter(QObject *t, QEvent *e);
private slots:
    void actualizar_divisas(float valor_divisa, QString divisaDest);

    void block_main();
    void unblock_main();
    void on_btn_bloquear_clicked();

    void handle_toolBar();

    void handle_permisosAgenda();

    void llenaravisos();
    void hideAvisos();
    void tpv_mostrar_desglose(double subtotal1,double subtotal2, double subtotal3,double subtotal4,double dto1,double dto2,
                              double dto3,double dto4,double base1, double base2, double base3, double base4, float porc_iva1,
                              float porc_iva2, float porc_iva3, float porc_iva4, double iva1, double iva2, double iva3, double iva4,
                              double total1,double total2,double total3,double total4);
private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *e);
    void blockMe(bool state);
    void showAvisos();

    void loadVentasModules(QSplashScreen *splash);
    void loadComprasModules(QSplashScreen *splash);
    void loadAlmacenModules(QSplashScreen *splash);
    void loadMantenModules(QSplashScreen *splash);
    void loadUtilsModules(QSplashScreen *splash);
    void loadAminModules(QSplashScreen *splash);
    void loadContaModules(QSplashScreen *splash);
    void loadSecMedModules(QSplashScreen *splash);

    void crear_barraMantenimiento();
    void crear_barraVentas();
    void crear_barraAlmacen();
    void crear_barraCompras();
    void crear_barraUtils();
    void crear_barraAdmin();
    void crear_barraContabilidad();
    void crear_barraClinica();

    void addShortCut(QPushButton* button);

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

    QHash<QPushButton*,QWidget*> _shortCuts;

    bool avisos_reducido;
    BarraAvisos * m_avisos;
};

#endif // MAINWINDOW_H
