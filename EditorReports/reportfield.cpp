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

QDomElement ReportField::xml(QDomDocument doc, QPointF relPos, QList<Section *> sectionPool)
{
    QDomElement node = doc.createElement("Element");
    node.setAttribute("id","Field");

    Container::apendXML(node,doc, relPos);

    node.setAttribute("Text","");
    node.setAttribute("Orientacion","H");
    node.setAttribute("Sql",m_sql);
    node.setAttribute("Expandable",m_expandable);
    node.setAttribute("Alineacion",m_Alineacion == Left ? "L" : m_Alineacion == Rigth ? "R" : "C");
    node.setAttribute("fontFamily",m_fontFamily);
    node.setAttribute("fontSize",m_fontSize);
    node.setAttribute("fontWeigth",m_fontWeigth);
    node.setAttribute("italicFont",m_italicFont);
    node.setAttribute("underlined",m_underlined);
    node.setAttribute("color",ColorString(m_fontColor));
    node.setAttribute("formato",m_formato);

    return node;
}

void ReportField::parseXml(QDomElement element, QPointF origin)
{
    this->setPos(element.attribute("x").toDouble() + origin.x(),element.attribute("y").toDouble()+origin.y());
    this->setSize(element.attribute("w").toDouble(),element.attribute("h").toDouble());
    this->setsql(element.attribute("Sql"));
    this->setexpandable(element.attribute("Expandable").toDouble());

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
