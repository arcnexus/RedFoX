#include "globoaviso.h"

GloboAviso::GloboAviso(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowFlags(this->windowFlags()^Qt::FramelessWindowHint);
    setStyleSheet("background:transparent;");
    setAttribute(Qt::WA_TranslucentBackground);
    m_Cache = 0;
}
void GloboAviso::paintEvent(QPaintEvent *pe) {
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
    if(m_Cache != 0)
    {
      QPainter painter(this);
      painter.setRenderHint(QPainter::Antialiasing);
      painter.drawPixmap(0, 0, *m_Cache);
    }
}
void GloboAviso::resizeEvent(QResizeEvent *)
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

        painter.setPen  (Qt::darkRed);
        painter.setBrush(QBrush(darkBlue));

        QPainterPath path;
        path.setFillRule( Qt::WindingFill );
        path.addRoundedRect( QRect(20,0,this->width()-20, this->height()), 5, 5 );

        QPolygonF polygon;
                polygon << QPointF(0,10 ) << QPointF(20, 20)<< QPointF(20, 10);

        path.addPolygon(polygon);

        painter.save();
            painter.drawPath( path.simplified() );
        painter.restore();

        painter.save();
            painter.setPen(Qt::SolidLine);
            painter.setBrush(Qt::black);
            painter.drawText(QRectF(0,0,width(),height()),Qt::AlignCenter,tr("Nuevos avisos"));
        painter.restore();
    }
}
