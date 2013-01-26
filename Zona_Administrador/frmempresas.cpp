#include "frmempresas.h"
#include "ui_frmempresas.h"
#include <QtSql>
#include <QSqlDatabase>
#include<QSqlQuery>
#include <QMessageBox>
#include "frmbuscarpoblacion.h"
#include <QFileDialog>
#include <QApplication>
#ifdef WIN32
    #define and &&
#endif
FrmEmpresas::FrmEmpresas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmEmpresas)
{
    ui->setupUi(this);
    ui->txtMensaje->setText("ATENCION: Modifique con cuidado. Cambiar estos\n valores puede hacer que el programa deje\n de funcionar");

    connect(ui->botCerrar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->botCerrar_2,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->txtcPoblacion,SIGNAL(editingFinished()),this,SLOT(txtcPoblacion_editingFinished()));
    connect(ui->txtcCP,SIGNAL(editingFinished()),this,SLOT(txtcCp_editingFinished()));

    on_botSiguiente_clicked();
    on_botSiguiente_user_clicked();
}

FrmEmpresas::~FrmEmpresas()
{
    delete ui;
}

void FrmEmpresas::LLenarCampos()
{
    ui->txtcCodigo->setText(oEmpresa.getcCodigo());
    ui->txtEmpresa->setText(oEmpresa.getcNombre());
    ui->txtRutaBd->setText(oEmpresa.getcRutaBDSqLite());
    ui->txtcHost->setText(oEmpresa.getcHost());
    ui->txtcNombreBd->setText(oEmpresa.getcNombreBD());
    ui->txtcUser->setText(oEmpresa.getcUser());
    ui->txtcPassword->setText(oEmpresa.getcContrasena());
    ui->txtcPuerto->setText(oEmpresa.getcPuerto());
    int indice=ui->txtcDriver->findText(oEmpresa.getcDriverBD());
    if(indice!=-1)
       ui->txtcDriver->setCurrentIndex(indice);

    ui->txtRutaBd->setText(oEmpresa.getcRutaBDSqLite());
    ui->txtcHostmedic->setText(oEmpresa.getcHostMed());
    ui->txtcNombreBdMedic->setText(oEmpresa.getcNombreBDMed());
    ui->txtcUserMedic->setText(oEmpresa.getcUserMed());
    ui->txtcPasswordBdMedic->setText(oEmpresa.getcContrasenaMed());
    ui->txtcPuertoMedic->setText(oEmpresa.getcPuertoMed());

    //indice=ui->txtcDriverMedica->findText(oEmpresa.getcDriverBDMed());
    //if(indice!=-1)
    //ui->txtcDriverMedica->setCurrentIndex(indice);


    ui->txtcDireccion->setText(oEmpresa.getcDireccion());
    ui->txtcCP->setText(oEmpresa.getcCp());
    ui->txtcPoblacion->setText(oEmpresa.getcPoblacion());
    ui->txtcProvincia->setText(oEmpresa.getcProvincia());
    ui->txtcPais->setText(oEmpresa.getcPais());
    ui->txtcTelefono1->setText(oEmpresa.getcTelefono1());
    ui->txtcTelefono2->setText(oEmpresa.getcTelefono2());
    ui->txtcFax->setText(oEmpresa.getcFax());
    ui->txtcMail->setText(oEmpresa.getcMail());
    ui->txtcWeb->setText(oEmpresa.getcWeb());
    ui->txtcCif->setText(oEmpresa.getcCif());
    ui->txtcInscripcion->setText(oEmpresa.getcInscripcion());
    ui->txtcCometarioAlbaran->setText(oEmpresa.getcComentarioAlbaran());
    ui->txtcComentarioFactura->setText(oEmpresa.getcComentarioFactura());
    ui->txtcComentarioTicket->setText(oEmpresa.getcComentarioTicket());
    ui->txtnEjercicio->setValue(oEmpresa.getnEjercicio());
    ui->txtnDigitosFactura->setText(QString::number(oEmpresa.getnDigitosFactura()));
    ui->txtcSerieFactura->setText(oEmpresa.getcSerie());
    ui->txtnDigitosCuentas->setText(QString::number(oEmpresa.getnDigitosCuentas()));
    ui->txtcCuentaCliente->setText(oEmpresa.getcCuentaClientes());
    ui->txtcCuentaProveedores->setText(oEmpresa.getcCuentaProveedores());
    ui->txtcCuentaAcreedores->setText(oEmpresa.getcCuentaAcreedores());
}

