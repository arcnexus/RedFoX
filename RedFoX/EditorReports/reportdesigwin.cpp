#include "reportdesigwin.h"
#include "ui_reportdesigwin.h"
#include <QFontDatabase>
#include "editpaperdlg.h"
#include <QtConcurrent/QtConcurrent>
#include "../db_table_view.h"

#include "savetobdfrm.h"

ReportDesigWin::ReportDesigWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReportDesigWin),
    scene(this),
    view(this)
{


    render = 0;
    insertor = 0;
    ui->setupUi(this);
    view.setScene(&scene);
    this->setCentralWidget(&view);


    QFontDatabase::addApplicationFont(":/codeBar/codebar/FRE3OF9X.TTF");
    //Free 3 of 9 Extended        

    scene.setBackgroundBrush(QBrush(Qt::gray));

    scene.addItem(&paper);
    paper.setSize(Paper::A4);
    paper.setOrientacion(Paper::Retrato);
    paper.setMargen(1.5);

    connect(&paper,SIGNAL(itemInserted()),this,SLOT(stopInsert()));
    connect(&paper,SIGNAL(itemInserted()),this,SLOT(refreshItems()));
    connect(ui->itemList,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(itemChanged(QListWidgetItem*)));

    connect(ui->actionRectangulo_redondeado,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionTexto,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionLinea,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionCodeBars,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionImagen,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionCampo,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionCampo_Relacional,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionParametro,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionAcumulador,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));

    connect(&scene,SIGNAL(selectionChanged()),this,SLOT(sceneSelectionChanged()));
}

ReportDesigWin::~ReportDesigWin()
{
    disconnect(this,SLOT(sceneSelectionChanged()));
    delete ui;
    if(render)
        delete render;
}

void ReportDesigWin::element_toggled(bool arg1)
{
    if(arg1)
    {
        if(insertor)
        {
            if(insertor->isChecked())
            {
                insertor->blockSignals(true);
                insertor->setChecked(false);
                insertor->blockSignals(false);
            }
        }
        QObject * send = sender();
        insertor = qobject_cast<QAction*>(send);
        if(send == ui->actionRectangulo_redondeado)
            paper.prepareItemInsert(Paper::RoundRectIt);
        else if(send == ui->actionTexto)
            paper.prepareItemInsert(Paper::Label);
        else if(send == ui->actionLinea)
            paper.prepareItemInsert(Paper::Linea);
        else if(send == ui->actionCodeBars)
            paper.prepareItemInsert(Paper::CodeBarIt);
        else if(send == ui->actionImagen)
            paper.prepareItemInsert(Paper::Imagen);
        else if(send == ui->actionCampo)
            paper.prepareItemInsert(Paper::Campo);
        else if(send == ui->actionCampo_Relacional)
            paper.prepareItemInsert(Paper::CampoRelacional);
        else if(send == ui->actionParametro)
            paper.prepareItemInsert(Paper::Parametro);
        else if(send == ui->actionAcumulador)
            paper.prepareItemInsert(Paper::Acumulador);
    }
    else
    {
        insertor = 0;
        paper.stopInsertingItems();
    }
}

void ReportDesigWin::on_actionGuardar_triggered()
{
    QString file = QFileDialog::getSaveFileName(qApp->activeWindow(),tr("Guardar Report"),QString(),"XML (*.xml)");
    if(!file.endsWith(".xml",Qt::CaseInsensitive))
        file.append(".xml");
    if(!file.isEmpty())
    {
        int i = paper.save(file);
        if(i<0)
        {
            if (i== -1)
                QMessageBox::critical(this,"Error","Error al abrir el archivo");
            else
                QMessageBox::warning(this,"Cuidado","Tiene elementos fuera de secciones.\nEstos no han sido guardados.");
        }
    }
}

void ReportDesigWin::on_actionAbrir_triggered()
{
    QString fil = QFileDialog::getOpenFileName(this,"Abir report",QString(),"XML (*.xml)");

    if(fil.isEmpty())
        return;

    QFile ff(fil);
    QString error;
    if(ff.open(QFile::ReadOnly))
    {
        QString x = ff.readAll();
        if(!paper.parseXML(x,error))
            QMessageBox::critical(this,"Error",error);
        refreshItems();
    }
    ff.close();
}

void ReportDesigWin::stopInsert()
{
    insertor->setChecked(false);
}

