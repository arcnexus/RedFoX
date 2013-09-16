#include "roundedrect.h"
#include "editrectdlg.h"
#include "section.h"
RoundedRect::RoundedRect(QGraphicsItem *parent) :
    Container(parent)
{
    m_penWidth = 1;
    m_RadiousX = m_RadiousY = 20;
    m_penColor = Qt::black;
    m_useGradient = false;
    m_color1 = m_color2 = Qt::transparent;
    m_GradientDirection = Vertical;
}
QDomElement RoundedRect::xml(QDomDocument doc, QPointF relPos, QList<Section *> sectionPool)
{
    QDomElement mainNode = doc.createElement("Element");
    mainNode.setAttribute("id","RoundRect");

    Container::apendXML(mainNode , doc, relPos);

    QString endSec;
    QString sPoint;
    Section* start;
    Section* end;

    QPointF top = mapRectToScene(this->rect()).topLeft();
    QPointF bt = mapRectToScene(this->rect()).bottomLeft();
    for(int i = 0; i< sectionPool.size();i++)
    {
        QRectF seCrec = QRectF(sectionPool.at(i)->pos(),sectionPool.at(i)->rect().size());
        if(seCrec.contains(top))
            start = sectionPool.at(i);
        if(seCrec.contains(bt))
        {
            endSec = sectionPool.at(i)->SectionName();
            if(endSec!= "Pie de pagina" && endSec != "Pie de report")
            {
                end = start;
                break;
            }
            QPointF endPoint = bt - seCrec.topRight();
            sPoint = QString("%1").arg(endPoint.y());
            end = sectionPool.at(i);
            break;
        }
    }

    if(start == end)
        endSec = "Self";

    mainNode.setAttribute("endPointName",endSec);
    mainNode.setAttribute("endPointPoint",sPoint);
    mainNode.setAttribute("PenWidth",m_penWidth);
    mainNode.setAttribute("PenColor",ColorString(m_penColor));
    mainNode.setAttribute("Color1",ColorString(m_color1));
    mainNode.setAttribute("Color2",ColorString(m_color2));
    mainNode.setAttribute("GradientUsed",m_useGradient);
    mainNode.setAttribute("GradientDirection",m_GradientDirection == Vertical ? "V" : "H");
    mainNode.setAttribute("RadiousX",m_RadiousX);
    mainNode.setAttribute("RadiousY",m_RadiousY);

    return mainNode;
}

void RoundedRect::parseXml(QDomElement element , QPointF origin)
{
    this->setPos(element.attribute("x").toDouble() + origin.x(),element.attribute("y").toDouble()+origin.y());
    this->setSize(element.attribute("w").toDouble(),element.attribute("h").toDouble());

    this->setpenWidth(element.attribute("PenWidth").toDouble());
    this->setpenColor(ColorFromString(element.attribute("PenColor")));
    this->setcolor1(ColorFromString(element.attribute("Color1")));
    this->setcolor2(ColorFromString(element.attribute("Color2")));
    this->setuseGradient( element.attribute("GradientUsed").toDouble());
    this->setGradientDirection(element.attribute("GradientDirection")== "V" ? Vertical : Horizontal);
    this->setRadiousX(element.attribute("RadiousX").toDouble());
    this->setRadiousY(element.attribute("RadiousY").toDouble());
}

void RoundedRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{   
   painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
   QPen pen(painter->pen());
    pen.setWidth(m_penWidth);
    pen.setColor(m_penColor);
    painter->setPen(pen);

    if(m_useGradient)
    {
        if(m_GradientDirection == Vertical)
        {
            QLinearGradient Gradient(this->rect().width()/2,0,this->rect().width()/2,this->rect().height());
            Gradient.setColorAt(0,m_color1);
            Gradient.setColorAt(1,m_color2);
            painter->setBrush(QBrush(Gradient));
        }
        else
        {
            QLinearGradient Gradient(0,this->rect().height()/2,this->rect().width(),this->rect().height()/2);
            Gradient.setColorAt(0,m_color1);
            Gradient.setColorAt(1,m_color2);
            painter->setBrush(QBrush(Gradient));
        }
    }
    else
    {
        painter->setBrush(QBrush(m_color1));
    }
    QRectF r(m_penWidth/2.0,
             m_penWidth/2.0,
             this->boundingRect().width()-m_penWidth,
             this->boundingRect().height()-m_penWidth);
    painter->drawRoundedRect(r,m_RadiousX,m_RadiousY);

    painter->restore();
    Container::paint(painter,option,widget);
}

int RoundedRect::penWidth() const {
    return m_penWidth;
}

void RoundedRect::setpenWidth(int arg) {
    if (m_penWidth != arg) {
        m_penWidth = arg;
        emit penWidthChanged(arg);
    }
}

qreal RoundedRect::RadiousX() const {
    return m_RadiousX;
}

void RoundedRect::setRadiousX(qreal arg) {
    if (m_RadiousX != arg) {
        m_RadiousX = arg;
        emit RadiousXChanged(arg);
    }
}

qreal RoundedRect::RadiousY() const {
    return m_RadiousY;
}

void RoundedRect::setRadiousY(qreal arg) {
    if (m_RadiousY != arg) {
        m_RadiousY = arg;
        emit RadiousYChanged(arg);
    }
}

QColor RoundedRect::penColor() const {
    return m_penColor;
}

void RoundedRect::setpenColor(QColor arg) {
    if (m_penColor != arg) {
        m_penColor = arg;
        emit penColorChanged(arg);
    }
}

bool RoundedRect::useGradient() const {
    return m_useGradient;
}

void RoundedRect::setuseGradient(bool arg) {
    if (m_useGradient != arg) {
        m_useGradient = arg;
        emit useGradientChanged(arg);
    }
}

QColor RoundedRect::color1() const {
    return m_color1;
}

void RoundedRect::setcolor1(QColor arg) {
    if (m_color1 != arg) {
        m_color1 = arg;
        emit color1Changed(arg);
    }
}

QColor RoundedRect::color2() const {
    return m_color2;
}

void RoundedRect::setcolor2(QColor arg) {
    if (m_color2 != arg) {
        m_color2 = arg;
        emit color2Changed(arg);
    }
}

RoundedRect::_Orientacion RoundedRect::GradientDirection() const {
    return m_GradientDirection;
}

void RoundedRect::drawElement(QDomElement e, QPainter *painter)
{
    RoundedRect r(0);
    r.parseXml(e,QPointF(0,0));
    painter->save();
    r.paint(painter,0,0);
    painter->restore();
}

void RoundedRect::setGradientDirection(RoundedRect::_Orientacion arg) {
    if (m_GradientDirection != arg) {
        m_GradientDirection = arg;
        emit GradientDirectionChanged(arg);
    }
}


void RoundedRect::editMe()
{
    EditRectDlg dlg(this,qApp->activeWindow());
    dlg.exec();
}
