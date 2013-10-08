#ifndef FRMNUEVOSAVISOS_H
#define FRMNUEVOSAVISOS_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class frmNuevosAvisos;
}

class frmNuevosAvisos : public QDialog
{
    Q_OBJECT

public:
    explicit frmNuevosAvisos(QWidget *parent = 0);
    ~frmNuevosAvisos();
    void setFecha(QDate fecha);
    void setHora(QTime hora);
    void setText(QString texto);
    void setEmpresa(int id_empresa);
    QDate getFecha();
    QTime getHora();
    QString getText();
    int get_id_Empresa();
    int get_id_usuario_destino();

private:
    Ui::frmNuevosAvisos *ui;
    QSqlQueryModel *m_empresas;
    QSqlQueryModel *m_usuarios;
};

#endif // FRMNUEVOSAVISOS_H
