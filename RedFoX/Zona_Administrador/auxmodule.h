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
    int id_user;
    QString tabla;
    MayaModule::module_zone zone;
    QString name;

    int nivel();
    void setNivel(int idAcceso);

    static QStringList model();
    static void setModel();

signals:
    
public slots:
private slots:
    void save(int index);
private:
    QLabel _label;
    QComboBox _combo;
    QHBoxLayout _layout;

    static QStringList _model;
};

#endif // AUXMODULE_H