void FrmEmpresas::CargarCamposEnEmpresa()
{
    oEmpresa.setcCodigo(ui->txtcCodigo->text());
    oEmpresa.setcNombre(ui->txtEmpresa->text());
    oEmpresa.setcRutaBDSqLite(ui->txtRutaBd->text());
    oEmpresa.setcHost(ui->txtcHost->text());
    oEmpresa.setcNombreBD(ui->txtcNombreBd->text());
    oEmpresa.setcUser(ui->txtcUser->text());
    oEmpresa.setcContrasena(ui->txtcPassword->text());
    oEmpresa.setcPuerto(ui->txtcPuerto->text());
    oEmpresa.setcDriverBD(ui->txtcDriver->currentText());

    oEmpresa.setcHostMed(ui->txtcHostmedic->text());
    oEmpresa.setcNombreBDMed(ui->txtcNombreBdMedic->text());
    oEmpresa.setcUserMed(ui->txtcUserMedic->text());
    oEmpresa.setcContrasenaMed(ui->txtcPasswordBdMedic ->text());
    oEmpresa.setcPuertoMed(ui->txtcPuertoMedic->text());
    oEmpresa.setcDriverBDMed(ui->txtcDriver->currentText());



    oEmpresa.setcSerie(ui->txtcSerieFactura->text());
    oEmpresa.setcDireccion(ui->txtcDireccion->text());
    oEmpresa.setcCP(ui->txtcCP->text());
    oEmpresa.setcPoblacion(ui->txtcPoblacion->text());
    oEmpresa.setcProvincia(ui->txtcProvincia->text());
    oEmpresa.setcPais(ui->txtcPais->text());
    oEmpresa.setcTelefono1(ui->txtcTelefono1->text());
    oEmpresa.setcTelefono2(ui->txtcPais->text());
    oEmpresa.setcFax(ui->txtcFax->text());
    oEmpresa.setcMail(ui->txtcMail->text());
    oEmpresa.setcWeb(ui->txtcWeb->text());
    oEmpresa.setcCif(ui->txtcCif->text());
    oEmpresa.setcInscripcion(ui->txtcInscripcion->text());
    oEmpresa.setcComemtarioAlbaran(ui->txtcCometarioAlbaran->toPlainText());
    oEmpresa.setcComentarioFactura(ui->txtcComentarioFactura->toPlainText());
    oEmpresa.setcComentarioTicket(ui->txtcComentarioTicket->toPlainText());
    oEmpresa.setnEjercicio(ui->txtnEjercicio->text().toInt());
    oEmpresa.setnDigitosCuentas(ui->txtnDigitosCuentas->text().toInt());
    oEmpresa.setcCodigoCuentaClientes(ui->txtcCuentaCliente->text());
    oEmpresa.setcCodigoCuentaProveedor(ui->txtcCuentaProveedores->text());
    oEmpresa.setcCodigoCuentaAcreedores(ui->txtcCuentaAcreedores->text());
}


void FrmEmpresas::on_botSiguiente_clicked()
{
    oEmpresa.Recuperar("select * from empresas where id > "+QString::number(oEmpresa.getid()),1);
    LLenarCampos();
}

void FrmEmpresas::on_botAnterior_clicked()
{
    oEmpresa.Recuperar("select * from empresas where id < "+QString::number(oEmpresa.getid())+" order by id desc",2);
    LLenarCampos();
}


