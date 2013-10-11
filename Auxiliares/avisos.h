#ifndef AVISOS_H
#define AVISOS_H

#include <QObject>
#include <../Auxiliares/Globlal_Include.h>

class Avisos : public QObject
{
    Q_OBJECT
public:
    explicit Avisos(QObject *parent = 0);
    int id;
    int id_tipo_aviso;
    QString tipo_aviso;
    int id_usuario_origen;
    QString usuario_origen;
    int id_usuario_destino;
    QString usuario_destino;
    QString aviso;
    QDateTime fecha_hora_aviso;
    bool completado;
    int id_empresa;
    QString empresa;

    int add_notice(QHash <QString,QVariant> h);
    bool update_notice(QHash <QString,QVariant> h);
    bool delete_notice();
signals:

public slots:

private:

};

#endif // AVISOS_H
