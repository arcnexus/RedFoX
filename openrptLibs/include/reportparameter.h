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

#ifndef REPORTPARAMETER_H
#define REPORTPARAMETER_H

#include <QDialog>

#include "ui_reportparameter.h"
#include "parsexmlutils.h"

class ReportParameter : public QDialog, public Ui::ReportParameter
{
    Q_OBJECT

public:
    ReportParameter(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~ReportParameter();

public slots:
    virtual QString paramName();
    virtual ORParameter paramData();
    virtual void setParamName( const QString & text );
    virtual void setParamData( const ORParameter & param );
    virtual void sSelectionChanged();
    virtual void sAdd();
    virtual void sEdit();
    virtual void sRemove();

protected slots:
    virtual void languageChange();

};

#endif // REPORTPARAMETER_H