void FrmEmpresas::on_botGuardar_clicked()
{
    if(ui->botAnadir->text() == "Deshacer")
    {
        int idriver = ui->txtcDriver->currentIndex();
        int idriverMedica = ui->txtcDriver->currentIndex();

        if(ui->txtRutaBd->text().isEmpty() && (idriver == 0 || idriverMedica == 0))
        {
            QMessageBox::critical(qApp->activeWindow(),tr("Ruta no valida"),tr("Especifique un ruta valida para la base de datos"),tr("&Aceptar"));
            return;
        }

        if( ui->txtcDriver->currentIndex() == 0 )
        {
            copy_db_progressFrm frm(this,2);
            frm.setWindowTitle(tr("Creando base de datos sqlite"));
            QApplication::processEvents();
            frm.set_Max_1(2);
            frm.setProgess_1(tr("Creando base de datos de empresa"),0);
            frm.setModal(true);
            frm.show();
            if(!crear_empresa_sqlite(&frm))
                return;
            frm.setProgess_1(tr("Creando base de datos medica"),1);
            frm.setProgess_2("",0);
            if(!crear_medica_sqlite(&frm))
                return;
        }
        else
        {
            copy_db_progressFrm frm(this,2);
            frm.setWindowTitle(tr("Creando base de datos mysql"));
            QApplication::processEvents();
            frm.set_Max_1(2);
            frm.setProgess_1(tr("Creando base de datos de empresa"),0);
            frm.setModal(true);
            frm.show();
            if(!crear_empresa_mysql(&frm))
                return;
            frm.setProgess_1(tr("Creando base de datos medica"),1);
            frm.setProgess_2("",0);
            if(!crear_medica_mysql(&frm))
                return;
        }

        oEmpresa.Anadir(ui->txtcCodigo->text());
        ui->botAnadir->setText("Añadir");
        ui->botAnadir->setIcon(QIcon(":/Icons/PNG/add.png"));
        CargarCamposEnEmpresa();
        oEmpresa.Guardar();
    }

    else
    {
        if(ui->btn_migrar->text() == "Migrando..." )
        {
            //TODO Migrar
        }
        else
        {
            CargarCamposEnEmpresa();
            oEmpresa.Guardar();
        }
    }
}

void FrmEmpresas::txtcPoblacion_editingFinished()
{
    ui->txtcPoblacion->setText(ui->txtcPoblacion->text().toUpper());
    if (ui->txtcCP->text().isEmpty() and !ui->txtcPoblacion->text().isEmpty())
    {
       FrmBuscarPoblacion BuscarPoblacion;
       BuscarPoblacion.setcPoblacion(ui->txtcPoblacion->text(),1);
       if(BuscarPoblacion.exec())
       {
        //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
         int nId = BuscarPoblacion.DevolverID();
         qDebug() << "nID: " <<nId;
         if(nId > 0)
         {
             QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
             QString cId;
             cId = QString::number(nId);
             qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
             qPoblacion.bindValue(":cId",cId);
             if(!qPoblacion.exec())
             {
                 /* qDebug() << qPoblacion.lastQuery();
                 qDebug() << qPoblacion.value(0).toString(); */
                 QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
             }
             else
             {
                 if (qPoblacion.next())
                 {
                     ui->txtcPoblacion->setText(qPoblacion.value(0).toString());
                     ui->txtcCP->setText(qPoblacion.value(1).toString());
                     ui->txtcProvincia->setText(qPoblacion.value(2).toString());
                     ui->txtcPais->setText("ESPAÑA");
                 }
             }
         }
       }
    }
}
void FrmEmpresas::txtcCp_editingFinished()
{
    if (!ui->txtcCP->text().isEmpty() and ui->txtcPoblacion->text().isEmpty())
    {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcCP->text(),0);
        if(BuscarPoblacion.exec())
        {

            int nId = BuscarPoblacion.DevolverID();
            //qDebug() <<nId;
            if(nId > 0)
            {
                QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
                QString cId;
                cId = QString::number(nId);
                qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
                qPoblacion.bindValue(":cId",cId);
                if(!qPoblacion.exec())
                {
                    /* qDebug() << qPoblacion.lastQuery();*/
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                }
                else
                {
                    qDebug() << "Poblacion" <<qPoblacion.value(0).toString();
                    qDebug() << qPoblacion.lastQuery() <<" ID:= " << cId;
                    if (qPoblacion.next())
                    {
                        ui->txtcCP->setText(qPoblacion.value(1).toString());
                        ui->txtcPoblacion->setText(qPoblacion.value(0).toString());
                        ui->txtcProvincia->setText(qPoblacion.value(2).toString());
                        ui->txtcPais->setText("ESPAÑA");
                    }
                }
            }
        }
    }
}

