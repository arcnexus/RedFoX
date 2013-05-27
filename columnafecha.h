#ifndef COLUMNAFECHA_H
#define COLUMNAFECHA_H

#include "../Auxiliares/Globlal_Include.h"
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
