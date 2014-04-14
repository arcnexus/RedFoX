#ifndef INIT_FORM_H
#define INIT_FORM_H

#include <QWidget>

namespace Ui {
class init_form;
}

class init_form : public QWidget
{
    Q_OBJECT
    
public:
    explicit init_form(QWidget *parent = 0);
    ~init_form();
    
private:
    Ui::init_form *ui;
};

#endif // INIT_FORM_H
