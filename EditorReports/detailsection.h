#ifndef DETAILSECTION_H
#define DETAILSECTION_H

#include "section.h"
class DetailSection : public Section
{
    Q_OBJECT
    Q_PROPERTY(QString SqlGlobal READ SqlGlobal WRITE setSqlGlobal NOTIFY SqlGlobalChanged)
    Q_PROPERTY(QString SqlInterno READ SqlInterno WRITE setSqlInterno NOTIFY SqlInternoChanged)
    Q_PROPERTY(QString ClausulaInterna READ ClausulaInterna WRITE setClausulaInterna NOTIFY ClausulaInternaChanged)
    Q_PROPERTY(bool colorear READ colorear WRITE setcolorear NOTIFY colorearChanged)
    Q_PROPERTY(QColor color1 READ color1 WRITE setcolor1 NOTIFY color1Changed)
    Q_PROPERTY(bool use2Colors READ use2Colors WRITE setuse2Colors NOTIFY use2ColorsChanged)
    Q_PROPERTY(QColor color2 READ color2 WRITE setcolor2 NOTIFY color2Changed)
public:
    explicit DetailSection(QGraphicsItem *parent = 0);
    
    bool header() const;
    bool foot() const;    

    QDomElement xml(QDomDocument doc, QList<Container*>& usedItems,QMap<QString, bool> &querys);
    void parseXml(QDomNode , QList<Container*>& itemPool);

    QString SqlGlobal() const;
    QString SqlInterno() const;
    QString ClausulaInterna() const;

    bool colorear() const;
    QColor color1() const;
    bool use2Colors() const;
    QColor color2() const;

    QString ColorString(QColor c);
    QColor  ColorFromString(QString s);
signals:
    
    void SqlGlobalChanged(QString arg);
    void SqlInternoChanged(QString arg);
    void ClausulaInternaChanged(QString arg);

    void colorearChanged(bool arg);
    void color1Changed(QColor arg);
    void use2ColorsChanged(bool arg);
    void color2Changed(QColor arg);

public slots:
    void setFoot(bool foot);
    void setHeader(bool b);
    void setSqlGlobal(QString arg);
    void setSqlInterno(QString arg);
    void setClausulaInterna(QString arg);

    void setcolorear(bool arg);
    void setcolor1(QColor arg);
    void setuse2Colors(bool arg);
    void setcolor2(QColor arg);

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    bool m_header;
    int m_headerSize;

    bool m_foot;
    int m_footSize;

    int hTxtCab;
    int wTxtCab;

    int wTxtFoot;
    int hTxtFoot;

    bool onResizeHead;
    bool onResizeFoot;

    void _parseElementsBlock(QDomNode eleNode, QPointF point,QList<Container*>& itemPool);
    QString m_SqlGlobal;
    QString m_SqlInterno;
    QString m_ClausulaInterna;
    bool m_colorear;
    QColor m_color1;
    bool m_use2Colors;
    QColor m_color2;
};

#endif // DETAILSECTION_H
