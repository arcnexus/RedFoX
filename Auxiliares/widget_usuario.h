#ifndef WIDGET_USUARIO_H
#define WIDGET_USUARIO_H

#include <QWidget>

namespace Ui {
class widget_usuario;
}

class widget_usuario : public QWidget
{
    Q_OBJECT

public:
    explicit widget_usuario(QWidget *parent = 0);
    ~widget_usuario();

private:
    Ui::widget_usuario *ui;
};

#endif // WIDGET_USUARIO_H
