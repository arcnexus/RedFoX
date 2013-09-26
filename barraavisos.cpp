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
