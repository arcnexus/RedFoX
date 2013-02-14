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

#ifndef QUERYLIST_H
#define QUERYLIST_H

class QuerySource;
class QuerySourceList;

#include <QDialog>

#include "ui_querylist.h"

class QueryList : public QDialog, public Ui::QueryList
{
    Q_OBJECT

public:
    QueryList(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~QueryList();

public slots:
    virtual void editQuery( QListWidgetItem * );
    virtual void btnEdit_clicked();
    virtual void btnDelete_clicked();
    virtual void btnAdd_clicked();
    virtual void init( QuerySourceList * );

protected:
    QuerySourceList * qsList;

protected slots:
    virtual void languageChange();

};

#endif // QUERYLIST_H
