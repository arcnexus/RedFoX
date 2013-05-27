#include "qchartpiece.h"

QChartPiece::QChartPiece(QObject *parent) :
    QObject(parent)
{
}

QChartPiece::QChartPiece(const QChartPiece & p)
{
    this->i_values = p.i_values;
    this->color = p.color;
    this->nombre = p.nombre;
    for(int i= 0; i<i_values.size();i++)
        values.append(0);
}

void QChartPiece::operator =(const QChartPiece &p)
{
    i_values.clear();
    values.clear();
    this->i_values = p.i_values;
    this->color = p.color;
    this->nombre = p.nombre;
    for(int i= 0; i<i_values.size();i++)
        values.append(0);
}

void QChartPiece::setPercent(float f)
{
    for (int i = 0;i<values.size();i++)
    {
        float f1 = f/100.0;
        values[i] = i_values.at(i)*f1;
    }
}

