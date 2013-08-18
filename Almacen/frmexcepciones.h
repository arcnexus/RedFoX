#ifndef FRMEXCEPCIONES_H
#define FRMEXCEPCIONES_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmExcepciones;
}

class FrmExcepciones : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmExcepciones(QWidget *parent = 0);
    void cargar_articulo(int id_art);
    ~FrmExcepciones();
    
private:
    Ui::FrmExcepciones *ui;

};

#endif // FRMEXCEPCIONES_H
