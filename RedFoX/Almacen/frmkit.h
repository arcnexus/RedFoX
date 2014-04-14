#ifndef FRMKIT_H
#define FRMKIT_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
namespace Ui {
class FrmKit;
}

class FrmKit : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmKit(QWidget *parent = 0);
    ~FrmKit();
    void set_articulo(QString codigo, QString Descripcion, int stock);
    double getCoste();
    void refreshCantidades();
private slots:
    void on_btnAnadir_clicked();

    void on_btnGuardar_clicked();

    void on_txtBuscar_textEdited(const QString &arg1);

    void on_btnAnadirKits_clicked();

    void on_btnQuitar_clicked();

    void valueChanged(double);
    void on_btnRomperKit_clicked();

private:
    Ui::FrmKit *ui;
    QSqlQueryModel *m_arts;
    QSqlQueryModel *m_kits;
    int id_componente;
    void refrescar_tabla_escandallo(QString codigo);
    int stock;
};

#endif // FRMKIT_H
