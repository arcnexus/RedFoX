#include "detailsection.h"
#include <QtGui>
DetailSection::DetailSection(QGraphicsItem *parent) :
    Section(parent)
{
    m_headerSize = m_footSize = 0;
    onResizeHead = onResizeFoot = false;
    m_header = m_foot = false;
    m_colorear = m_use2Colors = false;
}

bool DetailSection::header() const
{
    return m_header;
}

void DetailSection::setHeader(bool b)
{
    if(b)
    {
        m_headerSize = 18;
        m_Height+=18;
        this->setRect(0,0,this->Width(),this->rect().height()+18);
        emit resized(this);
    }
    else
        m_headerSize = 0;
    m_header = b;
}


void DetailSection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    if(m_header || m_foot)
    {
        QPen pen;
        pen.setStyle(Qt::DashDotDotLine);
        pen.setColor(Qt::darkRed);
        pen.setWidth(1);
        painter->setPen(pen);
        wTxtCab = painter->fontMetrics().width("Cabecera");
        if(m_header)
        {            
            hTxtCab = painter->fontMetrics().height();
            QRectF r(QPointF(rect().width()-wTxtCab,0),QSize(wTxtCab,hTxtCab));
            painter->drawLine(0,m_headerSize,rect().width(),m_headerSize);
            painter->drawLine(rect().width()-wTxtCab,m_headerSize+1,rect().width(),m_headerSize+1);
            painter->drawText(r,"Cabecera");
        }
        if(m_foot)
        {
            wTxtFoot = painter->fontMetrics().width("Pie");
            hTxtFoot = painter->fontMetrics().height();
            QRectF r;
            if(m_footSize > 20)
                r = QRectF(QPointF(rect().width()-wTxtFoot,rect().height() - m_footSize),QSize(wTxtFoot,hTxtFoot));
            else
                r = QRectF(QPointF(rect().width()-wTxtFoot,rect().height() - m_footSize - hTxtFoot),QSize(wTxtFoot,hTxtFoot));
            painter->drawLine(0,rect().height() - m_footSize,rect().width(),rect().height() - m_footSize);
            painter->drawLine(rect().width()-wTxtCab,rect().height() - m_footSize-1,rect().width(),rect().height() - m_footSize-1);
            painter->drawText(r,"Pie");            
        }
    }
    painter->restore();
    Section::paint(painter,option,widget);
}

void DetailSection::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QRectF r(QPointF(rect().width()-wTxtCab,m_headerSize-hTxtCab/2),QSize(wTxtCab,hTxtCab));
    QRectF r2(QPointF(rect().width()-wTxtCab,rect().height() - m_footSize - hTxtFoot),QSize(wTxtCab,hTxtFoot));
    if((r.contains(event->pos()) && m_header) || (r2.contains(event->pos()) &&  m_foot))
        setCursor(Qt::SizeVerCursor);
    else
        Section::hoverEnterEvent(event);
}

void DetailSection::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QRectF r(QPointF(rect().width()-wTxtCab,m_headerSize-hTxtCab/2),QSize(wTxtCab,hTxtCab));
    QRectF r2(QPointF(rect().width()-wTxtCab,rect().height() - m_footSize - hTxtFoot),QSize(wTxtCab,hTxtFoot));
    if((r.contains(event->pos()) && m_header) || (r2.contains(event->pos()) && m_foot))
         setCursor(Qt::SizeVerCursor);
    else
        Section::hoverMoveEvent(event);
}

void DetailSection::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF r(QPointF(rect().width()-wTxtCab,m_headerSize-hTxtCab/2),QSize(wTxtCab,hTxtCab));
    QRectF r2(QPointF(rect().width()-wTxtCab,rect().height() - m_footSize - hTxtFoot),QSize(wTxtCab,hTxtFoot));
    if(r.contains(event->pos()) && m_header)
        onResizeHead = true;
    else if(r2.contains(event->pos()) && m_foot)
        onResizeFoot = true;
    else
    {
        QRectF r(QPointF(rect().width()- _wTxt,rect().height()-_hTxt),QSize(_wTxt,_hTxt));
        if(r.contains(event->pos()))
            _onResize = true;
        else
            event->ignore();
    }
}

