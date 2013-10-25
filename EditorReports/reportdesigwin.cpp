#include "reportdesigwin.h"
#include "ui_reportdesigwin.h"
#include <QFontDatabase>
#include "editpaperdlg.h"
#include <QtConcurrent/QtConcurrent>
#include "../db_table_view.h"
#include "../sqlcalls.h"
#include "savetobdfrm.h"
ReportDesigWin::ReportDesigWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReportDesigWin),
    scene(this),
    view(this)
{
    render = 0;
    ui->setupUi(this);
    view.setScene(&scene);
    this->setCentralWidget(&view);

    QFontDatabase::addApplicationFont(":/codeBar/codebar/FRE3OF9X.TTF");
    //Free 3 of 9 Extended

    //TODO cambiar dentro de Maya
    /*QSqlDatabase gr = QSqlDatabase::addDatabase("QMYSQL","grupo");
    gr.setHostName("localhost");
    gr.setUserName("root");
    gr.setPassword("marco");
    gr.setDatabaseName("grupotesting");
    gr.open();
    QSqlDatabase em = QSqlDatabase::addDatabase("QMYSQL","empresa");
    em.setHostName("localhost");
    em.setUserName("root");
    em.setPassword("marco");
    em.setDatabaseName("empsarapin");
    em.open();*/

    scene.setBackgroundBrush(QBrush(Qt::gray));

    scene.addItem(&paper);
    paper.setSize(Paper::A4);
    paper.setOrientacion(Paper::Retrato);
    paper.setMargen(1.5);

    connect(&paper,SIGNAL(itemInserted()),this,SLOT(stopInsert()));

    connect(ui->actionRectangulo_redondeado,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionTexto,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionLinea,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionCodeBars,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionImagen,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionCampo,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionCampo_Relacional,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
    connect(ui->actionParametro,SIGNAL(toggled(bool)),this,SLOT(element_toggled(bool)));
}

ReportDesigWin::~ReportDesigWin()
{
    delete ui;
    if(render)
        delete render;
}

void ReportDesigWin::element_toggled(bool arg1)
{
    if(arg1)
    {
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
    }
    else
        paper.stopInsertingItems();
}

void ReportDesigWin::on_actionGuardar_triggered()
{
    QString file = QFileDialog::getSaveFileName(qApp->activeWindow(),tr("Guardar Report"),QString(),"XML (*.xml)");
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
        //qDebug() << form.selected_value << error;
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
                TimedMessageBox* t = new TimedMessageBox(qApp->activeWindow(),tr("Error:\n%1").arg(error),TimedMessageBox::Critical);
            else
                TimedMessageBox* t = new TimedMessageBox(qApp->activeWindow(),tr("Guardado con éxito"));
        }
        else
        {
            if(!SqlCalls::SqlUpdate(v,"report",Configuracion_global->empresaDB,clausulas,error))
                TimedMessageBox *t = new TimedMessageBox(qApp->activeWindow(),tr("Error:\n%1").arg(error),TimedMessageBox::Critical);
            else
                TimedMessageBox *t = new TimedMessageBox(qApp->activeWindow(),tr("Guardado con éxito"));
        }
    }
}
