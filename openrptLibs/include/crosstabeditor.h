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

#ifndef CROSSTABEDITOR_H
#define CROSSTABEDITOR_H

#include <QtGui/QDialog>

#include "ui_crosstabeditor.h"

class CrossTabEditor : public QDialog, public Ui::CrossTabEditor
{
    Q_OBJECT

public:
    CrossTabEditor(QWidget* parent = 0);
    ~CrossTabEditor();

    QFont _font;

public slots:
    virtual void btnFont_clicked();

protected slots:
    virtual void languageChange();

private slots:
    void on_valueQueryGroupBox_toggled(bool);

};

#endif // CROSSTABEDITOR_H
