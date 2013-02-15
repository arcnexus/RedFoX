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

#ifndef __GRAPHICSITEMS_H__
#define __GRAPHICSITEMS_H__

#include <QtGui>
#include <QDomDocument>

#include <parsexmlutils.h>




class ORResizeHandle : public QGraphicsRectItem
{
  public:
    enum { Type = UserType + 99 };
    enum Role {
      TopLeft = 0,
      Top,
      TopRight,
      Right,
      BottomRight,
      Bottom,
      BottomLeft,
      Left,
      StartLine,
      EndLine
    };

    ORResizeHandle(enum Role role, QGraphicsItem * parent);

    void reposition();

    int type() const { return Type; }

  protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	void		 mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  private:
    Role _role;
	bool _moved;
};

class ORGraphicsRectItem : public QGraphicsRectItem
{
  public:
    enum { Type = UserType + 100 };

    ORGraphicsRectItem(QGraphicsItem * parent = 0);
    ORGraphicsRectItem(const QDomNode &, QGraphicsItem * parent = 0);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    virtual void buildXML(QDomDocument & doc, QDomElement & parent);

    int type() const { return Type; }

    static void buildXML(QGraphicsItem * item, QDomDocument & doc, QDomElement & parent);
    static void buildXMLFont(QDomDocument &, QDomElement &, QFont);

    static QFont getDefaultEntityFont();
    static void  setDefaultEntityFont(const QFont &);

    virtual void properties(QWidget * parent = 0);

	qreal rotation() const { return _rotation; }
	void setRotation(qreal angle); 

  protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void buildXMLCommon(QDomDocument & doc, QDomElement & entity);
    void parseRect(const QDomNode & entity);

  private:
    ORResizeHandle *_rhTopLeft;
    ORResizeHandle *_rhTop;
    ORResizeHandle *_rhTopRight;
    ORResizeHandle *_rhRight;
    ORResizeHandle *_rhBottomRight;
    ORResizeHandle *_rhBottom;
    ORResizeHandle *_rhBottomLeft;
    ORResizeHandle *_rhLeft;
	qreal			_rotation;

	bool			_moved;
    static bool _readDefaultFont;
    static QFont _defaultFont;
};

class ORGraphicsLineItem : public QGraphicsLineItem
{
  public:
    enum { Type = UserType + 104 };

    ORGraphicsLineItem(QGraphicsItem * parent = 0);
    ORGraphicsLineItem(const QDomNode &, QGraphicsItem * parent = 0);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void buildXML(QDomDocument & doc, QDomElement & parent);

    int type() const { return Type; }

    virtual void properties(QWidget * parent = 0);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;

  protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

  private:
    ORResizeHandle * _rhStartPoint;
    ORResizeHandle * _rhEndPoint;
	bool			_moved;
};

class ORGraphicsLabelItem : public ORGraphicsRectItem
{
  public:
    enum { Type = UserType + 101 };

    ORGraphicsLabelItem(QGraphicsItem * parent = 0);
    ORGraphicsLabelItem(const QDomNode &, QGraphicsItem * parent = 0);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void buildXML(QDomDocument & doc, QDomElement & parent);

    int type() const { return Type; }

    // Label specific methods
    void setText(const QString& text);
    void setFont(const QFont& font);
    void setTextFlags(int flags);

    QString text() const { return _txt; }
    QFont font() const { return _font; }
    int textFlags() const { return _flags; }

    virtual void properties(QWidget * parent = 0);

  protected:
    // label specific data
    QString _txt;
    int     _flags;
    QFont   _font;
};


class ORGraphicsFieldItem : public ORGraphicsRectItem
{
  public:
    enum { Type = UserType + 102 };

    ORGraphicsFieldItem(QGraphicsItem * parent = 0);
    ORGraphicsFieldItem(const QDomNode &, QGraphicsItem * parent = 0);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QRectF boundingRect () const;

