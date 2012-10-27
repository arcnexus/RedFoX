#include "frmagendavisitas.h"
#include "ui_frmagendavisitas.h"
#include "QtSql"


frmAgendaVisitas::frmAgendaVisitas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmAgendaVisitas)
{
    ui->setupUi(this);
    QSqlQueryModel *prueba = new QSqlQueryModel(this);
    prueba->setQuery("Select * from agenda",QSqlDatabase::database("empresa"));
//    QxtScheduleView  *Calendar = new QxtScheduleView(this);

//    Calendar->setDateRange(QDate::currentDate(),QDate::currentDate().addDays(2));
//    Calendar->setModel(prueba);
    ;
//    ui->FrameCalendar->layout()->addWidget(Calendar);

}

frmAgendaVisitas::~frmAgendaVisitas()
{
    delete ui;
}
