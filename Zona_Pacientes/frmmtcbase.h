#ifndef FRMMTCBASE_H
#define FRMMTCBASE_H

#include <QDialog>

namespace Ui {
class FrmMTCbase;
}

class FrmMTCbase : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmMTCbase(QWidget *parent = 0);
    ~FrmMTCbase();
    
private:
    Ui::FrmMTCbase *ui;
};

#endif // FRMMTCBASE_H
