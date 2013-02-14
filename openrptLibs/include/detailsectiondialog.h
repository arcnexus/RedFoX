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

#ifndef DETAILSECTIONDIALOG_H
#define DETAILSECTIONDIALOG_H

class ORGraphicsSectionDetail;

#include <QDialog>

#include "ui_detailsectiondialog.h"

class DetailSectionDialog : public QDialog, public Ui::DetailSectionDialog
{
    Q_OBJECT

public:
    DetailSectionDialog(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~DetailSectionDialog();

    void setReportSectionDetail( ORGraphicsSectionDetail * gsd );

public slots:
    virtual void btnAdd_clicked();
    virtual void btnEdit_clicked();
    virtual void btnRemove_clicked();
    virtual void btnMoveUp_clicked();
    virtual void brnMoveDown_clicked();

protected:
    ORGraphicsSectionDetail * _gsd;

protected slots:
    virtual void languageChange();

};

#endif // DETAILSECTIONDIALOG_H
