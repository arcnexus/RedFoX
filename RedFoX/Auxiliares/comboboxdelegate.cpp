#include "comboboxdelegate.h"

comboboxDelegate::comboboxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    usingModel = false;
}

QWidget *comboboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox * combo = new QComboBox(parent);

    if(usingModel)
    {
        QSqlTableModel* model = new QSqlTableModel(combo,QSqlDatabase::database(db));
        model->setTable(table);
        model->select();
        combo->setModel(model);
        combo->setModelColumn(model->fieldIndex(column));
    }
    else
        combo->addItems(items);

    return combo;
}

void comboboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *combo= static_cast<QComboBox*>(editor);
    model->setData(index, combo->currentText() , Qt::EditRole);
}


void comboboxDelegate::setUpModel(QString db, QString table, QString column)
{
    this->db = db;
    this->table = table;
    this->column = column;
    usingModel = true;
}

void comboboxDelegate::setUpItems(QStringList items)
{
    this->items = items;
}

QString comboboxDelegate::Text()
{
    return m_text;
}

void comboboxDelegate::setText(QString s)
{
}
