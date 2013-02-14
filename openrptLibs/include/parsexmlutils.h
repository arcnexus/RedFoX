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

#ifndef __PARSEXMLUTILS_H__
#define __PARSEXMLUTILS_H__

// forward declarations
class QDomElement;
class QDomNode;

#include "querysource.h"
#include "reportpageoptions.h"

#include <QString>
#include <QRect>
#include <QFont>
#include <QColor>
#include <QMap>
#include <QList>
#include <QPair>
#include <QPen>
#include <QBrush>
#include <QSqlDatabase>

//
// Data Structures
//
class ORDataData
{
  public:
    ORDataData()
    {
      query = QString::null;
      column = QString::null;
    }
    ORDataData(const QString & q, const QString & c)
    {
      query = q;
      column = c;
    }
    ORDataData(const ORDataData & d)
    {
      query = d.query;
      column = d.column;
    }

    ORDataData & operator=(const ORDataData & d)
    {
      query = d.query;
      column = d.column;
      return *this;
    }

    bool operator==(const ORDataData & d) const
    {
      return ((query == d.query) && (column == d.column));
    }

    bool operator< (const ORDataData & d) const
    {
      if((query < d.query) || (query == d.query && column < d.column))
        return true;
      return false;
    }

    QString query;
    QString column;
};
class ORKeyData
{
  public:
    QString query;
    QString column;
};

class ORColorDefData
{
  public:
    QString name;
    int red, green, blue;
};
class ORTitleData
{
  public:
    QString string;
    QFont font;
    bool font_defined;
};
class ORStyleData
{
  public:
    bool bar;
    bool line;
    bool point;
};
class ORDataAxisData
{
  public:
    ORTitleData title;
    QString column;
    QFont font;
    bool font_defined;
};
class ORValueAxisData
{
  public:
    ORTitleData title;
    double min;
    double max;
    bool autominmax;
    QFont font;
    bool font_defined;
};

class ORCrossTabQueryData
{
public:
  QString m_query;
  QString m_hAlign;
  QString m_vAlign;
};

class ORCrossTabTablePropertiesData
{
public:
  bool   m_wrapPolicyColumnsFirst;

  bool   m_showColumnHeaderOnEachPart;
  bool   m_showRowHeaderOnEachPart;

  double m_cellLeftMargin;
  double m_cellRightMargin;
  double m_cellTopMargin;
  double m_cellBottomMargin;
};

class ORSeriesData
{
  public:
    QString name;
    QString color; 
    QString column;
    ORStyleData style;
};


class ORLineData;
class ORRectData;
class ORLabelData;
class ORFieldData;
class ORTextData;
class ORBarcodeData;
class ORImageData;
class ORGraphData;
class ORCrossTabData;

class ORObject
{
  public:
    ORObject();
    virtual ~ORObject();

    virtual bool isLine();
    virtual ORLineData * toLine();
    virtual bool isRect();
    virtual ORRectData * toRect();
    virtual bool isLabel();
    virtual ORLabelData * toLabel();
    virtual bool isField();
    virtual ORFieldData * toField();
    virtual bool isText();
    virtual ORTextData * toText();
    virtual bool isBarcode();
    virtual ORBarcodeData * toBarcode();
    virtual bool isImage();
    virtual ORImageData * toImage();
    virtual bool isGraph();
    virtual ORGraphData * toGraph();
    virtual bool isCrossTab();
    virtual ORCrossTabData * toCrossTab();

    QPen pen() const {return _pen;}
    void setPen(QPen p) {_pen = p;}

    QBrush brush() const {return _brush;}
    void setBrush(QBrush b) {_brush = b;}

	qreal rotation() const { return _rotation; }
	void setRotation(qreal angle) { _rotation = angle;}

private:
    QPen    _pen;
    QBrush  _brush;
	qreal   _rotation;
};

class ORLineData : public ORObject
{
  public:
    int xStart, yStart;
    int xEnd,   yEnd;
    int weight;

    virtual bool isLine();
    virtual ORLineData * toLine();
};

