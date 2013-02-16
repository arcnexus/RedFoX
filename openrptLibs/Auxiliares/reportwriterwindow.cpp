/*
 * OpenRPT report writer and rendering engine
 * Copyright (C) 2001-2011 by OpenMFG, LLC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * Please contact info@openmfg.com with any questions on this license.
 */

#include "reportwriterwindow.h" // The Primary MDI window
#include "../include/reporthandler.h"       // The Report Window Child

// common
#include <dbtools.h>
#include <xsqlquery.h>
#include <data.h>

// qt
#include <QSqlDatabase>
#include <QWorkspace>
#include <QMenuBar>
#include <QAction>
#include <QEvent>
#include <QStatusBar>
#include <QPixmap>
#include <QCloseEvent>
#include <QSqlQuery>
#include <QTimerEvent>
#include <QTranslator>
#include <QApplication>

// images
#include "images/OpenReportsIcon.xpm"
#include "../include/builtinSqlFunctions.h"

//
// ReportWriterWindow
//
ReportWriterWindow::ReportWriterWindow()
  : QMainWindow() {
    
    setWindowTitle("OpenMFG: Report Writer");
    setWindowIcon(QPixmap(OpenReportsIcon_xpm));

    // add the workspace
    ws = new QWorkspace();
    ws->setScrollBarsEnabled(TRUE);

    setCentralWidget(ws);

    // setup the menubar
    fileExitAction = new QAction(tr("E&xit"), this);
    fileExitAction->setObjectName("file exit");
    fileExitAction->setShortcut(Qt::ALT+Qt::Key_F4);
    connect(fileExitAction, SIGNAL(activated()), this, SLOT(appExit()));

    handler = new ReportHandler(this, "report handler");

#ifdef NODBSUPPORT
    handler->setNoDatabase(true);
#endif

    QAction * sepid = handler->populateMenuBar(menuBar(), fileExitAction);

    windowMenu = new QMenu(tr("&Windows"));
    //windowMenu->setCheckable(TRUE);
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(sPrepareWindowMenu()));
    menuBar()->insertMenu(sepid, windowMenu);

    // setup the toolbar
    handler->docToolBars(this);

    setCaption();

    dbTimerId = startTimer(60000);

    handler->setParentWindow(ws);

    connect(handler, SIGNAL(dbOpenClosed()), this, SLOT(setCaption()));
    connect(handler, SIGNAL(messageChanged(const QString &)),
            statusBar(), SLOT(message(const QString &)));
    connect(handler, SIGNAL(messageCleared()),
            statusBar(), SLOT(clear()));
    handler->onWinChanged(NULL);
    connect(ws, SIGNAL(windowActivated(QWidget*)), handler, SLOT(onWinChanged(QWidget*)));
    
}

//
// Perform any cleanup action that needs to take place
//
ReportWriterWindow::~ReportWriterWindow() {
     // everything should be handled by Qt
}


//
// slot implementations
//
void ReportWriterWindow::appExit() {
    close();
}


//
// event handler methods
//

void ReportWriterWindow::timerEvent(QTimerEvent * e) {
    if(e->timerId() == dbTimerId) {
        QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection,FALSE);
        if(db.isValid()) {
            QSqlQuery qry(getSqlFromTag("fmt07", db.driverName()));		// MANU

#if 0
            if(qry.first() == TRUE) {
                // Nothing to do.  We were just creating a little traffic
                qDebug("Keep alive succeeded");
            } else {
                // I really don't care
                qDebug("Keep alive failed");
            }
#endif
        }
    }
}

void ReportWriterWindow::closeEvent(QCloseEvent * e) {
    QWidgetList wl = ws->windowList();
    QWidget * w = 0;
    for(int i = 0; i < wl.size(); i++)
    {
      w = wl.at(i);
      if(w && !w->close())
      {
        e->ignore();
        return;
      }
    }
    e->accept();
}

void ReportWriterWindow::setCaption() {
    QString caption = QString("%1 %2 %3")
                          .arg(handler->name())
                          .arg(tr("Version"))
                          .arg(handler->version());

    QSqlDatabase db = QSqlDatabase::database(QSqlDatabase::defaultConnection, false);
    if(db.isValid()) {
		XSqlQuery dbname;
        dbname.prepare( getSqlFromTag("fmt08", db.driverName()));		// MANU

        if ((dbname.isActive()) && (dbname.size())) {
            QString protocol;
            QString server;
            QString database;
            QString port;

            dbname.next();

            parseDatabaseURL((const QString &)OpenRPT::databaseURL,
                             protocol, server, database, port);

            caption = QString( tr("%1 - %2 on %3/%4 AS %5") )
                            .arg(caption)
                            .arg(dbname.value("metric_value").toString())
                            .arg(server)
                            .arg(database)
                            .arg(dbname.value("username").toString());
        }
    }

    QMainWindow::setWindowTitle(caption);
}

void ReportWriterWindow::sPrepareWindowMenu()
{
  windowMenu->clear();

  QAction * intCascadeid = windowMenu->addAction(tr("&Cascade"), ws, SLOT(cascade()));
  QAction * intTileid = windowMenu->addAction(tr("&Tile"), ws, SLOT(tile()));

  windowMenu->addSeparator();

  int cnt = 0;
  QWidgetList windows = ws->windowList();
  for (int intCursor = 0; intCursor < windows.count(); intCursor++)
  {
    if(windows.at(intCursor)->isVisible())
    {
      QAction * intMenuid = windowMenu->addAction(windows.at(intCursor)->windowTitle(), windows.at(intCursor), SLOT(setFocus()));
      intMenuid->setChecked((ws->activeWindow() == windows.at(intCursor)));
      cnt++;
    }
  }

  if (cnt < 1)
  {
    intCascadeid->setEnabled(false);
    intTileid->setEnabled(false);
  }
}

void ReportWriterWindow::openReportFile(const QString &fileName)
{
  handler->fileOpen(fileName);
}

