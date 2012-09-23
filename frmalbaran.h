#ifndef FRMALBARAN_H
#define FRMALBARAN_H

#include <QDialog>

namespace Ui {
class FrmAlbaran;
}

class FrmAlbaran : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAlbaran(QWidget *parent = 0);
    ~FrmAlbaran();
    
private:
    Ui::FrmAlbaran *ui;
};

#endif // FRMALBARAN_H
