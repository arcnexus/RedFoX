#ifndef FRMASOCIARPROVEEDOR_H
#define FRMASOCIARPROVEEDOR_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
namespace Ui {
class FrmAsociarProveedor;
}

class FrmAsociarProveedor : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAsociarProveedor(QWidget *parent = 0);
    ~FrmAsociarProveedor();
    
private:
    Ui::FrmAsociarProveedor *ui;
    QSqlQueryModel *modelProveedor;
};

#endif // FRMASOCIARPROVEEDOR_H
