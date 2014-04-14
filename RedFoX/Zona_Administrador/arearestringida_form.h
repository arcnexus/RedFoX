#ifndef AREARESTRINGidA_FORM_H
#define AREARESTRINGidA_FORM_H

#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class AreaRestringida_form;
}

class AreaRestringida_form : public QDialog
{
    Q_OBJECT
    
public:
    explicit AreaRestringida_form(QWidget *parent = 0);
    ~AreaRestringida_form();
    bool es_valido();
private slots:
    void login();
private:
    Ui::AreaRestringida_form *ui;
    bool m_valido;
};

#endif // AREARESTRINGidA_FORM_H
