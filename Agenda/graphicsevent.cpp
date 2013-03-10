#include "graphicsevent.h"
#include "graphicstable.h"
#include "editeventform.h"
#include <QPointF>
#include <QGraphicsScene>
#include <QWidget>

QVector<qreal> GraphicsEvent::h_grid;
qreal GraphicsEvent::h_gridSize;

GraphicsEvent::GraphicsEvent(QGraphicsItem * parent) :
    QGraphicsObject(parent)
{
    heigth = 100;
    width = 100;
    this->setFlag(QGraphicsItem::ItemIsMovable,true);
    this->setFlag(QGraphicsItem::ItemIsSelectable,true);
    this->setFlag(QGraphicsItem::ItemIsFocusable,true);
    this->setAcceptHoverEvents(true);
    on_click = false;
    on_moving = false;
    on_Vresize = false;
    on_Hresize = false;
    under_mouse = false;

    qsrand(QDateTime::currentDateTime().toString("zzz").toInt());
    red = qrand()%255;
    green = qrand()%255;
    blue = qrand()%255;

    this->setToolTip("Test Tooltip");
    isReadOnly = false;
}

QRectF GraphicsEvent::boundingRect() const
{
    return QRectF(0,0,width,heigth);
}

void GraphicsEvent::setSize(qreal w, qreal h)
{
    //TODO - GraphicsEvent:: size to time

    if(this->pos().ry() + h <= main_jail.bottom())
    {
        prepareGeometryChange();
        width = w;
        this->heigth = h;
        this->update(this->boundingRect());
        emit updateScene();
    }
}

void GraphicsEvent::setWidth(qreal w)
{
    width = w;
}

void GraphicsEvent::setColor(QColor color)
{
    red = color.toRgb().red();
    green = color.toRgb().green();
    blue = color.toRgb().blue();
}

void GraphicsEvent::setTime(QDateTime start, QDateTime end)
{
    this->start = start;
    this->end = end;
    updateThis();
}

void GraphicsEvent::setHGrid(QVector<qreal> grid, qreal size)
{
    GraphicsEvent::h_grid = grid;
    GraphicsEvent::h_gridSize = size;
}

void GraphicsEvent::adjustTopGrid()
{
    for(int i = 0 ;i < h_grid.size(); i++)
    {
        if ((this->pos().ry() - h_grid.at(i) > 0)
                && ((this->pos().ry() - h_grid.at(i) < h_gridSize )))
        {
            this->setPos(pos().rx(),h_grid.at(i));
            break;
        }
    }
}

void GraphicsEvent::adjustBottomGrid()
{
    for(int i= 0; i< h_grid.size() ; i++)
    {
        if ((this->pos().ry() + heigth < h_grid.at(i))
                && ((this->pos().ry()+ heigth - h_grid.at(i) < h_gridSize )))
        {
            this->setSize(width ,h_grid.at(i) - this->pos().ry());
            break;
        }
    }
}

qreal GraphicsEvent::Heigth()
{
    return heigth;
}

QRectF GraphicsEvent::Rect()
{
    QRectF r(this->pos().rx(),this->pos().ry(),width,heigth);
    return r;
}

void GraphicsEvent::setAsunto(QString t, QString s)
{
    titulo = t;
    asunto = s;
    updateThis();
}

