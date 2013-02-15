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
#ifndef __ORPRERENDER_H__
#define __ORPRERENDER_H__

#include <QSqlDatabase>
#include <QDomDocument>
#include <QRectF>
#include <QString>
#include <QFont>

class ORPreRenderPrivate;
class ParameterList;
class ORODocument;

//
// ORPreRender
// This class takes a report definition and prerenders the result to
// an ORODocument that can be used to pass to any number of renderers.
//
class ORPreRender {
  public:
    ORPreRender(QSqlDatabase = QSqlDatabase());
    ORPreRender(const QDomDocument &, QSqlDatabase = QSqlDatabase());
    ORPreRender(const QDomDocument &, const ParameterList &, QSqlDatabase = QSqlDatabase());
    virtual ~ORPreRender();

    ORODocument * generate();

    void setDatabase(QSqlDatabase);
    QSqlDatabase database() const;

    bool setDom(const QDomDocument &);
    void setParamList(const ParameterList &);
    ParameterList paramList() const;

    bool isValid() const;
    bool doParamsSatisfy() const;

    void setWatermarkText(const QString &);
    void setWatermarkFont(const QFont &);
    void setWatermarkOpacity(unsigned char); // 0..255 : default 25

    QString watermarkText() const;
    QFont watermarkFont() const;
    unsigned char watermarkOpacity() const;

    void setBackgroundImage(const QImage &);
    void setBackgroundRect(const QRectF &);
    void setBackgroundRect(double, double, double, double);
    void setBackgroundOpacity(unsigned char);
    void setBackgroundAlignment(int);
    void setBackgroundScale(bool);
    void setBackgroundScaleMode(Qt::AspectRatioMode mode);

    QImage backgroundImage() const;
    QRectF backgroundRect() const;
    unsigned char backgroundOpacity() const;
    int backgroundAlignment() const;
    bool backgroundScale() const;
    Qt::AspectRatioMode backgroundScaleMode() const;


  protected:

  private:
    ORPreRenderPrivate* _internal;
};


#endif // __ORPRERENDER_H__
