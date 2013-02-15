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

/*
 *     This file contains the class definitions used for rendering reports
 * and is based largely on top of the openReports.h file done by Jeffrey Lyon.
 */

#ifndef __ORRENDERER_H__
#define __ORRENDERER_H__

class orReportPrivate;
class QWidget;
class ParameterList;

#include <qstringlist.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qimage.h>
#include <qfont.h>
#include <qdom.h>
#include <QSqlDatabase>


class orReport {
  private:
    void constructor(const QString &);

    orReportPrivate *_internal;
    static QPrinter* multiPrinter;
    static QPainter* multiPainter;

  public:
    orReport(QSqlDatabase = QSqlDatabase());
    orReport(const QString &, QSqlDatabase = QSqlDatabase());
    orReport(const QString &, const QStringList &, QSqlDatabase = QSqlDatabase());
    orReport(const char *, const ParameterList &, QSqlDatabase = QSqlDatabase());
    orReport(const QString &, const ParameterList &, QSqlDatabase = QSqlDatabase());
    ~orReport();

    bool    render(QPainter *, QPrinter * = 0);
    bool    print(QPrinter *prtThis = 0, bool boolSetupPrinter = TRUE, bool showPreview = false);
    bool    exportToPDF( const QString& fileName );

    static bool    beginMultiPrint(QPrinter *);
    static bool    beginMultiPrint(QPrinter *, bool & userCanceled);
    static bool    endMultiPrint(QPrinter *);

    void    setWatermarkText(const QString &);
    void    setWatermarkFont(const QFont &);
    void    setWatermarkOpacity(unsigned char);      // 0..255 : default 25

    QString watermarkText();
    QFont   watermarkFont();
    unsigned char watermarkOpacity();

    void    setBackgroundImage(const QImage &);
    void    setBackgroundRect(const QRect &);
    void    setBackgroundRect(int, int, int, int); 
    void    setBackgroundOpacity(unsigned char);     // 0..255 : default 25
    void    setBackgroundAlignment(int);             // Qt::AlignmentFlags
    void    setBackgroundScale(bool);
    void    setBackgroundScaleMode(Qt::AspectRatioMode);

    QImage  backgroundImage();
    QRect   backgroundRect();
    unsigned char backgroundOpacity();
    int     backgroundAlignment();
    bool    backgroundScale();
    Qt::AspectRatioMode backgroundScaleMode();
    
    void    setDatabase(QSqlDatabase);

    bool    setDom(const QDomDocument &docPReport);
    void    setParamList(const QStringList &);
    void    setParamList(const ParameterList &);
    ParameterList getParamList();
    bool    isValid();
    bool    doesReportExist();
    bool    doParamsSatisfy();
    bool    satisfyParams(QWidget *);
    int     reportError(QWidget *);
};

#endif

