#include "mayamodule.h"

MayaModule::MayaModule(module_zone zone , QString name , QWidget *parent) :
    QDialog(parent)
{
    _zone = zone;
    _name = name;
    _user_level = SinAcceso;
    _b_reducida = true;

    tryRegisterModule(zone,name);
}

MayaModule::~MayaModule()
{
}

bool MayaModule::userHaveAcces(int id_user)
{
    if(Configuracion_global->superUser)
    {
        _user_level = Administrador;
        return true;
    }

    QSqlQuery q(Configuracion_global->groupDB);
    q.prepare("SELECT * FROM accesousuarios where id_modulo = :id and id_user=:id_user");
    q.bindValue(":id",_id_modulo);
    q.bindValue(":id_user",id_user);
    if(q.exec())
    {
        if(q.next())
        {
            _user_level = static_cast<accessLevel>(q.record().value(3).toInt());
        }
    }
    return (_user_level != SinAcceso);
}

void MayaModule::_showBarraBusqueda(BarraBusqueda *b)
{
    if(!_b_reducida)
        return;
    b->setGeometry(0,40,0,this->height()-60);
    b->show();
    QPropertyAnimation* animation0 = new QPropertyAnimation(b, "size",this);
    connect(animation0,SIGNAL(finished()),animation0,SLOT(deleteLater()));
    animation0->setDuration(1000);
    animation0->setEasingCurve(QEasingCurve::OutElastic);

    animation0->setStartValue(QSize(0,b->height()));
    animation0->setEndValue(QSize(250,b->height()));

    QPropertyAnimation* animation = new QPropertyAnimation(b, "pos",this);
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::OutElastic);
    animation->setStartValue(QPoint(this->width(),b->pos().y()));
    animation->setEndValue(QPoint(this->width()-250,b->pos().y()));

    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    group->addAnimation(animation);
    group->addAnimation(animation0);

    connect(group,SIGNAL(finished()),group,SLOT(deleteLater()));
    group->start();
     _b_reducida = false;
     b->setShow(true);

}

void MayaModule::_hideBarraBusqueda(BarraBusqueda *b)
{
    if(_b_reducida)
        return;
    QPropertyAnimation* animation0 = new QPropertyAnimation(b, "size",this);
    connect(animation0,SIGNAL(finished()),animation0,SLOT(deleteLater()));
    animation0->setDuration(300);
    animation0->setEasingCurve(QEasingCurve::Linear);

    animation0->setStartValue(QSize(250,b->height()));
    animation0->setEndValue(QSize(20,b->height()));

    QPropertyAnimation* animation = new QPropertyAnimation(b, "pos",this);
    animation->setDuration(200);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->setStartValue(QPoint(this->width()-250,b->pos().y()));
    animation->setEndValue(QPoint(this->width()-20,b->pos().y()));

    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    group->addAnimation(animation);
    group->addAnimation(animation0);

    connect(group,SIGNAL(finished()),group,SLOT(deleteLater()));
    group->start();
    _b_reducida = true;
    b->setShow(false);
}

void MayaModule::_resizeBarraBusqueda(BarraBusqueda *b)
{
    b->resize(b->width(),this->height()-60);
    if(_b_reducida)
        b->move(this->width()-19,40);
    else
        b->move(this->width()-b->width(),40);
}

bool MayaModule::eventFilter(QObject *, QEvent *e)
{
    if(e->type()==QEvent::HoverMove)
    {
        QHoverEvent * ev = reinterpret_cast<QHoverEvent *>(e);
        int x = ev->pos().x();

        //if(x > this->width()-6 && _b_reducida)
        //    emit showBusqueda();

        if(x < this->width()-300 && !_b_reducida)
            emit hideBusqueda();
    }
    else if(e->type() == QEvent::KeyRelease)
    {
        QKeyEvent * ev = reinterpret_cast<QKeyEvent *>(e);
        if(ev->key() == Qt::Key_Escape)
            return true;
    }
    return false;
}

void MayaModule::tryRegisterModule(module_zone zone, QString name)
{
    QSqlQuery q(Configuracion_global->groupDB);
    q.prepare("SELECT * FROM modulos where module_name = :name");
    q.bindValue(":name",name);
    if(q.exec())
    {
        if(!q.next())
            RegisterModule(zone,name);
        else
            _id_modulo = q.record().value(0).toInt();
    }
}

void MayaModule::RegisterModule(module_zone zone , QString name)
{
    QSqlQuery q(Configuracion_global->groupDB);
    QSqlQuery q2(Configuracion_global->groupDB);

    q.prepare("INSERT INTO modulos (module_zone, module_name) VALUES (:zone, :name);");
    q.bindValue(":zone",zone);
    q.bindValue(":name",name);
    q.exec();
    _id_modulo = q.lastInsertId().toInt();
    q.prepare("SELECT id FROM usuarios;");
    q.exec();
    while(q.next())
    {
        //insertamos "Sin acceso" para todos los usuarios por defecto
        q2.prepare("INSERT INTO accesousuarios (id_user, id_modulo, id_nivel_acceso) "
                   "VALUES (:id, :id_modulo, 7);");
        q2.bindValue(":id",q.record().value(0).toInt());
        q2.bindValue(":id_modulo",_id_modulo);
        q2.exec();
    }
}
