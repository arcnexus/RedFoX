#ifndef BARRAAVISOS_H
#define BARRAAVISOS_H

#include <QWidget>
#include"../Auxiliares/Globlal_Include.h"

namespace Ui {
class BarraAvisos;
}

class BarraAvisos : public QWidget
{
    Q_OBJECT
    
public:
    explicit BarraAvisos(QWidget *parent = 0);
    ~BarraAvisos();
    void setPagina(int pagina);
    bool isShow() const;
    void setShow(bool isShow);
    void setColor(QColor c);
signals:
    void showMe();
    void hideMe();
protected:
    void paintEvent(QPaintEvent *pe);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);
private slots:

private:
    Ui::BarraAvisos *ui;
    QPixmap* m_Cache;
    bool m_show;
    QColor m_color;
    void llenarAvisos();
};

#endif // BARRAAVISOS_H
