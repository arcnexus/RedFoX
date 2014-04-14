#ifndef DELEGATEKIT_H
#define DELEGATEKIT_H

#include <QItemDelegate>
#include <QPainter>
#include <QLabel>
#include <QTableView>

class DelegateKit : public QItemDelegate
{
public:
DelegateKit(QObject * parent = 0);

void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
private:
QLabel *label;
QTableView *myWidget;
};

#endif // DELEGATEKIT_H
