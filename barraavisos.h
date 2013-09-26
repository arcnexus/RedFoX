#ifndef BARRAAVISOS_H
#define BARRAAVISOS_H

#include <QWidget>

namespace Ui {
class BarraAvisos;
}

class BarraAvisos : public QWidget
{
    Q_OBJECT
    
public:
    explicit BarraAvisos(QWidget *parent = 0);
    ~BarraAvisos();
signals:
    void closeMe();
protected:
    void paintEvent(QPaintEvent *pe);
private slots:
    void on_btnClose_clicked();

private:
    Ui::BarraAvisos *ui;
};

#endif // BARRAAVISOS_H
