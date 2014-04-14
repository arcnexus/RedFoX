#include "readonlydelegate.h"

ReadOnlyDelegate::ReadOnlyDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *ReadOnlyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return 0;    
}


