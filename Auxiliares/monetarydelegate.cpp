#include "monetarydelegate.h"
//#include "configuracion.h"
#include "Globlal_Include.h"

MonetaryDelegate::MonetaryDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

void MonetaryDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
    lineedit->text();
    QString valor = Configuracion_global->FormatoNumerico(lineedit->text());
    model->setData(index,valor,Qt::EditRole);
}
