#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QItemDelegate>
#include "Globlal_Include.h"
class comboboxDelegate : public QItemDelegate
{
    Q_OBJECT
    Q_PROPERTY(QString m_text READ Text WRITE setText USER true)
public:
    explicit comboboxDelegate(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;
    void setUpModel(QString db , QString table, QString column);
    void setUpItems(QStringList items);
signals:
    
public slots:
    QString Text();
    void setText(QString s);
private:
    QString db;
    QString table;
    QString column;
    QStringList items;
    QString m_text;
    bool usingModel;
};

#endif // COMBOBOXDELEGATE_H
