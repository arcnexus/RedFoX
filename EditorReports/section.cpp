#include "section.h"
#include <QtGui>
#include <QApplication>


Section::Section(QGraphicsItem *parent ) :
    QGraphicsRectItem(parent)
{
    this->setAcceptHoverEvents(true);
    QGraphicsRectItem * p = (QGraphicsRectItem*)(parent);
    _parent = p;
    m_Height = 20;
    m_Width = p->rect().width();
    m_SectionName = "Sin nombre";
    _onResize = false;
    _sizeChanged = false;
    m_onMaxsize = false;
    this->setRect(0,0,m_Width,m_Height);
    this->setPos(p->pos().x() , 0);

}

void Section::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen pen;
    pen.setColor(Qt::darkBlue);
    pen.setStyle(Qt::DashDotLine);
    pen.setWidth(2);
   // painter->drawRect(this->rect());
    painter->setPen(pen);
    _wTxt = painter->fontMetrics().width(m_SectionName + _typeName);
    _hTxt = painter->fontMetrics().height();

    this->setRect(0,0,_parent->rect().width() + 5 + _wTxt - margin.left(),rect().height());

    painter->drawLine(this->rect().width(),1,this->rect().width(),this->rect().height()-1);
   // painter->drawLine(0,1,this->rect().width(),1);
    painter->drawLine(0,this->rect().height()-1,this->rect().width(),this->rect().height()-1);
    QRectF r(QPointF(rect().width()- _wTxt,rect().height()-_hTxt-1),QSize(_wTxt,_hTxt));
    painter->drawText(r,Qt::AlignRight,m_SectionName+ _typeName);

    painter->restore();
}

void Section::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF r(QPointF(rect().width()- _wTxt,rect().height()-_hTxt),QSize(_wTxt,_hTxt));
    if(r.contains(event->pos()))    
        _onResize = true;
    else
        event->ignore();
}

void Section::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(_onResize)
    {       
        if(this->rect().top() + event->pos().ry()> 15)
        {
            if(mapToScene(event->pos()).ry()<margin.bottom())
            {
                if(!m_onMaxsize || event->pos().ry() - this->rect().top() < this->Height())
                {
                    this->setRect(0,0,this->rect().width(),event->pos().ry() - this->rect().top());
                    _sizeChanged = true;
                    m_Height = this->rect().height();
                    m_onMaxsize = false;
                    emit resized(this);
                }
            }
        }
    }
    else
        event->ignore();
}

void Section::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(_onResize)
    {
        _onResize = false;
        if(_sizeChanged)
        {
            m_Height = this->rect().height();
            emit resized(this);
        }
        _sizeChanged = false;
        m_onMaxsize = false;
    }
    else
        event->ignore();
}

void Section::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QRectF r(QPointF(rect().width()- _wTxt,rect().height()-_hTxt),QSize(_wTxt,_hTxt));
    if(r.contains(event->pos()))
        setCursor(Qt::SizeVerCursor);
    else
        event->ignore();
}

void Section::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
}

void Section::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QRectF r(QPointF(rect().width()- _wTxt,rect().height()-_hTxt),QSize(_wTxt,_hTxt));
    if(!r.contains(event->pos()))
    {
        setCursor(Qt::ArrowCursor);
        event->ignore();
    }
    else
        setCursor(Qt::SizeVerCursor);
}

QPointF Section::marginPoint()
{
    return mapToScene(QPointF(0,0));
}

void Section::_parseElementTag(QDomElement ele, QPointF point, QList<Container *> &itemPool)
{
    if (ele.tagName() == "Element")
    {
        //TODO other items
        if(ele.attribute("id")== "RoundRect")
            _insertRect(ele, point, itemPool);
        else if(ele.attribute("id")== "Label")
            _insertLabel(ele, point, itemPool);
        else if(ele.attribute("id")== "Line")
            _insertLine(ele, point, itemPool);
        else if(ele.attribute("id")== "CodeBar")
            _insertCodeBar(ele,point, itemPool);
        else if(ele.attribute("id")== "Image")
            _insertImage(ele,point, itemPool);
        else if(ele.attribute("id")== "Field")
            _insertField(ele,point, itemPool);
        else if(ele.attribute("id")== "RelationalField")
            _insertRelField(ele,point, itemPool);
    }
}

void Section::_insertRect(QDomElement ele, QPointF point, QList<Container *> &itemPool)
{
    RoundedRect * rect = new RoundedRect(_parent);
    rect->setMargins(this->margin);
    rect->parseXml(ele,point);
    itemPool.append(rect);
}

