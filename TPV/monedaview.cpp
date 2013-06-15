#include "monedaview.h"

MonedaView::MonedaView(QWidget *parent, QString nombre, double value) :
    QWidget(parent),
    _layout(this),
    _nombreMoneda(nombre,this),
    _valorMoneda(this)
{
    _valorCambio = value;
    connect(&_valorMoneda,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
    _valorMoneda.setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
    _valorMoneda.setMinimum(0);
    _valorMoneda.setMaximum(9999);

    _layout.addWidget(&_nombreMoneda);
    _layout.addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding));
    _layout.addWidget(&_valorMoneda);

    this->setLayout(&_layout);
    m_valorMoneda=0;
}


void MonedaView::setvalorMoneda(double arg)
{
    if (m_valorMoneda != arg) {
        m_valorMoneda = arg;
        emit valorMonedaChanged(arg);
    }
}

void MonedaView::valueChanged(int arg)
{
    setvalorMoneda(arg * _valorCambio);
}
