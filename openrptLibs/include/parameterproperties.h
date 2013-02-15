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

#ifndef PARAMETERPROPERTIES_H
#define PARAMETERPROPERTIES_H

#include <QDialog>

#include "C:\openrpt-build-Desktop_Qt_4_8_2_in_PATH_4_8_2-Debug\common\tmp/ui_parameterproperties.h"

class ParameterProperties : public QDialog, public Ui::ParameterProperties
{
  Q_OBJECT

  public:
    ParameterProperties(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~ParameterProperties();

    Q_INVOKABLE virtual bool            active();
    Q_INVOKABLE virtual QList<QVariant> list();
    Q_INVOKABLE virtual QString         name();
    Q_INVOKABLE virtual QVariant::Type  type();
    Q_INVOKABLE virtual QString         typeName();
    Q_INVOKABLE virtual QVariant        value();

  public slots:
    virtual void languageChange();
    virtual void editItem();
    virtual void deleteItem();
    virtual void moveItemUp();
    virtual void moveItemDown();
    virtual void newItem();
    virtual void setActive(bool p);
    virtual void setList(const QList<QVariant> &l);
    virtual void setName(QString p);
    virtual void setType(QString p);
    virtual void setType(QVariant::Type p);
    virtual void setValue(QVariant p);
};
#endif
