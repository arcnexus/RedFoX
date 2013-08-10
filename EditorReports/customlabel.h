#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include "container.h"
class CustomLabel : public  Container
{
    Q_OBJECT

    Q_PROPERTY(QString Text READ Text WRITE setText NOTIFY TextChanged)
    Q_PROPERTY(_Orientacion Orientacion READ Orientacion WRITE setOrientacion NOTIFY OrientacionChanged)
    Q_PROPERTY(_Aling Alineacion READ Alineacion WRITE setAlineacion NOTIFY AlineacionChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setfontFamily NOTIFY fontFamilyChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setfontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(int fontWeigth READ fontWeigth WRITE setfontWeigth NOTIFY fontWeigthChanged)
    Q_PROPERTY(bool italicFont READ italicFont WRITE setitalicFont NOTIFY italicFontChanged)
    Q_PROPERTY(bool underlined READ underlined WRITE setunderlined NOTIFY underlinedChanged)
    Q_PROPERTY(QFont Font READ Font WRITE setFont NOTIFY FontChanged)
    Q_PROPERTY(QColor fontColor READ fontColor WRITE setfontColor NOTIFY fontColorChanged)

  //  Q_ENUMS (_Orientacion)
  //  Q_ENUMS (_Aling)
public:
    explicit CustomLabel(QGraphicsItem  *parent = 0);
    QDomElement xml(QDomDocument doc, QPointF relPos);
    void parseXml(QDomElement element, QPointF origin);
    void editMe();

    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    QString Text() const;
    _Orientacion Orientacion() const;

    _Aling Alineacion() const;

    QString fontFamily() const;
    int fontSize() const;
    int fontWeigth() const;
    bool italicFont() const;

    QFont Font() const;

    QColor fontColor() const;

    bool underlined() const;

signals:

    void TextChanged(QString arg);
    void OrientacionChanged(_Orientacion arg);
    void AlineacionChanged(_Aling arg);

    void fontFamilyChanged(QString arg);
    void fontSizeChanged(int arg);
    void fontWeigthChanged(int arg);
    void italicFontChanged(bool arg);

    void FontChanged(QFont arg);

    void fontColorChanged(QColor arg);

    void underlinedChanged(bool arg);

public slots:

    void setText(QString arg);
    void setOrientacion(_Orientacion arg);
    void setAlineacion(_Aling arg);

    void setfontFamily(QString arg);
    void setfontSize(int arg);
    void setfontWeigth(int arg);
    void setitalicFont(bool arg);
    void setFont(QFont arg);

    void setfontColor(QColor arg);

    void setunderlined(bool arg);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    QString m_Text;
    _Orientacion m_Orientacion;
    _Aling m_Alineacion;
    QString m_fontFamily;
    int m_fontSize;
    int m_fontWeigth;
    bool m_italicFont;
    QFont m_Font;
    QColor m_fontColor;
    bool m_underlined;
};

#endif // CUSTOMLABEL_H