void FrmEmpresas::on_botAnadir_clicked()
{
    if(ui->botAnadir->text() == "Añadir")
    {
        ui->btn_migrar->setEnabled(false);
        ui->botAnadir->setText("Deshacer");
        ui->botAnadir->setIcon(QIcon(":/Icons/PNG/undo.png"));

        QSqlQuery get_last(QSqlDatabase::database("terra"));
        get_last.prepare("SELECT id FROM empresas ORDER BY id DESC LIMIT 1");
        if(get_last.exec())
        {
            limpiar_campos();
            if(get_last.next())
            {
                int last_id = get_last.record().value("id").toInt()  + 1;
                ui->txtcCodigo->setText(QString::number(last_id));
            }
            else
            {
                ui->txtcCodigo->setText("1");
            }
        }
    }
    else
    {
        ui->btn_migrar->setEnabled(true);
        ui->botAnadir->setText("Añadir");
        ui->botAnadir->setIcon(QIcon(":/Icons/PNG/add.png"));
        oEmpresa.Recuperar("select * from empresas where id > "+ QString::number(0),1);
        LLenarCampos();
    }
}

void FrmEmpresas::limpiar_campos()
{
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setText("");
    }
    QList<QTextEdit *> lineTxtList = this->findChildren<QTextEdit *>();
    QTextEdit *txtEdit;
    foreach (txtEdit, lineTxtList) {
        txtEdit->setText("");
    }
}

bool FrmEmpresas::crear_empresa_sqlite(copy_db_progressFrm *form)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString ruta = ui->txtRutaBd->text() + "\\";
    QString empresa = ui->txtEmpresa->text()+".sqlite";
    ruta = ruta + empresa;
    db.setDatabaseName(ruta);

    if (!db.open())
    {
        QMessageBox::critical(this,
                              tr("Error al crear base de datos"),
                              tr("Ha sido imposible crear la base de datos"),
                              tr("Aceptar"));
        return false;
    }
    bool valid = true;
    QFile file(":/Icons/sql/Db_empresa_sqlite.sql");
    if(file.open(QFile::ReadOnly))
    {
        QSqlQuery query;
        QString file_data = file.readAll();
        QStringList querys = file_data.split(";",QString::SkipEmptyParts);
        form->set_Max_2(querys.size());
        form->setProgess_2(tr("Creando tablas..."),0);
        int index = 0;
        QString _query;
        foreach(_query,querys)
        {
            _query = _query.remove("\r");
            _query = _query.remove("\n");
            if(_query == "")
                continue;
            valid &= query.exec(_query);
            index ++;
            int i = _query.indexOf("\"")+1;
            QString tabla = _query.mid(i,_query.indexOf("\"",i)-i);
            form->setProgess_2("Creando tabla: "+tabla , index);
        }
    }
    if(!valid)
    {
        db.close();
        QFile::remove(ruta);
    }
    return valid;
}

