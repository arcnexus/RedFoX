#ifndef DLGIMPORTODS_H
#define DLGIMPORTODS_H

#include <QDialog>
#include <QMap>

class QSqlTableModel;
class QSqlRecord;
class QStandardItemModel;

namespace Ui {
class DlgImportODS;
}

class DlgImportODS : public QDialog
{
    Q_OBJECT

public:
    explicit DlgImportODS(QWidget *parent = 0);
    ~DlgImportODS();

private slots:
    void on_btnFile_clicked();

    void on_btnImpor_clicked();

private:
    Ui::DlgImportODS *ui;

    bool alreadyOn(QSqlTableModel* model, QString cod, QString cod_b, QString cod_f, int &row);
    bool createAcumsAndTarifa(int id, QStandardItemModel* model, int row, double coste, double &pvp);

    bool _use_codigo;
    bool _use_codigo_b;
    bool _use_codigo_f;
    bool _use_desc;
    bool _use_desc_red;
    bool _check_on;
    bool _use_iva;
    bool _use_coste;
    bool _use_stock;
    bool _use_web;
    bool _use_secc;
    bool _use_fam;
    bool _use_subfam;

    QMap<int,int> empresas;
    QMap<int,QSqlRecord> tarifas;
    QMap<QString,int> secciones;
    QMap<QString,int> familias;
    QMap<QString,int> subfamilias;
    QMap<QString,int> proveedores;
    QMap<int,double> ivas;
    QStandardItemModel* ods;

    QMap<QString,bool> _cods;
    QMap<QString,bool> _cods_b;
    QMap<QString,bool> _cods_f;

    bool updateTarifa(int id_art,QStandardItemModel* model, int row, double coste, double &pvp);
    QMap<int,int> _tarifa_index;
    bool _update_tar;
};

#endif // DLGIMPORTODS_H

