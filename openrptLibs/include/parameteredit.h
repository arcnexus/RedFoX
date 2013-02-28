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

#ifndef PARAMETEREDIT_H
#define PARAMETEREDIT_H

#include <QDialog>
#include <QDomDocument>

#include "parameter.h"


#include "ui_parameteredit.h"


class ParameterEdit : public QWidget, public Ui::ParameterEdit
{
  Q_OBJECT

  public:
    ParameterEdit(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~ParameterEdit();

    Q_INVOKABLE static  QDialog      *ParameterEditDialog(ParameterEdit *p, QWidget *parent = 0, Qt::WindowFlags f = Qt::Dialog);
    Q_INVOKABLE virtual ParameterList getParameterList();
    Q_INVOKABLE         bool          setDocument(const QDomDocument & doc);

  public slots:
    virtual void clear();

  protected:
    QMap<QString,QVariant> _params;

  protected slots:
    virtual void languageChange();

    virtual void newItem();
    virtual void edit();
    virtual void editItem(int row);
    virtual void editItemList();
    virtual void doubleClick(int row, int col);
    virtual void selectionChange();
    virtual void deleteItem();

private:
    void updateParam(const QString & name, const QVariant & value, bool active);

    QMap<QString, QList<QPair<QString,QString> > > _lists;

};

#endif // PARAMETEREDIT_H
