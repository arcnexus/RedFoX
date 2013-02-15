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
 *     A central class that acts as the central pivot to
 * the rest of the Report Writer controls. It contains the
 * menu and toolbar code/connections as well as the mouse
 * handling events.
 */

#ifndef __REPORTHANDLER_H__
#define __REPORTHANDLER_H__

#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QPoint>
#include <QList>
#include <QActionGroup>
#include <QMouseEvent>
#include <QFontComboBox>
#include <QPushButton>
#include <QToolBar>

#include <parameter.h>

class QWidget;
class QMouseEvent;
class QMainWindow;
class QMenuBar;
class QAction;

class ReportGridOptions;
class ReportWriterSectionData;
class DocumentWindow;

class ReportHandler : public QObject {
    Q_OBJECT
    public:
        ReportHandler(QObject * parent = 0, const char * name = 0);

        virtual ~ReportHandler();

        void docToolBars(QMainWindow * mw, int edge = 0, bool newLine = FALSE);
        QAction * populateMenuBar(QMenuBar * menubar, QAction * exitAction = 0);

        void setParentWindow(QWidget * w);

        bool placeMenusOnWindows() { return _placeMenusOnWindows; }
        void setPlaceMenusOnWindows(bool b) { _placeMenusOnWindows = b; }
        bool placeToolbarsOnWindows() { return _placeToolbarsOnWindows; }
        void setPlaceToolbarsOnWindows(bool b) { _placeToolbarsOnWindows = b; }
        bool allowDBConnect() { return _allowDBConnect; }
        void setAllowDBConnect(bool b) { _allowDBConnect = b; }
        bool noDatabase() { return _noDatabase; }
        void setNoDatabase(bool b) { _noDatabase = b; }

        QString name();
        QString version();
        QString copyright();

    public slots:
        DocumentWindow* fileNew();
        DocumentWindow* createDocument(bool newDoc);
        void fileOpen();
        void fileOpen(const QString &);
        DocumentWindow* fileOpen(const QDomDocument &);
        DocumentWindow* fileOpen(const QDomDocument &, const QString &, int grade = -1);
        void fileSave();
        void fileSaveAs();
        void fileClose();

        void filePrint();
        void filePreview();
        void print(bool showPreview);
        void filePrintToPDF();
        void filePrintToPDF(QWidget *wnd, const QDomDocument & doc,  QString & pdfFileName );

        void editDelete();
		void editUndo();
		void editRedo();
        void editZoomIn();
        void editZoomOut();
        void editCut();
        void editCopy();
        void editPaste();
        void editPaste(const QPointF &);
        void editSelectAll();
        void editPreferences();
        void editProperties();

        void docTitle();
        void docPageSetup();
        void docQuerySourceList();
        void docSectionEditor();
        void docColorList();
        void docDefinedParams();
        void docLabelDefinitions();

        void itemLabel();
        void itemField();
        void itemText();
        void itemLine();
        void itemCrossTab();
        void itemRect();
        void itemBarcode();
        void itemImage();
        void itemGraph();

        void dbConnect();
        void dbLoadDoc();
        void dbSaveDoc();

        void sReportsChanged(int, bool);
		void		onWinChanged(QWidget* w);

        QDomElement databaseElt() {return _databaseElt;}
        void loadMemDB(const QString &filename, const QDomNode &it);

        int insertItemCode() const;
        void resetInsertItemCode();

        QPointF getCopyPoint() const;
        void setCopyPoint(const QPointF &);
        void buildItemContextMenu(QMenu *);

        void alignTop();
        void alignVCenter();
        void alignBottom();

        void alignLeft();
        void alignHCenter();
        void alignRight();

        void color();
        void fill();
		void rotation();

        void fontChange(const QString &text);
        void fontSizeChange(const QString &text);
        void fontWeightChange(bool);
        void fontStyleChange(bool);
        void setFontFamily(const QString &text);
        void setFontSize(const QString &text);
        void setFontWeight(bool v);
        void setFontStyle(bool v);

    signals:
        void messageChanged(const QString & message);
        void messageCleared();
        void dbOpenClosed();
        void reportsChanged(int, bool);

    protected:
        ReportGridOptions * gridOptions;
        ReportWriterSectionData * sectionData;

        unsigned int selectionCount();

        QWidget * _parentWindow;
        bool _parentWindowIsWorkspace;

    protected slots:
        void updateSelectedEntity();

    private:
        QString getIconName(const QString& defaultName);

        QAction * fileNewAction;
        QAction * fileNewTestAction;
        QAction * fileOpenAction;
        QAction * fileSaveAction;
        QAction * fileSaveAsAction;
        QAction * fileCloseAction;
        QAction * filePreviewAction;
        QAction * filePrintAction;
        QAction * filePrintToPDFAction;
        QAction * fileExitAction;

	QActionGroup*	grpEdit;
		QAction * editUndoAction;
		QAction * editRedoAction;
        QAction * editCutAction;
        QAction * editCopyAction;
        QAction * editPasteAction;
        QAction * editDeleteAction;
        QAction * editPreferencesAction;
        QAction * editSelectAllAction;
        QAction * editPropertiesAction;
        QAction * editZoomInAction;
        QAction * editZoomOutAction;

	QActionGroup*	grpGrid;
        QAction * gridShowAction;
        QAction * gridSnapAction;

	QActionGroup*	grpItem;
        QAction * itemLabelAction;
        QAction * itemFieldAction;
        QAction * itemTextAction;
        QAction * itemLineAction;
        QAction * itemCrossTabAction;
        QAction * itemRectAction;
        QAction * itemBarcodeAction;
        QAction * itemImageAction;
        QAction * itemGraphAction;

        QAction * dbConnectAction;
        QAction * dbLoadAction;
        QAction * dbSaveAction;

        QActionGroup * grpDoc;
        QAction * docTitleAction;
        QAction * docPageSetupAction;
        QAction * docQuerySourceListAction;
        QAction * docSectionEditorAction;
        QAction * docColorListAction;
        QAction * docDefinedParamsAction;
        QAction * docLabelDefinitionsAction;

        QActionGroup * grpAlign;
        QAction * alignTopAction;
        QAction * alignVCenterAction;
        QAction * alignBottomAction;
        QAction * alignLeftAction;
        QAction * alignHCenterAction;
        QAction * alignRightAction;

        QActionGroup *  grpFont;
        QFontComboBox*  cbFont;
        QComboBox*      cbFontSize;
        QPushButton*    btFontWeight;
        QPushButton*    btFontStyle;

        QActionGroup * grpProperties;
        QAction * colorAction;
        QAction * fillAction;
		QAction * rotationAction;

        DocumentWindow * activeDocumentWindow();
        void addDocumentWindow(DocumentWindow * gw);
        QList<DocumentWindow*> gwList;

        bool _placeMenusOnWindows;
        bool _placeToolbarsOnWindows;
        bool _allowDBConnect;
        bool _noDatabase;

        QString _strMenuConnect;
        QString _strMenuDisconnect;

        QDomElement _databaseElt;

    private slots:
        void removeReportWindow(QObject * obj);
        void dbMenuAboutToShow();
};

#endif

