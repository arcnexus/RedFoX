#include "frmtpv.h"
#include "ui_frmtpv.h"
#include <QGraphicsOpacityEffect>

FrmTPV::FrmTPV(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmTPV),
    toolButton(tr("TPV"),":/Icons/PNG/tpv.png",this),
    menuButton(QIcon(":/Icons/PNG/tpv.png"),tr("TPV"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/tpv.png"),"",this))
{
    ui->setupUi(this);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de Caja"));

    ui->btnPorc->setEnabled(false);
    ui->btnSumar->setEnabled(false);
    ui->btnRestar->setEnabled(false);
    ui->btnMultiplicar->setEnabled(false);
    ui->btnDividir->setEnabled(false);


}

FrmTPV::~FrmTPV()
{
    delete ui;
}

void FrmTPV::on_btnClientes_clicked()
{

}



void FrmTPV::on_btnBuscar_clicked()
{

}


void FrmTPV::on_txtCodigo_editingFinished()
{

}

void FrmTPV::on_btnScanear_clicked(bool checked)
{
    if(checked)
        ui->txtCodigo->setFocus();
}


void FrmTPV::on_btnbotones_clicked()
{
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
    animationOp->setEasingCurve(QEasingCurve::Linear);
    animationOp->setDuration(1000);
    animationOp->setStartValue(1.0);
    animationOp->setEndValue(0.0);
    connect(animationOp,SIGNAL(finished()),this,SLOT(fin_fade_buttons()));
    animationOp->start();


}

void FrmTPV::on_btnticket_clicked()
{
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
    animationOp->setEasingCurve(QEasingCurve::Linear);
    animationOp->setDuration(500);
    animationOp->setStartValue(1.0);
    animationOp->setEndValue(0.0);
    connect(animationOp,SIGNAL(finished()),this,SLOT(fin_fade_ticket()));
    animationOp->start();
}

void FrmTPV::fin_fade_ticket()
{
    ui->frame_ticket->setCurrentIndex(0);
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp2 = new QPropertyAnimation(fade_effect, "opacity");
    animationOp2->setEasingCurve(QEasingCurve::Linear);
    animationOp2->setDuration(500);
    animationOp2->setStartValue(0.0);
    animationOp2->setEndValue(1.0);
    animationOp2->start();

}
void FrmTPV::fin_fade_buttons()
{
    ui->frame_ticket->setCurrentIndex(2);
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp2 = new QPropertyAnimation(fade_effect, "opacity");
    animationOp2->setEasingCurve(QEasingCurve::Linear);
    animationOp2->setDuration(500);
    animationOp2->setStartValue(0.0);
    animationOp2->setEndValue(1.0);
    animationOp2->start();

}

void FrmTPV::on_btnlista_clicked()
{
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
    animationOp->setEasingCurve(QEasingCurve::Linear);
    animationOp->setDuration(500);
    animationOp->setStartValue(1.0);
    animationOp->setEndValue(0.0);
    connect(animationOp,SIGNAL(finished()),this,SLOT(fin_fade_lista()));
    animationOp->start();
}
void FrmTPV::fin_fade_lista()
{
    ui->frame_ticket->setCurrentIndex(1);
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp2 = new QPropertyAnimation(fade_effect, "opacity");
    animationOp2->setEasingCurve(QEasingCurve::Linear);
    animationOp2->setDuration(500);
    animationOp2->setStartValue(0.0);
    animationOp2->setEndValue(1.0);
    animationOp2->start();

}

void FrmTPV::on_btn1_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().append("1"));
}

void FrmTPV::on_btnBack_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().left(ui->txtCodigo->text().length()-1));
}

void FrmTPV::on_btn2_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().append("2"));
}

void FrmTPV::on_btn3_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().append("3"));
}

void FrmTPV::on_btn4_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().append("4"));
}

void FrmTPV::on_btn5_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().append("5"));
}

void FrmTPV::on_btn6_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().append("6"));
}

void FrmTPV::on_btn7_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().append("7"));
}

void FrmTPV::on_btn8_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().append("8"));
}

void FrmTPV::on_btn9_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().append("9"));
}

void FrmTPV::on_btnIntro_clicked()
{
    qreal resultado;
    if (ui->btnCalculadora->isChecked())
    {
        if(this->valor1 != 0 && !this->symbol.isEmpty())
            this->valor2 = ui->txtCodigo->text().toDouble();
            if(this->symbol == "+")
                resultado = valor1 + valor2;
            if(this->symbol == "-")
                resultado = valor1 - valor2;
            if(this->symbol == "*")
                resultado = valor1 * valor2;
            if(this->symbol == "/")
                resultado = valor1 / valor2;
            if(this->symbol == "%")
                resultado = valor1 * (valor2/100);

            ui->txtCodigo->setText(Configuracion_global->toFormatoMoneda(QString::number(resultado,'f',
                                                                  Configuracion_global->decimales_campos_totales)));

    }

}

void FrmTPV::on_btnSumar_clicked()
{
    this->valor1 = Configuracion_global->MonedatoDouble(ui->txtCodigo->text());
    ui->txtCodigo->clear();
    ui->txtCodigo->setFocus();
    this->symbol = "+";
}

void FrmTPV::on_btnRestar_clicked()
{
    this->valor1 = Configuracion_global->MonedatoDouble(ui->txtCodigo->text());
    ui->txtCodigo->clear();
    ui->txtCodigo->setFocus();
    this->symbol = "-";
}

void FrmTPV::on_btnMultiplicar_clicked()
{
    this->valor1 = Configuracion_global->MonedatoDouble(ui->txtCodigo->text());
    ui->txtCodigo->clear();
    ui->txtCodigo->setFocus();
    this->symbol = "*";
}

void FrmTPV::on_btnDividir_clicked()
{
    this->valor1 = Configuracion_global->MonedatoDouble(ui->txtCodigo->text());
    ui->txtCodigo->clear();
    ui->txtCodigo->setFocus();
    this->symbol = "/";
}

void FrmTPV::on_btnPorc_clicked()
{
    this->valor1 = Configuracion_global->MonedatoDouble(ui->txtCodigo->text());
    ui->txtCodigo->setText(0);
    ui->txtCodigo->setFocus();
    this->symbol = "%";
}


void FrmTPV::on_btnCalculadora_clicked(bool checked)
{
    ui->btnPorc->setEnabled(checked);
    ui->btnSumar->setEnabled(checked);
    ui->btnRestar->setEnabled(checked);
    ui->btnMultiplicar->setEnabled(checked);
    ui->btnDividir->setEnabled(checked);
    ui->txtCodigo->clear();

}

void FrmTPV::on_btn0_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().append("0"));
}

