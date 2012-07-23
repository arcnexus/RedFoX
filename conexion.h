/****************************************************************************
**
** Copyright (C) 2004-2008 Hevele Hegyi Istvan. All rights reserved.
**
** This file is part of the qtContgen - 2.0.0 project.
**
** Copyright (C) 2004-2006 Trolltech AS. All rights reserved.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef CONNECTION_H
#define CONNECTION_H

#include<QMessageBox>
#include<QSqlDatabase>
#include<qsql.h>
#include<QSqlError>

class QMessageBox;
class QSqlDatabase;
class QSqlError;
class QSqlQuery;



static QSqlDatabase createConnection()
{
    /*
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("terra");
    db.setHostName("localhost");
    return db;*/
}

#endif
