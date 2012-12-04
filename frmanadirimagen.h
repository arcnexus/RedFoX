#ifndef FRMANADIRIMAGEN_H
#define FRMANADIRIMAGEN_H

#include <QWidget>

namespace Ui {
class FrmAnadirImagen;
}

class FrmAnadirImagen : public QWidget
{
    Q_OBJECT
    
public:
    explicit FrmAnadirImagen(QWidget *parent = 0);
    ~FrmAnadirImagen();
    
private:
    Ui::FrmAnadirImagen *ui;
private slots:
    void RecuperarId(QString);
};

#endif // FRMANADIRIMAGEN_H
