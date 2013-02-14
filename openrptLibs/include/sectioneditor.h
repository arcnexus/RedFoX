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

#ifndef SECTIONEDITOR_H
#define SECTIONEDITOR_H

class DocumentScene;

#include <QDialog>

#include "ui_sectioneditor.h"

class SectionEditor : public QDialog, public Ui::SectionEditor
{
    Q_OBJECT

public:
    SectionEditor(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~SectionEditor();

    void init( DocumentScene * scene );

public slots:
    virtual void btnAdd_clicked();
    virtual void btnEdit_clicked();
    virtual void btnRemove_clicked();
    virtual void btnMoveUp_clicked();
    virtual void brnMoveDown_clicked();
    virtual void cbReportHeader_toggled( bool yes );
    virtual void cbReportFooter_toggled( bool yes );
    virtual void cbHeadFirst_toggled( bool yes );
    virtual void cbHeadLast_toggled( bool yes );
    virtual void cbHeadEven_toggled( bool yes );
    virtual void cbHeadOdd_toggled( bool yes );
    virtual void cbFootFirst_toggled( bool yes );
    virtual void cbFootLast_toggled( bool yes );
    virtual void cbFootEven_toggled( bool yes );
    virtual void cbFootOdd_toggled( bool yes );
    virtual void cbHeadAny_toggled( bool yes );
    virtual void cbFootAny_toggled( bool yes );

protected:
    DocumentScene * scene;

protected slots:
    virtual void languageChange();

};

#endif // SECTIONEDITOR_H
