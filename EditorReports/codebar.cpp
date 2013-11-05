#include "codebar.h"
#include "editcodebardlg.h"
CodeBar::CodeBar(QGraphicsItem *parent) :
    Container(parent)
{
    m_visibleCode = false;
    m_codeSize = 15;
    m_barSize = 25;
    m_vertical = true;
}

void CodeBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QFont f1 = painter->font();
    f1.setPointSize(m_codeSize);
    int fh = 0;
    if(m_visibleCode)
        fh += painter->fontMetrics().height();

    QFont f("Free 3 of 9 Extended",m_barSize);
    painter->setFont(f);

    int fh2 = fh + painter->fontMetrics().height() + 3;
    int fw = painter->fontMetrics().width(m_code);

    if(m_vertical)
    {
        this->setRect(0,0,fh2,fw);

        QRectF r(this->rect());
        QRectF aux1(0,0,r.height(),r.width());

        painter->translate(rect().bottomLeft());
        painter->rotate(270);

        painter->drawText(aux1,Qt::TextWordWrap|Qt::AlignHCenter,m_code);

        if(m_visibleCode)
        {
            painter->setFont(f1);
            painter->drawText(aux1,Qt::AlignBottom|Qt::AlignCenter|Qt::TextJustificationForced,m_code);
        }
    }
    else
    {
        this->setRect(0,0,fw,fh2);
        painter->drawText(this->rect(),Qt::AlignTop|Qt::AlignHCenter,m_code);

        if(m_visibleCode)
        {
            painter->setFont(f1);
            painter->drawText(this->rect(),Qt::AlignBottom|Qt::AlignCenter|Qt::TextJustificationForced,m_code);
        }
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
    node.setAttribute("visibleCode",m_visibleCode);

    node.setAttribute("codeSize",m_codeSize);
    node.setAttribute("barSize",m_barSize);
    node.setAttribute("vertical",m_vertical);

    node.setAttribute("Sql",m_sql);
    return node;
}

void CodeBar::parseXml(QDomElement element, QPointF origin)
{
    this->setPos(element.attribute("x").toDouble() + origin.x(),element.attribute("y").toDouble()+origin.y());
    this->setSize(element.attribute("w").toDouble(),element.attribute("h").toDouble());
    this->setcode(element.attribute("Code"));
    this->setvisibleCode(element.attribute("visibleCode").toDouble());

    this->setcodeSize(element.attribute("codeSize").toInt());
    this->setbarSize(element.attribute("barSize").toInt());
    this->setvertical(element.attribute("vertical").toInt());

    this->setsql(element.attribute("Sql"));
}

QString CodeBar::query()
{
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

int CodeBar::codeSize() const
{
    return m_codeSize;
}

int CodeBar::barSize() const
{
    return m_barSize;
}

bool CodeBar::vertical() const
{
    return m_vertical;
}

void CodeBar::setsql(QString arg) {
    if (m_sql != arg) {
        m_sql = arg;
        emit sqlChanged(arg);
    }
}

void CodeBar::setcodeSize(int arg)
{
    if (m_codeSize != arg) {
        m_codeSize = arg;
        emit codeSizeChanged(arg);
    }
}

void CodeBar::setbarSize(int arg)
{
    if (m_barSize != arg) {
        m_barSize = arg;
        emit barSizeChanged(arg);
    }
}

void CodeBar::setvertical(bool arg)
{
    if (m_vertical != arg) {
        m_vertical = arg;
        emit verticalChanged(arg);
    }
}
