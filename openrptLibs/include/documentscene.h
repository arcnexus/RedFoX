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

#ifndef __DOCUMENTSCENE_H__
#define __DOCUMENTSCENE_H__

#include <QtGui>
#include <QDomDocument>

#include "querylist.h" 
#include "parsexmlutils.h"
#include "graphicssection.h"

class ReportPageOptions;
class ReportGridOptions;
class ReportHandler;

class DocumentScene : public QGraphicsScene
{
  Q_OBJECT

  public:
    DocumentScene(bool newDoc, ReportHandler * handler, QObject * parent = 0);
    ~DocumentScene();

    ReportPageOptions * pageOptions() { return _pageOptions; }

    ReportGridOptions * gridOptions() { return _gridOptions; }
    void setGridOptions(ReportGridOptions * rgo);

    ReportHandler * handler() { return _handler; }

    QuerySourceList * qsList;
    QMap<QString, QColor> _colorMap;

    void sectionEditor(QWidget * parent = 0);
    void colorList(QWidget * parent = 0);
    void docInfoEditor(QWidget * parent = 0);
    void docPageSetup(QWidget * parent = 0);
    void querySourceList(QWidget * parent = 0);

    // Label Definitions
    void editLabelDefinitions(QWidget * parent = 0);

    ORGraphicsSectionItem * getReportHead();
    void removeReportHead();
    void insertReportHead();

    ORGraphicsSectionItem * getReportFoot();
    void removeReportFoot();
    void insertReportFoot();

    ORGraphicsSectionItem * getPageHeadFirst();
    void removePageHeadFirst();
    void insertPageHeadFirst();

    ORGraphicsSectionItem * getPageHeadOdd();
    void removePageHeadOdd();
    void insertPageHeadOdd();

    ORGraphicsSectionItem * getPageHeadEven();
    void removePageHeadEven();
    void insertPageHeadEven();

    ORGraphicsSectionItem * getPageHeadLast();
    void removePageHeadLast();
    void insertPageHeadLast();

    ORGraphicsSectionItem * getPageHeadAny();
    void removePageHeadAny();
    void insertPageHeadAny();

    ORGraphicsSectionItem * getPageFootFirst();
    void removePageFootFirst();
    void insertPageFootFirst();

    ORGraphicsSectionItem * getPageFootOdd();
    void removePageFootOdd();
    void insertPageFootOdd();

    ORGraphicsSectionItem * getPageFootEven();
    void removePageFootEven();
    void insertPageFootEven();

    ORGraphicsSectionItem * getPageFootLast();
    void removePageFootLast();
    void insertPageFootLast();

    ORGraphicsSectionItem * getPageFootAny();
    void removePageFootAny();
    void insertPageFootAny();

    int detailSectionCount();
    ORGraphicsSectionDetail * getSection(int i);
    void insertSection(int idx, ORGraphicsSectionDetail * rsd);
    int findSection(const QString & name);
    void removeSection(int idx, bool del = FALSE);

    void setReportTitle(const QString &);
    void setReportDescription(const QString &);
    void setReportName(const QString &);

    QString reportTitle() const { return _title; }
    QString reportDescription() const { return _descr; }
    QString reportName() const { return _name; }

    bool save(QWidget * parent = 0);
    bool save(QIODevice * iod, QWidget * parent = 0);
    bool saveAs(QWidget * parent = 0);
    bool saveToDb(QWidget * parent = 0);

    QDomDocument document();

    QString filename;
    bool lastSaveToDb;
    QString dbRecordName;
    int dbRecordGrade;

    void addColorDef(QString, QColor);

    // Watermark Properties
    int     watermarkOpacity() const { return _wmOpacity; }
    QFont   watermarkFont() const { return _wmFont; }
    bool    watermarkUseDefaultFont() const { return _wmUseDefaultFont; }
    bool    watermarkUseStaticText() const { return _wmUseStaticText; }
    QString watermarkText() const { return _wmText; }
    QString watermarkColumn() const { return _wmColumn; }
    QString watermarkQuery() const { return _wmQuery; }

    void    setWatermarkOpacity(int);
    void    setWatermarkFont(QFont);
    void    setWatermarkUseDefaultFont(bool);
    void    setWatermarkUseStaticText(bool);
    void    setWatermarkText(QString);
    void    setWatermarkColumn(QString);
    void    setWatermarkQuery(QString);

