#include "searchdelegate.h"

SearchDelegate::SearchDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *SearchDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return new Search_LineEdit(parent);
}
