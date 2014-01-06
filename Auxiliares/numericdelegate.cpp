#include "numericdelegate.h"
//#include "configuracion.h"
#include "Globlal_Include.h"

NumericDelegate::NumericDelegate(QObject *parent, bool readonly) :
    QItemDelegate(parent)
{
    this->readonly = readonly;
}

void NumericDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
    lineedit->text();
    lineedit->setReadOnly(this->readonly);
    QString valor = lineedit->text();

    model->setData(index,valor,Qt::EditRole);
    lineedit->setAlignment(Qt::AlignRight);
}

void NumericDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString texto = index.model()->data(index, Qt::DisplayRole).toString();
    QString valor_string = texto;

     QStyleOptionViewItem myOption = option;
     myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
     drawDisplay(painter, myOption, myOption.rect,texto);
     drawFocus(painter, myOption, myOption.rect);
}
