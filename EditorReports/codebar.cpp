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

QDomElement CodeBar::xml(QDomDocument doc, QPointF relPos)
{
    QDomElement node = doc.createElement("Element");
    node.setAttribute("id","CodeBar");

    Container::apendXML(node,doc, relPos);

    QDomElement Text = doc.createElement("Code");
    Text.setAttribute("value",m_code);

    QDomElement visibleCode = doc.createElement("visibleCode");
    visibleCode.setAttribute("value",m_visibleCode);

    QDomElement sqlCode = doc.createElement("Sql");
    sqlCode.setAttribute("value",m_sql);

    node.appendChild(visibleCode);
    node.appendChild(sqlCode);
    node.appendChild(Text);

    return node;
}

void CodeBar::parseXml(QDomElement element, QPointF origin)
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
        else if(tag== "Code")
            this->setcode(el.attribute("value"));
        else if(tag== "visibleCode")
            this->setvisibleCode(el.attribute("value").toInt());
        else if(tag== "Sql")
            this->setsql(el.attribute("value"));

    }
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
