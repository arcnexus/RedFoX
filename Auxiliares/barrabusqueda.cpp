#include "barrabusqueda.h"
#include "ui_barrabusqueda.h"

BarraBusqueda::BarraBusqueda(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BarraBusqueda)
{
    ui->setupUi(this);
    this->hide();
    this->setAutoFillBackground(true);
    this->setMouseTracking(true);
}

BarraBusqueda::~BarraBusqueda()
{
    delete ui;
}

bool BarraBusqueda::eventFilter(QObject *, QEvent *e)
{
    if(e->type() == QEvent::Leave)
    {
       emit closeME();
    }
    return false;
}
