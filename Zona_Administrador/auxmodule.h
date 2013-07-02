#ifndef AUXMODULE_H
#define AUXMODULE_H

#include <QWidget>
#include "../mayamodule.h"
class AuxModule : public QWidget
{
    Q_OBJECT
public:
    explicit AuxModule(QWidget *parent = 0, QString label="");
    ~AuxModule();
    int id;
    MayaModule::module_zone zone;
    QString name;

    int nivel();
    void setNivel(int idAcceso);

    static QSqlTableModel *model();
    static void setModel();

signals:
    
public slots:
private:
    QLabel _label;
    QComboBox _combo;
    QHBoxLayout _layout;

    static QSqlTableModel *_model;
    static bool _modelIsSet;

};

#endif // AUXMODULE_H
