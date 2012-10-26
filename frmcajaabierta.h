#ifndef FRMCAJAABIERTA_H
#define FRMCAJAABIERTA_H

#include <QDialog>

namespace Ui {
class Frmcajaabierta;
}

class Frmcajaabierta : public QDialog
{
    Q_OBJECT
    
public:
    explicit Frmcajaabierta(QWidget *parent = 0);
    ~Frmcajaabierta();
    
private:
    Ui::Frmcajaabierta *ui;
};

#endif // FRMCAJAABIERTA_H