    void buildXML(QDomDocument & doc, QDomElement & parent);

    int type() const { return Type; }

    // Field specific methods
    void setQuery(const QString&);
    void setColumn(const QString&);
    void setFont(const QFont&);
    void setTextFlags(int);
    void setTrackTotal(bool);
    void setFormat(const QString &, bool=FALSE);
    void setUseSubTotal(bool);
    void setArray(int lines, int columns, double xSpacing, double ySpacing, bool pageBreak, bool leftToRight);

    QString query() const { return _qry; }
    QString column() const { return _clmn; }
    QFont font() const { return _font; }
    bool trackTotal() const { return _trackTotal; }
    bool trackBuiltinFormat() const { return _trackBuiltinFormat; }
    bool useSubTotal() const { return _useSubTotal; }
    QString format() const { return _format; }

    int textFlags() const { return _flags; }

    int lines() const { return _lines; }
    int columns() const { return _columns; }
    qreal xSpacing() const  { return _xSpacing; }
    qreal ySpacing() const  { return _ySpacing; }
    qreal triggerPageBreak() const  { return _triggerPageBreak; }
    qreal leftToRight() const  { return _leftToRight; }

    virtual void properties(QWidget * parent = 0);

  protected:
    // Field specific data
    QString _qry;
    QString _clmn;
    int     _flags;
    QFont   _font;
    bool    _trackTotal;
    bool    _trackBuiltinFormat;
    bool    _useSubTotal;
    QString _format;
    int     _lines;
    int     _columns;
    qreal   _xSpacing;
    qreal   _ySpacing;
    bool    _triggerPageBreak;
    bool    _leftToRight;
};


class ORGraphicsTextItem : public ORGraphicsRectItem
{
  public:
    enum { Type = UserType + 103 };

    ORGraphicsTextItem(QGraphicsItem * parent = 0);
    ORGraphicsTextItem(const QDomNode &, QGraphicsItem * parent = 0);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void buildXML(QDomDocument & doc, QDomElement & parent);

    int type() const { return Type; }

    // Text specific methods
    void setBottomPadding(double bp);
    void setQuery(const QString&);
    void setColumn(const QString&);
    void setFont(const QFont&);
    void setTextFlags(int);

    double bottomPadding() const { return _bpad; }
    QString query() const { return _qry; }
    QString column() const { return _clmn; }
    QFont font() const { return _font; }
    int textFlags() const { return _flags; }

    virtual void properties(QWidget * parent = 0);

  protected:
    // Text specific data
    double _bpad;
    QString _qry;
    QString _clmn;
    int     _flags;
    QFont   _font;
};


class ORGraphicsBarcodeItem : public ORGraphicsRectItem
{
  public:
    enum { Type = UserType + 105 };

    ORGraphicsBarcodeItem(QGraphicsItem * parent = 0);
    ORGraphicsBarcodeItem(const QDomNode &, QGraphicsItem * parent = 0);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void buildXML(QDomDocument & doc, QDomElement & parent);

    int type() const { return Type; }

    // Barcode specific Methods
    void setFormat(const QString&);
    void setQuery(const QString&);
    void setColumn(const QString&);
    void setMaxLength(int i);
    void setAlignment(int i);
    void setNarrowBarWidth(double bw);

    QString format() const { return _frmt; }
    QString query() const { return _qry; }
    QString column() const { return _clmn; }
    int maxLength() const { return _maxlength; }
    int alignment() const { return _align; }
    double narrowBarWidth() const { return _narrowBarWidth; }

    virtual void properties(QWidget * parent = 0);

  protected:
    // Barcode specific data
    QString _qry;
    QString _clmn;
    QString _frmt;
    int _maxlength;
    int _align;
    double _narrowBarWidth;

  private:
    // all these values are in inches and
    // are for internal use only
    double _min_width_data;
    double _min_width_total;
    double _min_height;
};


class ORGraphicsImageItem : public ORGraphicsRectItem
{
  public:
    enum { Type = UserType + 106 };

