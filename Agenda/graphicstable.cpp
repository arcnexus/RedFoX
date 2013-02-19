#include "graphicstable.h"
#include <QVarLengthArray>
#include <QDebug>
#include <QDateTime>
GraphicsTable::GraphicsTable(QGraphicsItem * parent) :
    QGraphicsObject(parent)
{
    width = 100;
    heigth = 100;
}

QRectF GraphicsTable::boundingRect() const
{
    return QRectF(0,0,width,heigth);
}

void GraphicsTable::setSize(qreal w, qreal h)
{
    this->width = w;
    this->heigth = h;
    QRectF jail(80,62,w-90,h-81);
   // GraphicsEvent::setWidth(width-90);
    emit main_jail(jail);
}

void GraphicsTable::addEvento(QColor color, QDateTime start, QDateTime end , QString asunto)
{
    GraphicsEvent * evento = new GraphicsEvent(this); 

    evento->setColor(color);

    evento->setTime(start,end);

    evento->setAsunto(asunto);

    connect(evento,SIGNAL(updateScene()),this->scene(),SLOT(update()));

    connect(evento,SIGNAL(checkForCollisions()),this,SLOT(checkCollisions()));

    connect(this,SIGNAL(main_jail(QRectF)),evento,SLOT(setMain_jail(QRectF)));

    connect(this,SIGNAL(horizontalGrid(QVector<qreal>,qreal)),
            evento,SLOT(setHGrid(QVector<qreal>,qreal)));


    qreal y0 = timeToPos(start.time().hour(),start.time().minute());
    qreal y1 = timeToPos(end.time().hour(),end.time().minute());
    qreal h = y1 - y0;

    evento->setMain_jail(QRectF(80,62,width-90,heigth-81));
    evento->setSize(width-90,h);
    evento->setHGrid(h_grids,20);    
    evento->setPos(80,y0);

    eventos.append(evento);
    checkCollisions();
}

qreal GraphicsTable::timeToPos( int hour , int minute)
{
    qreal h_pos = 62 + (hour * 80);
    qreal m_pos = (minute * 80)/60;
    h_pos += m_pos;

    return h_pos;
}

void GraphicsTable::setDate(QDate d)
{
    //this->scene()->clear();
    this->eventos.clear();
    this->m_date = d;
    //TODO load "date" events
}

QDateTime GraphicsTable::posToTime(qreal pos)
{
    qreal start = pos-62;
    int hours = (int)start/80;

    int _minuts =  (int)start % 80;
    int minuts = (_minuts*60)/80;
    return QDateTime(m_date,QTime(hours,minuts));
}

void GraphicsTable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   h_grids.clear();
   painter->drawRect(this->boundingRect());

   QVarLengthArray<QRectF, 30> hour_rect;
   QVarLengthArray<QRectF, 30> desc_rect;
   QVarLengthArray<QLineF, 30> split_desc;
   QVarLengthArray<QLineF, 30> split_desc2;

   painter->save();
   qreal desc_width = this->boundingRect().width() - 90;
   qreal y = 62;
   for (int i=0 ;i<24; i++ )
   {
       //60x42
       h_grids.append(y);
       h_grids.append(y+20);
       h_grids.append(y+40);
       h_grids.append(y+60);
       hour_rect.append(QRectF(20,y,60,80));
       desc_rect.append(QRectF(80,y,desc_width,80));
       split_desc.append(QLineF(80,y+40,desc_width+80,y+40));
       split_desc2.append(QLineF(80,y+20,desc_width+80,y+20));
       split_desc2.append(QLineF(80,y+60,desc_width+80,y+60));
       QString hour = QString().sprintf("%02d:00",i);
       painter->drawText(QPoint(35,y+45),hour);
       y += 80;
   }
   emit horizontalGrid(h_grids,20);
   painter->restore();

   painter->save();
   painter->setPen(Qt::darkGray);
   painter->drawRects(hour_rect.data(), hour_rect.size());
   painter->setBrush(QBrush(Qt::gray));
   painter->drawRects(desc_rect.data(), desc_rect.size());
   painter->setPen(Qt::DashDotLine);
   painter->drawLines(split_desc.data(), split_desc.size());
   painter->setPen(Qt::DotLine);
   painter->drawLines(split_desc2.data(), split_desc2.size());
   painter->restore();

   //TODO GraphicsTable::Hour line - only if selected date is today
   int hour = QDateTime::currentDateTime().toString("hh").toInt();
   int minute = QDateTime::currentDateTime().toString("mm").toInt();

   qreal h_pos = timeToPos(hour , minute);

   painter->save();
   painter->setPen(Qt::darkRed);
   painter->drawLine(QLine(5,h_pos,this->boundingRect().width()-5,h_pos));
   painter->restore();
}


void GraphicsTable::checkCollisions()
{
    GraphicsEvent * e = qobject_cast<GraphicsEvent *>(sender());

    if(e)
    {
        qreal start_point = e->pos().ry();
        qreal end_point = start_point + e->Heigth();
        QDateTime start = posToTime(start_point);
        QDateTime end = posToTime(end_point);
        e->setTime(start,end);
    }

    for (int i=0;i<eventos.size();i++)
        eventos.at(i)->checkCollisions();
}
