#include "auxmodule.h"

QStringList AuxModule::_model;

AuxModule::AuxModule(QWidget *parent, QString label) :
    QWidget(parent),
    _label(label,this),
    _layout(this),
    _combo(this)
{
    name = label;
    AuxModule::setModel();

    _combo.addItems(AuxModule::model());

    _layout.addWidget(&_label);
    _layout.addWidget(&_combo);
    this->setLayout(&_layout);   
}

AuxModule::~AuxModule()
{

}

int AuxModule::nivel()
{
    return _combo.currentIndex() + 1;
}

void AuxModule::setNivel(int idAcceso)
{
     _combo.setCurrentIndex(idAcceso -1);

     connect(&_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(save(int)));
}

QStringList AuxModule::model()
{
    return _model;
}

void AuxModule::setModel()
{
    _model.clear();
    _model << "Sin Acceso"<<"Lectura parcial"<<"Lectura total"<<"Escritura parcial (editar)";
    _model << "Escritura parcial (añadir)" << "Escritural total" << "Administrador";
}

void AuxModule::save(int index)
{
    QSqlQuery q2(QSqlDatabase::database("temp_loadEmp"));
    QString s = QString("UPDATE %1 SET id_nivel_acceso=:lvl WHERE id_modulo=:mod AND id_user=:id;").arg(tabla);
    q2.prepare(s);
    q2.bindValue(":id", this->id_user);
    q2.bindValue(":mod",this->id);
    q2.bindValue(":lvl",index+1);
    q2.exec();
}
