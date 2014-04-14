#ifndef REPORTACUMULATOR_H
#define REPORTACUMULATOR_H

#include "customlabel.h"
class ReportAcumulator : public Container
{
    Q_OBJECT

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
    explicit ReportAcumulator(QString name, QGraphicsItem *parent = 0);

    QDomElement xml(QDomDocument doc, QPointF relPos,QList<Section*> sectionPool);
    void parseXml(QDomElement, QPointF origin);
    void editMe();

    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    _Aling Alineacion() const;
    QString fontFamily() const;
    int fontSize() const;
    int fontWeigth() const;
    bool italicFont() const;
    bool underlined() const;
    QFont Font() const;
    QColor fontColor() const;
    int formato() const;

    QString target() const;
    void setTarget(const QString &target);

signals:

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

    _Aling m_Alineacion;
    QString m_fontFamily;
    int m_fontSize;
    int m_fontWeigth;
    bool m_italicFont;
    bool m_underlined;
    QFont m_Font;
    QColor m_fontColor;
    int m_formato;
    QString m_target;
};

#endif // REPORTACUMULATOR_H
