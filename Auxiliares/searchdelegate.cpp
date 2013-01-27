#include "searchdelegate.h"

SearchDelegate::SearchDelegate(QObject *parent, Db_table_View *view) :
    QItemDelegate(parent)
{
    db_view = view;
}

QWidget *SearchDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return new Search_LineEdit(parent,db_view);
}
