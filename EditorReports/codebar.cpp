#include "codebar.h"
#include "editcodebardlg.h"
CodeBar::CodeBar(QGraphicsItem *parent) :
    Container(parent)
{
    m_visibleCode = false;
}

void CodeBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QFont f1 = painter->font();
    int fh = 0;
    if(m_visibleCode)
        fh += painter->fontMetrics().height();

    QFont f("Free 3 of 9 Extended",30);
    painter->setFont(f);
    int fh2 = fh + painter->fontMetrics().height() + 3;
    int fw = painter->fontMetrics().width(m_code);

    this->setRect(0,0,fw,fh2);
    painter->drawText(this->rect(),Qt::AlignTop|Qt::AlignHCenter,m_code);

    if(m_visibleCode)
    {
        painter->setFont(f1);
        painter->drawText(this->rect(),Qt::AlignBottom|Qt::AlignJustify|Qt::TextJustificationForced,m_code);
    }
    painter->restore();
    if(this->isSelected())
        Container::paint(painter,option,widget);
}

QDomElement CodeBar::xml(QDomDocument doc, QPointF relPos, QList<Section *> sectionPool)
{
    QDomElement node = doc.createElement("Element");
    node.setAttribute("id","CodeBar");

    Container::apendXML(node,doc, relPos);

    node.setAttribute("Code",m_code);
    node.setAttribute("visibleCodee",m_visibleCode);

    node.setAttribute("Sql",m_sql);
    return node;
}

void CodeBar::parseXml(QDomElement element, QPointF origin)
{
    this->setPos(element.attribute("x").toDouble() + origin.x(),element.attribute("y").toDouble()+origin.y());
    this->setSize(element.attribute("w").toDouble(),element.attribute("h").toDouble());
    this->setcode(element.attribute("Code"));
    this->setvisibleCode(element.attribute("visibleCode").toDouble());
    this->setsql(element.attribute("Sql"));
}

QString CodeBar::code() const {
    return m_code;
}

bool CodeBar::visibleCode() const {
    return m_visibleCode;
}

void CodeBar::editMe()
{
    EditCodeBarDlg dlg(this,qApp->activeWindow());
    dlg.exec();
}

void CodeBar::setcode(QString arg) {
    if (m_code != arg) {        
        m_code = arg;
        emit codeChanged(arg);
    }
}

void CodeBar::setvisibleCode(bool arg) {
    if (m_visibleCode != arg) {
        m_visibleCode = arg;
        emit visibleCodeChanged(arg);
    }
}

QString CodeBar::sql() const {
    return m_sql;
}

void CodeBar::setsql(QString arg) {
    if (m_sql != arg) {
        m_sql = arg;
        emit sqlChanged(arg);
    }
}