void DetailSection::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(onResizeHead)
    {
        if(this->rect().top() + event->pos().ry()> 20)
        {
            int diff = 18;
            if(m_foot)
                diff+=m_footSize;
            if(event->pos().ry() - this->rect().top() < this->rect().height()-diff)
            {
                m_headerSize = event->pos().ry() - this->rect().top();
                this->scene()->update();
            }
        }
    }
    else if(onResizeFoot)
    {
        int diff = 18;
        if(m_header)
            diff+= m_headerSize;
        if(event->pos().ry() - this->rect().top() > diff)
        {
            if(event->pos().ry() - this->rect().top() < this->rect().height()- 18)
            {
                m_footSize = rect().height() - event->pos().ry() - this->rect().top();
                this->scene()->update();
            }
        }
    }
    else if(_onResize)
    {
        int minH = 15;
        if(m_header)
            minH += m_headerSize;
        if(m_foot)
            minH += m_footSize;

        if(this->rect().top() + event->pos().ry()> minH)
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

void DetailSection::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    onResizeHead = onResizeFoot = false;
    Section::mouseReleaseEvent(event);
}

QString DetailSection::ColorString(QColor c)
{
    return QString("%1,%2,%3,%4").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
}

QColor DetailSection::ColorFromString(QString s)
{
    QStringList l= s.split(",");
    QColor c;
    c.setRed(l.at(0).toDouble());
    c.setGreen(l.at(1).toDouble());
    c.setBlue(l.at(2).toDouble());
    c.setAlpha(l.at(3).toDouble());
    return c;
}

void DetailSection::_parseElementsBlock(QDomNode eleNode , QPointF point, QList<Container *> &itemPool)
{
    while(!eleNode.isNull())
    {
        _parseElementTag(eleNode.toElement(),point,itemPool);
        eleNode = eleNode.nextSibling();
    }
}

bool DetailSection::foot() const
{
    return m_foot;
}

void DetailSection::setFoot(bool foot)
{
    if(foot)
    {
        m_footSize = 18;
        this->setRect(0,0,this->Width(),this->rect().height()+18);
        m_Height+=18;
        emit resized(this);
    }
    else
        m_footSize = 0;
    m_foot = foot;
}

QDomElement DetailSection::xml(QDomDocument doc, QList<Container *> &usedItems, QMap<QString, bool> &querys, QList<Section*> sectionPool)
{
    QDomElement node = doc.createElement("Section");
    node.setAttribute("id",sectionType());
    node.setAttribute("size",(int)rect().height());
    node.setAttribute("name",m_SectionName);
    node.setAttribute("SqlGlobal",m_SqlGlobal);
    node.setAttribute("SqlInterno",m_SqlInterno);
    node.setAttribute("ClausulaInterna",m_ClausulaInterna);
    node.setAttribute("haveSqlInterno",m_header || m_foot);
    node.setAttribute("colored",m_colorear);
    node.setAttribute("color1",ColorString(m_color1));
    node.setAttribute("alternative",m_use2Colors);
    node.setAttribute("color2",ColorString(m_color2));

    QPointF br(rect().bottomRight().x()-1,rect().bottomRight().y()-1);
    QPointF tl(rect().topLeft().x()+1,rect().topLeft().y()+1);
    if(m_header)
    {
        QDomElement headerNode = doc.createElement("Header");
        headerNode.setAttribute("size",m_headerSize);
        QRectF aux(tl,QPointF(br.x(),tl.y()+m_headerSize-2));
        QRectF headerRect(mapRectToScene(aux));

        QList<QGraphicsItem*>items =this->scene()->items(headerRect);
        QListIterator<QGraphicsItem*> it(items);
        while (it.hasNext())
        {
            QGraphicsItem* c = it.next();
            Container* cont = qgraphicsitem_cast<Container*>(c);
            if(cont)
            {
                if(!usedItems.contains(cont))
                {
                    headerNode.appendChild(cont->xml(doc,marginPoint(),sectionPool));
                    usedItems.append(cont);
                }
            }
        }
        node.appendChild(headerNode);
    }
    QDomElement bodyNode = doc.createElement("Body");
    bodyNode.setAttribute("size",this->rect().height() - m_headerSize - m_footSize);
    QRectF bodyRect(mapToScene(QPointF(tl.x(),tl.y()+m_headerSize)),mapToScene(QPointF(br.x(),br.y()-m_footSize)));
    QList<QGraphicsItem*>items =this->scene()->items(bodyRect);
    QListIterator<QGraphicsItem*> it(items);
    while (it.hasNext())
    {
        QGraphicsItem* c = it.next();
        Container* cont = qgraphicsitem_cast<Container*>(c);
        if(cont)
        {
            if(!usedItems.contains(cont))
            {
                bodyNode.appendChild(cont->xml(doc,mapToScene(QPointF(tl.x(),tl.y()+m_headerSize)),sectionPool));
                usedItems.append(cont);
            }
        }
    }
    node.appendChild(bodyNode);
    if(m_foot)
    {
        QDomElement footNode = doc.createElement("Foot");
        footNode.setAttribute("size",m_footSize);
        QRectF footRect(mapToScene(QPointF(tl.x(),br.y()-m_footSize+1)),mapToScene(QPointF(br)));
        QList<QGraphicsItem*>items =this->scene()->items(footRect);
        QListIterator<QGraphicsItem*> it(items);
        while (it.hasNext())
        {
            QGraphicsItem* c = it.next();
            Container* cont = qgraphicsitem_cast<Container*>(c);
            if(cont)
            {
                if(!usedItems.contains(cont))
                {
                    footNode.appendChild(cont->xml(doc,mapToScene(QPointF(tl.x(),br.y()-m_footSize)),sectionPool));
                    usedItems.append(cont);
                }
            }
        }
        node.appendChild(footNode);
    }
    return node;
}

