#ifndef REFRESCA_OFERTAS_H
#define REFRESCA_OFERTAS_H
#include <QThread>
#include "../Auxiliares/Globlal_Include.h"

class refresca_ofertas : public QThread
{
    Q_OBJECT
public:
    explicit refresca_ofertas(QObject *parent = 0);

protected:
    virtual void run();

signals:

public slots:

};

#endif // REFRESCA_OFERTAS_H
