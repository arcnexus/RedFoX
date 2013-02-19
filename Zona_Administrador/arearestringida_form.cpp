#include "arearestringida_form.h"
#include "ui_arearestringida_form.h"

AreaRestringida_form::AreaRestringida_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AreaRestringida_form)
{
    m_valido = false;
    ui->setupUi(this);
    connect(ui->btn_cancelar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->btn_aceptar,SIGNAL(clicked()),this,SLOT(login()));
    //TODO poner
}

AreaRestringida_form::~AreaRestringida_form()
{
    delete ui;
}

bool AreaRestringida_form::es_valido()
{
    return m_valido;
}

void AreaRestringida_form::login()
{
    QSqlQuery qryUsers(QSqlDatabase::database("terra"));

    qryUsers.prepare( "SELECT * FROM usuarios where nombre =:Nombre AND categoria = 'ADMINISTRADOR'" );
    qryUsers.bindValue(":Nombre",ui->lineUsuario->text());

    if( !qryUsers.exec() )
    {
        QMessageBox::critical(this, "error:", qryUsers.lastError().text());
    }
    else
    {
       if (qryUsers.next())
       {
            QSqlRecord rUsuario = qryUsers.record();
            if (ui->linePassword->text() == qryUsers.value(2).toString())
            {
                QSettings settings(qApp->applicationDirPath()+"/TerraConfig.ini", QSettings::IniFormat);
                settings.setValue("cUsuarioActivo",rUsuario.field("nombre").value().toString());
                settings.setValue("nNivelAcceso",rUsuario.field("nivelacceso").value().toInt());
                settings.setValue("cCategoria",rUsuario.field("categoria").value().toString());
                m_valido = true;
                AreaRestringida_form::done(QDialog::Accepted);
            }
            else
            {
                QMessageBox::critical(this,"ACCESO DENEGADO","El usuario y la contraseña no se corresponden\n\n Verifique los datos");
                ui->linePassword->setText("");
                ui->linePassword->setFocus();
            }
       }
       else
       {
           QMessageBox::critical(this,"Error","No existe ningún usuario con este nombre o\nUsted no tiene permiso para acceder a esta zona.");
           ui->lineUsuario->setText("");
           ui->lineUsuario->setFocus();
       }
    }
}
