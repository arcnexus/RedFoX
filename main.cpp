// Maya Software para profesionales de la Salud (Open Source GNU)
// Copyright (C) 2012-2013  Marc Miralles Biosca
//
// Maya Gestión empresarial open-source is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Maya Gestión empresarial open-source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Maya Gestión empresarial open-source.  If not, see <http://www.gnu.org/licenses/>.
//
// Email   : mmiralles +(simboloarroba)+ Mayamedicproject DOT org
// Web-Site: http://www.Mayamedicproject.org

#include "mainwindow.h"
#include "login.h"
#include <QStyleFactory>
#include "openrptLibs/include/data.h"

#ifdef staticSql
#include <QtPlugin>
Q_IMPORT_PLUGIN(qsqlmysql)
Q_IMPORT_PLUGIN(qsqlite)
#endif
bool cargarEmpresa(QString empresa)
{
    QSqlQuery QryEmpresa(QSqlDatabase::database("Maya"));
    QryEmpresa.prepare("Select * from empresas where nombre = :nombre");
    QryEmpresa.bindValue(":nombre",empresa.trimmed());
    if (QryEmpresa.exec())
    {
        QryEmpresa.next();

        QApplication::processEvents();
        QSqlRecord record = QryEmpresa.record();

        // DBEMpresa
        //splash.showMessage(tr("Cargando configuración de base de datos"),Qt::AlignBottom);
        Configuracion_global->cDriverBDEmpresa = record.field("driverBD").value().toString();
        Configuracion_global->cHostBDEmpresa = record.field("host").value().toString();
        Configuracion_global->cNombreBDEmpresa =record.field("nombreBD").value().toString();
        Configuracion_global->cPasswordBDEmpresa =record.field("contrasena").value().toString();
        Configuracion_global->cRutaBdEmpresa = record.field("RutaBDSqLite").value().toString();
        Configuracion_global->cUsuarioBDEmpresa = record.field("user").value().toString();
        Configuracion_global->DivisaLocal = Configuracion_global->Devolver_moneda(record.field("id_divisa").value().toInt());
        Configuracion_global->codDivisaLocal = Configuracion_global->Devolver_codDivisa(record.field("id_divisa").value().toInt());

        if(record.field("medica").value().toInt()==1)
            Configuracion_global->medic = true;
        else
            Configuracion_global->medic = false;

        if(record.field("internacional").value().toInt()==1)
            Configuracion_global->internacional = true;
        else
            Configuracion_global->internacional = false;

        Configuracion_global->EnlaceWeb = record.field("enlaceweb").value().toBool();

        QApplication::processEvents();

        //DBMedica
        //splash.showMessage(tr("Cargando configuración médica"),Qt::AlignBottom);
        Configuracion_global->cDriverBDMedica = record.field("driverBDMedica").value().toString();
        Configuracion_global->cHostBDMedica = record.field("hostBDMedica").value().toString();
        Configuracion_global->cNombreBDMedica =record.field("nombreBDMedica").value().toString();
        Configuracion_global->cPasswordBDMedica =record.field("contrasenaBDMedica").value().toString();
        Configuracion_global->cRutaBdMedica = record.field("RutaBDMedicaSqLite").value().toString();
        Configuracion_global->cUsuarioBDMedica = record.field("userBDMedica").value().toString();

        QApplication::processEvents();

        // Varios
        //splash.showMessage(tr("Cargando configuración financiera"),Qt::AlignBottom);
        Configuracion_global->cSerie = record.field("serie").value().toString();
        qDebug() << "desde db mainwindow" << record./*field("ndigitoscuenta").*/value("ndigitoscuenta").toInt();
        Configuracion_global->nDigitosCuentasContables = record./*field("ndigitoscuenta").*/value("ndigitoscuenta").toInt();
        Configuracion_global->cCuentaAcreedores = record.field("codigocuentaacreedores").value().toString();
        Configuracion_global->cCuentaClientes = record.field("codigocuentaclientes").value().toString();
        Configuracion_global->cCuentaProveedores = record.field("codigocuentaproveedores").value().toString();

        QApplication::processEvents();

        // Guardo preferencias
        QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
        settings.setValue("cSerie",Configuracion_global->cSerie);
        settings.setValue("ndigitoscuenta",Configuracion_global->nDigitosCuentasContables);
        settings.setValue("cCuentaClientes",Configuracion_global->cCuentaClientes);
        settings.setValue("cCuentaProveedores",Configuracion_global->cCuentaProveedores);
        settings.setValue("cCuentaAcreedores",Configuracion_global->cCuentaAcreedores);
        settings.setValue("Clave1",record.field("clave1").value().toString());
        settings.setValue("Clave2",record.field("clave2").value().toString());

        QApplication::processEvents();

        // Abro empresa activa
        //splash.showMessage(tr("Cargando datos de la empresa activa"),Qt::AlignBottom);
        QSqlDatabase dbEmpresa = QSqlDatabase::addDatabase(Configuracion_global->cDriverBDEmpresa,"empresa");
        if (Configuracion_global->cDriverBDEmpresa =="QSQLITE")
        {
            dbEmpresa.setDatabaseName(Configuracion_global->cRutaBdEmpresa);
            dbEmpresa.open();
        }
        else
        {
            dbEmpresa.setDatabaseName(Configuracion_global->cNombreBDEmpresa);
            dbEmpresa.setHostName(Configuracion_global->cHostBDEmpresa);
            dbEmpresa.open(Configuracion_global->cUsuarioBDEmpresa,Configuracion_global->cPasswordBDEmpresa);
        }

        QApplication::processEvents();

        // Abro bdmedica activa
        //splash.showMessage(tr("Abriendo base de datos médica"),Qt::AlignBottom);
        QSqlDatabase dbMedica = QSqlDatabase::addDatabase(Configuracion_global->cDriverBDEmpresa,"dbmedica");
        if (Configuracion_global->cDriverBDMedica =="QSQLITE")
        {
            dbMedica.setDatabaseName(Configuracion_global->cRutaBdMedica);
            qDebug() << "Medica:" << Configuracion_global->cRutaBdMedica;
            if(!dbMedica.open())
                QMessageBox::warning(qApp->activeWindow(),QObject::tr("ERROR DB"),QObject::tr("No se ha podido abrir la BD medica"),
                QObject::tr("Aceptar"));
        }
        else
        {
            dbMedica.setDatabaseName(Configuracion_global->cNombreBDMedica);
            dbMedica.setHostName(Configuracion_global->cHostBDMedica);
            dbMedica.open(Configuracion_global->cUsuarioBDMedica,Configuracion_global->cPasswordBDMedica);
        }
        if (dbMedica.lastError().isValid())
        {
            QMessageBox::critical(qApp->activeWindow(), "error:", dbMedica.lastError().text());
        }

        Configuracion_global->Cargar_iva();
        Configuracion_global->Cargar_paises();
        Configuracion_global->CargarClientes();
        Configuracion_global->CargarUsuarios();
        return true;
    }
    else
    {
        QMessageBox::critical(qApp->activeWindow(),"Error","Fallo la conexión al fichero Medico");
        return false;
    }
}

