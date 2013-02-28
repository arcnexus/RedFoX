#ifndef FRMTAREAS_H
#define FRMTAREAS_H

#include <QDialog>

namespace Ui {
class FrmTareas;
}

class FrmTareas : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmTareas(QWidget *parent = 0);
    ~FrmTareas();
    
private:
    Ui::FrmTareas *ui;
};

#endif // FRMTAREAS_H
