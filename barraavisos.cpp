#include "barraavisos.h"
#include "ui_barraavisos.h"
#include <QPainter>
BarraAvisos::BarraAvisos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BarraAvisos)
{    
    ui->setupUi(this);

//    this->setPalette(QPalette( QColor::fromRgba(qRgba(170,170,127,190)) ));
//    this->setAutoFillBackground(true);

//    ui->stackedWidget->setPalette(QPalette( QColor::fromRgba(qRgba(170,170,127,190)) ));
//    ui->page->setPalette(QPalette( QColor::fromRgba(qRgba(170,170,127,190)) ));
}

BarraAvisos::~BarraAvisos()
{
    delete ui;
}

void BarraAvisos::setPagina(int pagina)
{
    ui->stackedWidget->setCurrentIndex(pagina);
}

void BarraAvisos::set_tpv_desgloses(double subtotal1, double subtotal2, double subtotal3, double subtotal4, double dto1,
                               double dto2, double dto3, double dto4, double base1, double base2, double base3,
                               double base4, float porc_iva1, float porc_iva2, float porc_iva3, float porc_iva4,
                               double iva1, double iva2, double iva3, double iva4, double total1, double total2,
                               double total3, double total4)
{
    ui->txtSubtotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal1,'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
    ui->txtSubtotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal2,'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
    ui->txtSubtotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal3,'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
    ui->txtSubtotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal4,'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
    ui->txtDto1->setText(Configuracion_global->toFormatoMoneda(QString::number(dto1,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtDto2->setText(Configuracion_global->toFormatoMoneda(QString::number(dto2,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtDto3->setText(Configuracion_global->toFormatoMoneda(QString::number(dto3,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtDto4->setText(Configuracion_global->toFormatoMoneda(QString::number(dto4,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtBase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base1,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtBase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base2,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtBase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base3,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtBase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base4,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->lblIva1->setText(QString::number(porc_iva1,'f',2));
    ui->lblIva1_2->setText(QString::number(porc_iva2,'f',2));
    ui->lblIva1_3->setText(QString::number(porc_iva3,'f',2));
    ui->lblIva1_4->setText(QString::number(porc_iva4,'f',2));
    ui->lbldesglose1->setText(tr("Desglose IVA %1").arg(QString::number(porc_iva1,'f',2)));
    ui->lbldesglose2->setText(tr("Desglose IVA %1").arg(QString::number(porc_iva2,'f',2)));
    ui->lbldesglose3->setText(tr("Desglose IVA %1").arg(QString::number(porc_iva3,'f',2)));
    ui->lbldesglose4->setText(tr("Desglose IVA %1").arg(QString::number(porc_iva4,'f',2)));
    ui->txtIva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva1,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtIva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva2,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtIva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva3,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtIva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva4,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtTotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total1,'f',
                                                                                 Configuracion_global->decimales_campos_totales)));
    ui->txtTotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total2,'f',
                                                                                 Configuracion_global->decimales_campos_totales)));
    ui->txtTotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total3,'f',
                                                                                 Configuracion_global->decimales_campos_totales)));
    ui->txtTotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total4,'f',
                                                                                 Configuracion_global->decimales_campos_totales)));
}

void BarraAvisos::paintEvent(QPaintEvent *pe) {
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

void BarraAvisos::on_btnClose_clicked()
{
    emit closeMe();
}
