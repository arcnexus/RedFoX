#ifndef FRMKIT_H
#define FRMKIT_H

#include <QDialog>
#include <../Auxiliares/Globlal_Include.h>
namespace Ui {
class FrmKit;
}

class FrmKit : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmKit(QWidget *parent = 0);
    ~FrmKit();
    void set_articulo(QString codigo);
    
private slots:
    void on_btnAnadir_clicked();

    void on_btnGuardar_clicked();

    void on_txtBuscar_textEdited(const QString &arg1);

    void on_btnCrearKit_clicked();

    void on_btnAnadirKits_clicked();

    void on_btnQuitar_clicked();

private:
    Ui::FrmKit *ui;
    QSqlQueryModel *m_arts;
    QSqlQueryModel *m_kits;
    int id_componente;
    void refrescar_tabla_escandallo(QString codigo);
};

#endif // FRMKIT_H
