#ifndef AGENDAFORM_H
#define AGENDAFORM_H

#include <QDialog>
#include <QtGui>
#include <QGraphicsScene>
#include "graphicstable.h"
#include "../mayamodule.h"
namespace Ui {
class AgendaForm;
}

class AgendaForm : public MayaModule
{
    Q_OBJECT
    
public:
    explicit AgendaForm(QWidget *parent = 0);
    ~AgendaForm();

    moduleZone ModuleZone(){return Utilidades;}
    QString ModuleName(){return "Agenda";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
private slots:
    void on_pushButton_clicked();

    void on_calendarWidget_activated(const QDate &date);

    void on_combo_user_currentIndexChanged(int index);

    void on_btn_hoy_clicked();

    void on_btn_buscaUser_clicked();

    void on_btn_zoomIn_clicked();
    void on_btn_zoomOut_clicked();

private:
    Ui::AgendaForm *ui;
    QGraphicsScene * scene;
    void resizeEvent(QResizeEvent * e );
    GraphicsTable * table;
    QColor event_color;
    ToolBarButton toolButton;
    QAction menuButton;
};

#endif // AGENDAFORM_H
