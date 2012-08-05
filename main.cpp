#include <QtGui/QApplication>
#include "mainwindow.h"
#include <qsql.h>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTextCodec>
#include <QtSql>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(linuxCodec);
    QTextCodec::setCodecForCStrings(linuxCodec);
    QTextCodec::setCodecForLocale(linuxCodec);

    MainWindow w;
    w.setVisible(true);
    w.setWindowState(Qt::WindowMaximized );
    w.show();
    
    return a.exec();
}