void GraphicsEvent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (this->hasFocus())
    {
        if(on_moving)
            painter->setBrush(QColor::fromRgba(qRgba(red,green,blue,80)));
        else
            painter->setBrush(QColor::fromRgba(qRgba(red,green,blue,255)));
    }
    else
        painter->setBrush(QColor::fromRgba(qRgba(red,green,blue,200)));

    if(under_mouse || this->hasFocus())
    {
        painter->setPen(QColor::fromRgba(qRgba(245,61,0,180)));
    }

    painter->drawRoundedRect(this->boundingRect(),10,10);

    if(under_mouse || this->hasFocus())
    {
        if(heigth > 20 && width >20)
        {
            painter->save();
            painter->setPen(Qt::black);
            //resize lines
            painter->drawLine(width/4,heigth-7,width/4*3,heigth-7);
            painter->drawLine(width/4,heigth-4,width/4*3,heigth-4);
            painter->drawLine(width-7,heigth/4,width-7,heigth/4*3);
            painter->drawLine(width-4,heigth/4,width-4,heigth/4*3);
            painter->restore();
            painter->drawPixmap(width-20,4,16,16,QPixmap(":/Icons/images/close.png"));

        }
    }
    if(width > 100) //tir privado : 03:0 -> 17 length
    {
        int l = (width * 15)/100;
        QString sDate = QString("%1 : %2-%3").arg(titulo).arg(start.toString("hh:mm")).arg(end.toString("hh:mm"));
        if(sDate.length()>l)
        {
            QString aux = sDate.left(l-4);
            aux.append("...");
            sDate = aux;
        }
        QFont font = painter->font() ;
        font.setPointSize ( 8 );
        painter->setFont(font);
        painter->setPen(Qt::black);
        painter->drawText(this->boundingRect(),Qt::AlignHCenter,sDate);

        l= ((width-10) * 15)/100;
        QRectF body(5,15,width-5,Heigth()-20);
        QStringList lines = asunto.split("<br>",QString::SkipEmptyParts);
        QString display;
        QString line;
        int aux = 0;
        for (int i=0;i<lines.size();i++)
        {
            QStringList words = lines.at(i).split(" ",QString::SkipEmptyParts);
            for (int a = 0; a< words.size();a++)
            {
                if(aux>l)
                {
                    aux=0;
                    line.append("\n");
                }
                line.append(words.at(a));
                line.append(" ");
                aux+= words.at(a).size();
                aux++;
            }
            display.append(line);
            display.append("\n");
            aux=0;
        }
        painter->drawText(body,Qt::AlignHCenter,display,&body);
    }
}

void GraphicsEvent::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(isReadOnly || (isPrivado && (id_user != Configuracion_global->id_usuario_activo)))
        return;

    QGraphicsItem::mousePressEvent(event);

    on_click = false;
    on_moving = false;
    on_Vresize = false;
    on_Hresize = false;

    if(event->button()== Qt::LeftButton)
    {
        if(heigth >= 20 && width>=20)
        {
            if((event->pos().rx() > (width-20)) && event->pos().ry() < 20 )
            {
                if(QMessageBox::question(qApp->activeWindow(),
                                      tr("Eliminar"),
                                      tr("¿Borrar el evento?"),
                                      tr("No"),
                                      tr("Si")) == QMessageBox::Accepted)
                {
                    emit aboutClose();
                }
            }
            else if( (width/4 ) < event->pos().rx()
                     && event->pos().rx() < (width/4 * 3)
                     && heigth - 5 < event->pos().ry())
            {
                on_Vresize = true;
            }
            else if( (heigth/4 ) < event->pos().ry()
                     && event->pos().ry() < (heigth/4 * 3)
                     && width - 5 < event->pos().rx())
            {
                on_Hresize = true;
            }
            else
                on_click = true;
        }
        else
            on_click = true;
    }
}

void GraphicsEvent::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(isReadOnly || (isPrivado && (id_user != Configuracion_global->id_usuario_activo)))
        return;

    QGraphicsItem::mouseReleaseEvent(event);
    if(on_moving)
    {
        adjustTopGrid();
    }
    if(on_Vresize)
    {
        adjustBottomGrid();
    }

    on_moving = false;
    on_click = false;
    on_Vresize = false;    

    this->update(this->boundingRect());
    emit updateScene();
    emit checkForCollisions();
}

void GraphicsEvent::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isReadOnly || (isPrivado && (id_user != Configuracion_global->id_usuario_activo)))
        return;

    if(on_click)
    {
        on_moving=true;
        prepareGeometryChange();
        QGraphicsItem::mouseMoveEvent(event);

        checkJail();
    }
    else if (on_Vresize)
    {
        on_moving=true;
        if(mapToScene(event->pos()).ry() - pos().ry() > 5)
        {
            if(isInJail(mapToScene(event->pos())))
            {
                this->setSize(width,event->pos().ry());
                this->update(this->boundingRect());
            }
        }
        event->accept();
    }
    else if(on_Hresize)
    {
        on_moving=true;
        if(mapToScene(event->pos()).rx() - pos().rx() > 5)
        {
            if(isInJail(mapToScene(event->pos())))
            {
                this->setSize(event->pos().rx(),heigth);
                this->update(this->boundingRect());
            }
        }
        event->accept();
    }
    emit updateScene();
}

