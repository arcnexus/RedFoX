#ifndef FRMLISTADOPED_H
#define FRMLISTADOPED_H

#include <QDialog>

namespace Ui {
class frmListadoPed;
}

class frmListadoPed : public QDialog
{
    Q_OBJECT

public:
    explicit frmListadoPed(QWidget *parent = 0);
    ~frmListadoPed();

private slots:
    void on_rdb1Cliente_toggled(bool checked);

    void on_rdbRGClientes_toggled(bool checked);

    void on_rdbRGFechas_toggled(bool checked);

    void on_rdbRGImportes_toggled(bool checked);

    void on_btnPrew_clicked();

    void on_btnPrint_clicked();

    void on_btnPdf_clicked();

    void on_btnMail_clicked();

private:
    Ui::frmListadoPed *ui;
    QMap<QString, QString> getParametros();
    QString getClientesSql();
    QString getCaPreSql();
};

#endif // FRMLISTADOPED_H
