#ifndef FRMTPV_H
#define FRMTPV_H

#include "../Auxiliares/Globlal_Include.h"
#include "../mayamodule.h"
#include "../TPV/tpv.h"
# include "../Auxiliares/refresca_ofertas.h"
namespace Ui {
class FrmTPV;
}

class FrmTPV : public MayaModule
{
    Q_OBJECT



public:
    explicit FrmTPV(QWidget *parent = 0);
    ~FrmTPV();

    module_zone module_zone(){return Ventas;}
    QString module_name(){return "FormTPV";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
    void cargar_ticket(int id);
    void cargar_lineas(int id_cab);
    tpv *oTpv;


public slots:
    void cargar_ofertas();

private slots:
    void on_btnClientes_clicked();

    void on_btnBuscar_clicked();

    void on_txtCodigo_editingFinished();

    void on_btnScanear_toggled(bool checked);

    void on_btnbotones_clicked();

    void on_btnticket_clicked();
    void fin_fade_ticket();
    void fin_fade_ticket2();

    void fin_fade_buttons();
    void on_btnlista_clicked();

    void fin_fade_lista();
    void on_btn1_clicked();

    void on_btnBack_clicked();

    void on_btn2_clicked();

    void on_btn3_clicked();

    void on_btn4_clicked();

    void on_btn5_clicked();

    void on_btn6_clicked();

    void on_btn7_clicked();

    void on_btn8_clicked();

    void on_btn9_clicked();

    void on_btnIntro_clicked();

    void on_btnSumar_clicked();

    void on_btnRestar_clicked();

    void on_btnMultiplicar_clicked();

    void on_btnDividir_clicked();

    void on_btnPorc_clicked();

    void on_btnCalculadora_clicked(bool checked);

    void on_btn0_clicked();

    void on_lblDependiente_linkActivated(const QString &link);

    void on_btnDto_clicked(bool checked);

    void on_btnCantidad_clicked(bool checked);

    void on_btnBorrar_linea_clicked();

    void on_btnPrecio_clicked(bool checked);

    void on_btnCobrar_clicked();
    void final_anim_teclado();

    void on_btnTeclado_clicked();
    void final_anim_cobro();

    void on_txtEfectivo_editingFinished();

    void on_txtTarjeta_editingFinished();

    void calcularcambio();

    void on_txtCheque_editingFinished();

    void on_txtCredito_editingFinished();

    void on_txtVales_editingFinished();

    void on_txtInternet_editingFinished();

    void on_btnAnadir_ticket_clicked();

    void on_lista_tickets_doubleClicked(const QModelIndex &index);

    void on_btnDesglose_clicked();

    void on_btnBuscarArt_clicked();
    void final_anim_busqueda();

    void on_rt_clicked();

    void on_txtbuscar_art_textEdited(const QString &arg1);

    void on_btnAbrirCaja_clicked();

    void on_btnContinuarEdicionTicket_clicked();

    void on_btnEfectivo_clicked(bool checked);

    void on_btnTarjeta_clicked(bool checked);

    void on_btnCheque_clicked(bool checked);

    void on_btnCredito_clicked(bool checked);

    void on_btnVales_clicked(bool checked);

    void on_btnInternet_clicked(bool checked);

    void on_btnPunto_decimal_clicked();

    void on_btnPoner_en_espera_clicked();

    void on_btnCobrar__clicked();

    void on_btnCobrar_imprimir_nuevo_clicked();

    void on_btnComprar_imprimir_clicked();

    void on_btnCobrar_nuevo_clicked();

    void on_btnTraspasar_clicked();

    void on_btnAsignarCliente_clicked();

    void final_anim_edicion_ticket();

    void final_anim_abrir_caja();
    void on_btnCancelar_caja_clicked();

    void final_anim_abrir_caja_cancelado();
    void on_btnConfirmarAbertura_caja_clicked();

private:
    Ui::FrmTPV *ui;
    QAction menuButton;
    QPushButton* push;
    QTableView *tablabuscar;
    qreal valor1;
    qreal valor2;
    QString symbol;
    int tipo_dto_tarifa;
    int id,row_tabla;
    bool eventFilter(QObject *obj, QEvent *event);
    int teclado_height;
    int ticket_height;
    int control_width;
    bool scanning;

    QSqlQueryModel *model_lista_tpv;
    QSqlQueryModel *model_ofertas;
    refresca_ofertas *oRefresca;

    void estructura_lista();
    void llenar_campos();
signals:
    void mostrar_desglose(double subtotal1,double subtotal2, double subtotal3,double subtotal4,double dto1,double dto2,
                          double dto3,double dto4,double base1, double base2, double base3, double base4, float porc_iva1,
                          float porc_iva2, float porc_iva3, float porc_iva4, double iva1, double iva2, double iva3, double iva4,
                          double total1,double total2,double total3,double total4);



};

#endif // FRMTPV_H
