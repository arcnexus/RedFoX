#ifndef FRMLISTADOSPRE_H
#define FRMLISTADOSPRE_H

#include <QDialog>

namespace Ui {
class frmListadosPre;
}

class frmListadosPre : public QDialog
{
    Q_OBJECT

public:
    explicit frmListadosPre(QWidget *parent = 0);
    ~frmListadosPre();

private slots:
    void on_rdb1Cliente_toggled(bool checked);

    void on_rdbRGClientes_toggled(bool checked);

    void on_rdbRGFechas_toggled(bool checked);

    void on_rdbRGImportes_toggled(bool checked);

    void on_btnMail_clicked();

    void on_btnPrint_clicked();

    void on_btnPrew_clicked();

    void on_btnPdf_clicked();

private:
    Ui::frmListadosPre *ui;

    QString getCaPreSql();
    QString getClientesSql();
    QMap <QString,QString> getParametros();
};

#endif // FRMLISTADOSPRE_H
