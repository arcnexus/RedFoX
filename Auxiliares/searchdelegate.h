#ifndef SEARCHDELEGATE_H
#define SEARCHDELEGATE_H

#include <QItemDelegate>

#include "../db_table_view.h"
#include "search_lineedit.h"
class SearchDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SearchDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    
public slots:
private:
    Db_table_View* db_view;
};

#endif // SEARCHDELEGATE_H
