#ifndef REPORTLINE_H
#define REPORTLINE_H

#include "container.h"
class ReportLine : public Container
{
    Q_OBJECT

    Q_PROPERTY(int penWidth READ penWidth WRITE setpenWidth NOTIFY penWidthChanged)
    Q_PROPERTY(QColor penColor READ penColor WRITE setpenColor NOTIFY penColorChanged)
    Q_PROPERTY(Qt::PenStyle penStyle READ penStyle WRITE setpenStyle NOTIFY penStyleChanged)
    Q_PROPERTY(_Orientacion Orientacion READ Orientacion WRITE setOrientacion NOTIFY OrientacionChanged)

public:
    ReportLine(QGraphicsItem *parent = 0);
    QDomElement xml(QDomDocument doc, QPointF relPos,QList<Section*> sectionPool);
    void parseXml(QDomElement element, QPointF origin);
    void editMe();
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    int penWidth() const;
    QColor penColor() const;
    _Orientacion Orientacion() const;
    Qt::PenStyle penStyle() const;

public slots:

    void setpenWidth(int arg);
    void setpenColor(QColor arg);
    void setOrientacion(_Orientacion arg);
    void setpenStyle(Qt::PenStyle arg);

signals:
    void penWidthChanged(int arg);
    void penColorChanged(QColor arg);
    void OrientacionChanged(_Orientacion arg);
    void penStyleChanged(Qt::PenStyle arg);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:

    int m_penWidth;
    QColor m_penColor;
    _Orientacion m_Orientacion;
    Qt::PenStyle m_penStyle;
};

#endif // REPORTLINE_H
