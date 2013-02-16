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

#ifndef __REPORTWRITERWINDOW_H__
#define __REPORTWRITERWINDOW_H_

#include <QMainWindow>
#include <QTimerEvent>
#include <QCloseEvent>

//
// Prototypes
//
class QWorkspace;
class QAction;
class ReportHandler;
class QMenu;
class QString;

//
// Class: ReportWriterWindow
//
class ReportWriterWindow : public QMainWindow {
    Q_OBJECT
    public:
        ReportWriterWindow();
        ~ReportWriterWindow();

    public slots:
        virtual void setCaption();
        virtual void openReportFile(const QString &);

    protected:
        virtual void closeEvent(QCloseEvent*);
        virtual void timerEvent(QTimerEvent*);

    private:
        QWorkspace * ws;
        int dbTimerId;

        QAction * fileExitAction;

        ReportHandler * handler;

        QMenu *windowMenu;

    private slots:
        void appExit();
        void sPrepareWindowMenu();
};

#endif

