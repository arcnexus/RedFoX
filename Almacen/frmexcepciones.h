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
    void cargar_cliente(int id_cliente);

    ~FrmExcepciones();
    
private slots:
    void on_txt_codigo_articulo_editingFinished();


private:
    Ui::FrmExcepciones *ui;
    QSqlQueryModel *m;
    void buscar_id_cliente(QString cod_cli);
    void buscar_id_articulo(QString cod_art);
    bool eventFilter(QObject *obj, QEvent *event);
    void consultar_articulo();
    void consultar_cliente();
};

#endif // FRMEXCEPCIONES_H
