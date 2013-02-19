#ifndef AGENDAFORM_H
#define AGENDAFORM_H

#include <QDialog>
#include <QtGui>
#include <QGraphicsScene>
#include "graphicstable.h"
namespace Ui {
class AgendaForm;
}

class AgendaForm : public QDialog
{
    Q_OBJECT
    
public:
    explicit AgendaForm(QWidget *parent = 0);
    ~AgendaForm();
    void setButtonColor();
private slots:
    void on_pushButton_clicked();
    void timeChanged(const QTime &time);

    void on_btn_getColor_clicked();
private:
    Ui::AgendaForm *ui;
    QGraphicsScene * scene;
    void resizeEvent(QResizeEvent * e );
    GraphicsTable * table;
    QColor event_color;
};

#endif // AGENDAFORM_H