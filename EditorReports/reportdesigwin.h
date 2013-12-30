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
#include <QListWidgetItem>
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

    void on_actionAlinear_Izquierda_triggered();

    void on_actionAlinear_Derecha_triggered();

    void on_actionAlinear_Centro_triggered();

    void on_actionAlinear_Arriba_triggered();

    void on_actionAlinear_centro_triggered();

    void on_actionAlinear_abajo_triggered();

    void refreshItems();
    void itemChanged ( QListWidgetItem * item );
    void nameChanged (Container * c);
    void on_actionVer_contorno_objetos_triggered(bool checked);

    void sceneSelectionChanged();


    void on_itemList_itemDoubleClicked(QListWidgetItem *item);

    void on_itemList_itemSelectionChanged();

private:
    Ui::ReportDesigWin *ui;
    QGraphicsScene scene;
    Paper paper;
    ReportView view;
    QAction* insertor;
    ReportRenderer* render;

    QMap<QListWidgetItem* , Container*> _itemList;

};

#endif // REPORTDESIGWIN_H
