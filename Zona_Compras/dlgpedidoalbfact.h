#ifndef DLGPEDIDOALBFACT_H
#define DLGPEDIDOALBFACT_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
namespace Ui {
class DlgPedidoAlbFact;
}

class DlgPedidoAlbFact : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPedidoAlbFact(QString label , QWidget *parent = 0);
    ~DlgPedidoAlbFact();

    QMap<int, QSqlRecord> getLineas() const;
    void setLineas(const QMap<int, QSqlRecord> &value);

private slots:
    void on_btnAccept_clicked();
    void calcular(double);
private:
    Ui::DlgPedidoAlbFact *ui;
    QMap<int, QSqlRecord> lineas;
};

#endif // DLGPEDIDOALBFACT_H
