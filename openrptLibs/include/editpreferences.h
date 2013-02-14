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

#ifndef EDITPREFERENCES_H
#define EDITPREFERENCES_H

#include <QDialog>

#include "ui_editpreferences.h"

class EditPreferences : public QDialog, public Ui::EditPreferences
{
    Q_OBJECT

public:
    EditPreferences(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~EditPreferences();

    QString selectedLanguage() {return _selectedLanguage;}

public slots:
    virtual void setGridSize( double x, double y );
    virtual double gridSizeX();
    virtual double gridSizeY();
    virtual bool showGrid();
    virtual bool snapGrid();
    virtual void setShowGrid( bool show );
    virtual void setSnapGrid( bool snap );
    virtual void setDefaultFont( QFont fnt );
    virtual QFont defaultFont();

protected slots:
    virtual void languageChange();

    virtual void changeDefaultFont();
    virtual void selLanguage(QString sel);

private:
    QString _selectedLanguage;

};

#endif // EDITPREFERENCES_H
