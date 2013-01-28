
#ifndef GLOBLAL_INCLUDE_H
#define GLOBLAL_INCLUDE_H

#include <QApplication>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QSettings>

#include <QtGui>
#include <QDesktopWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QSpinBox>
#include <QMessageBox>
#include <QItemDelegate>
#include <QDoubleSpinBox>
#include <QWidget>
#include <QDialog>
#include <QTreeWidgetItem>
#include <QSqlRelationalTableModel>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QSqlRelationalDelegate>
#include <QCloseEvent>

#include <QFileDialog>

#include <QNetworkReply>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QDate>
#include <QtSql>
#include <QGridLayout>
#include <QToolBar>
#include <QMainWindow>
#include <QToolButton>
#include <QProgressBar>
#include <QLabel>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QInputDialog>
#include <QLineEdit>

#include <QXmlStreamAttributes>

#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>


#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/qwidget.h>
#include <QtWebKitWidgets/QWebPage>
#else
#include <QWebView>
#include <QtGui/qwidget.h>
#include <QWebPage>
#endif


#include <QDebug>
#include <QSqlError>
# include <QDomDocument>

#include "configuracion.h"
extern Configuracion * Configuracion_global;

#endif // GLOBLAL_INCLUDE_H