void DetailSection::parseXml(QDomNode elements, QList<Container *> &itemPool)
{
    int body_start = 0;
    int body_siz = 15;
    while (!elements.isNull())
    {
        QDomElement block = elements.toElement();
        if(block.tagName() == "Header")
        {
            m_header = true;
            m_headerSize = block.attribute("size").toDouble();
            body_start += m_headerSize;

            QDomNode eleNode = block.firstChild();
            _parseElementsBlock(eleNode, mapToScene(0,0),itemPool);
        }

        else if(block.tagName() == "Body")
        {
            body_siz  = block.attribute("size").toDouble();
            QDomNode eleNode = block.firstChild();
            _parseElementsBlock(eleNode, mapToScene(0,body_start),itemPool);
        }

        else if(block.tagName() == "Foot")
        {
            m_foot = true;
            m_footSize = block.attribute("size").toDouble();
            int foot_start = body_start + body_siz;
            QDomNode eleNode = block.firstChild();
            _parseElementsBlock(eleNode, mapToScene(0,foot_start),itemPool);
        }
        elements = elements.nextSibling();
    }
}

QString DetailSection::SqlGlobal() const {
    return m_SqlGlobal;
}

void DetailSection::setSqlGlobal(QString arg) {
    if (m_SqlGlobal != arg) {
        m_SqlGlobal = arg;
        emit SqlGlobalChanged(arg);
    }
}

QString DetailSection::SqlInterno() const {
    return m_SqlInterno;
}

void DetailSection::setSqlInterno(QString arg) {
    if (m_SqlInterno != arg) {
        m_SqlInterno = arg;
        emit SqlInternoChanged(arg);
    }
}

QString DetailSection::ClausulaInterna() const {
    return m_ClausulaInterna;
}

void DetailSection::setClausulaInterna(QString arg) {
    if (m_ClausulaInterna != arg) {
        m_ClausulaInterna = arg;
        emit ClausulaInternaChanged(arg);
    }
}

bool DetailSection::colorear() const {
    return m_colorear;
}

void DetailSection::setcolorear(bool arg) {
    if (m_colorear != arg) {
        m_colorear = arg;
        emit colorearChanged(arg);
    }
}

QColor DetailSection::color1() const {
    return m_color1;
}

void DetailSection::setcolor1(QColor arg) {
    if (m_color1 != arg) {
        m_color1 = arg;
        emit color1Changed(arg);
    }
}

bool DetailSection::use2Colors() const {
    return m_use2Colors;
}

void DetailSection::setuse2Colors(bool arg) {
    if (m_use2Colors != arg) {
        m_use2Colors = arg;
        emit use2ColorsChanged(arg);
    }
}

QColor DetailSection::color2() const {
    return m_color2;
}

void DetailSection::setcolor2(QColor arg) {
    if (m_color2 != arg) {
        m_color2 = arg;
        emit color2Changed(arg);
    }
}
