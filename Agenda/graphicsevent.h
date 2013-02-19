#ifndef GRAPHICSEVENT_H
#define GRAPHICSEVENT_H

#include <QGraphicsItem>
#include <QDateTime>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QAction>
class GraphicsEvent : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit GraphicsEvent(QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void setSize(qreal w, qreal h);
    void setWidth(qreal w);
    void setColor(QColor color);
    void setTime(QDateTime start,QDateTime end);
    void adjustTopGrid();
    void adjustBottomGrid();
    qreal Heigth();
    QRectF Rect();
    void setAsunto(QString s);
    void setCliente(int id);
signals:
    void updateScene();
    void checkForCollisions();
public slots:
    void setMain_jail(QRectF jail);
    void setHGrid(QVector<qreal> grid , qreal size);
    void checkCollisions();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    qreal width;
    qreal heigth;
    int red;
    int green;
    int blue;
    QDateTime start;
    QDateTime end;

    bool under_mouse;
    bool on_click;
    bool on_moving;
    bool on_Vresize;
    bool on_Hresize;

    QRectF main_jail;
    void checkJail();
    bool isInJail(QPointF pos);


    static QVector<qreal> h_grid;
    static qreal h_gridSize;
    QString asunto;
    int id_cliente;
};

#endif // GRAPHICSEVENT_H
