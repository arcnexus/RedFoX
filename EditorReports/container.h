#ifndef CONTAINER_H
#define CONTAINER_H

#include <QGraphicsRectItem>
#include <QApplication>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QLinearGradient>
#include <QDomElement>
#include <QDomDocument>

class Container : public QObject , public QGraphicsRectItem
{
    Q_OBJECT

public:
    explicit Container(QGraphicsItem *parent = 0);

    void setSize(int w, int h);

    enum _Orientacion { Vertical , Horizontal };
    enum _Aling { Left , Center , Rigth };

    virtual QDomElement xml(QDomDocument,QPointF)=0;
    virtual void parseXml(QDomElement,QPointF)=0;
    virtual void editMe()=0;
    virtual QString query(){return "";}

    enum { Type = UserType + 1 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }
    QRectF getMargins() const;
    void setMargins(const QRectF &value);

signals:

public slots:

protected:
       void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void apendXML(QDomElement & element, QDomDocument doc, QPointF relPos);

    QString ColorString(QColor c);
    QColor  ColorFromString(QString s);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    bool _onResize;
    int _resizeType;
    QRectF boundingRect() const;
    QRectF margins;
};

#endif // CONTAINER_H
