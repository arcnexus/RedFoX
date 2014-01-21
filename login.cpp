#include "login.h"
#include "ui_login.h"

#include "mainwindow.h"



#include "../Zona_Administrador/frmempresas.h"
#include "../Zona_Administrador/arearestringida_form.h"
#include "../Zona_Administrador/frmconfigmaya.h"
#include "../Zona_Administrador/frmusuarios.h"
#include "mainwindow.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{  
    ui->setupUi(this);    
    connect(ui->btnEmpresa,SIGNAL(clicked()),this,SLOT(btnEmpresa_clicked()));
    connect(ui->Crearconfiguracin,SIGNAL(clicked()),this,SLOT(Crearconfiguracion_clicked()));

    if (! QFile::exists(qApp->applicationDirPath()+"/MayaConfig.ini")){
        frmConfigmaya frmConf;
        if(frmConf.exec()==QDialog::Accepted)
            TimedMessageBox * t = new TimedMessageBox(this,tr("Configuración inicial realizada con éxito"));
    } else
    {
        QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
        QString nombreusuario = settings.value("cUsuarioActivo").toString();
        QString contrasena = Configuracion_global->DeCrypt(settings.value("contrasenaactiva").toString());
        Configuracion_global->caja = QString::number(settings.value("cajaactiva").toInt());
        Configuracion_global->Importe_apertura = Configuracion_global->MonedatoDouble(settings.value("importe_apertura").toString());
        ui->lineUsuario->setText(nombreusuario);
        ui->linePassword->setText(contrasena);
    }


    if(Configuracion_global->CargarDatosBD())
        init();
    else
    {
        if(Configuracion_global->globalDB.isOpen())
        {
            frmConfigmaya frmConf;
            if(frmConf.exec()==QDialog::Accepted)
                TimedMessageBox * t = new TimedMessageBox(this,tr("Configuración inicial realizada con éxito"));
        }
        else
            QMessageBox::critical(0, "error:", Configuracion_global->globalDB.lastError().text());
    }
}

Login::~Login()
{
    delete ui;
}

const QString Login::getUsuario() const
{
    return ui->lineUsuario->text();
}

const QString Login::getPass() const
{
    return ui->linePassword->text();
}

const QString Login::getEmpresaName()
{
    return getEmpresa().value("nombre").toString();
}

const QSqlRecord Login::getEmpresa() const
{
    return _empresas.value(ui->comboGroup->currentText()).rec_empresas.value(ui->cboEmpresa->currentText());
}

int Login::getid_user()
{
    return m_id;
}


void Login::on_btnAcceder_clicked()
{
    QSqlRecord rGrupo = _empresas.value(ui->comboGroup->currentText()).rec_grupo;
    QString bd_driver = rGrupo.value("bd_driver").toString();
    QString bd_name = rGrupo.value("bd_name").toString();
    QString bd_host = rGrupo.value("bd_host").toString();
    QString bd_user = rGrupo.value("bd_user").toString();
    QString bd_pass = rGrupo.value("bd_pass").toString();
    int bd_port = rGrupo.value("bd_port").toInt();


    Configuracion_global->group_host = bd_host;
    Configuracion_global->group_user = bd_user;
    Configuracion_global->group_pass = bd_pass;
    Configuracion_global->group_port = bd_port;
    Configuracion_global->group_Driver = bd_driver;
    Configuracion_global->group_DBName = bd_name;
    Configuracion_global->groupDB = QSqlDatabase::addDatabase(bd_driver , "Maya");

    Configuracion_global->groupDB.setHostName(bd_host);
    Configuracion_global->groupDB.setUserName(bd_user);
    Configuracion_global->groupDB.setPassword(bd_pass);
    Configuracion_global->groupDB.setPort(bd_port);
    Configuracion_global->groupDB.setDatabaseName(bd_name);
    if(!Configuracion_global->groupDB.open())
    {
        QMessageBox::critical(this,tr("Error"),Configuracion_global->groupDB.lastError().text());
        return;
    }

    QSqlRecord rEmpresa = _empresas.value(ui->comboGroup->currentText()).rec_empresas.value(ui->cboEmpresa->currentText());

    Configuracion_global->nombre_bd_empresa = rEmpresa.value("nombre_bd").toString();
    Configuracion_global->nombre_bd_conta = rEmpresa.value("nombre_db_conta").toString();
    Configuracion_global->nombre_bd_medica = rEmpresa.value("nombre_bd_medica").toString();
    Configuracion_global->decimales = rEmpresa.value("decimales").toInt();
    Configuracion_global->decimales_campos_totales = rEmpresa.value("decimales_campos_totales").toInt();
    Configuracion_global->importado_sp = rEmpresa.value("importada_sp").toBool();
    Configuracion_global->porc_irpf = rEmpresa.value("porc_irpf").toFloat();

    QSqlQuery qryUsers(Configuracion_global->groupDB);

    qryUsers.prepare( "SELECT * FROM usuarios where nombre =:Nombre" );
    qryUsers.bindValue(":Nombre",ui->lineUsuario->text());

    if( !qryUsers.exec() ) 
	{
        QMessageBox::critical(qApp->activeWindow(), tr("Error:"), qryUsers.lastError().text());
    } 
	else 
	{
       if (qryUsers.next()) 
	   {
            QSqlRecord rUsuario = qryUsers.record();
            if (Configuracion::SHA256HashString(ui->linePassword->text()) == rUsuario.value("contrasena").toString().trimmed())
			{
                m_id = qryUsers.value(0).toInt();
                QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
                settings.setValue("cUsuarioActivo",rUsuario.field("nombre").value().toString());
                settings.setValue("nNivelAcceso",rUsuario.field("nivelacceso").value().toInt());
                settings.setValue("cCategoria",rUsuario.field("categoria").value().toString());
                Configuracion_global->id_usuario_activo = this->getid_user();
                Login::done(QDialog::Accepted);
            }
       } 
    }

    QSqlQuery q(Configuracion_global->globalDB);
    q.prepare( "SELECT * FROM mayaglobal.usuarios where nombre =:Nombre" );
    q.bindValue(":Nombre",ui->lineUsuario->text());
    if( !q.exec() )
    {
        QMessageBox::critical(qApp->activeWindow(), tr("Error:"), q.lastError().text());
    }
    else
    {
        if (q.next())
        {
            QSqlRecord rUsuario = q.record();
            if (Configuracion::SHA256HashString(ui->linePassword->text()) == rUsuario.value("contrasena").toString().trimmed())
            {
                m_id = qryUsers.value(0).toInt();
                QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
                settings.setValue("cUsuarioActivo",rUsuario.field("nombre").value().toString());
                settings.setValue("nNivelAcceso",7);
                settings.setValue("cCategoria", "ADMINISTRADOR");
                Configuracion_global->id_usuario_activo = this->getid_user();
                Configuracion_global->superUser = true;
                Login::done(QDialog::Accepted);
            }
        }
        else
        {
            QMessageBox::critical(this,tr("Error"),tr("Datos de acceso incorrectos"));
            ui->lineUsuario->setText("");
            ui->linePassword->setText("");
            ui->lineUsuario->setFocus();
        }
    }
}
void Login::Crearconfiguracion_clicked()
{
    AreaRestringida_form check(this);
    check.exec();
    if(check.es_valido())    {
        frmConfigmaya frmConf;
        if(frmConf.exec()==QDialog::Accepted)
            TimedMessageBox * t = new TimedMessageBox(this,tr("Configuración inicial realizada con éxito"));
    }
}