void Section::_insertLabel(QDomElement ele, QPointF point, QList<Container *> &itemPool)
{
    CustomLabel * rect = new CustomLabel(_parent);
    rect->setMargins(this->margin);
    rect->parseXml(ele,point);
    itemPool.append(rect);
}

void Section::_insertLine(QDomElement ele, QPointF point, QList<Container *> &itemPool)
{
    ReportLine * line = new ReportLine(_parent);
    line->setMargins(this->margin);
    line->parseXml(ele,point);
    itemPool.append(line);
}

void Section::_insertCodeBar(QDomElement ele, QPointF point, QList<Container *> &itemPool)
{
    CodeBar* code = new CodeBar(_parent);
    code->setMargins(this->margin);
    code->parseXml(ele,point);
    itemPool.append(code);
}

void Section::_insertImage(QDomElement ele, QPointF point, QList<Container *> &itemPool)
{
    ReportImage* img = new ReportImage(_parent);
    img->setMargins(this->margin);
    img->parseXml(ele,point);
    itemPool.append(img);
}

void Section::_insertField(QDomElement ele, QPointF point, QList<Container *> &itemPool)
{
    ReportField * fld = new ReportField(_parent)   ;
    fld->setMargins(this->margin);
    fld->parseXml(ele,point);
    itemPool.append(fld );
}

void Section::_insertRelField(QDomElement ele, QPointF point, QList<Container *> &itemPool)
{
    RelationalField * fld = new RelationalField(_parent)   ;
    fld->setMargins(this->margin);
    fld->parseXml(ele,point);
    itemPool.append(fld );
}

QRectF Section::getMargin() const
{
    return margin;
}

void Section::setMargin(const QRectF &value)
{
    margin = value;
    this->setPos(value.left(),this->pos().y());
    validRect = QRectF(QPointF(0,0),value.size());
}

QString Section::SectionName() const {
    return m_SectionName;
}

void Section::setSectionName(QString arg) {
    if (m_SectionName != arg) {
        m_SectionName = arg;
        this->scene()->update();
        emit SectionNameChanged(arg);
    }
}

Section::SectionType Section::sectionType() const
{
    return _type;
}

void Section::setType(const SectionType &type)
{
    _type = type;
    switch (_type) {
    case Section::ReportHeader:
        _typeName = " (Cabezera de report)";
        break;
    case Section::PageHeader:
        _typeName = " (Cabezera de página)";
        break;
    case Section::Detail:
        _typeName = " (Detalle)";
        break;
    case Section::PageFooter:
        _typeName = " (Pie de página)";
        break;
    case Section::ReportFooter:
        _typeName = " (Pie de report)";
        break;
    }
}

void Section::setHeight(int arg) {
    if (m_Height != arg) {
        m_Height = arg;
        this->setRect(0,0,rect().width(),m_Height);
        emit heightChanged(arg);
    }
}

void Section::setvPos(int arg) {
    if (m_vPos != arg) {
        m_vPos = arg;
        this->setPos(pos().x(),arg);
        emit vPosChanged(arg);
    }
}

int Section::Height() const {
    return m_Height;
}

int Section::vPos() const {
    return m_vPos;
}

int Section::Width() const
{
    return m_Width;
}

void Section::setWidth(int Width)
{
    m_Width = Width + 200;
    this->setRect(0,0,m_Width,m_Height);
}

bool Section::onMaxsize() const
{
    return m_onMaxsize;
}

void Section::setOnMaxsize(bool onMaxsize)
{
    m_onMaxsize = onMaxsize;
}

QDomElement Section::xml(QDomDocument doc, QList<Container *> &usedItems , QMap<QString,bool> &querys)
{    
    QDomElement node = doc.createElement("Section");
    node.setAttribute("id",sectionType());
    node.setAttribute("size",m_Height);
    node.setAttribute("name",m_SectionName);

    QRectF myRect(mapRectToScene(this->rect()));
    QList<QGraphicsItem*>items =this->scene()->items(myRect);
    QListIterator<QGraphicsItem*> it(items);
    while (it.hasNext())
    {
        QGraphicsItem* c = it.next();
        Container* cont = qgraphicsitem_cast<Container*>(c);
        if(cont)
        {
            if(!usedItems.contains(cont))
            {
                querys[cont->query()] = true;
                node.appendChild(cont->xml(doc,marginPoint()));
                usedItems.append(cont);
            }
        }
    }

    return node;
}

void Section::parseXml(QDomNode elements, QList<Container *> &itemPool)
{
    while (!elements.isNull())
    {
        _parseElementTag(elements.toElement(), mapToScene(0,0),itemPool);
        elements = elements.nextSibling();
    }
}

