#include "agendaform.h"
#include "ui_agendaform.h"
#include "editeventform.h"
AgendaForm::AgendaForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgendaForm)
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
        table->addEvento(form.event_color,s,e,tituloEvento,asunto,isCita,id_cliente,form.isPrivado);
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
}
