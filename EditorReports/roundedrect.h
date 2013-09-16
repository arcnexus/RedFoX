#ifndef ROUNDEDRECT_H
#define ROUNDEDRECT_H
#include "container.h"
class RoundedRect : public Container
{
    Q_OBJECT

    Q_PROPERTY(int penWidth READ penWidth WRITE setpenWidth NOTIFY penWidthChanged)
    Q_PROPERTY(qreal RadiousX READ RadiousX WRITE setRadiousX NOTIFY RadiousXChanged)
    Q_PROPERTY(qreal RadiousY READ RadiousY WRITE setRadiousY NOTIFY RadiousYChanged)
    Q_PROPERTY(QColor penColor READ penColor WRITE setpenColor NOTIFY penColorChanged)

    Q_PROPERTY(bool useGradient READ useGradient WRITE setuseGradient NOTIFY useGradientChanged)
    Q_PROPERTY(QColor color1 READ color1 WRITE setcolor1 NOTIFY color1Changed)
    Q_PROPERTY(QColor color2 READ color2 WRITE setcolor2 NOTIFY color2Changed)
    Q_PROPERTY(_Orientacion GradientDirection READ GradientDirection WRITE setGradientDirection NOTIFY GradientDirectionChanged)

    //Q_ENUMS (_Orientacion)
public:
    explicit RoundedRect(QGraphicsItem *parent = 0);
    QDomElement xml(QDomDocument doc,QPointF,QList<Section*> sectionPool);
    void parseXml(QDomElement, QPointF);
    void editMe();
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    int penWidth() const;
    qreal RadiousX() const;
    qreal RadiousY() const;

    QColor penColor() const;
    bool useGradient() const;

    QColor color1() const;
    QColor color2() const;

    _Orientacion GradientDirection() const;

    static void drawElement(QDomElement e, QPainter * painter);
public slots:
    void setpenWidth(int arg);
    void setRadiousX(qreal arg);
    void setRadiousY(qreal arg);

    void setpenColor(QColor arg);
    void setuseGradient(bool arg);

    void setcolor1(QColor arg);
    void setcolor2(QColor arg);

    void setGradientDirection(_Orientacion arg);

signals:
    void penWidthChanged(int arg);
    void RadiousXChanged(qreal arg);
    void RadiousYChanged(qreal arg);

    void penColorChanged(QColor arg);
    void useGradientChanged(bool arg);

    void color1Changed(QColor arg);
    void color2Changed(QColor arg);

    void GradientDirectionChanged(_Orientacion arg);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    int m_penWidth;
    qreal m_RadiousX;
    qreal m_RadiousY;
    QColor m_penColor;
    bool m_useGradient;
    QColor m_color1;
    QColor m_color2;
    _Orientacion m_GradientDirection;
};

#endif // ROUNDEDRECT_H
