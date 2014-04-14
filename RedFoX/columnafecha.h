#ifndef COLUMNAFECHA_H
#define COLUMNAFECHA_H


/*
 * columnafecha.h
 *
 *  Created on: September, 2012
 *      Author: ArcNexus
 */
#include <QItemDelegate>

class  ColumnaFecha: public QItemDelegate
{
public:
    ColumnaFecha(QObject* parent);
    virtual ~ColumnaFecha();


    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;


};

#endif /* COLUMNAFECHA_H_ */
