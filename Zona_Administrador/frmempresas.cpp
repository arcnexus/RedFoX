#include "frmempresas.h"
#include "ui_frmempresas.h"

#include "../Busquedas/frmbuscarpoblacion.h"

FrmEmpresas::FrmEmpresas(QWidget *parent) :
    MayaModule(ModuleZone(),ModuleName(),parent),
    ui(new Ui::FrmEmpresas),
    toolButton(tr("Empresas"),":/Icons/PNG/empresa.png",this),
    menuButton(QIcon(":/Icons/PNG/empresa.png"),tr("Empresa"),this)
{
    ui->setupUi(this);
    ui->txtMensaje->setText("ATENCION: Modifique con cuidado. Cambiar estos\n valores puede hacer que el programa deje\n de funcionar");

    connect(ui->botCerrar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->txtcPoblacion,SIGNAL(editingFinished()),this,SLOT(txtcPoblacion_editingFinished()));
    connect(ui->txtcCP,SIGNAL(editingFinished()),this,SLOT(txtcCp_editingFinished()));
    //------------------
    // LLeno divisas
    //------------------
    QSqlQueryModel *modelDivisas = new QSqlQueryModel(this);
    modelDivisas->setQuery("select moneda from monedas",QSqlDatabase::database("Maya"));
    ui->cboDivisas->setModel(modelDivisas);

    //----------------
    // LLeno Tarifas
    //----------------
    QSqlQueryModel *modelTarifas = new QSqlQueryModel(this);
    modelTarifas->setQuery("select descripcion from codigotarifa",QSqlDatabase::database("Maya"));
    ui->cboTarifa->setModel(modelTarifas);

    on_botSiguiente_clicked();
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
    ui->txtnDigitosCuentas->setValue(oEmpresa.getnDigitosCuentas());
    ui->txtcCuentaCliente->setText(oEmpresa.getcCuentaClientes());
    ui->txtcCuentaProveedores->setText(oEmpresa.getcCuentaProveedores());
    ui->txtcCuentaAcreedores->setText(oEmpresa.getcCuentaAcreedores());
    QString cDivisa = Configuracion_global->Devolver_moneda(oEmpresa.id_divisa);
    int nIndex = ui->cboDivisas->findText(cDivisa);
    ui->cboDivisas->setCurrentIndex(nIndex);
    ui->chkContabilidad->setChecked(oEmpresa.contabilidad);
    ui->txtConsultas->setValue( oEmpresa.consultas);
    int index =ui->cboPrimer_dia_laborable->findText(oEmpresa.primer_dia_laborable);
    ui->cboPrimer_dia_laborable->setCurrentIndex(index);
    index = ui->cbo_ultimo_dia_semana->findText(oEmpresa.ultimo_dia_laborable);
    ui->cbo_ultimo_dia_semana->setCurrentIndex(index);
    ui->txt_horario_primer_dia->setText(oEmpresa.horario_primer_dia);
    ui->txt_horario_dia_normal->setText(oEmpresa.horario_dia_normal);
    ui->txt_horario_ultimo_dia->setText(oEmpresa.horario_ultimo_dia);

    ui->txtHost_contabilidad->setText(oEmpresa.HostBD_contabilidad);
    ui->txtNombre_BDConta->setText(oEmpresa.NombreBD_contabilidad);
    ui->txtUsuario_contabilidad->setText(oEmpresa.UsuarioBD_contabilidad);
    ui->txtPasswordBDConta->setText(oEmpresa.ContrasenaBD_contabilidad);
    ui->txtPuertoBDConta->setText(oEmpresa.puertoDB_contabilidad);
    ui->txtRutaBDConta->setText(oEmpresa.RutaBD_Contabilidad_sqlite);
    ui->chk_ticket_factura->setChecked(oEmpresa.ticket_factura);
    index = ui->cboTarifa->findText(Configuracion_global->Devolver_tarifa(oEmpresa.id_tarifa_predeterminada));
    ui->cboTarifa->setCurrentIndex(index);
    ui->chk_upate_divisas->setChecked(oEmpresa.actualizardivisas);
    ui->dboEmpresaMedica->setChecked(oEmpresa.empresa_medica);
    ui->cboGestionInternacional->setChecked(oEmpresa.empresa_internacional);
    ui->txtCuenta_venta_mercaderias->setText(oEmpresa.cCuenta_venta_mercaderias);
    ui->txtCuenta_venta_servicios->setText(oEmpresa.cCuenta_venta_servicios);
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
    oEmpresa.cCuenta_venta_mercaderias = ui->txtCuenta_venta_mercaderias->text();
    oEmpresa.cCuenta_venta_servicios = ui->txtCuenta_venta_servicios->text();
    oEmpresa.id_divisa = Configuracion_global->Devolver_id_moneda(ui->cboDivisas->currentText());
    oEmpresa.contabilidad = ui->chkContabilidad->isChecked();
    oEmpresa.consultas = ui->txtConsultas->value();

    oEmpresa.primer_dia_laborable = ui->cboPrimer_dia_laborable->currentText();
    oEmpresa.ultimo_dia_laborable = ui->cbo_ultimo_dia_semana->currentText();
    oEmpresa.horario_dia_normal = ui->txt_horario_dia_normal->text();
    oEmpresa.horario_primer_dia = ui->txt_horario_primer_dia->text();
    oEmpresa.horario_ultimo_dia = ui->txt_horario_ultimo_dia->text();

    oEmpresa.HostBD_contabilidad = ui->txtHost_contabilidad->text();
    oEmpresa.NombreBD_contabilidad = ui->txtNombre_BDConta->text();
    oEmpresa.UsuarioBD_contabilidad = ui->txtUsuario_contabilidad->text();
    oEmpresa.ContrasenaBD_contabilidad = ui->txtPasswordBDConta->text();
    oEmpresa.puertoDB_contabilidad = ui->txtPuertoBDConta->text();
    oEmpresa.DriverDB_contabilidad = ui->txtcDriver->currentText();
    oEmpresa.RutaBD_Contabilidad_sqlite = ui->txtRutaBDConta->text();
    oEmpresa.ticket_factura = ui->chk_ticket_factura->isChecked();
    oEmpresa.id_tarifa_predeterminada = Configuracion_global->Devolver_id_tarifa(ui->cboTarifa->currentText());
    oEmpresa.actualizardivisas = ui->chk_upate_divisas->isChecked();
    oEmpresa.empresa_internacional = ui->cboGestionInternacional->isChecked();
    oEmpresa.empresa_medica = ui->dboEmpresaMedica->isChecked();


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
    if (ui->txtcCP->text().isEmpty() && !ui->txtcPoblacion->text().isEmpty())
    {
       FrmBuscarPoblacion BuscarPoblacion;
       BuscarPoblacion.setcPoblacion(ui->txtcPoblacion->text(),1);
       if(BuscarPoblacion.exec())
       {
        //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
         int nId = BuscarPoblacion.DevolverID();
         if(nId > 0)
         {
             QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
             QString cId;
             cId = QString::number(nId);
             qPoblacion.prepare("select poblacion, CP, provincia from poblaciones where id = :cId");
             qPoblacion.bindValue(":cId",cId);
             if(!qPoblacion.exec())
             {
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
    if (!ui->txtcCP->text().isEmpty() && ui->txtcPoblacion->text().isEmpty())
    {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcCP->text(),0);
        if(BuscarPoblacion.exec())
        {

            int nId = BuscarPoblacion.DevolverID();
            if(nId > 0)
            {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cId;
                cId = QString::number(nId);
                qPoblacion.prepare("select poblacion, CP,provincia from poblaciones where id = :cId");
                qPoblacion.bindValue(":cId",cId);
                if(!qPoblacion.exec())
                {
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                }
                else
                {
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

        QSqlQuery get_last(QSqlDatabase::database("Maya"));
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
        if(oEmpresa.getcDriverBD() == "QSQLITE")
            borrar_sqlite();
        else
            borrar_mysql();
    }
}
