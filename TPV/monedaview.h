#ifndef MONEDAVIEW_H
#define MONEDAVIEW_H

#include <QWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

class MonedaView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double valorMoneda READ valorMoneda NOTIFY valorMonedaChanged)
public:
    explicit MonedaView(QWidget *parent = 0, QString nombre = "" , double value = 0);
    
    double valorMoneda() const
    {
        return _valorMoneda.value() * _valorCambio;
    }

signals:
    
    void valorMonedaChanged(double arg);

public slots:
private slots:
     void setvalorMoneda(double arg);
     void valueChanged(int arg);
private:
    QHBoxLayout _layout;
    QLabel _nombreMoneda;
    QSpinBox _valorMoneda;
    double _valorCambio;
    double m_valorMoneda;
};

#endif // MONEDAVIEW_H
