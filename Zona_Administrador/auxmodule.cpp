#include "auxmodule.h"

QSqlTableModel* AuxModule::_model;
bool AuxModule::_modelIsSet = false;

AuxModule::AuxModule(QWidget *parent, QString label) :
    QWidget(parent),
    _label(label,this),
    _layout(this),
    _combo(this)
{
    name = label;
    if(!_modelIsSet)
        AuxModule::setModel();

    _combo.setModel(AuxModule::model());
    _combo.setModelColumn(1);

    _layout.addWidget(&_label);
    _layout.addWidget(&_combo);
    this->setLayout(&_layout);
}

AuxModule::~AuxModule()
{
    if(AuxModule::_model)
        AuxModule::_model->deleteLater();
    _modelIsSet = false;
}


int AuxModule::nivel()
{
    return AuxModule::model()->index(_combo.currentIndex(),0).data().toInt();
}

void AuxModule::setNivel(int idAcceso)
{
     for (int i = 0 ; i< AuxModule::model()->rowCount();i++)
     {
         if(AuxModule::model()->index(i,0).data().toInt() == idAcceso)
         {
             _combo.setCurrentIndex(i);
             break;
         }
     }
}

QSqlTableModel *AuxModule::model()
{
    return _model;
}

void AuxModule::setModel()
{
    _model = new QSqlTableModel(0,Configuracion_global->groupDB);
    _model->setTable("nivelacceso");
    _model->select();
}
