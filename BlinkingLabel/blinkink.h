#ifndef BLINKINK_H
#define BLINKINK_H

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QColor>
#include <QPropertyAnimation>

#if defined(SHAREDLIB_LIBRARY)
#  define SHAREDLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHAREDLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

class SHAREDLIBSHARED_EXPORT Blinkink : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setcolor NOTIFY colorChanged)
    Q_PROPERTY(bool blink READ blink WRITE setblink NOTIFY blinkChanged)
    Q_PROPERTY(int blinkDuration READ blinkDuration WRITE setblinkDuration NOTIFY blinkDurationChanged)
    Q_PROPERTY(QPixmap icon READ icon WRITE seticon NOTIFY iconChanged)
    Q_PROPERTY(QString topTex READ topTex WRITE settopTex NOTIFY topTexChanged)
    Q_PROPERTY(QString bottomTex READ bottomTex WRITE setbottomTex NOTIFY bottomTexChanged)
    Q_PROPERTY(double step READ step WRITE setStep NOTIFY stepChanged)
public:
    Blinkink(QWidget *parent = 0);

    bool blink() const;

    int blinkDuration() const;

    QPixmap icon() const;

    QString topTex() const;

    QString bottomTex() const;

    double step() const;

    QColor color() const;

public slots:

    void setblink(bool arg);

    void setblinkDuration(int arg);

    void seticon(QPixmap arg);

    void settopTex(QString arg);

    void setbottomTex(QString arg);

    void setStep(double arg);

    void setcolor(QColor arg);

signals:
    void blinkChanged(bool arg);

    void blinkDurationChanged(int arg);

    void iconChanged(QPixmap arg);

    void topTexChanged(QString arg);

    void bottomTexChanged(QString arg);

    void stepChanged(double arg);

    void colorChanged(QColor arg);

private slots:
    void aniEnd();

private:
    QLabel _icon;
    QLabel _top;
    QLabel _bottom;
    QGridLayout _layout;
    QPropertyAnimation _animation;
    double m_step;
    QColor m_color;

    QString colorToString(QColor c);
    QString backGroundString(QColor c);
    bool m_blink;
    int m_blinkDuration;
    QPixmap m_icon;
    QString m_topTex;
    QString m_bottomTex;
};

#endif
