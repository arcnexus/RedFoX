#include "toolbarbutton.h"
#include "ui_toolbarbutton.h"

ToolBarButton::ToolBarButton(QString label, QString icon, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ToolBarButton)
{
    ui->setupUi(this);
    setText(label);
    setIcon(icon);
    if(parent)
        _linkedWidget = parent;
}

void ToolBarButton::setText(QString text)
{
    ui->lbl_text->setText(text);
}

void ToolBarButton::setIcon(QString icon)
{
    ui->lbl_icon->setPixmap(QPixmap(icon));
}

ToolBarButton::~ToolBarButton()
{
    delete ui;
}

void ToolBarButton::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    this->setStyleSheet("QFrame {"
                        "border:1px;"
                        "border-style:solid;"
                        "border-radius: 10px;"
                        "padding: 2px;"
                        "border-color: rgb(0, 0, 127);"
                        "background-color:#87A5D1;"
                         "}");
}

void ToolBarButton::mouseReleaseEvent(QMouseEvent *e)
{
    this->setStyleSheet("QFrame {"
                        "border:1px;"
                        "border-style:solid;"
                        "border-radius: 10px;"
                        "padding: 2px;"
                        "border-color: rgb(0, 0, 127);"
                        "background-color:#AFC3E1;"
                         "}");
    if(this->rect().contains(e->pos()))
        emit clicked();
}
