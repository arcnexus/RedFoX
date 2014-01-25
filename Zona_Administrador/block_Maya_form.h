#ifndef BLOCK_MAYA_FORM_H
#define BLOCK_MAYA_FORM_H

#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class block_Maya_form;
}

class block_Maya_form : public QDialog
{
    Q_OBJECT
    
public:
    explicit block_Maya_form(QWidget *parent = 0);
    ~block_Maya_form();

private slots:
    void on_pushButton_clicked();

private:
    Ui::block_Maya_form *ui;
    void keyPressEvent(QKeyEvent *);
    void closeEvent(QCloseEvent *);
    QString user;
    QString pass;
};

#endif // BLOCK_MAYA_FORM_H
