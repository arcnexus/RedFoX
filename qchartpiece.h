#ifndef QCHARTPIECE_H
#define QCHARTPIECE_H
#include "global_header.h"
#include <QObject>
#include <QColor>
#include <QVector>
class  SHAREDLIBSHARED_EXPORT QChartPiece : public QObject
{
    Q_OBJECT
public:
    explicit QChartPiece(QObject *parent = 0);
    QChartPiece(const  QChartPiece&p);
    void operator =(const  QChartPiece& p);

    float value() const{return (values.isEmpty()) ?  0 : values.at(0);}
    QVector<float> getValues() const{return values;}

    void addValue(float f){i_values.append(f);values.append(0);}
    void addValue(QVector<float> f){i_values=f;}
    void setPercent(float f);

    QString nombre;
    QColor color;
private:
    QVector<float> values;
    QVector<float> i_values;
signals:

public slots:

};

#endif // QCHARTPIECE_H
