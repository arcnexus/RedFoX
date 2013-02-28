#include "editeventform.h"
#include "ui_editeventform.h"
#include "../Busquedas/frmbuscarcliente.h"
#include "./Zona_Pacientes/cliente.h"
EditEventForm::EditEventForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditEventForm)
{
    ui->setupUi(this);
    this->setWindowTitle("Editar evento");

    ui->combo_cliente->setModel(Configuracion_global->client_model);
    ui->combo_cliente->setModelColumn(Configuracion_global->client_model->fieldIndex("cNombreFiscal"));
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

EditEventForm::~EditEventForm()
{
    delete ui;
}

void EditEventForm::setButtonColor()
{
    int r = event_color.red();
    int g = event_color.green();
    int b = event_color.blue();
    QString style = QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b);
    ui->btn_getColor->setStyleSheet(style);
}

void EditEventForm::setColor(QColor c)
{
    event_color = c;
    setButtonColor();
}

void EditEventForm::setCita(bool is, int id)
{
    isCita = is;
    id_cliente = id;
    ui->reunion_group->setChecked(is);

    for (int i=0;i<Configuracion_global->client_model->rowCount();i++)
    {
        if (Configuracion_global->client_model->record(i).value("Id").toInt() == id)
        {
                ui->combo_cliente->setCurrentIndex(i);
                break;
        }
    }
}

void EditEventForm::setAsunto(QString t, QString a)
{
    titulo = t;
    asunto = a;
    ui->txt_asunto->setPlainText(a);
    ui->txt_tituloEvento->setText(t);
}

void EditEventForm::on_btn_getColor_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        event_color = d.currentColor();
        setButtonColor();
    }
}

void EditEventForm::on_btn_cancelar_clicked()
{
    EditEventForm::done(QDialog::Rejected);
}

void EditEventForm::on_btn_guardar_clicked()
{
    isCita = ui->reunion_group->isChecked();
    titulo = ui->txt_tituloEvento->text();
    asunto = ui->txt_asunto->toPlainText();
    asunto.replace("\n","<br>");
    isCita = ui->reunion_group->isChecked();
    QSqlRelationalTableModel* client_model = (QSqlRelationalTableModel*)ui->combo_cliente->model();
    id_cliente = client_model->record(ui->combo_cliente->currentIndex()).value("Id").toInt();
    id_spec = 0;//FIXME especialidad / departamento en agenda
    id_depart = 0;
    EditEventForm::done(QDialog::Accepted);
}

void EditEventForm::on_pushButton_clicked()
{
    FrmBuscarCliente BuscarClientes(this);
    if(BuscarClientes.exec() == QDialog::Accepted)
    {
        int nId = BuscarClientes.DevolverID();
        QString cId = QString::number(nId);

        Cliente oCliente;
        oCliente.Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
        QString name = oCliente.cNombreFiscal;
        int i = ui->combo_cliente->findText(name);
        ui->combo_cliente->setCurrentIndex(i);
    }
}