    ORGraphicsImageItem(QGraphicsItem * parent = 0);
    ORGraphicsImageItem(const QDomNode &, QGraphicsItem * parent = 0);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void buildXML(QDomDocument & doc, QDomElement & parent);

    int type() const { return Type; }

    // Image specific methods
    void setInline(bool);
    void setInlineImageData(QString);
    void setQuery(const QString&);
    void setColumn(const QString&);
    void setMode(QString);

    bool isInline() const { return _img_inline; }
    QString inlineImageData() const { return img_data; }
    QString query() const { return _qry; }
    QString column() const { return _clmn; }
    QString mode() const { return _mode; }

    virtual void properties(QWidget * parent = 0);

  protected:
    // Image specific data
    bool _img_inline;
    QString _qry;
    QString _clmn;
    QString _mode;
    QImage  img;
    QString img_data;
    QString _frmt;
};


class ORGraphicsGraphItem : public ORGraphicsRectItem
{
  public:
    enum { Type = UserType + 107 };

    ORGraphicsGraphItem(QGraphicsItem * parent = 0);
    ORGraphicsGraphItem(const QDomNode &, QGraphicsItem * parent = 0);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void buildXML(QDomDocument & doc, QDomElement & parent);

    int type() const { return Type; }

    // Graph specific methods
    void setData(const ORGraphData &);
    void copyData(ORGraphData &);

    void setQuery(const QString &);
    QString query() const;

    virtual void properties(QWidget * parent = 0);

  protected:
    // Graph specific data
    ORGraphData _graphData;
};


class ORGraphicsCrossTabItem : public ORGraphicsRectItem
{
  public:
    enum { Type = UserType + 108 };

    ORGraphicsCrossTabItem(QGraphicsItem * parent = 0);
    ORGraphicsCrossTabItem(const QDomNode &, QGraphicsItem * parent = 0);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
 
    void buildXML(QDomDocument & doc, QDomElement & parent);

    int type() const { return Type; }

    // CrossTab specific methods
    void setQuery(const QString&);
    void setColumn(const QString&, const QString&);
    void setFont(const QFont&);
    void setHAlign(const QString&, const QString&);
    void setVAlign(const QString&, const QString&);
    void setTableWrapping(const bool& policyColumnsFirst);
    void setCellMargins(const double& left, const double& right,
                        const double& top,  const double& bottom);
    void setDisplayHeaderColumn(const bool& display);
    void setDisplayHeaderRow(const bool& display);

    QString query() const { return m_query; }
    QString column(const QString & type) const { return m_queryColumn.value(type); }
    QFont font() const { return m_font; }
    QString HAlign(const QString& type) const { return m_hAlign.value(type); }
    QString VAlign(const QString& type) const { return m_vAlign.value(type); }
    bool tableWrapping() const { return m_tableWrapPolicyColumnsFirst; }
    void cellMargins(double& left, double& right,
                     double& top,  double& bottom) const
         { left = m_leftMargin; right = m_rightMargin;
           top = m_topMargin; bottom = m_bottomMargin; }
    bool displayHeaderColumn() const { return m_columnHeaderOnEachPart; }
    bool displayHeaderRow() const { return m_rowHeaderOnEachPart; }

    virtual void properties(QWidget * parent = 0);

  protected:
    // CrossTab specific data
    QString                     m_query;
    QMultiMap<QString,QString>  m_queryColumn;
    QMap<QString,QString>       m_hAlign;
    QMap<QString,QString>       m_vAlign;
    QFont                       m_font;
    bool                        m_tableWrapPolicyColumnsFirst;
    bool                        m_rowHeaderOnEachPart;
    bool                        m_columnHeaderOnEachPart;
    double                      m_leftMargin;
    double                      m_rightMargin;
    double                      m_topMargin;
    double                      m_bottomMargin;

    void setTableRect();
    QRectF tableRect();
};


#endif

