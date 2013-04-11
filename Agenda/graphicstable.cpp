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
    checkCollisions();
}

void GraphicsTable::appendEvento(int id ,QDateTime start, QDateTime end, QColor color,QString titulo, QString asunto ,int id_cliente , bool isCita , bool isPrivado)
{
    GraphicsEvent * evento = new GraphicsEvent(this);

    evento->id = id;

    evento->isPrivado = isPrivado;

    evento->id_cliente = id_cliente;

    evento->id_user = m_user;

    evento->isCita = isCita;

    evento->setColor(color);

    evento->setTime(start,end);

    evento->setAsunto(titulo,asunto);

    connect(evento,SIGNAL(aboutClose()),this,SLOT(eventClosing()));

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

void GraphicsTable::addEvento(QColor color, QDateTime start, QDateTime end ,QString titulo, QString asunto, bool isCita , int id_cliente , bool isPrivado)
{
    QSqlQuery q(QSqlDatabase::database("Maya"));
    q.prepare("INSERT INTO agenda"
              "(dFecha, cHora, id_Usuario, cInicio, cFin, cAsunto,"
              "cDescripcionAsunto, cEstado, cAvisarTiempo, cImportancia, color,"
              "id_especialidad, id_departamento, isMedica, Id_Cliente, isCita , isPrivado)"
              "VALUES "
              "(:dFecha, :cHora, :id_Usuario, :cInicio, :cFin, :cAsunto,"
              ":cDescripcionAsunto, :cEstado, :cAvisarTiempo, :cImportancia, :color,"
              ":id_especialidad, :id_departamento, :isMedica, :Id_Cliente, :isCita , :isPrivado)");

    QString sColor = QString("%1;%2;%3").arg(color.red()).arg(color.green()).arg(color.blue());
    q.bindValue(":dFecha",m_date);
    q.bindValue(":cHora",0);
    q.bindValue(":id_Usuario",m_user);
    q.bindValue(":cInicio",start);
    q.bindValue(":cFin",end);
    q.bindValue(":cAsunto",titulo);
    q.bindValue(":cDescripcionAsunto",asunto);
    q.bindValue(":cEstado",0);
    q.bindValue(":cAvisarTiempo",0);
    q.bindValue(":cImportancia",0);
    q.bindValue(":color",sColor);
    q.bindValue(":id_especialidad",0);//FIXME - Agenda: parametros evento
    q.bindValue(":id_departamento",0);
    q.bindValue(":isMedica",Configuracion_global->medic);
    q.bindValue(":Id_Cliente",id_cliente);
    q.bindValue(":isCita",isCita);
    q.bindValue(":isPrivado",isPrivado);

    int id = 0;
    if(q.exec())
        id = q.lastInsertId().toInt();
    appendEvento(id , start, end, color,titulo, asunto , id_cliente, isCita, isPrivado );
}

qreal GraphicsTable::timeToPos( int hour , int minute)
{
    qreal h_pos = 62 + (hour * 80);
    qreal m_pos = (minute * 80)/60;
    h_pos += m_pos;

    return h_pos;
}

void GraphicsTable::UpdateEventos()
{
    GraphicsEvent * e;
    foreach(e,eventos)
        /*e->deleteLater()*/delete e;
    this->eventos.clear();

    QSqlQuery q(QSqlDatabase::database("Maya"));
    QString fecha = m_date.toString("yyyy-MM-dd");
    QString sql = QString("SELECT * FROM agenda where dFecha = '%1' and id_Usuario = %2")
            .arg(fecha).arg(m_user);
    q.prepare(sql);
    if(q.exec())
    {
        while(q.next())
            showEvento(q.record());
    }
    checkCollisions();

    if(m_user == Configuracion_global->id_usuario_activo)
    {
        emit allowEdits(true);
    }
    else
    {
        QString sPermision = QString("SELECT * FROM permisos_agenda WHERE id_Usuario_editor = %1 AND id_Usuario_agenda= %2")
                .arg(Configuracion_global->id_usuario_activo).arg(m_user);
        if(q.exec(sPermision))
        {
            if(!q.next())
            {
                for (int i=0;i<eventos.size();i++)
                    eventos.at(i)->setReadOnly(true);
                emit allowEdits(false);
            }
        }
    }
}

void GraphicsTable::setDate(QDate d)
{
    this->m_date = d;

    UpdateEventos();
}

void GraphicsTable::setUser(int user)
{
    m_user = user;

    UpdateEventos();
}

void GraphicsTable::eventClosing()
{
    GraphicsEvent* e = qobject_cast<GraphicsEvent*>(sender());
    if(e)
    {
        eventos.remove(eventos.indexOf(e));
        int id = e->id;
        QSqlQuery q(QSqlDatabase::database("Maya"));
        QString sql = QString("DELETE FROM agenda WHERE Id = %1").arg(id);
        if(!q.exec(sql))
            qDebug() << q.lastError();
        e->deleteLater();
    }
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

void GraphicsTable::showEvento(QSqlRecord r)
{
    int id = r.value("Id").toInt();

    bool isCita = r.value("isCita").toBool();
    int id_cliente = r.value("Id_Cliente").toInt();
    QDateTime start = r.value("cInicio").toDateTime();
    QDateTime end = r.value("cFin").toDateTime();
    QString auxColor = r.value("color").toString();
    QStringList desglose_color = auxColor.split(";");
    int red = 100;
    int green = 100;
    int blue = 100;
    if(desglose_color.size() == 3)
    {
        red = desglose_color.at(0).toInt();
        green = desglose_color.at(1).toInt();
        blue = desglose_color.at(2).toInt();
    }
    QColor color = QColor::fromRgb(red,green,blue);
    QString titulo = r.value("cAsunto").toString();
    QString asunto = r.value("cDescripcionAsunto").toString();
    bool priv = r.value("isPrivado").toBool();
    appendEvento(id,start,end,color,titulo,asunto,id_cliente,isCita,priv);
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
