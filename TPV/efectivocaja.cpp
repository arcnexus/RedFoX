#include "efectivocaja.h"
#include "ui_efectivocaja.h"
//#include <QApplication>
EfectivoCaja::EfectivoCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EfectivoCaja),
    _container()
{
    ui->setupUi(this);
    fillMonedas();
    ui->frame->setLayout(&_container);
    ui->txtFecha->setDateTime(QDateTime::currentDateTime());
}

EfectivoCaja::~EfectivoCaja()
{
    delete ui;
}

void EfectivoCaja::on_btnAceptar_clicked()
{
    ui->txtFecha->setDateTime(QDateTime::currentDateTime());
}

void EfectivoCaja::valueChanged(double)
{
    recalc();
}

void EfectivoCaja::fillMonedas()
{
    QSqlQuery q(QSqlDatabase::database("empresa"));
    q.exec("SELECT * FROM moneda_caja");
    while(q.next())
    {
        QSqlRecord r = q.record();
        _monedas.append(new MonedaView(this,r.value(1).toString(),r.value(2).toDouble()));
    }
    if(!_monedas.isEmpty())
    {
        int maxRows = (_monedas.size()+1)/2;
        int currentRow = 0;
        int currentColumn = 0;
        QVector<MonedaView*>::Iterator i = _monedas.begin();
        while(i != _monedas.end())
        {
            _container.addWidget(*i,currentRow,currentColumn);
            bool b = connect(*i,SIGNAL(valorMonedaChanged(double)),this,SLOT(valueChanged(double)));
            currentRow++;
            if(currentRow == maxRows)
            {
               currentRow = 0;
               currentColumn++;
            }
            ++i;
        }
        _container.addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding),
                           _container.rowCount(),0);
    }
    else
        ui->frame->hide();
}

void EfectivoCaja::recalc()
{
    double total = 0;
    QVector<MonedaView*>::Iterator i = _monedas.begin();
    while(i != _monedas.end())
    {
        total += (*i)->valorMoneda();
        ++i;
    }
    ui->spitotal->setValue(total);
}

void EfectivoCaja::on_checkBox_toggled(bool checked)
{
    ui->frame->setVisible(checked);
    QApplication::processEvents();
    this->resize(0,0);
}
