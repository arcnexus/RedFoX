#include "blinkink.h"

Blinkink::Blinkink(QWidget *parent) :
    QFrame(parent),
    _layout(this),
    _icon(this),
    _top(this),
    _bottom(this),
    _animation(this,"step",this)
{
    _icon.setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);
    _layout.addWidget(&_icon,0,0,2,1);
    _layout.addWidget(&_top,0,1);
    _layout.addWidget(&_bottom,1,1);
    _layout.setSpacing(0);
    _layout.setMargin(2);

    _top.setStyleSheet("font-weight: bold;");
    _bottom.setStyleSheet("font-style: italic;");

    this->setLayout(&_layout);
    _animation.setDuration(2000);
    _animation.setStartValue(1);
    _animation.setEndValue(0.2);
    m_blinkDuration = 2000;

    connect(&_animation,SIGNAL(finished()),this,SLOT(aniEnd()));
}

QString Blinkink::colorToString(QColor c)
{
    return QString("rgba(%1, %2, %3, %4)").arg(c.red()).arg(c.green()).arg(c.blue()).arg(m_step);
}

QString Blinkink::backGroundString(QColor c)
{
    return QString(" background-color: %1;").arg(colorToString(c));
}

bool Blinkink::blink() const {
    return m_blink;
}

void Blinkink::setblink(bool arg) {
    if (m_blink != arg) {
        m_blink = arg;
        if(arg)
            _animation.start();
        else
        {
            _animation.stop();
            setStep(1);
        }
        emit blinkChanged(arg);
    }
}

int Blinkink::blinkDuration() const {
    return m_blinkDuration;
}

void Blinkink::setblinkDuration(int arg) {
    if (m_blinkDuration != arg) {
        m_blinkDuration = arg;
        _animation.setDuration(arg);
        emit blinkDurationChanged(arg);
    }
}


void Blinkink::aniEnd()
{
    _animation.setStartValue(1);
    _animation.setEndValue(0);
    _animation.start();
}

QPixmap Blinkink::icon() const {
    return m_icon;
}

void Blinkink::seticon(QPixmap arg) {
        m_icon = arg;
        _icon.setPixmap(arg);
        emit iconChanged(arg);
}

QString Blinkink::topTex() const {
    return m_topTex;
}

QString Blinkink::bottomTex() const {
    return m_bottomTex;
}

void Blinkink::settopTex(QString arg) {
    if (m_topTex != arg) {
        m_topTex = arg;
        _top.setText(arg);
        emit topTexChanged(arg);
    }
}

void Blinkink::setbottomTex(QString arg) {
    if (m_bottomTex != arg) {
        m_bottomTex = arg;
        _bottom.setText(arg);
        emit bottomTexChanged(arg);
    }
}

void Blinkink::setStep(double arg) {
    if (m_step != arg) {
        m_step = arg;
        this->setStyleSheet(backGroundString(m_color));
        emit stepChanged(arg);
    }
}

void Blinkink::setcolor(QColor arg) {
    if (m_color != arg) {
        m_color = arg;
        setStep(1);
        emit colorChanged(arg);
    }
}

double Blinkink::step() const {
    return m_step;
}

QColor Blinkink::color() const {
    return m_color;
}
