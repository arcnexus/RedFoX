#include "barrabusqueda.h"
#include "ui_barrabusqueda.h"
#include <QPainter>
#include <QMouseEvent>
BarraBusqueda::BarraBusqueda(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BarraBusqueda)
{
    ui->setupUi(this);
    m_Cache = 0;

    this->setAutoFillBackground(true);
    this->setMouseTracking(true);

    connect(ui->cboModo,SIGNAL(currentIndexChanged(QString)),this,SLOT(textChanged(QString)));
    connect(ui->txtBuscar,SIGNAL(textEdited(QString)),this,SLOT(textChanged(QString)));
    connect(ui->cboOrden,SIGNAL(currentIndexChanged(QString)),this,SLOT(textChanged(QString)));
}

BarraBusqueda::~BarraBusqueda()
{
    delete ui;
}

void BarraBusqueda::setOrderCombo(QStringList l)
{
    ui->cboOrden->clear();
    ui->cboOrden->addItems(l);
}

void BarraBusqueda::setModeCombo(QStringList l)
{
    ui->cboModo->clear();
    ui->cboModo->addItems(l);
}

void BarraBusqueda::addWidget(QWidget *w)
{
    ui->layout->addWidget(w);
}

void BarraBusqueda::addLayout(QLayout *l)
{
    ui->layout->addLayout(l);
}

void BarraBusqueda::paintEvent(QPaintEvent *pe)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
    if(m_Cache != 0)
    {
      QPainter painter(this);

      painter.drawPixmap(0, 0, *m_Cache);
    }
}

void BarraBusqueda::on_btnLimpiar_clicked()
{
    ui->txtBuscar->clear();
    ui->cboModo->setCurrentIndex(0);
    ui->cboOrden->setCurrentIndex(0);
    emit doSearch(ui->txtBuscar->text(),ui->cboOrden->currentText(),ui->cboModo->currentText());
}

void BarraBusqueda::textChanged(const QString &)
{
    emit doSearch(ui->txtBuscar->text(),ui->cboOrden->currentText(),ui->cboModo->currentText());
}

void BarraBusqueda::mousePressEvent(QMouseEvent * e)
{
    if(e->pos().x() < 21)
        if(e->pos().y()>40 && e->pos().y() < 80)
            emit showMe();
}

void BarraBusqueda::resizeEvent(QResizeEvent *)
{
    if(m_Cache)
      delete m_Cache;

    m_Cache = new QPixmap(size());

    m_Cache->fill(Qt::transparent);

    QPainter painter(m_Cache);
    if(painter.isActive())
    {
        painter.setRenderHint(QPainter::Antialiasing);

        QColor darkBlue ( 182,159,64,230);

        painter.setPen  (Qt::NoPen/*QPen  (darkBlue)*/);
        painter.setBrush(QBrush(darkBlue));

        QPainterPath path;
        path.setFillRule( Qt::WindingFill );
        path.addRoundedRect( QRect(0,20, 20, 80), 10, 10 );
        path.addRect( QRect( 10, 20, 10, 80 ) );
        painter.drawPath( path.simplified() ); // Only Top left & bottom right corner

       // painter.drawRoundedRect(0,20,20,80,10,10);
        //painter.drawRect(10,20,10,80);

        if(width()>20)
            painter.drawRoundedRect(20,0,width()-20,height(),20,20);

        QPixmap p(":/Icons/PNG/search.png");
        painter.drawPixmap(QRect(2,50,20,20),p);
    }
}