void GraphicsEvent::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    under_mouse = true;
    this->update(this->boundingRect());
}

void GraphicsEvent::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    under_mouse = false;
    this->update(this->boundingRect());
}

void GraphicsEvent::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if( (width/4 ) < event->pos().rx()
            && event->pos().rx() < (width/4 * 3)
            && heigth - 5 < event->pos().ry())
    {
        setCursor(Qt::SizeVerCursor);
    }
    else if( (heigth/4 ) < event->pos().ry()
            && event->pos().ry() < (heigth/4 * 3)
            && width - 5 < event->pos().rx())
    {
        setCursor(Qt::SizeHorCursor);
    }
    else
        setCursor(Qt::ArrowCursor);
}

void GraphicsEvent::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(isReadOnly || (isPrivado && (id_user != Configuracion_global->id_usuario_activo)))
        return;
   QMenu menu;
   menu.addAction("Editar");
   menu.addAction("Compartir evento");
   menu.addSeparator();
   menu.addAction("Eliminar");
   QAction *a = menu.exec(event->screenPos());
   if(a)
   {
       if(a->text() == "Editar")
       {
            EditThis();
       }
       else if(a->text() == "Compartir evento")
       {
           shareThis();
       }
       else
       {
            emit aboutClose();
       }
   }
}

void GraphicsEvent::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    EditThis();
}

void GraphicsEvent::checkJail()
{
    if(this->pos().ry()< main_jail.top())
        this->setPos(pos().rx(),main_jail.top());

    if(this->pos().ry() + heigth > main_jail.bottom())
        this->setPos(pos().rx(),main_jail.bottom() - heigth);

    if(this->pos().rx() < main_jail.left())
        this->setPos(main_jail.left(),pos().ry());

    if( (this->pos().rx() + width) > (main_jail.right()))
        this->setPos(main_jail.right()-width,pos().ry());
}

bool GraphicsEvent::isInJail(QPointF pos)
{
    return (
                   pos.ry() > main_jail.top()
                && pos.ry() < main_jail.bottom()
                && pos.rx() > main_jail.left()
                && pos.rx() < main_jail.right()
                );
}

void GraphicsEvent::EditThis()
{
    if(isReadOnly || (isPrivado && (id_user != Configuracion_global->id_usuario_activo)))
        return;
    QString aux = asunto.replace("<br>","\n");
    EditEventForm form(qApp->activeWindow());
    form.setColor(QColor::fromRgb(red,green,blue));
    form.setCita(isCita,id_cliente);
    form.setAsunto(titulo,asunto);
    form.setIsPrivado(isPrivado);
    if(form.exec() == QDialog::Accepted)
    {
        this->id_spec = form.id_spec;
        this->id_depart = form.id_depart;
        this->id_cliente = form.id_cliente;
        this->isCita = form.isCita;
        this->red = form.event_color.red();
        this->green = form.event_color.green();
        this->blue = form.event_color.blue();
        this->titulo = form.titulo;
        this->asunto = form.asunto.replace("\n","<br>");
        this->isPrivado = form.isPrivado;
        updateThis();
    }

}

