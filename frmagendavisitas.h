#ifndef FRMAGENDAVISITAS_H
#define FRMAGENDAVISITAS_H

#include <QDialog>

namespace Ui {
class frmAgendaVisitas;
}

class frmAgendaVisitas : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmAgendaVisitas(QWidget *parent = 0);
    ~frmAgendaVisitas();
    
private:
    Ui::frmAgendaVisitas *ui;
};

#endif // FRMAGENDAVISITAS_H
