#include "delegatekit.h"


DelegateKit::DelegateKit(QObject *parent) :
    QItemDelegate(parent)
{
}


void DelegateKit::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionButton buttonOption;
     buttonOption.rect = option.rect;
     buttonOption.direction = Qt::LeftToRight;
     buttonOption.state = QStyle::State_Enabled;

     painter->save();
     painter->setRenderHint(QPainter::Antialiasing);
     painter->setBrush(QBrush(QColor(0,128,0)));
     painter->setPen(QColor("black"));
     if(index.model()->data(index,Qt::DisplayRole).toInt() == 1)
        painter->drawRoundedRect(option.rect.x()+5, option.rect.y()+5, option.rect.width()-10, option.rect.height()-10,10,10);

}