bool FrmEmpresas::crear_medica_sqlite(copy_db_progressFrm *form)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString ruta = ui->txtRutaBd->text() + "\\";
    QString empresa = ui->txtEmpresa->text()+"med.sqlite";
    ruta = ruta + empresa;
    db.setDatabaseName(ruta);

    if (!db.open())
    {
        QMessageBox::critical(0,
                              tr("Error al crear base de datos"),
                              tr("Ha sido imposible crear la base de datos"),
                              tr("Aceptar"));
        return false;
    }
    bool valid = true;
    //FIXME  Cambiar fichero
    QFile file(":/Icons/sql/Db_empresa_sqlite.sql");
    if(file.open(QFile::ReadOnly))
    {
        QSqlQuery query;
        QString file_data = file.readAll();
        QStringList querys = file_data.split(";",QString::SkipEmptyParts);
        form->set_Max_2(querys.size());
        form->setProgess_2(tr("Creando tablas..."),0);
        int index = 0;
        QString _query;
        foreach(_query,querys)
        {
            _query = _query.remove("\r");
            _query = _query.remove("\n");
            if(_query == "")
                continue;
            valid &= query.exec(_query);
            index ++;
            int i = _query.indexOf("\"")+1;
            QString tabla = _query.mid(i,_query.indexOf("\"",i)-i);
            form->setProgess_2("Creando tabla: "+tabla , index);
            qDebug() << "---------------------";
            if(!valid)
            {
            qDebug() << query.lastQuery();
            qDebug() << query.lastError();
            }
        }
    }
    else
        valid=false;
    if(!valid)
    {
        db.close();
        QFile::remove(ruta);        
    }
    return valid;
}


bool FrmEmpresas::crear_empresa_mysql(copy_db_progressFrm *form)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ui->txtcHost->text());
    db.setUserName(ui->txtcUser->text());
    db.setPassword(ui->txtcPassword->text());
    if(!ui->txtcPuerto->text().isEmpty())
        db.setPort(ui->txtcPuerto->text().toInt());

    if (!db.open())
    {
        QMessageBox::critical(this,
                              tr("Error al crear base de datos"),
                              tr("Ha sido imposible crear la base de datos"),
                              tr("Aceptar"));
        return false;
    }

    bool valid = true;
    QFile file(":/Icons/sql/Db_empresa_mysql.sql");
    if(file.open(QFile::ReadOnly))
    {
        QSqlQuery query;
        QString qs = QString("CREATE DATABASE %1;").arg(ui->txtcNombreBd->text());
        if(query.exec(qs))
        {
            db.close();
            db.setDatabaseName(ui->txtcNombreBd->text());
            db.open();
        }
        else
        {
            QMessageBox::critical(this,
                                  tr("Error al crear base de datos"),
                                  tr("Ha sido imposible crear la base de datos"),
                                  tr("Aceptar"));
            return false;
        }
        QString file_data = file.readAll();
        QStringList querys = file_data.split(";",QString::SkipEmptyParts);
        form->set_Max_2(querys.size());
        form->setProgess_2(tr("Creando tablas..."),0);
        int index = 0;
        QString _query;
        foreach(_query,querys)
        {
            _query = _query.remove("\r");
            _query = _query.remove("\n");
            if(_query.isEmpty())
                continue;
            valid &= query.exec(_query);

            index ++;
            int i = _query.indexOf("`")+1;
            QString tabla = _query.mid(i,_query.indexOf("`",i)-i);
            form->setProgess_2("Creando tabla: "+tabla , index);

            if(!valid)
                return false;            
        }
    }
    else
        valid=false;
    if(!valid)
    {
        db.close();
    }
    return valid;
}

bool FrmEmpresas::crear_medica_mysql(copy_db_progressFrm *form)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ui->txtcHostmedic->text());
    db.setUserName(ui->txtcUserMedic->text());
    db.setPassword(ui->txtcPasswordBdMedic->text());

    if(!ui->txtcPuerto->text().isEmpty())
        db.setPort(ui->txtcPuerto->text().toInt());

    if (!db.open())
    {
        QMessageBox::critical(this,
                              tr("Error al crear base de datos"),
                              tr("Ha sido imposible crear la base de datos"),
                              tr("Aceptar"));
        return false;
    }
    bool valid = true;
    QFile file(":/Icons/sql/Db_empresa_medica_mysql.sql");
    if(file.open(QFile::ReadOnly))
    {
        QSqlQuery query;
        QString qs = QString("CREATE DATABASE %1;").arg(ui->txtcNombreBdMedic->text());
        if(query.exec(qs))
        {
            db.close();
            db.setDatabaseName(ui->txtcNombreBdMedic->text());
            db.open();
        }
        else
        {
            QMessageBox::critical(this,
                                  tr("Error al crear base de datos"),
                                  tr("Ha sido imposible crear la base de datos"),
                                  tr("Aceptar"));
            return false;
        }
        QString file_data = file.readAll();
        QStringList querys = file_data.split(";",QString::SkipEmptyParts);

        form->set_Max_2(querys.size());
        form->setProgess_2(tr("Creando tablas..."),0);
        int index = 0;

        QString _query;
        foreach(_query,querys)
        {
            _query = _query.remove("\r");
            _query = _query.remove("\n");
            if(_query.isEmpty())
                continue;

            index ++;
            int i = _query.indexOf("`")+1;
            QString tabla = _query.mid(i,_query.indexOf("`",i)-i);
            form->setProgess_2("Creando tabla: "+tabla , index);

            valid &= query.exec(_query);
            if(!valid)
                return false;
        }
    }
    else
        valid=false;
    if(!valid)
    {
        db.close();
    }
    return valid;
}

