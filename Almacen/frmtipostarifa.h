#ifndef FRMTIPOSTARIFA_H
#define FRMTIPOSTARIFA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../Almacen/tipostarifa.h"

namespace Ui {
class FrmTiposTarifa;
}

class FrmTiposTarifa : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmTiposTarifa(QWidget *parent = 0);
    ~FrmTiposTarifa();
    void cargarDatos();
    
private slots:
    void on_btnAgregarTarifa_clicked();

    void on_listatarifas_clicked(const QModelIndex &index);

private:
    Ui::FrmTiposTarifa *ui;
    TiposTarifa *oTipostarifa;

};

#endif // FRMTIPOSTARIFA_H
