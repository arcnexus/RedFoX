#ifndef BARRABUSQUEDA_H
#define BARRABUSQUEDA_H

#include <QWidget>

namespace Ui {
class BarraBusqueda;
}

class BarraBusqueda : public QWidget
{
    Q_OBJECT
    
public:
    explicit BarraBusqueda(QWidget *parent = 0);
    ~BarraBusqueda();

    void setOrderCombo(QStringList l);
    void setModeCombo(QStringList l);
    void addWidget(QWidget* w);
    void addSpacer();
    void addLayout(QLayout *l);
signals:
    void doSearch(QString texto, QString orden, QString modo);
    void showMe();
private slots:
    void on_btnLimpiar_clicked();
    void textChanged(const QString&);

protected:
    void mousePressEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *pe);
    Ui::BarraBusqueda *ui;
    QPixmap* m_Cache;
};

#endif // BARRABUSQUEDA_H
