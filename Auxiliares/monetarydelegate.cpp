#include "monetarydelegate.h"
//#include "configuracion.h"
#include "Globlal_Include.h"

MonetaryDelegate::MonetaryDelegate(QObject *parent, bool readonly) :
    QItemDelegate(parent)
{
    this->readonly = readonly;
}

void MonetaryDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
    lineedit->text();
    lineedit->setReadOnly(this->readonly);
    double valor_moneda = lineedit->text().toDouble();
    QString valor_moneda_string = QString::number(valor_moneda,'f',2);
    QString valor = Configuracion_global->toFormatoMoneda(valor_moneda_string);
    model->setData(index,valor,Qt::EditRole);
    lineedit->setAlignment(Qt::AlignRight);
}

void MonetaryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString texto = index.model()->data(index, Qt::DisplayRole).toString();
    double valor_moneda = texto.toDouble();
    QString valor_moneda_string = QString::number(valor_moneda,'f',2);
     texto = Configuracion_global->toFormatoMoneda(valor_moneda_string);

     QStyleOptionViewItem myOption = option;
     myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
     drawDisplay(painter, myOption, myOption.rect,texto);
     drawFocus(painter, myOption, myOption.rect);
}
