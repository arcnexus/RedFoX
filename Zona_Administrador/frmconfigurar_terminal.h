#ifndef FRMCONFIGURAR_TERMINAL_H
#define FRMCONFIGURAR_TERMINAL_H

#include <QDialog>
#include <../Auxiliares/Globlal_Include.h>
namespace Ui {
class frmConfigurar_terminal;
}

class frmConfigurar_terminal : public QDialog
{
    Q_OBJECT

public:
    explicit frmConfigurar_terminal(QWidget *parent = 0);
    ~frmConfigurar_terminal();

private slots:

    void on_btnAceptar_clicked();

    void on_txtImporte_abertura_editingFinished();

private:
    Ui::frmConfigurar_terminal *ui;
};

#endif // FRMCONFIGURAR_TERMINAL_H
