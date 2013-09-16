#include "reportline.h"
#include "editlinedlg.h"
#include "section.h"
ReportLine::ReportLine(QGraphicsItem *parent):
    Container(parent)
{
    m_penWidth = 1;
    m_penColor = Qt::black;
    m_Orientacion = Horizontal;
    m_penStyle = Qt::SolidLine;
}

QDomElement ReportLine::xml(QDomDocument doc, QPointF relPos,QList<Section*> sectionPool)
{
    QDomElement node = doc.createElement("Element");
    node.setAttribute("id","Line");

    Container::apendXML(node,doc, relPos);

    QString endSec;
    QString sPoint;
    Section* start;
    Section* end;
    if(m_Orientacion == Vertical)
    {
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
    }
    else
        endSec = "Self";

    if(start == end)
        endSec = "Self";

    node.setAttribute("penWidth",m_penWidth);
    node.setAttribute("endPointName",endSec);
    node.setAttribute("endPointPoint",sPoint);
    node.setAttribute("penColor",ColorString(m_penColor));
    node.setAttribute("penStyle",m_penStyle);
    node.setAttribute("Orientacion",m_Orientacion == Vertical ? "V" : "H");

    return node;
}

void ReportLine::parseXml(QDomElement element , QPointF origin)
{
    this->setPos(element.attribute("x").toDouble() + origin.x(),element.attribute("y").toDouble()+origin.y());
    this->setSize(element.attribute("w").toDouble(),element.attribute("h").toDouble());

    this->setpenWidth(element.attribute("penWidth").toDouble());
    this->setpenColor(ColorFromString(element.attribute("penColor")));
    this->setpenStyle(static_cast<Qt::PenStyle>(element.attribute("penStyle").toDouble()));

    element.attribute("Orientacion") == "V" ? setOrientacion(Vertical)
                                 : setOrientacion(Horizontal);

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
