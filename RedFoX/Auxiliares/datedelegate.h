#ifndef DATEDELEGATE_H
#define DATEDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QtGui>
#include <QtCore>

class DateDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DateDelegate(QObject *parent = 0);
    void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
signals:
    
public slots:
    
};

#endif // DATEDELEGATE_H
