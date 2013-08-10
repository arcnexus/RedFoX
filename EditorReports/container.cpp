#include "container.h"

Container::Container(QGraphicsItem *parent) :
    QGraphicsRectItem(parent)
{
    this->_onResize = false;

    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemIsMovable);
}

void Container::setSize(int w, int h)
{
    this->setRect(0,0,w,h);
}

void Container::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    if(this->isSelected())
    {
        if(_onResize)
            painter->fillRect(this->rect(),QColor(199,196,196,150));
        QRectF r = this->rect();
        qreal a = 16;
        qreal b = 5;
        painter->drawRect(r.width()/2 -8,1           ,a,b);
        painter->drawRect(1,r.bottom()/2-8           ,b,a);
        painter->drawRect(r.right()-6,r.bottom()/2-8 ,b,a);
        painter->drawRect(r.width()/2 -8,r.bottom()-6,a,b);
        painter->setPen(QPen(Qt::DotLine));
        QGraphicsRectItem::paint(painter,option,widget);
    }
    painter->restore();
}


void Container::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

}

void Container::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{

}

void Container::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    if(this->isSelected())
    {
        QRectF r = this->rect();
        QRectF corner1(r.width()/2 -8,0,16,5);
        QRectF corner2(0,r.bottom()/2-8,5,16);
        QRectF corner3(r.right()-5,r.bottom()/2-8,5,16);
        QRectF corner4(r.width()/2 -8,r.bottom()-5,16,5);
        if(corner1.contains(event->pos()))
        {
            _onResize = true;
            _resizeType = 0;
        }
        else if(corner2.contains(event->pos()))
        {
            _onResize = true;
            _resizeType = 1;
        }
        else if(corner3.contains(event->pos()))
        {
            _onResize = true;
            _resizeType = 2;
        }
        else if(corner4.contains(event->pos()))
        {
            _onResize = true;
            _resizeType = 3;
        }
    }
}

void Container::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    _onResize = false;
    QGraphicsItem::mouseReleaseEvent(event);
}

void Container::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    prepareGeometryChange();
    if(_onResize)
    {        
        switch (_resizeType) {
        case 0://TOP
            if(this->rect().bottom()-event->pos().ry() > 15 && mapToScene(event->pos()).ry()>0)
            {
                this->setRect(0,
                              0,
                              this->rect().width(),
                              this->rect().bottom()-event->pos().ry()
                              );
                this->setPos(pos().x(),mapToScene(event->pos()).ry());
            }
            break;
        case 1://LEFT
            if(this->rect().right() - event->pos().rx()> 15 && mapToScene(event->pos()).rx()>margins.left())
            {
                this->setRect(0,
                              0,
                              this->rect().right() - event->pos().rx(),
                              this->rect().height()
                              );
                this->setPos(mapToScene(event->pos()).rx(),pos().y());
            }
            break;
        case 2://RIGTH
            if(this->rect().left() + event->pos().rx()> 15 && mapToScene(event->pos()).rx()<margins.right())
            {
                this->setRect(0,
                              0,
                              event->pos().rx() - this->rect().left(),
                              this->rect().height()
                              );
            }
            break;
        default://BOTTOM
            if(this->rect().top() + event->pos().ry()> 15 && mapToScene(event->pos()).ry()<margins.bottom())
            {
                this->setRect(0,
                              0,
                              this->rect().width(),
                              event->pos().ry() - this->rect().top()
                              );
            }
            break;
        }
    }
    else
        QGraphicsItem::mouseMoveEvent(event);
}

void Container::apendXML(QDomElement &element, QDomDocument doc , QPointF relPos)
{
    QDomElement pos = doc.createElement("Pos");
    pos.setAttribute("x",(int)this->pos().x() - (int)relPos.x());
    pos.setAttribute("y",(int)this->pos().y() - (int)relPos.y());

    QDomElement siz = doc.createElement("Size");
    siz.setAttribute("w",(int)this->rect().width());
    siz.setAttribute("h",(int)this->rect().height());

    element.appendChild(pos);
    element.appendChild(siz);
}

QString Container::ColorString(QColor c)
{
    return QString("%1,%2,%3,%4").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
}

QColor Container::ColorFromString(QString s)
{
    QStringList l= s.split(",");
    QColor c;
    c.setRed(l.at(0).toInt());
    c.setGreen(l.at(1).toInt());
    c.setBlue(l.at(2).toInt());
    c.setAlpha(l.at(3).toInt());
    return c;
}

QVariant Container::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
            // value is the new position.
            QPointF newPos = value.toPointF();
            QPointF rigthBottom = QPointF(newPos.x()+this->rect().width(),newPos.y()+this->rect().height());
            if (!margins.contains(newPos) || !margins.contains(rigthBottom)) {
                newPos.setX(qMin(margins.right(), qMax(newPos.x(), margins.left()+1)));
                newPos.setY(qMin(margins.bottom(), qMax(newPos.y(), margins.top()+1)));
                if(!margins.contains(rigthBottom))
                {
                    newPos.setX(qMax(margins.left(), qMin(newPos.x(),margins.right()-this->rect().width())));
                    newPos.setY(qMax(margins.top(), qMin(newPos.y(),margins.bottom()-this->rect().height())));
                }
                return newPos;
            }
        }
    return QGraphicsItem::itemChange(change, value);
}

QRectF Container::boundingRect() const
{
    return this->rect();
}

QRectF Container::getMargins() const
{
    return margins;
}

void Container::setMargins(const QRectF &value)
{
    margins = value;
}


void Container::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    editMe();
}
