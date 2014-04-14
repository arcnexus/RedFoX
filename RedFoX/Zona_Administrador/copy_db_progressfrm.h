#ifndef COPY_DB_PROGRESSFRM_H
#define COPY_DB_PROGRESSFRM_H

#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class copy_db_progressFrm;
}

class copy_db_progressFrm : public QDialog
{
    Q_OBJECT
    
public:
    explicit copy_db_progressFrm(QWidget *parent = 0 , int barras = 3);
    void setProgess_1(QString text , int progress);
    void set_Max_1(int max);

    void setProgess_2(QString text , int progress);
    void set_Max_2(int max);

    void setProgess_3(QString text , int progress);
    void set_Max_3(int max);
    ~copy_db_progressFrm();
    
private:
    Ui::copy_db_progressFrm *ui;
};

#endif // COPY_DB_PROGRESSFRM_H
