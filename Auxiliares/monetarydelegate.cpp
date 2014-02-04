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
    qDebug() << editor;
    if(QLineEdit *lineedit = qobject_cast<QLineEdit*>(editor))
    {
    lineedit->text();
    lineedit->setReadOnly(this->readonly);
    double valor_moneda = lineedit->text().toDouble();
    QString valor = QString::number(valor_moneda,'f',Configuracion_global->decimales);

    model->setData(index,valor,Qt::EditRole);
    lineedit->setAlignment(Qt::AlignRight);
    }
    else if(QDoubleSpinBox *spin = qobject_cast<QDoubleSpinBox*>(editor))
    {
        model->setData(index,spin->value());
    }
}

void MonetaryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString texto = index.model()->data(index, Qt::DisplayRole).toString();
    double valor_moneda = texto.toDouble();
    QString valor_moneda_string = QString::number(valor_moneda,'f',Configuracion_global->decimales);
     texto = Configuracion_global->toFormatoMoneda(valor_moneda_string);

     QStyleOptionViewItem myOption = option;
     myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
     drawDisplay(painter, myOption, myOption.rect,texto);
     drawFocus(painter, myOption, myOption.rect);
}
