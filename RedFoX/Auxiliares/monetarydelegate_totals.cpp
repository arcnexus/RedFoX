#include "monetarydelegate_totals.h"
//#include "configuracion.h"
#include "Globlal_Include.h"

MonetaryDelegate_totals::MonetaryDelegate_totals(QObject *parent, bool readonly) :
    QItemDelegate(parent)
{
    this->readonly = readonly;
}

void MonetaryDelegate_totals::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
    lineedit->text();
    lineedit->setReadOnly(this->readonly);
    double valor_moneda = lineedit->text().toDouble();
    QString valor = QString::number(valor_moneda,'f',Configuracion_global->decimales_campos_totales);

    model->setData(index,valor,Qt::EditRole);
    lineedit->setAlignment(Qt::AlignRight);
}

void MonetaryDelegate_totals::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString texto = index.model()->data(index, Qt::DisplayRole).toString();
    double valor_moneda = texto.toDouble();
    QString valor_moneda_string = QString::number(valor_moneda,'f',Configuracion_global->decimales_campos_totales);
     texto = Configuracion_global->toFormatoMoneda(valor_moneda_string);

     QStyleOptionViewItem myOption = option;
     myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
     drawDisplay(painter, myOption, myOption.rect,texto);
     drawFocus(painter, myOption, myOption.rect);
}
