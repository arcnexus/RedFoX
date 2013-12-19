#include "reportparama.h"
#include "editparamdlg.h"
reportParama::reportParama(QString name,QGraphicsItem *parent) :
    Container(name,parent)
{
    m_fontFamily = "Arial";
    m_fontSize = 10;
    m_fontWeigth = QFont::Normal;
    m_italicFont = false;
    m_underlined = false;
    m_Alineacion = Left;
    m_Orientacion = Horizontal;
    m_fontColor = Qt::black;
}

QDomElement reportParama::xml(QDomDocument doc, QPointF relPos, QList<Section *> sectionPool)
{
    QDomElement node = doc.createElement("Element");
    node.setAttribute("id","Param");

    Container::apendXML(node,doc, relPos);

    node.setAttribute("Text",m_Text);
    node.setAttribute("Orientacion",m_Orientacion == Vertical ? "V" : "H");
    node.setAttribute("Alineacion",m_Alineacion == Left ? "L" : m_Alineacion == Rigth ? "R" : "C");
    node.setAttribute("fontFamily",m_fontFamily);
    node.setAttribute("fontSize",m_fontSize);
    node.setAttribute("fontWeigth",m_fontWeigth);
    node.setAttribute("italicFont",m_italicFont);
    node.setAttribute("underlined",m_underlined);
    node.setAttribute("color",ColorString(m_fontColor));
    node.setAttribute("Arg",m_arg);

    return node;
}

void reportParama::parseXml(QDomElement element, QPointF origin)
{
    this->setPos(element.attribute("x").toDouble() + origin.x(),element.attribute("y").toDouble()+origin.y());
    this->setSize(element.attribute("w").toDouble(),element.attribute("h").toDouble());
    this->setText(element.attribute("Text"));

    element.attribute("Orientacion") == "V" ? setOrientacion(Vertical)
                                            : setOrientacion(Horizontal);

    element.attribute("Alineacion") == "L" ? setAlineacion(Left) :
    element.attribute("Alineacion") == "R" ? setAlineacion(Rigth):
                                   setAlineacion(Center);

    this->setfontFamily(element.attribute("fontFamily"));
    this->setfontSize(element.attribute("fontSize").toDouble());
    this->setfontWeigth(element.attribute("fontWeigth").toDouble());
    this->setitalicFont(element.attribute("italicFont").toDouble());
    this->setunderlined(element.attribute("underlined").toDouble());
    this->setfontColor(ColorFromString(element.attribute("color")));
    this->setarg(element.attribute("Arg"));
    this->setName(element.attribute("name"));
}

void reportParama::editMe()
{
    editParamDlg dlg(this,qApp->activeWindow());
    dlg.exec();
}

QString reportParama::Text() const
{
    return m_Text;
}

Container::_Orientacion reportParama::Orientacion() const
{
    return m_Orientacion;
}

Container::_Aling reportParama::Alineacion() const
{
    return m_Alineacion;
}

QString reportParama::fontFamily() const
{
    return m_fontFamily;
}

int reportParama::fontSize() const
{
    return m_fontSize;
}

int reportParama::fontWeigth() const
{
    return m_fontWeigth;
}

bool reportParama::italicFont() const
{
    return m_italicFont;
}

bool reportParama::underlined() const
{
    return m_underlined;
}

QFont reportParama::Font() const
{
    return m_Font;
}

QColor reportParama::fontColor() const
{
    return m_fontColor;
}

QString reportParama::arg() const
{
    return m_arg;
}

void reportParama::setText(QString arg)
{
    if (m_Text != arg) {
        m_Text = arg;
        m_arg = arg;
        emit TextChanged(arg);
    }
}

void reportParama::setOrientacion(Container::_Orientacion arg)
{
    if (m_Orientacion != arg) {
        m_Orientacion = arg;
        emit OrientacionChanged(arg);
    }
}

void reportParama::setAlineacion(Container::_Aling arg)
{
    if (m_Alineacion != arg) {
        m_Alineacion = arg;
        emit AlineacionChanged(arg);
    }
}

void reportParama::setfontFamily(QString arg)
{
    if (m_fontFamily != arg) {
        m_fontFamily = arg;
        emit fontFamilyChanged(arg);
    }
}

void reportParama::setfontSize(int arg)
{
    if (m_fontSize != arg) {
        m_fontSize = arg;
        emit fontSizeChanged(arg);
    }
}

void reportParama::setfontWeigth(int arg)
{
    if (m_fontWeigth != arg) {
        m_fontWeigth = arg;
        emit fontWeigthChanged(arg);
    }
}

void reportParama::setitalicFont(bool arg)
{
    if (m_italicFont != arg) {
        m_italicFont = arg;
        emit italicFontChanged(arg);
    }
}

void reportParama::setunderlined(bool arg)
{
    if (m_underlined != arg) {
        m_underlined = arg;
        emit underlinedChanged(arg);
    }
}

void reportParama::setFont(QFont arg)
{
    if (m_Font != arg) {
        m_Font = arg;
        emit FontChanged(arg);
    }
}

void reportParama::setfontColor(QColor arg)
{
    if (m_fontColor != arg) {
        m_fontColor = arg;
        emit fontColorChanged(arg);
    }
}

void reportParama::setarg(QString arg)
{
    if (m_arg != arg) {
        m_arg = arg;
        emit argChanged(arg);
    }
}

void reportParama::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QFont f(m_fontFamily,m_fontSize);
    f.setUnderline(m_underlined);
    f.setWeight(m_fontWeigth);
    f.setItalic(m_italicFont);
    painter->setPen(m_fontColor);
    painter->setFont(f);
    if(m_Orientacion == Vertical)
    {
        QRectF r(this->rect());
        QRectF aux1(0,0,r.height(),r.width());
        QRectF r2(painter->fontMetrics().boundingRect(aux1.toRect(),Qt::TextWordWrap,m_Text));
        QRectF finalRect(0,0,r2.height(),r2.width()+10);
    //    this->setRect(finalRect);
        painter->translate(rect().bottomLeft());
        painter->rotate(270);
        switch (m_Alineacion) {
        case Center:
            painter->drawText(r2,Qt::TextWordWrap|Qt::AlignCenter,m_Text);
            break;
        case Rigth:
            painter->drawText(r2,Qt::TextWordWrap|Qt::AlignRight,m_Text);
            break;
        default:
            painter->drawText(r2,Qt::TextWordWrap|Qt::AlignLeft,m_Text);
            break;
        }
    }
    else
    {
        //NOTE Expansion de texto:
        QRectF r(this->rect());
        QRectF r2(painter->fontMetrics().boundingRect(r.toRect(),Qt::TextWordWrap,m_Text));
       // this->setRect(0,0,r2.width()+10,r2.height());
        switch (m_Alineacion) {
        case Center:
            painter->drawText(r2,Qt::TextWordWrap|Qt::AlignCenter,m_Text);
            break;
        case Rigth:
            painter->drawText(r2,Qt::TextWordWrap|Qt::AlignRight,m_Text);
            break;
        default:
            painter->drawText(r2,Qt::TextWordWrap|Qt::AlignLeft,m_Text);
            break;
        }
    }
    painter->restore();
    Container::paint(painter,option,widget);
}
