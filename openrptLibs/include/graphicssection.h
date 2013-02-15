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

#ifndef __GRAPHICSSECTION_H__
#define __GRAPHICSSECTION_H__

#include <QtGui>
#include <QDomDocument>
#include <QList>

#include <parsexmlutils.h>

class ORGraphicsSectionItem;
class DocumentScene;

class ORSectionHandle : public QGraphicsRectItem
{
  public:
    enum { Type = UserType + 49 };

    ORSectionHandle(QGraphicsItem * parent = 0);

    int type() const { return Type; }

  protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
};

class ORGraphicsSectionItem : public QGraphicsRectItem
{
  public:
    enum { Type = UserType + 50 };
    enum Highlight {
      Normal = 0,
      Origin,
      Destination
    };

    ORGraphicsSectionItem(QGraphicsItem * parent = 0);

    void setTitle(const QString & s);

    void buildXML(QDomDocument & doc, QDomElement & section);
    void initFromXML(QDomNode & section);

    int type() const { return Type; }

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    void highlight(enum Highlight mode);

  protected:
    //void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

    qreal _previousWidth;

  private:
    ORSectionHandle * _handle;
    QGraphicsSimpleTextItem * _title;
};

class ORGraphicsSectionDetail;
class ORGraphicsSectionDetailGroup : public QObject
{
  Q_OBJECT

  public:
    ORGraphicsSectionDetailGroup(const QString &, ORGraphicsSectionDetail *, QObject * parent);
    ~ORGraphicsSectionDetailGroup();

    enum PageBreak
    {
      BreakNone = 0,
      BreakAfterGroupFooter = 1
    };

    void setTitle(const QString & s);
    void setColumn(const QString &);
    void showGroupHead(bool yes = TRUE);
    void showGroupFoot(bool yes = TRUE);
    void setPageBreak(int);

    QString getTitle() const { return _name; }
    QString column() const { return _column; }
    bool isGroupHeadShowing() const;
    bool isGroupFootShowing() const;
    int  pageBreak() const { return _pagebreak; }

    ORGraphicsSectionItem * getGroupHead() { return _head; }
    ORGraphicsSectionItem * getGroupFoot() { return _foot; }

  protected:
    QString _name;
    QString _column;

    ORGraphicsSectionItem * _head;
    ORGraphicsSectionItem * _foot;

    ORGraphicsSectionDetail * _rsd;

    int _pagebreak;
};

class ORGraphicsSectionDetail : public QObject
{
  Q_OBJECT

  public:
    ORGraphicsSectionDetail(DocumentScene * rptwin, QObject * parent);
    ~ORGraphicsSectionDetail();

    enum PageBreak
    {
      BreakNone = 0,
      BreakAtEnd = 1
    };

    void setTitle(const QString & s);
    void setQuery(const QString &);
    void setPageBreak(int);

    QString getTitle() const { return _name; }
    QString query() const { return _query; }
    int pageBreak() const { return _pagebreak; }

    ORGraphicsSectionItem * getDetail() { return _detail; }
    DocumentScene * reportWindow() { return _rw; }


    void buildXML(QDomDocument & doc, QDomElement & section);
    void initFromXML(QDomNode & node);

    int groupSectionCount() const;
    ORGraphicsSectionDetailGroup * getSection(int i);
    void insertSection(int idx, ORGraphicsSectionDetailGroup * rsd);
    int findSection(const QString & name);
    void removeSection(int idx, bool del = FALSE);

    void adjustSize();

  protected:
    QString _query;

    QString _name;
    ORGraphicsSectionItem * _detail;
    DocumentScene * _rw;

    QList<ORGraphicsSectionDetailGroup*> groupList;

    int _pagebreak;
};

#endif

