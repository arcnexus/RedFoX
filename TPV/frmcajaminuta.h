#ifndef FRMCAJAMINUTA_H
#define FRMCAJAMINUTA_H

#include "../Auxiliares/Globlal_Include.h"
#include "../db_table_view.h"
#include "ticket.h"
namespace Ui {
class FrmCajaMinuta;
}

class FrmCajaMinuta : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmCajaMinuta(QWidget *parent = 0);
    ~FrmCajaMinuta();
    
private slots:
    void on_btnBuscarArt_clicked();

private:
    Ui::FrmCajaMinuta *ui;
    bool eventFilter(QObject *target, QEvent *event);
    void keys_onCodigo(int key);
    void keys_onDescripcion(int key);
    void keys_onPvp(int key);

    void rellenarArticulo(QString cCodigo);
    Ticket ticket;
};

#endif // FRMCAJAMINUTA_H
