#include "pageheadersection.h"

PageHeaderSection::PageHeaderSection(QGraphicsItem *parent) :
    Section(parent)
{
    m_onFisrtPage = false;
}

QDomElement PageHeaderSection::xml(QDomDocument doc, QList<Container *> &usedItems,QMap<QString,bool> &querys, QList<Section*> sectionPool)
{
    QDomElement ele = Section::xml(doc,usedItems,querys,sectionPool);
    ele.setAttribute("OnFistPage", m_onFisrtPage);
    return ele;
}

bool PageHeaderSection::onFisrtPage() const
{
    return m_onFisrtPage;
}

void PageHeaderSection::setOnFisrtPage(bool onFisrtPage)
{
    m_onFisrtPage = onFisrtPage;
}
