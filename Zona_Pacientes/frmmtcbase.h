#ifndef FRMMTCBASE_H
#define FRMMTCBASE_H

#include <QDialog>
#include"Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmMTCbase;
}

class FrmMTCbase : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmMTCbase(QWidget *parent = 0);
    ~FrmMTCbase();
public slots:
    void cargarSindromes(int id);
private:
    Ui::FrmMTCbase *ui;
};

#endif // FRMMTCBASE_H
