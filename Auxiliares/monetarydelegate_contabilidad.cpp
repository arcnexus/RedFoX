#include "monetarydelegate_contabilidad.h"

#include "Globlal_Include.h"

MonetaryDelegate_contabilidad::MonetaryDelegate_contabilidad(QObject *parent, bool readonly) :
    QItemDelegate(parent)
{
    this->readonly = readonly;
}

void MonetaryDelegate_contabilidad::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
    lineedit->text();
    lineedit->setReadOnly(this->readonly);

    QString valor = Configuracion_global->FormatoNumerico(lineedit->text());
    model->setData(index,valor,Qt::EditRole);
    lineedit->setAlignment(Qt::AlignRight);
}

void MonetaryDelegate_contabilidad::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString texto = index.model()->data(index, Qt::DisplayRole).toString();
     texto = Configuracion_global->FormatoNumerico(texto);
     if(texto =="0,00")
         texto = "";
     QStyleOptionViewItem myOption = option;
     myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
     if(texto.left(1)=="-")
        myOption.palette.setColor(QPalette::Text, Qt::red);
     else
        myOption.palette.setColor(QPalette::Text, Qt::blue);
     drawDisplay(painter, myOption, myOption.rect,texto);
     drawFocus(painter, myOption, myOption.rect);

}
