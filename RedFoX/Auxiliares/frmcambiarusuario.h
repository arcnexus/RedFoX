#ifndef FRMCAMBIARUSUARIO_H
#define FRMCAMBIARUSUARIO_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class frmCambiarUsuario;
}

class frmCambiarUsuario : public QDialog
{
    Q_OBJECT

public:
    explicit frmCambiarUsuario(QWidget *parent = 0);
    ~frmCambiarUsuario();

private:
    Ui::frmCambiarUsuario *ui;
};

#endif // FRMCAMBIARUSUARIO_H
