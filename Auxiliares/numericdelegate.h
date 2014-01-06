#ifndef NUMERICDELEGATE_H
#define NUMERICDELEGATE_H

#include <QItemDelegate>
#include<QLineEdit>;
#include <QtGui>
#include <QtCore>

class NumericDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit NumericDelegate(QObject *parent = 0, bool readonly = false);

  void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
signals:
    
public slots:

private:
    QStandardItemModel *model;
    bool readonly;
    
};

#endif // NUMERICDELEGATE_H
