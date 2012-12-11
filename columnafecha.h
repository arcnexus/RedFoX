#ifndef COLUMNAFECHA_H
#define COLUMNAFECHA_H
#include <QtGui>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QItemDelegate>
/*
 * columnafecha.h
 *
 *  Created on: September, 2012
 *      Author: ArcNexus
 */


class  ColumnaFecha: public QItemDelegate
    {
public:
    ColumnaFecha();
    virtual ~ColumnaFecha();


    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;


    };

#endif /* COLUMNAFECHA_H_ */
