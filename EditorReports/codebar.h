#ifndef CODEBAR_H
#define CODEBAR_H

#include "customlabel.h"
class CodeBar : public Container
{
    Q_OBJECT
    Q_PROPERTY(QString code READ code WRITE setcode NOTIFY codeChanged)
    Q_PROPERTY(bool visibleCode READ visibleCode WRITE setvisibleCode NOTIFY visibleCodeChanged)
    Q_PROPERTY(QString sql READ sql WRITE setsql NOTIFY sqlChanged)
    Q_PROPERTY(int codeSize READ codeSize WRITE setcodeSize NOTIFY codeSizeChanged)
    Q_PROPERTY(int barSize READ barSize WRITE setbarSize NOTIFY barSizeChanged)
    Q_PROPERTY(bool vertical READ vertical WRITE setvertical NOTIFY verticalChanged)
public:
    explicit CodeBar(QGraphicsItem *parent = 0);

    QDomElement xml(QDomDocument doc, QPointF relPos,QList<Section*> sectionPool);
    void parseXml(QDomElement element, QPointF origin);
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }


    QString code() const;
    bool visibleCode() const;
    void editMe();
    QString sql() const;

    int codeSize() const;
    int barSize() const;
    bool vertical() const;

signals:

    void codeChanged(QString arg);
    void visibleCodeChanged(bool arg);

    void sqlChanged(QString arg);

    void codeSizeChanged(int arg);

    void barSizeChanged(int arg);

    void verticalChanged(bool arg);

public slots:

    void setcode(QString arg);
    void setvisibleCode(bool arg);    
    void setsql(QString arg);

    void setcodeSize(int arg);
    void setbarSize(int arg);
    void setvertical(bool arg);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    QString m_code;
    bool m_visibleCode;
    QString m_sql;
    int m_codeSize;
    int m_barSize;
    bool m_vertical;
};

#endif // CODEBAR_H
