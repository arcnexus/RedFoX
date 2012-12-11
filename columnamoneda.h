#ifndef COLUMNAMONEDA_H
#define COLUMNAMONEDA_H

#include <QtGui>
#include <QStyleOptionViewItem>
#include <QItemDelegate>
/*
 * columnamoneda.h
 *
 *  Created on: September, 2012
 *      Author: ArcNexus
 */


class  ColumnaMoneda: public QItemDelegate
    {
public:
    ColumnaMoneda();
    virtual ~ColumnaMoneda();


    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;


    };

#endif /* COLUMNAMONEDA_H_ */
