#ifndef SPINBOXDELEGATE_H
#define SPINBOXDELEGATE_H

#include <QItemDelegate>
#include <QSpinBox>
#include <QDoubleSpinBox>
class SpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SpinBoxDelegate(QObject *parent = 0,bool isDecimal = false,int min = 1, int max = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:
    
public slots:
private:
    int min;
    int max;
    bool decimal;
};

#endif // SPINBOXDELEGATE_H
