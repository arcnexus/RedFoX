#ifndef REPORTVIEW_H
#define REPORTVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QDebug>
class ReportView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ReportView(QWidget *parent = 0);
    
signals:
    
public slots:

protected:
    virtual void wheelEvent(QWheelEvent* event);
    
};

#endif // REPORTVIEW_H
