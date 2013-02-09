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
    void linea_itemSelectionChanged();
private:
    Ui::FrmCajaMinuta *ui;
    void focusInEvent(QFocusEvent * e);
    bool eventFilter(QObject *target, QEvent *event);
    void keys_onCodigo(int key);
    bool keys_lineas(int key);

    bool rellenarArticulo(QString cCodigo);
    Ticket ticket;
    int linea_row;
    int linea_column;
};

#endif // FRMCAJAMINUTA_H
