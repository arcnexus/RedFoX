#include "frmagenda.h"
#include "ui_frmagenda.h"
#include "QxtScheduleView"
#include "QtSql"

frmAgenda::frmAgenda(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmAgenda)
{
    ui->setupUi(this);
    QSqlQueryModel *prueba = new QSqlQueryModel(this);
    prueba->setQuery("Select * from agenda",QSqlDatabase::database("empresa"));
    QxtScheduleView  *Calendar = new QxtScheduleView(this);

    Calendar->setDateRange(QDate::currentDate(),QDate::currentDate().addDays(2));
    Calendar->setModel(prueba);
    ;
    ui->FrameCalendar->layout()->addWidget(Calendar);

}

frmAgenda::~frmAgenda()
{
    delete ui;
}
