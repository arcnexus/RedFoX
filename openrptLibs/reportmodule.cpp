#include "reportmodule.h"

ReportModule::ReportModule(QWidget *parent) :
    MayaModule(ModuleZone(),ModuleName(),parent),
    toolButton(tr("Editar \nreportes"),":/Icons/PNG/reports.png",this),
    menuButton(QIcon(":/Icons/PNG/reports.png"),tr("Editar reportes"),this),
    _report(),
    _layout(this)
{
    _layout.setMargin(0);
    _layout.addWidget(&_report);
}
