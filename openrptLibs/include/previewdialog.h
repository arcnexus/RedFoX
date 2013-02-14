///////////////////////////////////////////////////////////////////////////////
// previewdialog.h
// -------------------
// Copyright (c) 2007 David Johnson <david@usermode.org>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////

#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QAbstractScrollArea>
#include <QDialog>

class PreviewWidget;
class QAbstractScrollArea;
class ORODocument;

///////////////////////////////////////////////////////////////////////////////
/// \class PreviewDialog
/// PreviewDialog is a print preview dialog for use with the TextPrinter class.
/// It is typically created with the TextPrinter::preview() method.
///////////////////////////////////////////////////////////////////////////////

class PreviewDialog : public QDialog
{
    Q_OBJECT
public:
    /// Construct a PreviewDialog dialog
    PreviewDialog(ORODocument *document,
                  QPrinter *pPrinter,
                  QWidget *parent=0);
    /// Destructor
    virtual ~PreviewDialog();

private:
    PreviewWidget *_view;
};

// Text preview widget
class PreviewWidget : public QAbstractScrollArea
{
    Q_OBJECT
public:
    PreviewWidget(ORODocument *document,
                  QPrinter *pPrinter,
                  QWidget *parent=0);
    virtual ~PreviewWidget();
    void updateView();

private slots:
    // zoom preview
    void zoomIn();
    void zoomOut();

private:
    virtual void paintEvent(QPaintEvent *e);
    virtual void resizeEvent(QResizeEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    QRectF paperRect(QPaintDevice *device);
    int columnWidth();
    int nbColumns();


private:
    ORODocument * _doc;
    QPrinter *	_pPrinter;
    double _zoom;
    QPoint mousepos;
    QPoint scrollpos;
};

#endif // PREVIEWDIALOG_H
