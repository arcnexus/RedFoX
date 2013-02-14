/*
 * OpenRPT report writer and rendering engine
 * Copyright (C) 2001-2011 by OpenMFG, LLC
 *
 * This file copyright (C) 2011 by Paladin Logic, Ltd.
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

#ifndef LABELDEFINITIONEDITOR_H
#define LABELDEFINITIONEDITOR_H

#include <QDialog>
#include <labelsizeinfo.h>

#include "ui_labeldefinitioneditor.h"

class LabelDefinitionEditor : public QDialog, public Ui::LabelDefinitionEditor
{
    Q_OBJECT

  public:
    LabelDefinitionEditor(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~LabelDefinitionEditor();

  public slots:
    virtual void init(LabelSizeInfo);
    virtual LabelSizeInfo getLabelDefinition() { return labelDefinition; }

  protected slots:
    virtual void languageChange();
    virtual void saveLabelDef();

  protected:
    virtual bool nameExists(QString name);
    LabelSizeInfo labelDefinition;
    QStringList paperDefs;
};

#endif // LABELDEFINITIONEDITOR_H
