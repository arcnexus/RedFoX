#include "agendaform.h"
#include "ui_agendaform.h"
#include "editeventform.h"
#include "../db_table_view.h"
AgendaForm::AgendaForm(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::AgendaForm),
    toolButton(tr("Agenda"),":/Icons/PNG/Calender.png",this),
    menuButton(QIcon(":/Icons/PNG/Calender.png"),tr("Agenda"),this)
{
    table = new GraphicsTable;
    table->setObjectName("MainTable");

    ui->setupUi(this);    
    connect(table,SIGNAL(allowEdits(bool)),ui->pushButton,SLOT(setEnabled(bool)));


    ui->combo_user->setModel(Configuracion_global->usuarios_model);
    ui->combo_user->setModelColumn(Configuracion_global->usuarios_model->fieldIndex("nombre"));

    event_color = QColor::fromRgb(qRgb(51,102,255));

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(0,0,ui->graphicsView->frameSize().width()-20,1008);

    scene->addItem(table);

    QTextCharFormat currentDayHighlight;
    currentDayHighlight.setBackground(Qt::green);
    QDate today = QDate::currentDate();
    ui->calendarWidget->setDateTextFormat(today, currentDayHighlight);

    connect(ui->calendarWidget,SIGNAL(clicked(QDate)),table,SLOT(setDate(QDate)));
    ui->calendarWidget->setSelectedDate(today);
    //int id_user = Configuracion_global->usuarios_model->record(ui->combo_user->currentIndex()).value("id").toInt();
    table->setDate(today);
}

AgendaForm::~AgendaForm()
{
    delete ui;
}

void AgendaForm::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    ui->graphicsView->setSceneRect(0,0,ui->graphicsView->frameSize().width()-20,2000);
    table->setSize(ui->graphicsView->frameSize().width() - 21,1998);
}

void AgendaForm::on_pushButton_clicked()
{
    EditEventForm form(this);
    if (form.exec() == QDialog::Accepted)
    {
        QDate date(ui->calendarWidget->selectedDate());
        QDateTime s(date,form.start);
        QDateTime e(date,form.end);
        QString asunto = form.asunto;
        QString tituloEvento = form.titulo;
        asunto.replace("\n","<br>");
        bool isCita = form.isCita;
        int id_cliente = form.id_cliente;
        table->addEvento(form.event_color,s,e,tituloEvento,asunto,isCita,id_cliente,form.isprivado);
    }
}


void AgendaForm::on_calendarWidget_activated(const QDate &date)
{
    table->setDate(date);
}

void AgendaForm::on_combo_user_currentIndexChanged(int index)
{
    int id_user = Configuracion_global->usuarios_model->record(index).value("id").toInt();
    table->setUser(id_user);
}

void AgendaForm::on_btn_hoy_clicked()
{
    ui->calendarWidget->setSelectedDate(QDate::currentDate());
    table->setDate(ui->calendarWidget->selectedDate());
}

void AgendaForm::on_btn_buscaUser_clicked()
{
    QString s = QInputDialog::getText(this,"Nombre","Nombre de usuario");
    Db_table_View form(this);
    form.set_db("Maya");
    form.set_table("usuarios");

    form.setWindowTitle(tr("Usuarios"));

    QStringList headers;
    headers << tr("Nombre");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.set_columnHide(2);
    form.set_columnHide(3);
    form.set_columnHide(4);
    form.set_noInsertDeleteRows();
    form.set_readOnly(true);

    form.set_selection("nombre");

    QString filt = QString("nombre like '%%1%'").arg(s);
    form.set_filter(filt);
    if(form.exec()==QDialog::Accepted)
    {
        int i = ui->combo_user->findText(form.selected_value);
        ui->combo_user->setCurrentIndex(i);
    }
}

void AgendaForm::on_btn_zoomIn_clicked()
{
    double scaleFactor = 1.15;
    ui->graphicsView->scale(scaleFactor, scaleFactor);
}

void AgendaForm::on_btn_zoomOut_clicked()
{
    double scaleFactor = 1.15;
    ui->graphicsView->scale(1/scaleFactor, 1/scaleFactor);
}
