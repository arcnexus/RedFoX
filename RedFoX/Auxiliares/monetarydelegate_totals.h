#ifndef MONETARYDELEGATE_TOTALS_H
#define MONETARYDELEGATE_TOTALS_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QtGui>
#include <QtCore>

class MonetaryDelegate_totals : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MonetaryDelegate_totals(QObject *parent = 0, bool readonly = false);

  void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

private:
    QStandardItemModel *model;
    bool readonly;

};

#endif // MONETARYDELEGATE_TOTALS_H