void FrmEmpresas::borrar_mysql()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(oEmpresa.getcHost());
    db.setUserName(oEmpresa.getcUser());
    db.setPassword(oEmpresa.getcContrasena());
    if(!oEmpresa.getcPuerto().isEmpty())
        db.setPort(oEmpresa.getcPuerto().toInt());

    db.transaction();
    bool valid = true;

    if(db.open())
    {
        QSqlQuery query;
        query.prepare("DROP DATABASE "+oEmpresa.getcNombreBD());
        if(!query.exec())
        {
            qDebug() << query.lastQuery();
            qDebug() << query.lastError();
            valid = false;
        }
        query.prepare("DROP DATABASE "+oEmpresa.getcNombreBDMed());
        if(!query.exec())
        {
            qDebug() << query.lastQuery();
            qDebug() << query.lastError();
            valid = false;
        }
    }
    else
        valid = false;

    if (valid)
    {
        if(oEmpresa.Borrar(oEmpresa.getid()))
            db.commit();
        else
            db.rollback();
    }
    else
    {
        db.rollback();
        QMessageBox::critical(this,
                              tr("Error al borrar base de datos"),
                              tr("Ha sido imposible borrar la base de datos"),
                              tr("Aceptar"));
    }

}

void FrmEmpresas::borrar_sqlite()
{
    QFile::remove(oEmpresa.getcRutaBDSqLite());
    oEmpresa.Borrar(oEmpresa.getid());
}

void FrmEmpresas::llenar_user(QSqlRecord record)
{
    ui->txt_id_user->setText(record.value(0).toString());
    ui->txt_nombre_user->setText(record.value(1).toString());
    ui->txt_pass_user->setText(record.value(2).toString());
    ui->spin_nacceso_user->setValue(record.value(3).toInt());
    ui->txt_categoria_user->setText(record.value(4).toString());
}

void FrmEmpresas::on_btn_ruta_db_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    ui->txtRutaBd->setText(dir);
}

void FrmEmpresas::on_txtcDriver_currentIndexChanged(int index)
{
    ui->sqlite_frame->setEnabled(index == 0);
    ui->mysql_frame->setEnabled(!(index == 0));
    if(index == 0)
        ui->btn_migrar->setText("Migrar a BD Multipuesto");
}


void FrmEmpresas::on_btn_migrar_clicked()
{
    ui->txtcDriver->setCurrentIndex(1);
    ui->btn_migrar->setText("Migrando...");
}

void FrmEmpresas::on_botBorrar_clicked()
{
    if(QMessageBox::question(this,tr("¿Está seguro?"),
                          tr("Esta acción no tiene vuelta atras, ¿Desea continuar?"),
                          tr("No"),tr("Si")) == QMessageBox::Accepted)
    {
        qDebug() << "si";
        if(oEmpresa.getcDriverBD() == "QSQLITE")
            borrar_sqlite();
        else
            borrar_mysql();
    }
}