void GraphicsEvent::updateThis()
{
    QString st;
    if(isPrivado && (id_user != Configuracion_global->id_usuario_activo))
    {
        this->setToolTip(tr("Evento privado"));
        return;
    }
    if(isCita)
    {
        QString nombre_cliente;
        QString telefono_cliente;
        for (int i=0;i<Configuracion_global->client_model->rowCount();i++)
        {
            if(Configuracion_global->client_model->record(i).value("Id").toInt() == id_cliente)
            {
                nombre_cliente = Configuracion_global->client_model->record(i).value("cNombreFiscal").toString();
                telefono_cliente = QString("%1 , %2 , %3")
                        .arg(Configuracion_global->client_model->record(i).value("cTelefono1").toString())
                        .arg(Configuracion_global->client_model->record(i).value("cTelefono2").toString())
                        .arg(Configuracion_global->client_model->record(i).value("cMovil").toString());
                break;
            }
        }

        st = QString(
                    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
                    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
                    "p, li { white-space: pre-wrap; }"
                    "</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">"
                    "<p style=\"text-align: center;-qt-block-indent:1\"><b>%1</b></p>"
                    "<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-list-indent: 1;\">"
                    "<li style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Cliente: %2</li>"
                    "<li style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Telefonos: %3</li>"
                    "<li style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Hora de comienzo: %4</li>"
                    "<li style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Hora de finalizaci&oacute;n: %5</li></ul>"
                    "<br>%6</body></html>")
                .arg(/*titulo*/titulo)
                .arg(/*cliente*/nombre_cliente)
                .arg(/*Telefonos*/telefono_cliente)
                .arg(/*Hora inicio*/start.toString("hh:mm"))
                .arg(/*Hora final*/end.toString("hh:mm"))
                .arg(/*Asunto*/asunto);
    }
    else
    {
        st = QString(
                    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
                    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
                    "p, li { white-space: pre-wrap; }"
                    "</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">"
                    "<p style=\"text-align: center;-qt-block-indent:1\"><b>%1</b></p>"
                    "<ul style=\"margin-top: 0px; margin-bottom: 0px; margin-left: 0px; margin-right: 0px; -qt-list-indent: 1;\">"
                    "<li style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Hora de comienzo: %2</li>"
                    "<li style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Hora de finalizaci&oacute;n: %3</li></ul>"
                    "<br>%6</body></html>")
                .arg(/*titulo*/titulo)
                .arg(/*Hora inicio*/start.toString("hh:mm"))
                .arg(/*Hora final*/end.toString("hh:mm"))
                .arg(/*Asunto*/asunto);
    }
    this->setToolTip(st);

    QSqlQuery q(QSqlDatabase::database("terra"));
    q.prepare("UPDATE agenda SET "
              " cHora=:cHora, cInicio=:cInicio, cFin=:cFin, cAsunto=:cAsunto,"
              "cDescripcionAsunto=:cDescripcionAsunto, cEstado=:cEstado, cAvisarTiempo=:cAvisarTiempo,"
              "cImportancia=:cImportancia, color=:color,"
              "id_especialidad=:id_especialidad, id_departamento=:id_departamento,"
              "isMedica=:isMedica, Id_Cliente=:Id_Cliente, isCita=:isCita, isPrivado=:isPrivado "
              " WHERE Id =:id;");

    QString sColor = QString("%1;%2;%3").arg(red).arg(green).arg(blue);
    q.bindValue(":cHora",0);
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
    q.bindValue(":isMedica",medic);
    q.bindValue(":Id_Cliente",id_cliente);
    q.bindValue(":isCita",isCita);
    q.bindValue(":id",id);
    q.bindValue(":isPrivado",isPrivado);

    if(!q.exec())
    {
        qDebug() << q.executedQuery();
        qDebug() << q.lastError();
    }
}

