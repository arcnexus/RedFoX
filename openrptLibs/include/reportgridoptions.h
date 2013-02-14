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

#ifndef __REPORTGRIDOPTIONS_H__
#define __REPORTGRIDOPTIONS_H_

// qt
#include <qobject.h>

// forward declarations
class QPoint;
class QPointF;


class ReportGridOptions : public QObject {
    Q_OBJECT
    public:
        ReportGridOptions(int, int, QObject* = 0, const char* = 0);

        bool isVisible();
        bool isSnap();

        double xInterval();
        double yInterval();

        QPointF snapPoint(const QPointF & pos);

    public slots:
        void setVisible(bool yes = TRUE);
        void show();
        void hide();
        void setXInterval(double i);
        void setYInterval(double i);
        void setSnap(bool yes = TRUE);

    signals:
        void gridOptionsChanged();

    private:
        bool show_grid;
        bool snap_grid;

        double x_interval;
        double y_interval;

        int _realDpiX;
        int _realDpiY;
};


#endif

