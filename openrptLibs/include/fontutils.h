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
*     This is a collection of various functions used to parse the
* different xml structures used in the Report definitions.  Most
* of these are structures that are common to several or the complex
* structures.
*/

#ifndef __FONTUTILS_H__
#define __FONTUTILS_H__

#include <QGraphicsItem>

QString getItemFontFamily(QGraphicsItem *gi);
QString getItemFontSize(QGraphicsItem *gi);
bool getItemFontWeight(QGraphicsItem *gi);
bool getItemFontStyle(QGraphicsItem *gi);

void setItemFontFamily(QGraphicsItem *gi, QString s);
void setItemFontSize(QGraphicsItem *gi, QString s);
void setItemFontWeight(QGraphicsItem *gi, bool v);
void setItemFontStyle(QGraphicsItem *gi, bool v);

#endif
