#ifndef BLOCK_TERRA_FORM_H
#define BLOCK_TERRA_FORM_H

#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class block_terra_form;
}

class block_terra_form : public QDialog
{
    Q_OBJECT
    
public:
    explicit block_terra_form(QWidget *parent = 0);
    ~block_terra_form();
    void set_user(QString user, QString pass);
private slots:
    void on_pushButton_clicked();

private:
    Ui::block_terra_form *ui;
    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *);
    QString user;
    QString pass;
};

#endif // BLOCK_TERRA_FORM_H
