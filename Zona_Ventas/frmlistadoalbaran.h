#ifndef FRMLISTADOALBARAN_H
#define FRMLISTADOALBARAN_H

#include <QDialog>

namespace Ui {
class frmListadoAlbaran;
}

class frmListadoAlbaran : public QDialog
{
    Q_OBJECT

public:
    explicit frmListadoAlbaran(QWidget *parent = 0);
    ~frmListadoAlbaran();

private slots:
    void on_rdbRGClientes_toggled(bool checked);
    void on_rdbRGFechas_toggled(bool checked);
    void on_rdbRGImportes_toggled(bool checked);
    void on_btnPrew_clicked();
    void on_btnPrint_clicked();
    void on_btnPdf_clicked();
    void on_btnMail_clicked();
private:
    Ui::frmListadoAlbaran *ui;

    QMap<QString, QString> getParametros();
    QString getClientesSql();
    QString getCaPreSql();
};

#endif // FRMLISTADOALBARAN_H