void GraphicsEvent::shareThis()
{
    QList<int> id_users;
    QSqlQuery getUser(QSqlDatabase::database("terra"));
    QString sGetUser = QString("SELECT id_Usuario_agenda FROM  permisos_agenda WHERE id_Usuario_editor = %1").arg(Configuracion_global->id_usuario_activo);
    if(getUser.exec(sGetUser))
    {
        while(getUser.next())
        {
            id_users << getUser.record().value("id_Usuario_agenda").toInt();
        }
    }
    QStringList users;
    if(getUser.exec("SELECT * FROM usuarios"))
    {
        while (getUser.next())
        {
            QSqlRecord r = getUser.record();
            if(id_users.contains(r.value("id").toInt()))
                if(r.value("id")!= id_user)
                    users << r.value("nombre").toString();
        }
    }
    if(id_user != Configuracion_global->id_usuario_activo)
        users << Configuracion_global->cUsuarioActivo;
    if(users.isEmpty())
    {
        QMessageBox::information(qApp->activeWindow(),"Compartir evento","No puede compartir eventos con ningun usuario.\nConsulte al administrador de sistema");
        return;
    }
    bool ok;
    QString sUser = QInputDialog::getItem(qApp->activeWindow(),"Seleccionar usuario","Seleccionar usuario",users,0,false,&ok);
    if(ok)
    {
        int iUser;
        QSqlQuery user(QSqlDatabase::database("terra"));
        QString sql = QString("SELECT id FROM usuarios WHERE nombre = '%1'").arg(sUser);
        if(user.exec(sql))
        {
            if(user.next())
            {
                iUser = user.record().value("id").toInt();
            }
            else
            {
                QMessageBox::critical(qApp->activeWindow(),tr("Error"),tr("Error al recuperar usuario"));
                return;
            }
        }
        QSqlQuery q(QSqlDatabase::database("terra"));
        q.prepare("INSERT INTO agenda"
                  "(dFecha, cHora, id_Usuario, cInicio, cFin, cAsunto,"
                  "cDescripcionAsunto, cEstado, cAvisarTiempo, cImportancia, color,"
                  "id_especialidad, id_departamento, isMedica, Id_Cliente, isCita , isPrivado)"
                  "VALUES "
                  "(:dFecha, :cHora, :id_Usuario, :cInicio, :cFin, :cAsunto,"
                  ":cDescripcionAsunto, :cEstado, :cAvisarTiempo, :cImportancia, :color,"
                  ":id_especialidad, :id_departamento, :isMedica, :Id_Cliente, :isCita , :isPrivado)");

        QString sColor = QString("%1;%2;%3").arg(red).arg(green).arg(blue);
        q.bindValue(":dFecha",start.date());
        q.bindValue(":cHora",0);
        q.bindValue(":id_Usuario",iUser);
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
        q.bindValue(":isMedica",medic);
        q.bindValue(":Id_Cliente",id_cliente);
        q.bindValue(":isCita",isCita);
        q.bindValue(":isPrivado",isPrivado);

        q.exec();
    }
}

