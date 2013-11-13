#ifndef BARRAAVISOS_H
#define BARRAAVISOS_H

#include <QWidget>
#include"../Auxiliares/Globlal_Include.h"
#include "Auxiliares/globoaviso.h"

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
    void llenarAvisos();
public slots:
    void showGlobo();
signals:
    void showMe();
    void hideMe();
protected:
    void paintEvent(QPaintEvent *pe);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);
private slots:

private:
    Ui::BarraAvisos *ui;
    QPixmap* m_Cache;
    bool m_show;
    QColor m_color;
    QPointer<GloboAviso> m_globo;
};

#endif // BARRAAVISOS_H
