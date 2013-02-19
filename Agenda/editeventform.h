#ifndef EDITEVENTFORM_H
#define EDITEVENTFORM_H

#include <QDialog>

namespace Ui {
class EditEventForm;
}

class EditEventForm : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditEventForm(QWidget *parent = 0);
    ~EditEventForm();
    
private:
    Ui::EditEventForm *ui;
};

#endif // EDITEVENTFORM_H
