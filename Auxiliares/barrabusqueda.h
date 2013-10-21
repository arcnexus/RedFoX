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
    void addLayoutZ1(QLayout *z);
    void addSpacer();
    void addLayout(QLayout *l);
    void block(bool state);
    void doFocustoText();
signals:
    void doSearch(QString texto, QString orden, QString modo);
    void key_Down_Pressed();
    void key_F2_Pressed();
    void showMe();
private:
    bool eventFilter(QObject *obj, QEvent *event);
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
