#ifndef DLGPEDIDOALBFACT_H
#define DLGPEDIDOALBFACT_H

#include <QDialog>

namespace Ui {
class DlgPedidoAlbFact;
}

class DlgPedidoAlbFact : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPedidoAlbFact(QString label , QWidget *parent = 0);
    ~DlgPedidoAlbFact();

private slots:
    void on_btnAccept_clicked();

private:
    Ui::DlgPedidoAlbFact *ui;
};

#endif // DLGPEDIDOALBFACT_H
