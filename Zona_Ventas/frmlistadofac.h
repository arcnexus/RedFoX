#ifndef FRMLISTADOFAC_H
#define FRMLISTADOFAC_H

#include <QDialog>

namespace Ui {
class frmListadoFac;
}

class frmListadoFac : public QDialog
{
    Q_OBJECT

public:
    explicit frmListadoFac(QWidget *parent = 0);
    ~frmListadoFac();

private slots:
    void on_rdbRGClientes_toggled(bool checked);
    void on_rdbRGFechas_toggled(bool checked);
    void on_rdbRGImportes_toggled(bool checked);
    void on_btnPrew_clicked();
    void on_btnPrint_clicked();
    void on_btnPdf_clicked();
    void on_btnMail_clicked();
    void on_rdb1Cliente_toggled(bool checked);
private:
    Ui::frmListadoFac *ui;
    QMap<QString, QString> getParametros();
    QString getClientesSql();
    QString getCaPreSql();
};

#endif // FRMLISTADOFAC_H
