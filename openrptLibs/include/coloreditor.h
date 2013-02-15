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

#ifndef COLOREDITOR_H
#define COLOREDITOR_H

#include <QVariant>
#include <QDialog>

#include "ui_coloreditor.h"

class ColorEditor : public QDialog, public Ui::ColorEditor
{
    Q_OBJECT

public:
    ColorEditor(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~ColorEditor();

public slots:
    virtual void _btnColor_clicked();
    virtual void setColorName( QString name );
    virtual void setColor( const QColor & col );
    virtual QColor getColor();
    virtual QString getColorName();

protected slots:
    virtual void languageChange();

};

#endif // COLOREDITOR_H
