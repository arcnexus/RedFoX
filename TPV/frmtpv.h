#ifndef FRMTPV_H
#define FRMTPV_H

#include "../Auxiliares/Globlal_Include.h"
#include "../mayamodule.h"

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

private slots:
    void on_btnClientes_clicked();

    void on_btnBuscar_clicked();

    void on_txtCodigo_editingFinished();

    void on_btnScanear_clicked(bool checked);

    void on_btnbotones_clicked();

    void on_btnticket_clicked();
    void fin_fade_ticket();

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

    void on_btnScanear_clicked();

    void on_lblDependiente_linkActivated(const QString &link);

    void on_btnDto_clicked(bool checked);

    void on_btnCantidad_clicked(bool checked);

    void on_btnBorrar_linea_clicked();

private:
    Ui::FrmTPV *ui;
    QAction menuButton;
    QPushButton* push;
    QTableView *tablabuscar;
    qreal valor1;
    qreal valor2;
    QString symbol;
    int tipo_dto_tarifa;
    int id;
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif // FRMTPV_H
