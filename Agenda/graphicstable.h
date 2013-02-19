#ifndef GRAPHICSTABLE_H
#define GRAPHICSTABLE_H

#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QRectF>
#include <QPainter>
#include <QVector>
#include "graphicsevent.h"
class GraphicsTable : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit GraphicsTable(QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void setSize(qreal w, qreal h);
    void addEvento(QColor color , QDateTime start , QDateTime end, QString asunto);//TODO parametros evento
    qreal timeToPos(int hour, int minute);    
    QDateTime posToTime(qreal pos);
signals:
    void main_jail(QRectF jail);
    void horizontalGrid(QVector<qreal>,qreal);
public slots:
    void checkCollisions();
    void setDate(QDate);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    qreal width;
    qreal heigth;
    QVector<GraphicsEvent*> eventos;
    QVector<qreal> h_grids;
    QDate m_date;
};

#endif // GRAPHICSTABLE_H