void FrmEmpresas::on_botAnadir_user_clicked()
{
    if(ui->botAnadir_user->text() == "Añadir")
    {
        ui->botAnadir_user->setText("Deshacer");
        ui->botAnadir_user->setIcon(QIcon(":/Icons/PNG/undo.png"));

        QSqlQuery get_last(QSqlDatabase::database("terra"));
        get_last.prepare("SELECT * FROM usuarios ORDER BY id DESC LIMIT 1");
        if(get_last.exec())
        {
            limpiar_campos();
            if(get_last.next())
            {
                int last_id = get_last.record().value("id").toInt()  + 1;
                ui->txt_id_user->setText(QString::number(last_id));
            }
            else
            {
                ui->txt_id_user->setText("1");
            }
        }
    }
    else
    {
        ui->botAnadir_user->setText("Añadir");
        ui->botAnadir_user->setIcon(QIcon(":/Icons/PNG/add.png"));
    }
}

void FrmEmpresas::on_botSiguiente_user_clicked()
{
    QSqlQuery next_user(QSqlDatabase::database("terra"));
    QString sql = QString("SELECT * FROM usuarios WHERE id > %1 LIMIT 1 ")
            .arg(ui->txt_id_user->text());
    next_user.prepare(sql);
    if(next_user.exec())
    {
        if(next_user.next())
            llenar_user(next_user.record());
        else
            QMessageBox::information(this,tr("Final de archivo"),
                                     tr("No exsten mas usuarios"),tr("Aceptar"));

    }
}

void FrmEmpresas::on_botAnterior_user_clicked()
{
    QSqlQuery next_user(QSqlDatabase::database("terra"));
    QString sql = QString("SELECT * FROM usuarios WHERE id < %1 ORDER BY id DESC LIMIT 1")
            .arg(ui->txt_id_user->text());
    next_user.prepare(sql);
    if(next_user.exec())
    {
        if(next_user.next())
            llenar_user(next_user.record());
        else
            QMessageBox::information(this,tr("Inicio de archivo"),
                                     tr("Este es el primer usuario"),tr("Aceptar"));

    }
}

void FrmEmpresas::on_botBuscar_user_clicked()
{
    //TODO buscar user
}

void FrmEmpresas::on_botGuardar_user_clicked()
{
    if(ui->botAnadir_user->text() == "Añadir")
    {
        //TODO modificar user actual
    }
    else
    {

        QSqlQuery add_user(QSqlDatabase::database("terra"));
        /*add_user.prepare("INSERT INTO usuarios (id,nombre,contrasena,nivelacceso,categoria) "
                         "VALUES (?1,?2,?3,?4,?5)");
         Id es un campo autoincremental y no se debe sobreescribir ya que puede dar problemas, siendo la propia bd
           la  que debe ser la encargada de la gestión de ese campo

        add_user.bindValue("?1",ui->txt_id_user->text());
        add_user.bindValue("?2",ui->txt_nombre_user->text());
        add_user.bindValue("?3",ui->txt_pass_user->text());
        add_user.bindValue("?4",ui->spin_nacceso_user->value());
        add_user.bindValue("?5",ui->txt_categoria_user->text());*/

        // NOTE - Así no falla la entrada de datos a la bd en linux, prueba a ver si funciona bien en windows.

        add_user.prepare("INSERT INTO usuarios (id,nombre,contrasena,nivelacceso,categoria) "
                                 "VALUES (:id,:nombre,:contrasena,:nivelacceso,:categoria)");

                add_user.bindValue(":id",ui->txt_id_user->text());
                add_user.bindValue(":nombre",ui->txt_nombre_user->text());
                add_user.bindValue(":contrasena",ui->txt_pass_user->text());
                add_user.bindValue(":nivelacceso",ui->spin_nacceso_user->value());
                add_user.bindValue(":categoria",ui->txt_categoria_user->text());


        if(add_user.exec())
            QMessageBox::information(this,tr("Guardado"),tr("Se ha guardado con exito"),tr("Aceptar"));
        else
            QMessageBox::critical(this,tr("Error"),tr("No se ha podido guardar el nuevo usuario.\n%1").arg(add_user.lastError().text()),tr("Aceptar"));

        ui->botAnadir_user->setText("Añadir");
        ui->botAnadir_user->setIcon(QIcon(":/Icons/PNG/add.png"));
    }
}

void FrmEmpresas::on_botBorrar_user_clicked()
{
    //TODO borrar user
}
