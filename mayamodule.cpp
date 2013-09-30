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
    QSqlQuery q(Configuracion_global->groupDB);
    q.prepare("SELECT * FROM accesousuarios where id_modulo = :id and id_user=:id_user");
    q.bindValue(":id",_id_modulo);
    q.bindValue(":id_user",id_user);
    if(q.exec())
    {
        if(q.next())
        {
            _user_level = static_cast<accessLevel>(q.record().value(3).toInt());
            Configuracion_global->nivel = _user_level;
        }
    }
    return (_user_level != SinAcceso);
}

void MayaModule::mouseMoveEvent(QMouseEvent * mouse)
{
    int x = mouse->pos().x();

    if(x > this->width()-6 && _b_reducida)
    {
        emit showBusqueda();
        _b_reducida = false;
    }
    if(x < this->width()-300 && !_b_reducida)
    {
        emit hideBusqueda();
        _b_reducida = true;
    }
}

void MayaModule::_showBarraBusqueda(BarraBusqueda *b)
{
    b->setGeometry(0,0,0,this->height()-80);
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
}

void MayaModule::_hideBarraBusqueda(BarraBusqueda *b)
{
    QPropertyAnimation* animation0 = new QPropertyAnimation(b, "size",this);
    connect(animation0,SIGNAL(finished()),animation0,SLOT(deleteLater()));
    animation0->setDuration(1000);
    animation0->setEasingCurve(QEasingCurve::OutElastic);

    animation0->setStartValue(QSize(250,b->height()));
    animation0->setEndValue(QSize(0,b->height()));

    QPropertyAnimation* animation = new QPropertyAnimation(b, "pos",this);
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::OutElastic);
    animation->setStartValue(QPoint(this->width()-250,b->pos().y()));
    animation->setEndValue(QPoint(this->width(),b->pos().y()));

    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    group->addAnimation(animation);
    group->addAnimation(animation0);

    connect(group,SIGNAL(finished()),group,SLOT(deleteLater()));
    connect(group,SIGNAL(finished()),b,SLOT(hide()));
    group->start();
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
