#include "agendaform.h"
#include "ui_agendaform.h"

void AgendaForm::setButtonColor()
{
    int r = event_color.red();
    int g = event_color.green();
    int b = event_color.blue();
    QString style = QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b);
    ui->btn_getColor->setStyleSheet(style);
}

AgendaForm::AgendaForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgendaForm)
{
    table = new GraphicsTable;
    table->setObjectName("MainTable");

    ui->setupUi(this);    
    connect(table,SIGNAL(allowEdits(bool)),ui->pushButton,SLOT(setEnabled(bool)));
    connect(ui->time_start,SIGNAL(timeChanged(QTime)),this,SLOT(timeChanged(QTime)));
    connect(ui->time_end,SIGNAL(timeChanged(QTime)),this,SLOT(timeChanged(QTime)));

    ui->combo_cliente->setModel(Configuracion_global->client_model);
    ui->combo_cliente->setModelColumn(Configuracion_global->client_model->fieldIndex("cNombreFiscal"));

    ui->combo_user->setModel(Configuracion_global->usuarios_model);
    ui->combo_user->setModelColumn(Configuracion_global->usuarios_model->fieldIndex("nombre"));

    ui->time_start->setTime(QTime(QTime::currentTime().hour()+1,0));

    event_color = QColor::fromRgb(qRgb(51,102,255));

    setButtonColor();

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

    if(!medic)
    {
        ui->especialidad_group->hide();
        ui->reunion_group->setTitle("Reunion con:");
    }
    else
    {
        ui->departamento_group->hide();
        ui->reunion_group->setTitle("Cita con:");
    }
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
    QDate date(ui->calendarWidget->selectedDate());
    QDateTime s(date,ui->time_start->time());
    QDateTime e(date,ui->time_end->time());
    QString asunto = ui->txt_asunto->toPlainText();
    QString tituloEvento = ui->txt_tituloEvento->text();
    asunto.replace("\n","<br>");
    int id_user = Configuracion_global->usuarios_model->record(ui->combo_user->currentIndex()).value("Id").toInt();
    bool isCita = ui->reunion_group->isChecked();
    int id_cliente = Configuracion_global->client_model->record(ui->combo_cliente->currentIndex()).value("Id").toInt();
    table->addEvento(event_color,s,e,tituloEvento,asunto,isCita,id_cliente);
}

void AgendaForm::timeChanged(const QTime &time)
{
    int h = time.hour();
    int m = time.minute();

    if(m<= 14)
    switch (m)
    {
    case 1:
    case 29:
        m=15;
        break;
    case 14:
        m=0;
        break;
    case 16:
    case 44:
        m=30;
        break;
    case 31:
        m=45;
        break;
    case 46:
        m=0;
        h++;
        break;
    default:
        m=0;
    }

    QTimeEdit * timeEd = qobject_cast<QTimeEdit *>(sender());
    if(timeEd)
    {
        timeEd->setTime(QTime(h,m));
        if(timeEd == ui->time_start)
            ui->time_end->setTime(QTime(h,m+15));
        else
        {
            if(ui->time_start->time() >= ui->time_end->time())
            {
                QTime t = ui->time_start->time();
                ui->time_end->setTime(QTime(t.hour(),t.minute()+15));
            }
        }
    }
}

void AgendaForm::on_btn_getColor_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        event_color = d.currentColor();
        setButtonColor();
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
