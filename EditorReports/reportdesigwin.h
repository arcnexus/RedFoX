#ifndef REPORTDESIGWIN_H
#define REPORTDESIGWIN_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QFile>
#include <QTime>
#include <QProgressDialog>
#include <QApplication>
#include <QThread>
#include "paper.h"
#include "reportview.h"
#include "reportrenderer.h"
namespace Ui {
class ReportDesigWin;
}

class ReportDesigWin : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ReportDesigWin(QWidget *parent = 0);
    ~ReportDesigWin();
private slots:

    void element_toggled(bool arg1);

    void on_actionGuardar_triggered();

    void on_actionAbrir_triggered();

    void stopInsert();
    void on_actionBorrar_triggered();

    void on_actionConfigurar_pagina_triggered();

    void on_actionVista_Previa_triggered();

    void previewReady();
    void on_actionAbrir_desde_BD_triggered();

    void on_actionGuardar_en_BD_triggered();

private:
    Ui::ReportDesigWin *ui;
    QGraphicsScene scene;
    Paper paper;
    ReportView view;
    QAction* insertor;
    ReportRenderer* render;
};

#endif // REPORTDESIGWIN_H
