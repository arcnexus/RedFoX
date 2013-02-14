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

#ifndef REPORTPARAMETERLIST_H
#define REPORTPARAMETERLIST_H

#include <QDialog>

#include "ui_reportparameterlist.h"
#include "parsexmlutils.h"

class ReportParameterList : public QDialog, public Ui::ReportParameterList
{
    Q_OBJECT

public:
    ReportParameterList(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~ReportParameterList();

    virtual void setList( QMap<QString, ORParameter> * m );

protected:
    QMap<QString,ORParameter> *_map;

protected slots:
    virtual void languageChange();

    virtual void sAdd();
    virtual void sEdit();
    virtual void sDelete();
    virtual void sEdit( QListWidgetItem * );

};

#endif // REPORTPARAMETERLIST_H
