#ifndef TOOLBARBUTTON_H
#define TOOLBARBUTTON_H

#include <QFrame>
#include <QMouseEvent>
#include <QDebug>
namespace Ui {
class ToolBarButton;
}

class ToolBarButton : public QFrame
{
    Q_OBJECT
signals:
    void clicked();
public:
    explicit ToolBarButton(QString label = "" , QString icon = "" , QWidget *parent = 0);
    void setText(QString text);
    void setIcon(QString icon);
    ~ToolBarButton();
    
private:
    Ui::ToolBarButton *ui;
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // TOOLBARBUTTON_H
