#include "repdesignmodule.h"

RepDesignModule::RepDesignModule(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    menuButton(QIcon(":/Icons/PNG/reports.png"),tr("Editar reportes"),this),
    _report(),
    _layout(this)
{
    _layout.setMargin(0);
    _layout.addWidget(&_report);
}
