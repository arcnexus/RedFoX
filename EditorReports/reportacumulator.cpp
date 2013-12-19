#include "reportacumulator.h"
#include "section.h"
#include "editacum.h"
#include "detailsection.h"
ReportAcumulator::ReportAcumulator(QString name, QGraphicsItem *parent) :
    Container(name,parent)
{
    m_fontFamily = "Arial";
    m_fontSize = 10;
    m_fontWeigth = QFont::Normal;
    m_italicFont = false;
    m_underlined = false;
    m_Alineacion = Left;
    m_fontColor = Qt::black;
    m_formato = 0;
    m_target = "Empty";
    this->setName(name);
}

QDomElement ReportAcumulator::xml(QDomDocument doc, QPointF relPos, QList<Section *> sectionPool)
{
    QDomElement node = doc.createElement("Element");
    node.setAttribute("id","Acumulador");

    Container::apendXML(node,doc, relPos);

    node.setAttribute("Text","");
    node.setAttribute("Orientacion","H");
    node.setAttribute("Alineacion",m_Alineacion == Left ? "L" : m_Alineacion == Rigth ? "R" : "C");
    node.setAttribute("fontFamily",m_fontFamily);
    node.setAttribute("fontSize",m_fontSize);
    node.setAttribute("fontWeigth",m_fontWeigth);
    node.setAttribute("italicFont",m_italicFont);
    node.setAttribute("underlined",m_underlined);
    node.setAttribute("color",ColorString(m_fontColor));
    node.setAttribute("formato",m_formato);
    node.setAttribute("target",m_target);
    QPointF top = mapRectToScene(this->rect()).topLeft();
    for(int i = 0; i< sectionPool.size();i++)
    {
        QRectF seCrec = QRectF(sectionPool.at(i)->pos(),sectionPool.at(i)->rect().size());
        if(seCrec.contains(top))
        {
            QString endSec = sectionPool.at(i)->SectionName();
            if(endSec == "Pie de pagina" || endSec == "Pie de report")
            {
                QString acum = doc.firstChild().firstChild().toElement().attribute("acum");
                if(!acum.isEmpty())
                    acum.append(",");
                acum.append(this->target());
                doc.firstChild().firstChild().toElement().setAttribute("acum",acum);
                break;
            }
            else if(DetailSection* ds = reinterpret_cast<DetailSection*>(sectionPool.at(i)))
            {
                ds->addAcumulator(this->target());
            }
            break;
        }
    }

    return node;
}

void ReportAcumulator::parseXml(QDomElement element, QPointF origin)
{
    this->setPos(element.attribute("x").toDouble() + origin.x(),element.attribute("y").toDouble()+origin.y());
    this->setSize(element.attribute("w").toDouble(),element.attribute("h").toDouble());

    element.attribute("Alineacion") == "L" ? setAlineacion(Left) :
    element.attribute("Alineacion") == "R" ? setAlineacion(Rigth):
                                   setAlineacion(Center);

    this->setfontFamily(element.attribute("fontFamily"));
    this->setfontSize(element.attribute("fontSize").toDouble());
    this->setfontWeigth(element.attribute("fontWeigth").toDouble());
    this->setitalicFont(element.attribute("italicFont").toDouble());
    this->setunderlined(element.attribute("underlined").toDouble());
    this->setfontColor(ColorFromString(element.attribute("color")));
    this->setformato(element.attribute("formato").toDouble());
    this->setName(element.attribute("name"));
    this->setTarget(element.attribute("target"));
}

void ReportAcumulator::editMe()
{
    EditAcum e(this,qApp->activeWindow());
    e.exec();
}

Container::_Aling ReportAcumulator::Alineacion() const
{
    return m_Alineacion;
}

QString ReportAcumulator::fontFamily() const
{
    return m_fontFamily;
}

int ReportAcumulator::fontSize() const
{
    return m_fontSize;
}

int ReportAcumulator::fontWeigth() const
{
    return m_fontWeigth;
}

bool ReportAcumulator::italicFont() const
{
    return m_italicFont;
}

bool ReportAcumulator::underlined() const
{
    return m_underlined;
}

QFont ReportAcumulator::Font() const
{
    return m_Font;
}

QColor ReportAcumulator::fontColor() const
{
    return m_fontColor;
}

int ReportAcumulator::formato() const
{
    return m_formato;
}

void ReportAcumulator::setAlineacion(Container::_Aling arg)
{
    if (m_Alineacion != arg) {
        m_Alineacion = arg;
        emit AlineacionChanged(arg);
    }
}

void ReportAcumulator::setfontFamily(QString arg)
{
    if (m_fontFamily != arg) {
        m_fontFamily = arg;
        emit fontFamilyChanged(arg);
    }
}

void ReportAcumulator::setfontSize(int arg)
{
    if (m_fontSize != arg) {
        m_fontSize = arg;
        emit fontSizeChanged(arg);
    }
}

void ReportAcumulator::setfontWeigth(int arg)
{
    if (m_fontWeigth != arg) {
        m_fontWeigth = arg;
        emit fontWeigthChanged(arg);
    }
}

void ReportAcumulator::setitalicFont(bool arg)
{
    if (m_italicFont != arg) {
        m_italicFont = arg;
        emit italicFontChanged(arg);
    }
}

void ReportAcumulator::setunderlined(bool arg)
{
    if (m_underlined != arg) {
        m_underlined = arg;
        emit underlinedChanged(arg);
    }
}

void ReportAcumulator::setFont(QFont arg)
{
    if (m_Font != arg) {
        m_Font = arg;
        emit FontChanged(arg);
    }
}

void ReportAcumulator::setfontColor(QColor arg)
{
    if (m_fontColor != arg) {
        m_fontColor = arg;
        emit fontColorChanged(arg);
    }
}

void ReportAcumulator::setformato(int arg)
{
    if (m_formato != arg) {
        m_formato = arg;
        emit formatoChanged(arg);
    }
}

void ReportAcumulator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QFont f(m_fontFamily,m_fontSize);
    f.setUnderline(m_underlined);
    f.setWeight(m_fontWeigth);
    f.setItalic(m_italicFont);
    painter->setPen(m_fontColor);
    painter->setFont(f);
    painter->drawText(this->rect(),Qt::AlignLeft|Qt::AlignTop,"@"+m_target+"@");
//    if(this->isSelected())
        Container::paint(painter,option,widget);
    painter->restore();
}
QString ReportAcumulator::target() const
{
    return m_target;
}

void ReportAcumulator::setTarget(const QString &target)
{
    m_target = target;
}

