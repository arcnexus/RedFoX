#include "reportline.h"
#include "editlinedlg.h"
ReportLine::ReportLine(QGraphicsItem *parent):
    Container(parent)
{
    m_penWidth = 1;
    m_penColor = Qt::black;
    m_Orientacion = Horizontal;
    m_penStyle = Qt::SolidLine;
}

QDomElement ReportLine::xml(QDomDocument doc, QPointF relPos)
{
    QDomElement node = doc.createElement("Element");
    node.setAttribute("id","Line");

    Container::apendXML(node,doc, relPos);

    QDomElement penWidth = doc.createElement("penWidth");
    penWidth.setAttribute("value",m_penWidth);

    QDomElement penColor = doc.createElement("penColor");
    penColor.setAttribute("value",ColorString(m_penColor));

    QDomElement penStyle = doc.createElement("penStyle");
    penStyle.setAttribute("value",m_penStyle);

    QDomElement Orientacion = doc.createElement("Orientacion");
    Orientacion.setAttribute("value",m_Orientacion == Vertical ? "V" : "H");

    node.appendChild(penWidth);
    node.appendChild(penColor);
    node.appendChild(penStyle);
    node.appendChild(Orientacion);

    return node;
}

void ReportLine::parseXml(QDomElement element , QPointF origin)
{
    QDomNodeList list = element.childNodes();
    for(int i=0;i<list.size();i++)
    {
        QDomElement el = list.at(i).toElement();
        QString tag = el.tagName();
        //Common tag for every container subclass
        if(tag == "Pos")
            this->setPos(el.attribute("x").toInt() + origin.x(),el.attribute("y").toInt()+origin.y());
        else if(tag=="Size")
            this->setSize(el.attribute("w").toInt(),el.attribute("h").toInt());

        //Specific tags
        else if(tag== "penWidth")
            this->setpenWidth(el.attribute("value").toInt());
        else if(tag== "penColor")
            this->setpenColor(ColorFromString(el.attribute("value")));
        else if(tag== "penStyle")
            this->setpenStyle(static_cast<Qt::PenStyle>(el.attribute("value").toInt()));
        else if(tag== "Orientacion")
            el.attribute("value") == "V" ? setOrientacion(Vertical)
                                         : setOrientacion(Horizontal);
    }
}

void ReportLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPen pen;
    pen.setWidth(m_penWidth);
    pen.setStyle(m_penStyle);
    pen.setColor(m_penColor);
    painter->setPen(pen);
    if(m_Orientacion == Horizontal)
        painter->drawLine(0,rect().height()/2,rect().width(),rect().height()/2);
    else
        painter->drawLine(rect().width()/2,0,rect().width()/2,rect().height());
    painter->restore();
    Container::paint(painter,option,widget);
}

int ReportLine::penWidth() const {
    return m_penWidth;
}

QColor ReportLine::penColor() const {
    return m_penColor;
}

Container::_Orientacion ReportLine::Orientacion() const {
    return m_Orientacion;
}

void ReportLine::setpenWidth(int arg) {
    if (m_penWidth != arg) {
        m_penWidth = arg;
        emit penWidthChanged(arg);
    }
}

void ReportLine::setpenColor(QColor arg) {
    if (m_penColor != arg) {
        m_penColor = arg;
        emit penColorChanged(arg);
    }
}

void ReportLine::setOrientacion(Container::_Orientacion arg) {
    if (m_Orientacion != arg) {
        m_Orientacion = arg;
        emit OrientacionChanged(arg);
    }
}

Qt::PenStyle ReportLine::penStyle() const {
    return m_penStyle;
}

void ReportLine::setpenStyle(Qt::PenStyle arg) {
    if (m_penStyle != arg) {
        m_penStyle = arg;
        emit penStyleChanged(arg);
    }
}


void ReportLine::editMe()
{
    EditLineDlg dlg(this,qApp->activeWindow());
    dlg.exec();
}