class ORRectData : public ORObject
{
public:
    int x, y;
    int width,  height;
    int weight;

    virtual bool isRect();
    virtual ORRectData * toRect();
};

class ORLabelData : public ORObject
{
  public:
    QRect rect;
    QFont font;
    int align;
    QString string;

    virtual bool isLabel();
    virtual ORLabelData * toLabel();
};

class ORFieldData : public ORObject
{
  public:
    QRect rect;
    QFont font;
    int align;
    ORDataData data;

    bool trackTotal;
    bool sub_total;
    bool builtinFormat;
    QString format;

    int lines;
    int columns;
    qreal xSpacing;
    qreal ySpacing;
    bool triggerPageBreak;
    bool leftToRight;

    virtual bool isField();
    virtual ORFieldData * toField();
};

class ORTextData : public ORObject
{
  public:
    QRect rect;
    QFont font;
    int align;
    ORDataData data;
    int bottompad;

    virtual bool isText();
    virtual ORTextData * toText();
};

class ORBarcodeData : public ORObject
{
  public:
    QRect rect;
    QString format;
    int maxlength;
    ORDataData data;
    int align; // 0 = left, 1 = center, 2 = right
    double narrowBarWidth;

    virtual bool isBarcode();
    virtual ORBarcodeData * toBarcode();

    static double defaultNarrowBarWidth() { return 0.01; }
};

class ORImageData : public ORObject
{
  public:
    QRect rect;

    QString mode;

    QString format;      // } 
    QString inline_data; // } INLINE
                         //     OR
    ORDataData data;     // } FROM DB

    virtual bool isImage();
    virtual ORImageData * toImage();
};

class ORGraphData : public ORObject
{
  // TODO need a destructor that can delete the series entries
  public:
	  ~ORGraphData() {qDeleteAll(series);}

    ORDataData data;

    QFont font;
    QRect rect;

    ORTitleData title;
    ORDataAxisData dataaxis;
    ORValueAxisData valueaxis;

    QList<ORSeriesData*> series;

    virtual bool isGraph();
    virtual ORGraphData * toGraph();
};


class ORCrossTabData : public ORObject
{
  public:
    ORDataData data;

    QFont font;
    QRect rect;

    // Table properties
    ORCrossTabTablePropertiesData m_tableProperties;

    //Data
    ORCrossTabQueryData m_column;
    ORCrossTabQueryData m_row;
    ORCrossTabQueryData m_value;

    virtual bool isCrossTab();
    virtual ORCrossTabData * toCrossTab();
};


//
// ORWatermarkData
//
class ORWatermarkData
{
  public:
    ORWatermarkData();

    int opacity;
    bool useDefaultFont;
    QFont font;
    bool staticText;
    QString text;
    ORDataData data;

    bool valid;
};

//
// ORBackgroundData
//
class ORBackgroundData
{
  public:
    ORBackgroundData();

    bool enabled;
    bool staticImage;
    QString image;
    ORDataData data;
    int opacity;
    QString mode;
    int align;
    QRect rect;
};

//
// ORSectionData is used to store the information about a specific
// section. A section has a name and optionally extra data. `name'
// rpthead, rptfoot, pghead, pgfoot, grphead, grpfoot or detail.
// In the case of pghead and pgfoot extra would contain the page
// designation (firstpage, odd, even or lastpage).
//
class ORSectionData
{
  public:
	  ~ORSectionData() {qDeleteAll(objects);}

    QString name;
    QString extra; // extra info about the section
    qreal height;

    QList<ORObject*> objects;
    QList<ORDataData> trackTotal;
};

class ORDetailGroupSectionData
{
  public:
    ORDetailGroupSectionData();
    ~ORDetailGroupSectionData();

    enum PageBreak {
      BreakNone = 0,
      BreakAfterGroupFoot = 1
    };

    QString name;
    QString column;
    int pagebreak;

    QMap<ORDataData,double> _subtotCheckPoints;

    ORSectionData * head;
    ORSectionData * foot;
};

