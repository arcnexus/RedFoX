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
// Email   : mmiralles +(simboloarroba)+ mayasoftware DOT es
// Web-Site: http://www.mayasoftware.es


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
        Configuracion_global->driver_bd_empresa = record.field("driver_bd").value().toString();
        Configuracion_global->host_bd_empresa = record.field("host").value().toString();
        Configuracion_global->nombre_bd_empresa =record.field("nombre_bd").value().toString();
        Configuracion_global->password_bd_empresa =record.field("contrasena").value().toString();
        Configuracion_global->ruta_bd_empresa = record.field("ruta_bd_sqlite").value().toString();
        Configuracion_global->usuario_bd_empresa = record.field("user").value().toString();
        Configuracion_global->divisa_local = Configuracion_global->Devolver_moneda(record.field("id_divisa").value().toInt());
        Configuracion_global->cod_divisa_local = Configuracion_global->Devolver_codDivisa(record.field("id_divisa").value().toInt());
        Configuracion_global->divisa_local_reducida = "";
        Configuracion_global->driver_db_conta = record.field("driver_db_conta").value().toString();
        Configuracion_global->host_db_conta= record.field("host_db_conta").value().toString();
        Configuracion_global->nombre_db_conta = record.value("nombre_db_conta").toString();
        Configuracion_global->usuario_db_conta = record.field("user_db_conta").value().toString();
        Configuracion_global->password_db_conta = record.field("password_db_conta").value().toString();
        Configuracion_global->port_db_conta = record.field("puerto_db_conta").value().toString();
        Configuracion_global->contabilidad = record.field("contabilidad").value().toBool();
        Configuracion_global->ticket_factura = record.field("ticket_factura").value().toBool(); // el tiquet y la factura son correlativos
        Configuracion_global->id_tarifa_predeterminada = record.field("id_tarifa_predeterminada").value().toInt();
        Configuracion_global->actualizar_divisas = record.field("actualizar_divisas").value().toBool();

        if(record.field("medica").value().toInt()==1)
            Configuracion_global->medic = true;
        else
            Configuracion_global->medic = false;

        if(record.field("internacional").value().toInt()==1)
            Configuracion_global->internacional = true;
        else
            Configuracion_global->internacional = false;

        Configuracion_global->enlace_web = record.field("enlace_web").value().toBool();

        QApplication::processEvents();

        //DBMedica
        //splash.showMessage(tr("Cargando configuración médica"),Qt::AlignBottom);
        Configuracion_global->driver_bd_medica = record.field("driver_bd_medica").value().toString();
        Configuracion_global->host_bd_medica = record.field("host_bd_medica").value().toString();
        Configuracion_global->nombre_bd_medica =record.field("nombre_bd_medica").value().toString();
        Configuracion_global->password_bd_medica =record.field("contrasena_bd_medica").value().toString();
        Configuracion_global->ruta_bd_medica = record.field("ruta_bd_medica_sqlite").value().toString();
        Configuracion_global->usuario_bd_medica = record.field("user_bd_medica").value().toString();

        QApplication::processEvents();

        // Varios
        //splash.showMessage(tr("Cargando configuración financiera"),Qt::AlignBottom);
        Configuracion_global->serie = record.field("serie").value().toString();
        Configuracion_global->digitos_cuentas_contables = record.value("digitos_cuenta").toInt();
        Configuracion_global->cuenta_acreedores = record.field("codigo_cuenta_acreedores").value().toString();
        Configuracion_global->cuenta_clientes = record.field("codigo_cuenta_clientes").value().toString();
        Configuracion_global->cuenta_proveedores = record.field("codigo_cuenta_proveedores").value().toString();

        QApplication::processEvents();

        // Guardo preferencias
        QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
        settings.setValue("serie",Configuracion_global->serie);
        settings.setValue("digitos_cuenta",Configuracion_global->digitos_cuentas_contables);
        settings.setValue("cuenta_clientes",Configuracion_global->cuenta_clientes);
        settings.setValue("cuenta_proveedores",Configuracion_global->cuenta_proveedores);
        settings.setValue("cuenta_acreedores",Configuracion_global->cuenta_acreedores);
        settings.setValue("Clave1",record.field("clave1").value().toString());
        settings.setValue("Clave2",record.field("clave2").value().toString());

        QApplication::processEvents();

        // Abro empresa activa
        //splash.showMessage(tr("Cargando datos de la empresa activa"),Qt::AlignBottom);
        QSqlDatabase dbEmpresa = QSqlDatabase::addDatabase(Configuracion_global->driver_bd_empresa,"empresa");
        if (Configuracion_global->driver_bd_empresa =="QSQLITE")
        {
            dbEmpresa.setDatabaseName(Configuracion_global->ruta_bd_empresa);
            dbEmpresa.open();
        }
        else
        {
            dbEmpresa.setDatabaseName(Configuracion_global->nombre_bd_empresa);
            dbEmpresa.setHostName(Configuracion_global->host_bd_empresa);
            dbEmpresa.open(Configuracion_global->usuario_bd_empresa,Configuracion_global->password_bd_empresa);
        }

        QApplication::processEvents();
        //----------------------
        // Abro bdmedica activa
        //----------------------

        //splash.showMessage(tr("Abriendo base de datos médica"),Qt::AlignBottom);
        QSqlDatabase dbMedica = QSqlDatabase::addDatabase(Configuracion_global->driver_bd_empresa,"dbmedica");
        if (Configuracion_global->driver_bd_medica =="QSQLITE")
        {
            dbMedica.setDatabaseName(Configuracion_global->ruta_bd_medica);
            qDebug() << "Medica:" << Configuracion_global->ruta_bd_medica;
            if(!dbMedica.open())
                QMessageBox::warning(qApp->activeWindow(),QObject::tr("ERROR DB"),QObject::tr("No se ha podido abrir la BD medica"),
                QObject::tr("Aceptar"));
        }
        else
        {
            dbMedica.setDatabaseName(Configuracion_global->nombre_bd_medica);
            dbMedica.setHostName(Configuracion_global->host_bd_medica);
            dbMedica.open(Configuracion_global->usuario_bd_medica,Configuracion_global->password_bd_medica);
        }
        if (dbMedica.lastError().isValid())
        {
            QMessageBox::critical(qApp->activeWindow(), "error:", dbMedica.lastError().text());
        }
        if(Configuracion_global->contabilidad)
        {
            //----------------------------
            // Abro empresa Contabilidad
            //----------------------------
            QSqlDatabase dbConta = QSqlDatabase::addDatabase(Configuracion_global->driver_db_conta,"dbconta");
            if (Configuracion_global->driver_db_conta =="QSQLITE")
            {
                dbMedica.setDatabaseName(Configuracion_global->RutaBD_Conta);
                if(!dbConta.open())
                    QMessageBox::warning(qApp->activeWindow(),QObject::tr("ERROR DB"),
                                         QObject::tr("No se ha podido abrir la BD de Contabilidad"),
                                         QObject::tr("Aceptar"));
            }
            else
            {
                dbConta.setDatabaseName(Configuracion_global->nombre_db_conta);
                dbConta.setHostName(Configuracion_global->host_db_conta);
                dbConta.open(Configuracion_global->usuario_db_conta,Configuracion_global->password_db_conta);
            }
            if (dbConta.lastError().isValid())
            {
                QMessageBox::critical(qApp->activeWindow(), "error al conectar a la BD de contabilidad:", dbConta.lastError().text());
            }

        }

        Configuracion_global->Cargar_iva();
        Configuracion_global->Cargar_paises();
        Configuracion_global->CargarClientes();
        Configuracion_global->CargarUsuarios();
        return true;
    }
    else
    {
        QMessageBox::critical(qApp->activeWindow(),"Error","Fallo la conexión al fichero de Empresa");
        return false;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "drivers: "<< QSqlDatabase::drivers();
    QStringList d = QSqlDatabase::drivers();

    if(d.isEmpty())
    {
        QMessageBox::critical(0,QObject::tr("Driver"),QObject::tr("No sql drivers.\n"
                                                                  "La aplicación va a cerrarse.\n"
                                                                  "Por favor instala un driver Sql para usar Maya"));
        return 0;
    }

    QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        QTextCodec::setCodecForTr(linuxCodec);
        QTextCodec::setCodecForCStrings(linuxCodec);
   #endif
   QTextCodec::setCodecForLocale(linuxCodec);

   a.setStyle("fusion");

   QFile file(":Icons/maya.qss");


   if(file.open(QFile::ReadOnly))
        a.setStyleSheet(file.readAll());

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
           Configuracion_global->id_usuario_activo = l.getid_user();
#ifdef Q_OS_WIN32
           w.show();
           w.setWindowState(Qt::WindowMinimized);
           w.showMaximized();
#else
           QRect dr= QApplication::desktop()->rect();
           int ancho = dr.width();
           w.setMaximumWidth(ancho);
           w.setWindowState(Qt::WindowMaximized);
           w.setFixedWidth(ancho);
           w.showMaximized();
#endif
           return a.exec();
       }
   }
   return 0;
}

