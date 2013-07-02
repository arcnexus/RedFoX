#ifndef REPORTMODULE_H
#define REPORTMODULE_H

#include <QDialog>
#include "Auxiliares/reportwriterwindow.h"
#include "../mayamodule.h"
class ReportModule : public MayaModule
{
    Q_OBJECT
public:
    explicit ReportModule(QWidget *parent = 0);
    module_zone module_zone(){return Utilidades;}
    QString module_name(){return "Editor de reportes";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}QPushButton* wantShortCut(bool& ok){ok = false; return 0;}
signals:
    
public slots:
private:
     ToolBarButton toolButton;
     QAction menuButton;
     ReportWriterWindow _report;
     QVBoxLayout _layout;
};

#endif // REPORTMODULE_H
