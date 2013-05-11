#include "datedelegate.h"

DateDelegate::DateDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
    lineedit->text();
    QString valor = lineedit->text();
    model->setData(index,valor,Qt::EditRole);
    lineedit->setAlignment(Qt::AlignRight);
}


void DateDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString texto = index.model()->data(index, Qt::DisplayRole).toDate().toString("dd/MM/yyyy");


     QStyleOptionViewItem myOption = option;
     myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
     drawDisplay(painter, myOption, myOption.rect,texto);
     drawFocus(painter, myOption, myOption.rect);
}