int main(int argc, char *argv[])
{
    qDebug() << "drivers: "<< QSqlDatabase::drivers();
    QStringList d = QSqlDatabase::drivers();

    qDebug() << QStyleFactory::keys();
    QApplication a(argc, argv);

    QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        QTextCodec::setCodecForTr(linuxCodec);
        QTextCodec::setCodecForCStrings(linuxCodec);
   #endif
   QTextCodec::setCodecForLocale(linuxCodec);;

   if(d.isEmpty())
       QMessageBox::information(0,"Driver","No sql drivers");

   //a.setStyle("fusion");

   a.setStyle("Plastique");

   QFile file(":Icons/maya.qss");


   if(file.open(QFile::ReadOnly))
        a.setStyleSheet(file.readAll());
   else
       QMessageBox::warning(qApp->activeWindow(),
                            QObject::tr("Maya"),
                            QObject::tr("No se puede cargar el archivo de tema"),
                            QObject::tr("Aceptar"));



   QTranslator qtTranslator;
   qtTranslator.load("qt_" + QLocale::system().name());
   a.installTranslator(&qtTranslator);
   OpenRPT::languages.addTranslationToDefault(":/openRPTLangs/openrptLibs/traduccion/common_es.qm");
   OpenRPT::languages.addTranslationToDefault(":/openRPTLangs/openrptLibs/traduccion/wrtembed_es.qm");
   OpenRPT::languages.addTranslationToDefault(":/openRPTLangs/openrptLibs/traduccion/renderer_es.qm");
   OpenRPT::languages.addTranslationToDefault(":/openRPTLangs/openrptLibs/traduccion/writer_es.qm");
   OpenRPT::languages.installSelected();

   Login l;
   if ( l.exec()==QDialog::Accepted)
   {
       if(cargarEmpresa(l.getEmpresa()))
       {
           MainWindow w;
           w.empresa = l.getEmpresa();
           w.user =l.getUsuario();
           w.pass =l.getPass();
           w.showInfo();
           Configuracion_global->id_usuario_activo = l.getIdUser();
          //w.setWindowState(Qt::WindowMaximized);


           w.showMaximized();

  //          w.move(0,0);
           return a.exec();
       }
   }
   return 0;
}