void GraphicsEvent::checkCollisions()
{
    QRectF toJail(main_jail.left(),pos().ry(),main_jail.width(),heigth);
    QList<QGraphicsItem *> allItems =  this->scene()->items();
    QList<GraphicsEvent *> collide_items;
    QGraphicsItem * item;
    foreach (item,allItems)
    {
        GraphicsTable * t = 0;
        t = qobject_cast<GraphicsTable *>(item->toGraphicsObject());
        QRectF r = item->boundingRect();
        r.translate(item->pos());
        if(r.intersects(toJail) && t==0)
            collide_items.append(qobject_cast<GraphicsEvent *>(item->toGraphicsObject()));
    }

    if(collide_items.size() == 1)//only this object
    {
        this->setPos(toJail.left(),toJail.top());
        this->setSize(toJail.width(),toJail.height());
    }
    else
    {
        QList<GraphicsEvent *> allEvents;
        foreach (item,allItems)
        {
            GraphicsEvent * e = 0;
            e = qobject_cast<GraphicsEvent *>(item->toGraphicsObject());
            if(e!=0)
                allEvents.append(e);
        }

        for (int i=0;i<collide_items.size();i++)
        {
            QRectF  i_r(main_jail.left(),collide_items.at(i)->pos().ry(),main_jail.width(),collide_items.at(i)->Heigth());
            GraphicsEvent * e;
            foreach(e,allEvents)
            {
                if(collide_items.contains(e))
                    continue;
                if(e->Rect().intersects(i_r))
                {
                    collide_items.append(e);
                    i=0;
                }
            }
        }
        QList<GraphicsEvent *> heigth_o_Events;
        GraphicsEvent * _collideItem;
        foreach(_collideItem,collide_items)
        {
            //qDebug() << _collideItem->Heigth();
            if(heigth_o_Events.isEmpty())
                heigth_o_Events.append(_collideItem);
            else if(heigth_o_Events.last()->Heigth() >= _collideItem->Heigth())
                heigth_o_Events.append(_collideItem);
            else if(heigth_o_Events.at(0)->Heigth()<=_collideItem->Heigth())
                heigth_o_Events.prepend(_collideItem);
            else
            {
                for (int a = 0; a<heigth_o_Events.size(); a++)
                {
                    //qDebug() << "inside loop" << heigth_o_Events.at(a)->Heigth() << _collideItem->Heigth();
                    if(heigth_o_Events.at(a)->Heigth() <= _collideItem->Heigth())
                    {
                        heigth_o_Events.insert(a,_collideItem);
                        break;
                    }
                }
            }
        }
        QList<qreal> Hlist1;
        foreach(_collideItem,heigth_o_Events)
        {
            if(!Hlist1.contains(_collideItem->pos().ry()))
                Hlist1.append(_collideItem->pos().ry());
        }
        QList<qreal> Hlist;
        qreal aux;
        foreach (aux,Hlist1)
        {
            if(Hlist.isEmpty())
                Hlist.append(aux);
            else if(Hlist.last() <= aux)
                Hlist.append(aux);
            else if(Hlist.at(0)>=aux)
                Hlist.prepend(aux);
            else
            {
                for (int a = 0; a<Hlist.size(); a++)
                {
                    if(Hlist.at(a) >= aux)
                    {
                        Hlist.insert(a,aux);
                        break;
                    }
                }
            }
        }
        QList<GraphicsEvent *> EventsOrdered;
        for(int hli = 0; hli<Hlist.size();hli++)
        {
            for(int x = 0; x<heigth_o_Events.size();x++)
                if(heigth_o_Events.at(x)->pos().ry()== Hlist.at(hli))
                    EventsOrdered.append(heigth_o_Events.at(x));
        }
        QHash <GraphicsEvent *,int> ItemInRowWith;
        foreach(_collideItem,EventsOrdered)
        {
            int x =1;
            for (qreal y = _collideItem->pos().ry();y<_collideItem->pos().ry()+_collideItem->Heigth();y+=h_gridSize)
            {
                int _x = 1;
                QRectF rowRect(toJail.left(),y,toJail.width(),h_gridSize);
                GraphicsEvent * _aux;
                foreach(_aux,EventsOrdered)
                {
                    if(_aux == _collideItem)
                        continue;
                    if(_aux->Rect().intersects(rowRect))
                        _x++;
                }
                x = qMax(x,_x);
            }
            ItemInRowWith.insert(_collideItem,x);
            _collideItem->setPos(toJail.left(),_collideItem->pos().ry());
        }
        QList<GraphicsEvent *> usedEvents;
        qreal z = 10;
        foreach(_collideItem,EventsOrdered)
        {
            QRectF _cRect(_collideItem->pos().rx(),_collideItem->pos().ry()+5,
                          toJail.width(),h_gridSize-10);

            QList<GraphicsEvent *> intersectingEvenst;
            qreal xPos=toJail.left();
            QRectF finalRect(_collideItem->pos().rx(),_collideItem->pos().ry(),
                             toJail.width(),_collideItem->Heigth());
            for (int c=0;c<usedEvents.size();c++)
            {
                if(intersectingEvenst.contains(usedEvents.at(c)))
                    continue;
                QRectF cRect = usedEvents.at(c)->Rect();
                if(cRect.intersects(_cRect))
                {
                    if(usedEvents.at(c)->Rect().left() == xPos)
                    {
                        xPos += usedEvents.at(c)->Rect().width();
                    }
                    else
                    {
                        qreal x = qMin(finalRect.width(),usedEvents.at(c)->Rect().left()-xPos);
                        finalRect.setWidth(x);
                    }
                    intersectingEvenst.append(usedEvents.at(c));
                    c=-1;
                }
            }

            qreal w = toJail.width() / ItemInRowWith[_collideItem];
            qreal _fw = qMin(finalRect.width(),w);
            _collideItem->setWidth(_fw);
            _collideItem->setPos(xPos,_collideItem->pos().y());
            _collideItem->setZValue(z);
            z+= 10;
            usedEvents.append(_collideItem);
        }
    }
    //qDebug()<<"end call";
}


void GraphicsEvent::setMain_jail(QRectF jail)
{
    //Ignore first call
    if(main_jail.width()!=0)
    {
        qreal wxj = width / main_jail.width();
        qreal pxj = (pos().rx() - main_jail.left()) / main_jail.width();

        qreal width_after = jail.width() * wxj;
        qreal posx_after = (jail.width() * pxj) + main_jail.left();

        this->setSize(width_after,heigth);
        if(pos().rx() != main_jail.left())
            this->setPos(posx_after, pos().ry());
    }
    this->main_jail = jail;
    checkJail();
}


void GraphicsEvent::setReadOnly(bool b)
{
    isReadOnly = b;
}
