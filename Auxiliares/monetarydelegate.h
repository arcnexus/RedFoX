#ifndef MONETARYDELEGATE_H
#define MONETARYDELEGATE_H

#include <QItemDelegate>
#include<QLineEdit>;
#include <QtGui>
#include <QtCore>

class MonetaryDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MonetaryDelegate(QObject *parent = 0);

  void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
signals:
    
public slots:

private:
    QStandardItemModel *model;
    
};

#endif // MONETARYDELEGATE_H