void ReportDesigWin::on_actionBorrar_triggered()
{
    paper.removeItems(scene.selectedItems());
}

void ReportDesigWin::on_actionConfigurar_pagina_triggered()
{
    EditPaperDlg dlg(&paper,this);
    dlg.exec();
}

void ReportDesigWin::on_actionVista_Previa_triggered()
{
    QProgressDialog* pDlg = new QProgressDialog(this);
    pDlg->setMaximum(0);
    pDlg->setValue(0);
    pDlg->show();

    QPrinter * printer = new QPrinter(QPrinter::HighResolution);

   // QPrintDialog dlg(printer,this);//Default printer

    QDomDocument doc = paper.preview();
    render = new ReportRenderer();

    render->setPrinter(printer);
    render->setDocIn(doc);
    render->setLimit(ui->actionLimitar_vista_previa->isChecked());

    QtConcurrent::run(render, &ReportRenderer::PreRender);
    connect(render,SIGNAL(end()),pDlg,SLOT(deleteLater()));
    connect(render,SIGNAL(end()),this,SLOT(previewReady()));
}

void ReportDesigWin::previewReady()
{
    QPrintPreviewDialog * pre = new QPrintPreviewDialog(render->getPrinter(),this);
    connect(pre, SIGNAL(paintRequested(QPrinter*)), render, SLOT(Print(QPrinter*)));
    pre->show();
}

void ReportDesigWin::on_actionAbrir_desde_BD_triggered()
{
    Db_table_View form(this);
    form.set_readOnly(true);
    form.set_db("empresa");
    form.set_table("report");

    form.setWindowTitle(tr("Informes"));

    QStringList headers;

    form.set_columnHide(0);
    form.set_columnHide(3);

    headers << tr("Nombre")  << tr("Descripcion") << tr("Grado");
    form.set_table_headers(headers);

    form.set_selection("report_source");
    if(form.exec())
    {
        QString error;
        paper.parseXML(form.selected_value,error);
        refreshItems();
    }
}

void ReportDesigWin::on_actionGuardar_en_BD_triggered()
{
    SaveToBDFrm frm(this);
    if(frm.exec() == QDialog::Accepted)
    {
        QString error = "";
        QString nombre = frm.nombre;
        QString desc = frm.desc;
        int grad = frm.grad;
        QStringList clausulas;
        clausulas << QString("report_name = '%1'").arg(nombre);
        clausulas << QString("report_grade = %1").arg(grad);
        int id = SqlCalls::SelectOneField("report","report_id",clausulas,Configuracion_global->empresaDB,error).toInt();
        QHash<QString,QVariant> v;
        v["report_name"] = nombre;
        v["report_descrip"] = desc;
        v["report_source"] = paper.preview().toString();
        v["report_grade"] = grad;
        if(id == 0)//no está en la bd
        {
            id = SqlCalls::SqlInsert(v,"report",Configuracion_global->empresaDB,error);
            if(id < 0)
                TimedMessageBox::Box(qApp->activeWindow(),tr("Error:\n%1").arg(error),TimedMessageBox::Critical);
            else
                TimedMessageBox::Box(qApp->activeWindow(),tr("Guardado con éxito"));
        }
        else
        {
            if(!SqlCalls::SqlUpdate(v,"report",Configuracion_global->empresaDB,clausulas,error))
                TimedMessageBox::Box(qApp->activeWindow(),tr("Error:\n%1").arg(error),TimedMessageBox::Critical);
            else
                TimedMessageBox::Box(qApp->activeWindow(),tr("Guardado con éxito"));
        }
    }
}

void ReportDesigWin::on_actionAlinear_Izquierda_triggered()
{
  QList<QGraphicsItem*> items = scene.selectedItems();
  if(items.isEmpty())
      return;

  qreal _left = items.at(0)->x();
  foreach (QGraphicsItem* i, items) {
      if(i->x() < _left)
          _left = i->x();
  }

  foreach (QGraphicsItem* i, items) {
      i->setPos(_left,i->y());
  }
}

void ReportDesigWin::on_actionAlinear_Derecha_triggered()
{
    QList<QGraphicsItem*> items = scene.selectedItems();
    if(items.isEmpty())
        return;

    qreal _rigth = items.at(0)->x() + items.at(0)->boundingRect().width();
    foreach (QGraphicsItem* i, items) {
        if(i->x() + i->boundingRect().width() > _rigth)
            _rigth = i->x() + i->boundingRect().width();
    }

    foreach (QGraphicsItem* i, items) {
        i->setPos(_rigth - i->boundingRect().width(), i->y());
    }
}

