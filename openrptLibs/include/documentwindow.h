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

#ifndef __DOCUMENTWINDOW_H__
#define __DOCUMENTWINDOW_H__

#include <QtGui>

class DocumentScene;
class DocumentView;
class ReportGridOptions;
class ReportHandler;

//
// Class DocumentWindow
//
class DocumentWindow : public QMainWindow
{  
  Q_OBJECT

  friend class ReportHandler;

  public:
    DocumentWindow(bool newDoc, ReportGridOptions * rgo, ReportHandler * handler, QWidget * parent);

  public slots:
    virtual void setCaption();

  protected:
    virtual void closeEvent(QCloseEvent * e);

	ReportHandler * _handler;
    DocumentScene * _scene;
    DocumentView * _view;
};

#endif

