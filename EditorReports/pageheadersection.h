#ifndef PAGEHEADERSECTION_H
#define PAGEHEADERSECTION_H
#include "section.h"
class PageHeaderSection : public Section
{
    Q_OBJECT
public:
    explicit PageHeaderSection(QGraphicsItem *parent = 0);
    
    QDomElement xml(QDomDocument doc, QList<Container*>& usedItems,QMap<QString,bool> &querys);
    bool onFisrtPage() const;

signals:
    
public slots:
    void setOnFisrtPage(bool onFisrtPage);
private:
    bool m_onFisrtPage;
};

#endif // PAGEHEADERSECTION_H
