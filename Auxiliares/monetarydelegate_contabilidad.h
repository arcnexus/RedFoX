#ifndef MONETARYDELEGATE_CONTABILidAD_H
#define MONETARYDELEGATE_CONTABILidAD_H

#include <QObject>
#include <QItemDelegate>
#include<QLineEdit>;
#include <QtGui>
#include <QtCore>

class MonetaryDelegate_contabilidad : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MonetaryDelegate_contabilidad(QObject *parent = 0, bool readonly = false);
    void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

  signals:

  public slots:

  private:
      QStandardItemModel *model;
      bool readonly;
    
};

#endif // MONETARYDELEGATE_CONTABILidAD_H