void Login::btnEmpresa_clicked()
{
    AreaRestringida_form check(this);
    check.exec();
    if(check.es_valido())
    {
        FrmEmpresas formEmpresa(this);
        formEmpresa.setWindowState(Qt::WindowMaximized);        
        formEmpresa.exec();
        init();
    }
}

void Login::on_pushButton_clicked()
{
	this->hide();
}

void Login::init()
{
    ui->comboGroup->clear();
    _empresas.clear();
    QString grupo = "";

    QSqlQuery QryEmpresas(QSqlDatabase::database("Global"));

    QryEmpresas.prepare("Select * from mayaglobal.grupos");

    if(QryEmpresas.exec())
	{
        while (QryEmpresas.next())
		{
            strc_empresa current;
            QSqlRecord rEmpresa = QryEmpresas.record();
            current.rec_grupo = rEmpresa;
            QString nombreGrupo = rEmpresa.field("nombre").value().toString();
            ui->comboGroup->addItem(nombreGrupo);

            if(grupo == "")
                grupo = nombreGrupo;

            QString bd_driver = rEmpresa.value("bd_driver").toString();
            QString bd_name = rEmpresa.value("bd_name").toString();
            QString bd_host = rEmpresa.value("bd_host").toString();
            QString bd_user = rEmpresa.value("bd_user").toString();
            QString bd_pass = rEmpresa.value("bd_pass").toString();
            int bd_port = rEmpresa.value("bd_port").toInt();

            QSqlDatabase db = QSqlDatabase::addDatabase(bd_driver);
            db.setHostName(bd_host);
            db.setUserName(bd_user);
            db.setPassword(bd_pass);
            db.setPort(bd_port);
            db.setDatabaseName(bd_name);

            if(!db.open()){
                QMessageBox::warning(qApp->activeWindow(),tr("Estableciendo conexión"),
                                     tr("No se pudo conectar con la base de datos:%1").arg(db.lastError().text()),
                                     tr("Aceptar"));
                qDebug() << db.lastError().text();
                continue;
            }

            QString query = QString("SELECT * FROM `%1`.empresas;").arg(bd_name);

            QSqlQuery q(db);
            q.exec(query);

            while(q.next())
            {
                QString emp = q.record().value("nombre").toString();

                current.empresas.append(emp);
                current.rec_empresas.insert(emp,q.record());
            }
            _empresas.insert(nombreGrupo,current);
		}
	}

	this->ui->lineUsuario->setFocus();
    ui->cboEmpresa->addItems(_empresas.value(grupo).empresas);

   // this->ui->lineUsuario->setText("marc");
    //this->ui->linePassword->setText("patata");
}

void Login::on_comboGroup_currentTextChanged(const QString &arg1)
{
    ui->cboEmpresa->clear();
    ui->cboEmpresa->addItems(_empresas.value(arg1).empresas);
}
