#ifndef SAVETOBDFRM_H
#define SAVETOBDFRM_H

#include <QDialog>

namespace Ui {
class SaveToBDFrm;
}

class SaveToBDFrm : public QDialog
{
    Q_OBJECT
    
public:
    explicit SaveToBDFrm(QWidget *parent = 0);
    ~SaveToBDFrm();
    QString nombre;
    QString desc;
    int grad;
private slots:
    void currentRowChanged(const QModelIndex & current, const QModelIndex & previous);
    void on_txtNombre_textChanged(const QString &arg1);

    void on_txtDesc_textChanged(const QString &arg1);

    void on_spinGrd_valueChanged(int arg1);

private:
    Ui::SaveToBDFrm *ui;
};

#endif // SAVETOBDFRM_H
