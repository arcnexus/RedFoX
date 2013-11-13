#ifndef GLOBOAVISO_H
#define GLOBOAVISO_H

#include <QDialog>
#include"../Auxiliares/Globlal_Include.h"
class GloboAviso : public QDialog
{
    Q_OBJECT
public:
    explicit GloboAviso(QWidget *parent = 0);
    
signals:
    
public slots:
    
protected:
    void paintEvent(QPaintEvent *pe);
    void resizeEvent(QResizeEvent *);
private:
    QPixmap* m_Cache;
};

#endif // GLOBOAVISO_H
