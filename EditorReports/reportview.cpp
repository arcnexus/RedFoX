#include "reportview.h"

ReportView::ReportView(QWidget *parent) :
    QGraphicsView(parent)
{
    //setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

void ReportView::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier)
    {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        double scaleFactor = 1.15;
        if(event->delta() > 0) {
            // Zoom in
            scale(scaleFactor, scaleFactor);
        } else {
            // Zooming out
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
    }
    else
        QGraphicsView::wheelEvent(event);
}
