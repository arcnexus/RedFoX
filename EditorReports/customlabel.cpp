#include "customlabel.h"
#include "edittextdlg.h"
CustomLabel::CustomLabel(QGraphicsItem *parent) :
    Container(parent)
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

QDomElement CustomLabel::xml(QDomDocument doc , QPointF relPos)
{
    QDomElement node = doc.createElement("Element");
    node.setAttribute("id","Label");

    Container::apendXML(node,doc, relPos);

    QDomElement Text = doc.createElement("Text");
    Text.setAttribute("value",m_Text);

    QDomElement Orientacion = doc.createElement("Orientacion");
    Orientacion.setAttribute("value",m_Orientacion == Vertical ? "V" : "H");

    QDomElement Alineacion = doc.createElement("Alineacion");
    Alineacion.setAttribute("value",m_Alineacion == Left ? "L" : m_Alineacion == Rigth ? "R" : "C");

    QDomElement fontFamily = doc.createElement("fontFamily");
    fontFamily.setAttribute("value",m_fontFamily);

    QDomElement fontSize = doc.createElement("fontSize");
    fontSize.setAttribute("value",m_fontSize);

    QDomElement fontWeigth = doc.createElement("fontWeigth");
    fontWeigth.setAttribute("value",m_fontWeigth);

    QDomElement italicFont = doc.createElement("italicFont");
    italicFont.setAttribute("value",m_italicFont);

    QDomElement underlinedFont = doc.createElement("underlined");
    underlinedFont.setAttribute("value",m_underlined);

    QDomElement colorFont = doc.createElement("color");
    colorFont.setAttribute("value",ColorString(m_fontColor));

    node.appendChild(Text);
    node.appendChild(Orientacion);
    node.appendChild(Alineacion);
    node.appendChild(fontFamily);
    node.appendChild(fontSize);
    node.appendChild(fontWeigth);
    node.appendChild(italicFont);
    node.appendChild(underlinedFont);
    node.appendChild(colorFont);

    return node;
}

void CustomLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
        this->setRect(finalRect);
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
        this->setRect(0,0,r2.width()+10,r2.height());
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



void CustomLabel::setText(QString arg) {
    if (m_Text != arg) {
        m_Text = arg;
        emit TextChanged(arg);
    }
}

QString CustomLabel::Text() const {
    return m_Text;
}

CustomLabel::_Orientacion CustomLabel::Orientacion() const {
    return m_Orientacion;
}

void CustomLabel::setOrientacion(CustomLabel::_Orientacion arg) {
    if (m_Orientacion != arg) {
        m_Orientacion = arg;
        emit OrientacionChanged(arg);
    }
}

CustomLabel::_Aling CustomLabel::Alineacion() const {
    return m_Alineacion;
}

void CustomLabel::setAlineacion(CustomLabel::_Aling arg) {
    if (m_Alineacion != arg) {
        m_Alineacion = arg;
        emit AlineacionChanged(arg);
    }
}


void CustomLabel::parseXml(QDomElement element,QPointF origin)
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
        //"Text""Orientacion""Alineacion""fontFamily""fontSize""fontWeigth""italicFont"
        else if(tag== "Text")
            this->setText(el.attribute("value"));
        else if(tag== "Orientacion")
            el.attribute("value") == "V" ? setOrientacion(Vertical)
                                         : setOrientacion(Horizontal);
        else if(tag== "Alineacion")
            el.attribute("value") == "L" ? setAlineacion(Left) :
            el.attribute("value") == "R" ? setAlineacion(Rigth):
                                           setAlineacion(Center);
        else if(tag== "fontFamily")
            this->setfontFamily(el.attribute("value"));
        else if(tag== "fontSize")
            this->setfontSize(el.attribute("value").toInt());
        else if(tag== "fontWeigth")
            this->setfontWeigth(el.attribute("value").toInt());
        else if(tag== "italicFont")
            this->setitalicFont(el.attribute("value").toInt());
        else if(tag== "underlined")
            this->setunderlined(el.attribute("value").toInt());
         else if(tag== "color")
            this->setfontColor(ColorFromString(el.attribute("value")));
    }
}

void CustomLabel::setfontFamily(QString arg) {
    if (m_fontFamily != arg) {
        m_fontFamily = arg;
        m_Font.setFamily(arg);
        emit fontFamilyChanged(arg);
    }
}

void CustomLabel::setfontSize(int arg) {
    if (m_fontSize != arg) {
        m_fontSize = arg;
        m_Font.setPointSize(arg);
        emit fontSizeChanged(arg);
    }
}

void CustomLabel::setfontWeigth(int arg) {
    if (m_fontWeigth != arg) {
        m_fontWeigth = arg;
        m_Font.setWeight(arg);
        emit fontWeigthChanged(arg);
    }
}

void CustomLabel::setitalicFont(bool arg) {
    if (m_italicFont != arg) {
        m_italicFont = arg;
        m_Font.setItalic(arg);
        emit italicFontChanged(arg);
    }
}

QString CustomLabel::fontFamily() const {
    return m_fontFamily;
}

int CustomLabel::fontSize() const {
    return m_fontSize;
}

int CustomLabel::fontWeigth() const {
    return m_fontWeigth;
}

bool CustomLabel::italicFont() const {
    return m_italicFont;
}

QFont CustomLabel::Font() const {
    return m_Font;
}

void CustomLabel::setFont(QFont arg) {
    if (m_Font != arg) {
        m_Font = arg;
        emit FontChanged(arg);
    }
}

QColor CustomLabel::fontColor() const {
    return m_fontColor;
}

void CustomLabel::setfontColor(QColor arg) {
    if (m_fontColor != arg) {
        m_fontColor = arg;
        emit fontColorChanged(arg);
    }
}


void CustomLabel::editMe()
{
    EditTextDlg dlg(this,qApp->activeWindow());
    dlg.exec();
}

bool CustomLabel::underlined() const {
    return m_underlined;
}

void CustomLabel::setunderlined(bool arg) {
    if (m_underlined != arg) {
        m_underlined = arg;
        emit underlinedChanged(arg);
    }
}
