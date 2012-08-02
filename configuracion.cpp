#include "configuracion.h"
#include <QMessageBox>
#include<QSqlError>
#include <QSqlQuery>
#include <QSettings>
#include <QDebug>
#include <QFileDialog>

QSettings fichConf("infint","terra");

Configuracion::Configuracion(QObject *parent) :
    QObject(parent)
{

}





