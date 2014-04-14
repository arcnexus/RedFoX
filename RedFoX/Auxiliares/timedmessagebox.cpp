#include "timedmessagebox.h"

#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QApplication>
#include <QStyle>
#include <QMutexLocker>

#include <QDebug>
int TimedMessageBox::count = 0;
QMutex TimedMessageBox::mutex;

TimedMessageBox::TimedMessageBox(QWidget *parent, QString message, Icons icon) :
    QDialog(parent),
    _icon("",this),
    _message(message,this),
    _layout(this)
{    
  //  parent->setFocus();
    this->setWindowFlags(this->windowFlags()^Qt::FramelessWindowHint);

    _icon.setPixmap(getIcon(icon));

    _layout.addWidget(&_icon);
    _layout.addWidget(&_message);

    this->setLayout(&_layout);
    this->show();
    QTimer::singleShot(0,this,SLOT(startAnimation()));
}

TimedMessageBox::~TimedMessageBox()
{
    count--;
}

void TimedMessageBox::Box(QWidget *parent, QString message, TimedMessageBox::Icons icon)
{
    TimedMessageBox * t = new TimedMessageBox(parent,message,icon);
    Q_UNUSED(t);
}

void TimedMessageBox::startAnimation()
{
    QMutexLocker locker(&mutex);
    count++;
    QRect dr= QApplication::desktop()->rect();
    this->move(dr.right()-this->size().width(),dr.bottom());
    QPropertyAnimation *ani = new QPropertyAnimation(this,"pos",this);
    connect(ani,SIGNAL(finished()),ani,SLOT(deleteLater()));
    connect(ani,SIGNAL(finished()),this,SLOT(doHide()));
    ani->setStartValue(this->pos());
    ani->setEndValue(QPoint(this->pos().x(),dr.bottom()-this->frameGeometry().height()-this->frameGeometry().height()*count));
    ani->setDuration(500);
    ani->start();
}

void TimedMessageBox::doHide()
{
    QPropertyAnimation *ani = new QPropertyAnimation(this,"windowOpacity",this);
    connect(ani,SIGNAL(finished()),ani,SLOT(deleteLater()));
    connect(ani,SIGNAL(finished()),this,SLOT(deleteLater()));
    ani->setStartValue(1);
    ani->setEndValue(0);
    ani->setDuration(5000);
    ani->start();
}

QPixmap TimedMessageBox::getIcon(TimedMessageBox::Icons icon)
{
    QStyle *style = this ? this->style() : QApplication::style();
    int iconSize = style->pixelMetric(QStyle::PM_MessageBoxIconSize, 0, this);
    QIcon tmpIcon;
    switch (icon) {
    case Infomation:
    tmpIcon = style->standardIcon(QStyle::SP_MessageBoxInformation, 0, this);
    break;
    case Warning:
    tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, this);
    break;
    case Critical:
    tmpIcon = style->standardIcon(QStyle::SP_MessageBoxCritical, 0, this);
    break;
    }
    if (!tmpIcon.isNull())
        return tmpIcon.pixmap(iconSize, iconSize);
    return QPixmap();
}
