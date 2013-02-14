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

#ifndef __REPORTPAGEOPTIONS_H__
#define __REPORTPAGEOPTIONS_H__

#include <qobject.h>
#include <qstring.h>

class ReportPageOptions : public QObject
{
  Q_OBJECT
  public:
    ReportPageOptions();
    ReportPageOptions(const ReportPageOptions &);

    ReportPageOptions & operator=(const ReportPageOptions &);

    enum PageOrientation {
      Landscape = 0, // essentially false
      Portrait = 1   // and true
    };

    double getMarginTop() const;
    void setMarginTop(double v);
    double getMarginBottom() const;
    void setMarginBottom(double v);
    double getMarginLeft() const;
    void setMarginLeft(double v);
    double getMarginRight() const;
    void setMarginRight(double v);

    const QString & getPageSize() const;
    void setPageSize(const QString & s);
    double getCustomWidth() const;
    void setCustomWidth(double v);
    double getCustomHeight() const;
    void setCustomHeight(double v);

    PageOrientation getOrientation() const;
    bool isPortrait() const;
    void setOrientation(PageOrientation o);
    void setPortrait(bool yes);

    void setLabelType(const QString &);
    const QString & getLabelType() const;

  signals:
    void pageOptionsChanged();

  private:
    double _marginTop;
    double _marginBottom;
    double _marginLeft;
    double _marginRight;

    QString _pagesize;
    double _customWidth;
    double _customHeight;

    PageOrientation _orientation;

    QString _labelType;
};

#endif
