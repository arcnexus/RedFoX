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
    QSqlQuery qryUsers(Configuracion_global->globalDB);

    if( !qryUsers.exec("SELECT id,nombre,contrasena FROM `mayaglobal`.`usuarios`") )
    {
        QMessageBox::critical(this, "error:", qryUsers.lastError().text());
    }
    else
    {
       if (qryUsers.next())
       {
           do
           {
               QSqlRecord rUsuario = qryUsers.record();
               if(ui->lineUsuario->text() == rUsuario.value("nombre").toString().trimmed())
               {
                   if (Configuracion::SHA256HashString(ui->linePassword->text()) == rUsuario.value("contrasena").toString().trimmed())
                   {
                       m_valido = true;
                       AreaRestringida_form::done(QDialog::Accepted);
                   }
               }
           }while(qryUsers.next());

           if(m_valido == false)
           {
               QMessageBox::critical(this,tr("ACCESO DENEGADO"),tr("El usuario y la contraseña no se corresponden\n\n Verifique los datos"));
               ui->linePassword->setText("");
               ui->linePassword->setFocus();
           }
       }
       else
       {
           //Si no existen super usuarios
           m_valido = true;
           AreaRestringida_form::done(QDialog::Accepted);
       }
    }
}
