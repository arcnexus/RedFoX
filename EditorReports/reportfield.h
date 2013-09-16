#ifndef REPORTFIELD_H
#define REPORTFIELD_H

#include "container.h"
class ReportField : public Container
{
    Q_OBJECT
    Q_PROPERTY(QString sql READ sql WRITE setsql NOTIFY sqlChanged)
    Q_PROPERTY(bool expandable READ expandable WRITE setexpandable NOTIFY expandableChanged)

    Q_PROPERTY(_Aling Alineacion READ Alineacion WRITE setAlineacion NOTIFY AlineacionChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setfontFamily NOTIFY fontFamilyChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setfontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(int fontWeigth READ fontWeigth WRITE setfontWeigth NOTIFY fontWeigthChanged)
    Q_PROPERTY(bool italicFont READ italicFont WRITE setitalicFont NOTIFY italicFontChanged)
    Q_PROPERTY(bool underlined READ underlined WRITE setunderlined NOTIFY underlinedChanged)
    Q_PROPERTY(QFont Font READ Font WRITE setFont NOTIFY FontChanged)
    Q_PROPERTY(QColor fontColor READ fontColor WRITE setfontColor NOTIFY fontColorChanged)
    Q_PROPERTY(int formato READ formato WRITE setformato NOTIFY formatoChanged)

public:
    explicit ReportField(QGraphicsItem  *parent = 0);

    QDomElement xml(QDomDocument doc, QPointF relPos,QList<Section*> sectionPool);
    void parseXml(QDomElement, QPointF origin);
    void editMe();
    QString query();
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    QString sql() const;
    bool expandable() const;
    _Aling Alineacion() const;
    QString fontFamily() const;
    int fontSize() const;
    int fontWeigth() const;
    bool italicFont() const;
    bool underlined() const;
    QFont Font() const;
    QColor fontColor() const;
    int formato() const;

signals:

    void sqlChanged(QString arg);
    void expandableChanged(bool arg);
    void AlineacionChanged(_Aling arg);
    void fontFamilyChanged(QString arg);
    void fontSizeChanged(int arg);
    void fontWeigthChanged(int arg);
    void italicFontChanged(bool arg);
    void underlinedChanged(bool arg);
    void FontChanged(QFont arg);
    void fontColorChanged(QColor arg);        
    void formatoChanged(int arg);

public slots:

    void setsql(QString arg);
    void setexpandable(bool arg);
    void setAlineacion(_Aling arg);
    void setfontFamily(QString arg);
    void setfontSize(int arg);
    void setfontWeigth(int arg);
    void setitalicFont(bool arg);
    void setunderlined(bool arg);
    void setFont(QFont arg);
    void setfontColor(QColor arg);        
    void setformato(int arg);

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QString m_sql;
    bool m_expandable;
    _Aling m_Alineacion;
    QString m_fontFamily;
    int m_fontSize;
    int m_fontWeigth;
    bool m_italicFont;
    bool m_underlined;
    QFont m_Font;
    QColor m_fontColor;
    int m_formato;
};

#endif // REPORTFIELD_H