void ReportDesigWin::on_actionAlinear_Centro_triggered()
{
    QList<QGraphicsItem*> items = scene.selectedItems();
    if(items.isEmpty())
        return;

    qreal _center = items.at(0)->x() + items.at(0)->boundingRect().width() / 2;
    foreach (QGraphicsItem* i, items) {
        i->setPos(_center - i->boundingRect().width()/2, i->y());
    }
}

void ReportDesigWin::on_actionAlinear_Arriba_triggered()
{
    QList<QGraphicsItem*> items = scene.selectedItems();
    if(items.isEmpty())
        return;

    qreal _up = items.at(0)->y();
    foreach (QGraphicsItem* i, items) {
        if(i->y() <  _up)
            _up = i->y();
    }

    foreach (QGraphicsItem* i, items) {
        i->setPos(i->x(),_up);
    }
}

void ReportDesigWin::on_actionAlinear_centro_triggered()
{
    QList<QGraphicsItem*> items = scene.selectedItems();
    if(items.isEmpty())
        return;

    qreal _center = items.at(0)->y() + items.at(0)->boundingRect().height()/2;

    foreach (QGraphicsItem* i, items) {
        i->setPos(i->x(),_center - i->boundingRect().height()/2);
    }
}

void ReportDesigWin::on_actionAlinear_abajo_triggered()
{
    QList<QGraphicsItem*> items = scene.selectedItems();
    if(items.isEmpty())
        return;

    qreal _center = items.at(0)->y() + items.at(0)->boundingRect().height();
    foreach (QGraphicsItem* i, items) {
        if(i->y() + i->boundingRect().height() >  _center)
            _center = i->y()  + i->boundingRect().height();
    }

    foreach (QGraphicsItem* i, items) {
        i->setPos(i->x(),_center - i->boundingRect().height());
    }
}

void ReportDesigWin::refreshItems()
{
    _itemList.clear();
    ui->itemList->clear();    

    ui->itemList->blockSignals(true);
    QMapIterator<Container*,QString> it(Container::items());
    while(it.hasNext())
    {
        it.next();
        QListWidgetItem *listItem = new QListWidgetItem(it.value(),ui->itemList);
        _itemList.insert(listItem,it.key());
        listItem->setCheckState(it.key()->isActive() ? Qt::Checked : Qt::Unchecked);
        connect(it.key(),SIGNAL(nameChanged(Container*)),this,SLOT(nameChanged(Container*)));
    }
    ui->itemList->blockSignals(false);
}

void ReportDesigWin::itemChanged(QListWidgetItem *item)
{
    if(_itemList.contains(item))
        if(_itemList.value(item))
            _itemList.value(item)->setActive(item->checkState() == Qt::Checked);
}

void ReportDesigWin::nameChanged(Container *c)
{
    QMapIterator<QListWidgetItem*, Container*> it(_itemList);
    while(it.hasNext())
    {
        it.next();
        if(it.value() == c)
        {
            it.key()->setText(c->name());
            break;
        }
    }
}

void ReportDesigWin::on_actionVer_contorno_objetos_triggered(bool checked)
{
    Container::setShowBorder(checked);
}

void ReportDesigWin::sceneSelectionChanged()
{
    QList<QGraphicsItem*> _selecteds = scene.selectedItems();
    QMapIterator<QListWidgetItem*,Container*> it(_itemList);
    ui->itemList->blockSignals(true);
    while(it.hasNext())
    {
        it.next();
        it.key()->setSelected(_selecteds.contains(it.value()));
    }
    ui->itemList->blockSignals(false);
}


void ReportDesigWin::on_itemList_itemDoubleClicked(QListWidgetItem *item)
{
    if(item)
        if(_itemList.contains(item))
            _itemList.value(item)->editMe();
}

void ReportDesigWin::on_itemList_itemSelectionChanged()
{
    QListIterator<QListWidgetItem*> it (ui->itemList->selectedItems());
    scene.blockSignals(true);
    scene.clearSelection();
    while(it.hasNext())
    {
        QListWidgetItem* current = it.next();
        if(_itemList.contains(current))
        {
            _itemList.value(current)->setSelected(true);
        }
    }
    scene.blockSignals(false);
}

void ReportDesigWin::on_actionNuevo_triggered()
{
    paper.newDoc();
    ui->itemList->clear();
}
