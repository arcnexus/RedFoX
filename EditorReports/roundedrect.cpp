#include "roundedrect.h"
#include "editrectdlg.h"
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
QDomElement RoundedRect::xml(QDomDocument doc, QPointF relPos)
{
    QDomElement mainNode = doc.createElement("Element");
    mainNode.setAttribute("id","RoundRect");

    Container::apendXML(mainNode , doc, relPos);

    QDomElement pen = doc.createElement("Pen");
    pen.setAttribute("width",m_penWidth);
    pen.setAttribute("color",ColorString(m_penColor));

    QDomElement color = doc.createElement("Color");
    color.setAttribute("c1",ColorString(m_color1));
    color.setAttribute("c2",ColorString(m_color2));

    QDomElement gradient = doc.createElement("Gradient");
    gradient.setAttribute("used",m_useGradient);
    gradient.setAttribute("direction",m_GradientDirection == Vertical ? "V" : "H");

    QDomElement radious = doc.createElement("Radious");
    radious.setAttribute("x",m_RadiousX);
    radious.setAttribute("y",m_RadiousY);

    mainNode.appendChild(radious);
    mainNode.appendChild(pen);
    mainNode.appendChild(color);
    mainNode.appendChild(gradient);
    return mainNode;
}

void RoundedRect::parseXml(QDomElement element , QPointF origin)
{
    QDomNodeList list = element.childNodes();
    for(int i=0;i<list.size();i++)
    {
        QDomElement el = list.at(i).toElement();
        QString tag = el.tagName();//"Pos"Size"Pen""Color""Gradient""Radious"
        if(tag == "Pos")
            this->setPos(el.attribute("x").toInt() + origin.x(),el.attribute("y").toInt()+origin.y());
        else if(tag=="Size")
            this->setSize(el.attribute("w").toInt(),el.attribute("h").toInt());
        else if(tag== "Pen")
        {
            this->setpenColor(ColorFromString(el.attribute("color")));
            this->setpenWidth(el.attribute("width").toInt());
        }
        else if(tag== "Color")
        {
            this->setcolor1(ColorFromString(el.attribute("c1")));
            this->setcolor2(ColorFromString(el.attribute("c2")));
        }
        else if(tag== "Gradient")
        {
            this->setuseGradient(el.attribute("used").toInt());
            this->setGradientDirection(el.attribute("direction") == "V" ? Vertical : Horizontal);
        }
        else if(tag== "Radious")
        {
            this->setRadiousX(el.attribute("x").toDouble());
            this->setRadiousY(el.attribute("y").toDouble());
        }
    }
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