    // Background Properties
    bool    bgEnabled() const { return _bgEnabled; }
    bool    bgStatic() const { return _bgStatic; }
    QString bgImage() const { return _bgImage; }
    QString bgQuery() const { return _bgQuery; }
    QString bgColumn() const { return _bgColumn; }
    QString bgResizeMode() const { return _bgResizeMode; }
    int     bgAlign() const { return _bgAlign; }
    int     bgBoundsX() const { return _bgBoundsX; }
    int     bgBoundsY() const { return _bgBoundsY; }
    int     bgBoundsWidth() const { return _bgBoundsWidth; }
    int     bgBoundsHeight() const { return _bgBoundsHeight; }
    int     bgOpacity() const { return _bgOpacity; }

    void    setBgEnabled(bool);
    void    setBgStatic(bool);
    void    setBgImage(QString);
    void    setBgQuery(QString);
    void    setBgColumn(QString);
    void    setBgResizeMode(QString);
    void    setBgAlign(int);
    void    setBgBoundsX(int);
    void    setBgBoundsY(int);
    void    setBgBoundsWidth(int);
    void    setBgBoundsHeight(int);
    void    setBgOpacity(int);

    qreal   scaledTo() { return _scaledTo; }
    void    setScaledTo(qreal v) { _scaledTo = v; }

    void addDefinedParameter(const QString &, const ORParameter &);
    void editDefinedParameters(QWidget * parent = 0);

    bool isModified() const { return _modified; }

    void repositionSections();
    ORGraphicsSectionItem * getSection(QPointF scenePos);
	bool undo();
	bool redo();
	void loadDocument(const QDomElement &root, QWidget *parent, bool loadFromfile=true);
	void updateSectionsOfMovedItems(Qt::KeyboardModifiers keyModifiers);
	void setSectionsColors(QGraphicsItem *item, QPointF lastPos, QPointF newPos, Qt::KeyboardModifiers keyModifiers);
	void highlightSections(ORGraphicsSectionItem::Highlight color);

    void checkValidity(QWidget * parent = 0);

  public slots:
    void setModified(bool = true);
    void deleteSelected();
    void refreshFontToolBox();

  signals:
    void modified();

  protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * contextMenuEvent);
    void drawForeground(QPainter * painter, const QRectF & rect);

    ReportPageOptions * _pageOptions;
    ReportGridOptions * _gridOptions;

    QString _title;
    QString _name;
    QString _descr;

    ORGraphicsSectionItem * rptHead;
    ORGraphicsSectionItem * pageHeadFirst;
    ORGraphicsSectionItem * pageHeadOdd;
    ORGraphicsSectionItem * pageHeadEven;
    ORGraphicsSectionItem * pageHeadLast;
    ORGraphicsSectionItem * pageHeadAny;

    ORGraphicsSectionItem * pageFootFirst;
    ORGraphicsSectionItem * pageFootOdd;
    ORGraphicsSectionItem * pageFootEven;
    ORGraphicsSectionItem * pageFootLast;
    ORGraphicsSectionItem * pageFootAny;
    ORGraphicsSectionItem * rptFoot;

    QList<ORGraphicsSectionDetail*> detailList;
    QMap<QString,ORParameter> _definedParams;

    // Watermark variables
    bool    _wmUseStaticText;
    QString _wmText;
    QString _wmColumn;
    QString _wmQuery;
    bool    _wmUseDefaultFont;
    QFont   _wmFont;
    int     _wmOpacity;

    // background variables
    bool    _bgEnabled;
    bool    _bgStatic;
    QString _bgImage;
    QString _bgQuery;
    QString _bgColumn;
    QString _bgResizeMode;
    int     _bgAlign;
    int     _bgBoundsX;
    int     _bgBoundsY;
    int     _bgBoundsWidth;
    int     _bgBoundsHeight;
    int     _bgOpacity;


    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

  protected slots:
    virtual void pageOptionsChanged();

  private:

	void initData();
	void clear();
	void recordSnapshot();

	QList<QDomDocument> _snapshots;
	int					_undoIndex;

    QGraphicsRectItem * _page;
    QGraphicsRectItem * _pageShadow;
    QGraphicsRectItem * _pageMargin;
    qreal               _scaledTo;

	ReportHandler * _handler;
	QWidget * _parentWindow;
    bool _modified;
	int _maxSnaphots;
	bool _loadingInProgress;
};

#endif

