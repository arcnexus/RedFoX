#include "frmnuevosavisos.h"
#include "ui_frmnuevosavisos.h"
#include "avisos.h"

frmNuevosAvisos::frmNuevosAvisos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmNuevosAvisos)
{
    ui->setupUi(this);
    m_empresas = new QSqlQueryModel(this);
    m_usuarios = new QSqlQueryModel(this);
    m_empresas->setQuery("select nombre from empresas",Configuracion_global->groupDB);
    m_usuarios->setQuery("select nombre from usuarios",Configuracion_global->groupDB);
    ui->cboEmpresa->setModel(m_empresas);
    ui->cboUsuarios->setModel(m_usuarios);
}

frmNuevosAvisos::~frmNuevosAvisos()
{
    delete ui;
}

void frmNuevosAvisos::setFecha(QDate fecha)
{
    ui->txtFecha->setDate(fecha);
}

void frmNuevosAvisos::setHora(QTime hora)
{
    ui->txtHora->setTime(hora);
}

void frmNuevosAvisos::setText(QString texto)
{
    ui->txtAviso->setText(texto);
}

void frmNuevosAvisos::setEmpresa(int id_empresa)
{
    QString error;
    QString empresa = SqlCalls::SelectOneField("empresas","nombre",QString("id=%1").arg(id_empresa),
                                               Configuracion_global->groupDB,error).toString();
    int index = ui->cboEmpresa->findText(empresa);
    ui->cboEmpresa->setCurrentIndex(index);

}

QDate frmNuevosAvisos::getFecha()
{
    return ui->txtFecha->date();
}

QTime frmNuevosAvisos::getHora()
{
    return ui->txtHora->time();
}

QString frmNuevosAvisos::getText()
{
    ui->txtAviso->toPlainText();
}

int frmNuevosAvisos::get_id_Empresa()
{
    QString error;
    int id_empresa = SqlCalls::SelectOneField("empresas","id",QString("nombre = '%1'").arg(ui->cboEmpresa->currentText()),
                                              Configuracion_global->groupDB,error).toInt();
    return id_empresa;

}

int frmNuevosAvisos::get_id_usuario_destino()
{
    QString error;
    int id = SqlCalls::SelectOneField("usuarios","id",QString("nombre ='%1'").arg(ui->cboUsuarios->currentText()),
                                      Configuracion_global->groupDB,error).toInt();
    return id;
}