class ORDetailSectionData
{
  public:
    ORDetailSectionData();
	~ORDetailSectionData();

    enum PageBreak {
      BreakNone = 0,
      BreakAtEnd = 1
    };

    QString name;
    int pagebreak;

    ORKeyData key;

    ORSectionData * detail;

    QList<ORDetailGroupSectionData*> groupList;
    QList<ORDataData> trackTotal;
};

class ORParameter
{
  public:
    ORParameter() : active(false) {};
    ORParameter(const QString & n) : name(n), active(false) {}
    ORParameter(const ORParameter & d)
    {
      name = d.name;
      type = d.type;
      defaultValue = d.defaultValue;
      description = d.description;
      listtype = d.listtype;
      query = d.query;
      values = d.values;
      active = d.active;
    }

    ORParameter & operator=(const ORParameter & d)
    {
      name = d.name;
      type = d.type;
      defaultValue = d.defaultValue;
      description = d.description;
      listtype = d.listtype;
      query = d.query;
      values = d.values;
      active = d.active;
      return *this;
    }

    QString name;
    QString type;
    QString defaultValue;
    QString description;
    QString listtype;
    QString query;
    QList<QPair<QString,QString> > values;
    bool active;
};

class ORReportData
{
  public:
    ORReportData();
        ~ORReportData();

    QString title;
    QString name;
    QString description;

    QMap<QString,ORParameter> definedParams;

    ORWatermarkData wmData;
    ORBackgroundData bgData;

    ReportPageOptions page;
    QuerySourceList queries;

    ORSectionData * pghead_first;
    ORSectionData * pghead_odd;
    ORSectionData * pghead_even;
    ORSectionData * pghead_last;
    ORSectionData * pghead_any;

    ORSectionData * rpthead;
    ORSectionData * rptfoot;

    ORSectionData * pgfoot_first;
    ORSectionData * pgfoot_odd;
    ORSectionData * pgfoot_even;
    ORSectionData * pgfoot_last;
    ORSectionData * pgfoot_any;

    QList<ORDetailSectionData*> sections;
    QMap<QString, QColor> color_map;
    QList<ORDataData> trackTotal;
};


bool parseReportRect(const QDomElement & elemSource, QRect &, ORObject &);
bool parseReportRect(const QDomElement &, QRect &);
bool parseReportFont(const QDomElement &, QFont &);

QColor parseColor(const QDomNode &n);

bool parseReportData(const QDomElement &, ORDataData &);
bool parseReportKey(const QDomElement &, ORKeyData &);

bool parseReportLine(const QDomElement &, ORLineData &);
bool parseReportLabel(const QDomElement &, ORLabelData &);
bool parseReportField(const QDomElement &, ORFieldData &);
bool parseReportText(const QDomElement &, ORTextData &);

bool parseReportBarcode(const QDomElement &, ORBarcodeData &);
bool parseReportImage(const QDomElement &, ORImageData &);

bool parseReportWatermark(const QDomElement &, ORWatermarkData &);
bool parseReportBackground(const QDomElement &, ORBackgroundData &);

bool parseReportColorDefData(const QDomElement &, ORColorDefData &);

bool parseReportGraphData(const QDomElement &, ORGraphData &);

bool parseReportCrossTabData(const QDomElement &, ORCrossTabData &);

bool parseReportTitleData(const QDomElement &, ORTitleData &);
bool parseReportStyleData(const QDomElement &, ORStyleData &);
bool parseReportDataAxisData(const QDomElement &, ORDataAxisData &);
bool parseReportValueAxisData(const QDomElement &, ORValueAxisData &);
bool parseReportSeriesData(const QDomElement &, ORSeriesData &);

bool parseReportSection(const QDomElement &, ORSectionData &);
bool parseReportDetailSection(const QDomElement &, ORDetailSectionData &);
bool parseReport(const QDomElement &, ORReportData &, QSqlDatabase db);

bool parseReportParameter(const QDomElement &, ORReportData &);

#endif
