#ifndef SECTION_H
#define SECTION_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>

#include <QDomElement>
#include <QDomDocument>

#include "customlabel.h"
#include "roundedrect.h"
#include "reportline.h"
#include "codebar.h"
#include "reportimage.h"
#include "reportfield.h"
#include "relationalfield.h"
//#include "container.h"



class Section : public QObject , public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(QString SectionName READ SectionName WRITE setSectionName NOTIFY SectionNameChanged)
    Q_PROPERTY(int vPos READ vPos WRITE setvPos NOTIFY vPosChanged)
    Q_PROPERTY(int Height READ Height WRITE setHeight NOTIFY heightChanged)

public:
    explicit Section(QGraphicsItem *parent = 0);
    QRectF getMargin() const;


    void setMargin(const QRectF &value);
    QString SectionName() const;

    enum SectionType { ReportHeader , PageHeader , Detail , PageFooter , ReportFooter };

    SectionType sectionType() const;
    void setType(const SectionType &sectionType);
    int vPos() const;
    int Height() const;

    int Width() const;
    void setWidth(int Width);

    bool onMaxsize() const;
    void setOnMaxsize(bool onMaxsize);

    virtual QDomElement xml(QDomDocument, QList<Container*>& usedItems, QMap<QString, bool> &querys);
    virtual void parseXml(QDomNode elements, QList<Container*>& itemPool);
signals:
    
    void SectionNameChanged(QString arg);
    void resized(Section*);
    void vPosChanged(int arg);
    void heightChanged(int arg);

public slots:
    void setSectionName(QString arg);    
    void setvPos(int arg);
    void setHeight(int arg);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    QRectF margin;
    QRectF validRect;
    QPointF marginPoint();

    bool _onResize;
    bool _sizeChanged;
    int _wTxt;
    int _hTxt;
    SectionType _type;
    QString _typeName;

    QString m_SectionName;
    int m_vPos;
    int m_Height;
    int m_Width;
    QGraphicsRectItem * _parent;
    bool m_onMaxsize;

    void _parseElementTag(QDomElement ele, QPointF point , QList<Container *> &itemPool);
    void _insertRect(QDomElement ele, QPointF point , QList<Container *> &itemPool);
    void _insertLabel(QDomElement ele, QPointF point , QList<Container *> &itemPool);
    void _insertLine(QDomElement ele, QPointF point , QList<Container *> &itemPool);
    void _insertCodeBar(QDomElement ele, QPointF point , QList<Container *> &itemPool);
    void _insertImage(QDomElement ele, QPointF point , QList<Container *> &itemPool);
    void _insertField(QDomElement ele, QPointF point , QList<Container *> &itemPool);
    void _insertRelField(QDomElement ele, QPointF point , QList<Container *> &itemPool);

};

#endif // SECTION_H
