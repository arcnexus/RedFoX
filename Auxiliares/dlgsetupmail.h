#ifndef DLGSETUPMAIL_H
#define DLGSETUPMAIL_H

#include <QDialog>
#include "ui_dlgsetupmail.h"
namespace Ui {
class DlgSetUpMail;
}

class DlgSetUpMail : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSetUpMail(QWidget *parent = 0);
    ~DlgSetUpMail();
    Ui::DlgSetUpMail *ui;
private slots:
    void on_btnCli_clicked();

    void on_btnDist_clicked();

private:

};

#endif // DLGSETUPMAIL_H
