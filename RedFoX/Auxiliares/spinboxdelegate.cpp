#include "spinboxdelegate.h"

SpinBoxDelegate::SpinBoxDelegate(QObject *parent ,bool isDecimal, int min, int max) :
    QItemDelegate(parent)
{
    this->max = max;
    this->min = min;

    decimal = isDecimal;
}


QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(decimal)
    {
        QDoubleSpinBox * spin = new QDoubleSpinBox(parent);
        spin->setMinimum(min);
        if(max != 0)
            spin->setMaximum(max);
        else
            spin->setMaximum(999999999);
        spin->setAlignment(Qt::AlignRight);
        return spin;
    }
    else
    {
        QSpinBox * spin = new QSpinBox(parent);
        spin->setMinimum(min);
        if(max != 0)
            spin->setMaximum(max);
        else
            spin->setMaximum(999999999);
        spin->setAlignment(Qt::AlignRight);
        return spin;
    }
}
