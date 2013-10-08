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


    void on_btnAnadir_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_btnDeshacer_clicked();

    void on_tabla_clicked(const QModelIndex &index);

    void on_btnGenerarAvisoInicio_clicked();

private:
    Ui::FrmExcepciones *ui;
    QSqlQueryModel *m;
    void buscar_id_cliente(QString cod_cli);
    void buscar_id_articulo(QString cod_art);
    bool eventFilter(QObject *obj, QEvent *event);
    void consultar_articulo();
    void consultar_cliente();
    void clear();
    void cargar_excepcion(int id);
    void setControlsReadOnly(bool state);
    void setButtonsEditMode(bool state);
    int id;
    int id_cliente;
    int id_familia;
    int id_familia_cliente;
    int id_subfamilia_cliente;
    int id_proveedor;
    int id_agente;
    int id_articulo;
    int id_aviso_ini;
    int id_aviso_fin;
    bool adding;
    QSqlQueryModel *modelo;
};

#endif // FRMEXCEPCIONES_H
