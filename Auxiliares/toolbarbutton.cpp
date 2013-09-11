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
                        "border-color: rgb(50,50,50);"
                        "background-color:#b7b65d;"
                         "}");
}

void ToolBarButton::mouseReleaseEvent(QMouseEvent *e)
{
    this->setStyleSheet("QFrame {"
                        "border:1px;"
                        "border-style:solid;"
                        "border-radius: 10px;"
                        "padding: 2px;"
                        "border-color: rgb(93, 93, 69);"
                        "background-color:rgb(248, 245, 194);"
                         "}");
    if(this->rect().contains(e->pos()))
        emit clicked();
}
