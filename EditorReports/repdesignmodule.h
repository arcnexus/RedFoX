#ifndef REPDESIGNMODULE_H
#define REPDESIGNMODULE_H

#include <QDialog>
#include "../mayamodule.h"
#include "reportdesigwin.h"
class RepDesignModule : public MayaModule
{
    Q_OBJECT
public:
    explicit RepDesignModule(QWidget *parent = 0);
    module_zone module_zone(){return Utilidades;}
    QString module_name(){return "Editor de reportes";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    QPushButton* wantShortCut(bool& ok){ok = false; return 0;}
signals:

public slots:
private:
     QAction menuButton;
     ReportDesigWin _report;
     QVBoxLayout _layout;
};

#endif // REPDESIGNMODULE_H
