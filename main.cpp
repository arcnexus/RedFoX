// Terra Software médico profesional (Open Source GNU)
// Copyright (C) 2012-2013  Marc Miralles Biosca
//
// Terra Gestión empresarial open-source is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Terra Gestión empresarial open-source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Terra Gestión empresarial open-source.  If not, see <http://www.gnu.org/licenses/>.
//
// Email   : marcmirallesbiosca +(simboloarroba)+ gmail DOT com
// Web-Site: http://www.informatica-intelligent.com


#include "mainwindow.h"
#include <qsql.h>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTextCodec>
#include <QtSql>
//#include <QWindowsStyle>
#include <QStyleFactory>
#include <QApplication>


/*   #THEFOX#  */ // ARCNEXUS //



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	
    QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        QTextCodec::setCodecForTr(linuxCodec);
        QTextCodec::setCodecForCStrings(linuxCodec);
   #endif
        QTextCodec::setCodecForLocale(linuxCodec);;
//   a.setStyle("fusion");
//   QFile file(":Icons/Terra.qss");
//   if(file.open(QFile::ReadOnly)) {
//        QString styleSheet = QString::fromLatin1(file.readAll());
//        a.setStyleSheet(styleSheet);
//   } else
//       QMessageBox::warning(NULL,QObject::tr("Terra"),QObject::tr("No se puede cargar el archivo de tema"),
//                            QObject::tr("Aceptar"));
        QFile qss(":Icons/Terra.qss");
        qss.open(QFile::ReadOnly);
        a.setStyle("fusion");
        a.setStyleSheet(qss.readAll());
        qss.close();
	

  MainWindow w;
  //  w.setVisible(true);
   w.setWindowState(Qt::WindowMaximized );
  //w.show();

    
    return a.exec();
}

