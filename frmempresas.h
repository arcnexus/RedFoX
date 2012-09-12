#ifndef FRMEMPRESAS_H
#define FRMEMPRESAS_H

#include <QDialog>

namespace Ui {
class FrmEmpresas;
}

class FrmEmpresas : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmEmpresas(QWidget *parent = 0);
    ~FrmEmpresas();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::FrmEmpresas *ui;
};

#endif // FRMEMPRESAS_H
