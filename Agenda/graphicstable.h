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
    void addEvento(QColor color , QDateTime start , QDateTime end,QString titulo, QString asunto, bool isCita, int id_cliente);//TODO parametros evento
    qreal timeToPos(int hour, int minute);    
    QDateTime posToTime(qreal pos);
    void UpdateEventos();
    void appendEvento(int id ,QDateTime start, QDateTime end, QColor color,QString titulo, QString asunto ,int id_cliente , bool isCita);
signals:
    void main_jail(QRectF jail);
    void horizontalGrid(QVector<qreal>,qreal);
    void allowEdits(bool);
public slots:
    void checkCollisions();
    void setDate(QDate);
    void setUser(int user);
    void eventClosing();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void showEvento(QSqlRecord r);
private:
    qreal width;
    qreal heigth;
    QVector<GraphicsEvent*> eventos;
    QVector<qreal> h_grids;
    QDate m_date;
    int m_user;
};

#endif // GRAPHICSTABLE_H
