// Terra Gestión empresarial open-source y multi-sectorial
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
#include <QWindowsStyle>
#include <QWindowsVistaStyle>
#include <QStyleFactory>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(linuxCodec);
    QTextCodec::setCodecForCStrings(linuxCodec);
    QTextCodec::setCodecForLocale(linuxCodec);
     //QStyle *style = QStyleFactory::create("oxygen");
    QStyle *style = QStyleFactory::create("plastique");
    QApplication::setStyle(style);
    MainWindow w;
    w.setVisible(true);
    w.setWindowState(Qt::WindowMaximized );
    w.show();
    
    return a.exec();
}
