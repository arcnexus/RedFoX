#include "reportfield.h"
#include "editfielddlg.h"
ReportField::ReportField(QGraphicsItem *parent) :
    Container(parent)
{
    m_sql = ":Campo";
    m_expandable = false;
    m_fontFamily = "Arial";
    m_fontSize = 10;
    m_fontWeigth = QFont::Normal;
    m_italicFont = false;
    m_underlined = false;
    m_Alineacion = Left;
    m_fontColor = Qt::black;
    m_formato = 0;
}

QDomElement ReportField::xml(QDomDocument doc, QPointF relPos)
{
    QDomElement node = doc.createElement("Element");
    node.setAttribute("id","Field");

    Container::apendXML(node,doc, relPos);

    QDomElement Text = doc.createElement("Text");
    Text.setAttribute("value","");

    QDomElement Orientacion = doc.createElement("Orientacion");
    Orientacion.setAttribute("value","H");

    QDomElement Sql = doc.createElement("Sql");
    Sql.setAttribute("value",m_sql);

    QDomElement Expandable = doc.createElement("Expandable");
    Expandable.setAttribute("value",m_expandable);

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

    QDomElement formato = doc.createElement("formato");
    formato.setAttribute("value",m_formato);

    node.appendChild(formato);
    node.appendChild(Sql);
    node.appendChild(Expandable);
    node.appendChild(Alineacion);
    node.appendChild(fontFamily);
    node.appendChild(fontSize);
    node.appendChild(fontWeigth);
    node.appendChild(italicFont);
    node.appendChild(underlinedFont);
    node.appendChild(colorFont);
    node.appendChild(Text);
    node.appendChild(Orientacion);
    return node;
}

void ReportField::parseXml(QDomElement element, QPointF origin)
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
        else if(tag== "Sql")
            this->setsql(el.attribute("value"));
        else if(tag== "Expandable")
            this->setexpandable(el.attribute("value").toInt());
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
        else if(tag== "formato")
           this->setformato(el.attribute("value").toInt());
    }
}

void ReportField::editMe()
{
    editFieldDlg dlg(this,qApp->activeWindow());
    dlg.exec();
}

QString ReportField::sql() const {
    return m_sql;
}

void ReportField::setsql(QString arg) {
    if (m_sql != arg) {
        m_sql = arg;
        emit sqlChanged(arg);
    }
}

void ReportField::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QFont f(m_fontFamily,m_fontSize);
    f.setUnderline(m_underlined);
    f.setWeight(m_fontWeigth);
    f.setItalic(m_italicFont);
    painter->setPen(m_fontColor);
    painter->setFont(f);
    painter->drawText(this->rect(),Qt::AlignLeft|Qt::AlignTop,m_sql);
    if(this->isSelected())
        Container::paint(painter,option,widget);
    painter->restore();
}

bool ReportField::expandable() const {
    return m_expandable;
}

void ReportField::setexpandable(bool arg) {
    if (m_expandable != arg) {
        m_expandable = arg;
        emit expandableChanged(arg);
    }
}

Container::_Aling ReportField::Alineacion() const {
    return m_Alineacion;
}

QString ReportField::fontFamily() const {
    return m_fontFamily;
}

int ReportField::fontSize() const {
    return m_fontSize;
}

int ReportField::fontWeigth() const {
    return m_fontWeigth;
}

bool ReportField::italicFont() const {
    return m_italicFont;
}

bool ReportField::underlined() const {
    return m_underlined;
}

QFont ReportField::Font() const {
    return m_Font;
}

QColor ReportField::fontColor() const {
    return m_fontColor;
}

void ReportField::setAlineacion(Container::_Aling arg) {
    if (m_Alineacion != arg) {
        m_Alineacion = arg;
        emit AlineacionChanged(arg);
    }
}

void ReportField::setfontFamily(QString arg) {
    if (m_fontFamily != arg) {
        m_fontFamily = arg;
        emit fontFamilyChanged(arg);
    }
}

void ReportField::setfontSize(int arg) {
    if (m_fontSize != arg) {
        m_fontSize = arg;
        emit fontSizeChanged(arg);
    }
}

void ReportField::setfontWeigth(int arg) {
    if (m_fontWeigth != arg) {
        m_fontWeigth = arg;
        emit fontWeigthChanged(arg);
    }
}

void ReportField::setitalicFont(bool arg) {
    if (m_italicFont != arg) {
        m_italicFont = arg;
        emit italicFontChanged(arg);
    }
}

void ReportField::setunderlined(bool arg) {
    if (m_underlined != arg) {
        m_underlined = arg;
        emit underlinedChanged(arg);
    }
}

void ReportField::setFont(QFont arg) {
    if (m_Font != arg) {
        m_Font = arg;
        emit FontChanged(arg);
    }
}

void ReportField::setfontColor(QColor arg) {
    if (m_fontColor != arg) {
        m_fontColor = arg;
        emit fontColorChanged(arg);
    }
}

QString ReportField::query() {
    QStringList l = sql().split(".");
    if(l.size()==3)
    {
        QString zona = l.at(0);
        QString tabla = l.at(1);
        QString s = QString("%1.%2").arg(zona).arg(tabla);
        return s;
    }
    else
        return "";
}

int ReportField::formato() const {
    return m_formato;
}

void ReportField::setformato(int arg) {
    if (m_formato != arg) {
        m_formato = arg;
        emit formatoChanged(arg);
    }

    /*0 = sin formato
     *1 = 999.999.999,99
     *2 = 999,999,999.99
     *3 = 99999999999,99
     *4 = 99999999999.99*/

}
