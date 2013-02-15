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

#ifndef PAGESETUP_H
#define PAGESETUP_H

#include <QDialog>
#include <QString>

#include "reportpageoptions.h"
#include "ui_pagesetup.h"

class PageSetup : public QDialog, public Ui::PageSetup
{
    Q_OBJECT

public:
    PageSetup(QWidget* parent = 0);
    ~PageSetup();

    static const char strLetter[];
    static const char strLegal[];
    static const char strA4[];
    static const char strCustom[];
    static const char strLabel[];

    void            setData(const ReportPageOptions& rpo);
    void            getData(ReportPageOptions& rpo);

public slots:
    void            onPaperSize(int idx);

protected slots:
    virtual void languageChange();

private:
    virtual void enableCustom( bool yes );
};

#endif // PAGESETUP_H
