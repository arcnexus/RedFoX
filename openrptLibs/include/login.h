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

#ifndef __LOGIN_H__
#define __LOGIN_H__

class QSplashScreen;

#include <QDialog>

#include "parameter.h"

#include "C:\openrpt-build-Desktop_Qt_4_8_2_in_PATH_4_8_2-Debug\common\tmp/ui_login.h"

class login : public QDialog, public Ui::login
{
    Q_OBJECT

  public:
    login(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~login();

    QString _databaseURL;

    virtual int set( const ParameterList & pParams, QSplashScreen * pSplash );
    virtual void populateDatabaseInfo();
    virtual QString username();
    virtual QString password();

  public slots:
    virtual int set( const ParameterList & pParams );

  protected slots:
    virtual void languageChange();

    virtual void sLogin();
    virtual void sOptions();


  private:
    bool _captive;
    QSplashScreen *_splash;
    QString _cUsername;
    QString _cPassword;
};

#endif
