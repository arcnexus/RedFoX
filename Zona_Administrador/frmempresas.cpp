#include "frmempresas.h"
#include "ui_frmempresas.h"

#include "../Busquedas/frmbuscarpoblacion.h"
#include "addgroupfrom.h"
FrmEmpresas::FrmEmpresas(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmEmpresas),
    toolButton(tr("Empresas"),":/Icons/PNG/empresa.png",this),
    menuButton(QIcon(":/Icons/PNG/empresa.png"),tr("Empresa"),this)
{
    ui->setupUi(this);
    ui->txtMensaje->setText("ATENCION: Modifique con cuidado. Cambiar estos\n valores puede hacer que el programa deje\n de funcionar");

    connect(ui->botCerrar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->txtpoblacion,SIGNAL(editingFinished()),this,SLOT(txtpoblacion_editingFinished()));
    connect(ui->txtcp,SIGNAL(editingFinished()),this,SLOT(txtcp_editingFinished()));
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
    ui->txtcodigo->setText(oEmpresa.getcodigo());
    ui->txtEmpresa->setText(oEmpresa.getnombre());
    ui->txtRutaBd->setText(oEmpresa.getruta_bd_sqlite());
    ui->txtcHost->setText(oEmpresa.getcHost());
    ui->txtnombre_bd->setText(oEmpresa.getnombre_bd());
    ui->txtcUser->setText(oEmpresa.getcUser());
    ui->txtcPassword->setText(oEmpresa.getcContrasena());
    ui->txtcPuerto->setText(oEmpresa.getcPuerto());
    int indice=ui->txtcDriver->findText(oEmpresa.getcDriverBD());
    if(indice!=-1)
       ui->txtcDriver->setCurrentIndex(indice);

    ui->txtRutaBd->setText(oEmpresa.getruta_bd_sqlite());
    ui->txtcHostmedic->setText(oEmpresa.getcHostMed());
    ui->txtnombre_bdMedic->setText(oEmpresa.getnombre_bdMed());
    ui->txtcUserMedic->setText(oEmpresa.getcUserMed());
    ui->txtcPasswordBdMedic->setText(oEmpresa.getcContrasenaMed());
    ui->txtcPuertoMedic->setText(oEmpresa.getcPuertoMed());

    //indice=ui->txtcDriverMedica->findText(oEmpresa.getcDriverBDMed());
    //if(indice!=-1)
    //ui->txtcDriverMedica->setCurrentIndex(indice);


    ui->txtdireccion1->setText(oEmpresa.getdireccion1());
    ui->txtcp->setText(oEmpresa.getcp());
    ui->txtpoblacion->setText(oEmpresa.getpoblacion());
    ui->txtprovincia->setText(oEmpresa.getprovincia());
    ui->txtpais->setText(oEmpresa.getpais());
    ui->txttelefono1->setText(oEmpresa.gettelefono1());
    ui->txttelefono2->setText(oEmpresa.gettelefono2());
    ui->txtfax->setText(oEmpresa.getfax());
    ui->txtcMail->setText(oEmpresa.getcMail());
    ui->txtweb->setText(oEmpresa.getweb());
    ui->txtcif->setText(oEmpresa.getcif());
    ui->txtcInscripcion->setText(oEmpresa.getcInscripcion());
    ui->txtcCometarioAlbaran->setText(oEmpresa.getccomentario_albaran());
    ui->txtccomentario_factura->setText(oEmpresa.getccomentario_factura());
    ui->txtccomentario_ticket->setText(oEmpresa.getccomentario_ticket());
    ui->txtejercicio->setValue(oEmpresa.getejercicio());
    ui->txtndigitos_factura->setText(QString::number(oEmpresa.getndigitos_factura()));
    ui->txtserieFactura->setText(oEmpresa.getserie());
    ui->txtdigitos_cuentas->setValue(oEmpresa.getdigitos_cuentas());
    ui->txtcuentaCliente->setText(oEmpresa.getcuenta_clientes());
    ui->txtcuenta_proveedores->setText(oEmpresa.getcuenta_proveedores());
    ui->txtcuenta_acreedores->setText(oEmpresa.getcuenta_acreedores());
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
    ui->txtNombre_BDConta->setText(oEmpresa.nombre_bd_contabilidad);
    ui->txtUsuario_contabilidad->setText(oEmpresa.UsuarioBD_contabilidad);
    ui->txtpassword_bd_conta->setText(oEmpresa.ContrasenaBD_contabilidad);
    ui->txtPuertoBDConta->setText(oEmpresa.puertoDB_contabilidad);
    ui->txtruta_bd_conta->setText(oEmpresa.RutaBD_Contabilidad_sqlite);
    ui->chk_ticket_factura->setChecked(oEmpresa.ticket_factura);
    index = ui->cboTarifa->findText(Configuracion_global->Devolver_tarifa(oEmpresa.id_tarifa_predeterminada));
    ui->cboTarifa->setCurrentIndex(index);
    ui->chk_upate_divisas->setChecked(oEmpresa.actualizar_divisas);
    ui->dboEmpresaMedica->setChecked(oEmpresa.empresa_medica);
    ui->cboGestionInternacional->setChecked(oEmpresa.empresa_internacional);
    ui->txtCuenta_venta_mercaderias->setText(oEmpresa.cuenta_venta_mercaderias);
    ui->txtCuenta_venta_servicios->setText(oEmpresa.cuenta_venta_servicios);
    ui->ivarepercutido1->setText(oEmpresa.cuenta_iva_repercutido1);
    ui->ivarepercutido2->setText(oEmpresa.cuenta_iva_repercutido2);
    ui->ivarepercutido3->setText(oEmpresa.cuenta_iva_repercutido3);
    ui->ivarepercutido4->setText(oEmpresa.cuenta_iva_repercutido4);
    ui->ivarepercutidore1->setText(oEmpresa.cuenta_iva_repercutido_re1);
    ui->ivarepercutidore2->setText(oEmpresa.cuenta_iva_repercutido_re2);
    ui->ivarepercutidore3->setText(oEmpresa.cuenta_iva_repercutido_re3);
    ui->ivarepercutidore4->setText(oEmpresa.cuenta_iva_repercutido_re4);
    ui->ivasoportado1->setText(oEmpresa.cuenta_iva_soportado1);
    ui->ivasoportado2->setText(oEmpresa.cuenta_iva_soportado2);
    ui->ivasoportado3->setText(oEmpresa.cuenta_iva_soportado3);
    ui->ivasoportado4->setText(oEmpresa.cuenta_iva_soportado4);
    ui->ivasoportadore1->setText(oEmpresa.cuenta_iva_soportado_re1);
    ui->ivasoportadore2->setText(oEmpresa.cuenta_iva_soportado_re2);
    ui->ivasoportadore3->setText(oEmpresa.cuenta_iva_soportado_re3);
    ui->ivasoportadore4->setText(oEmpresa.cuenta_iva_soportado_re4);

}

void FrmEmpresas::CargarCamposEnEmpresa()
{
    oEmpresa.setcodigo(ui->txtcodigo->text());
    oEmpresa.setnombre(ui->txtEmpresa->text());
    oEmpresa.setruta_bd_sqlite(ui->txtRutaBd->text());
    oEmpresa.setcHost(ui->txtcHost->text());
    oEmpresa.setnombre_bd(ui->txtnombre_bd->text());
    oEmpresa.setcUser(ui->txtcUser->text());
    oEmpresa.setcContrasena(ui->txtcPassword->text());
    oEmpresa.setcPuerto(ui->txtcPuerto->text());
    oEmpresa.setcDriverBD(ui->txtcDriver->currentText());

    oEmpresa.setcHostMed(ui->txtcHostmedic->text());
    oEmpresa.setnombre_bdMed(ui->txtnombre_bdMedic->text());
    oEmpresa.setcUserMed(ui->txtcUserMedic->text());
    oEmpresa.setcContrasenaMed(ui->txtcPasswordBdMedic ->text());
    oEmpresa.setcPuertoMed(ui->txtcPuertoMedic->text());
    oEmpresa.setcDriverBDMed(ui->txtcDriver->currentText());



    oEmpresa.setserie(ui->txtserieFactura->text());
    oEmpresa.setdireccion1(ui->txtdireccion1->text());
    oEmpresa.setcp(ui->txtcp->text());
    oEmpresa.setpoblacion(ui->txtpoblacion->text());
    oEmpresa.setprovincia(ui->txtprovincia->text());
    oEmpresa.setpais(ui->txtpais->text());
    oEmpresa.settelefono1(ui->txttelefono1->text());
    oEmpresa.settelefono2(ui->txtpais->text());
    oEmpresa.setfax(ui->txtfax->text());
    oEmpresa.setcMail(ui->txtcMail->text());
    oEmpresa.setweb(ui->txtweb->text());
    oEmpresa.setcif(ui->txtcif->text());
    oEmpresa.setcInscripcion(ui->txtcInscripcion->text());
    oEmpresa.setcComemtarioAlbaran(ui->txtcCometarioAlbaran->toPlainText());
    oEmpresa.setccomentario_factura(ui->txtccomentario_factura->toPlainText());
    oEmpresa.setccomentario_ticket(ui->txtccomentario_ticket->toPlainText());
    oEmpresa.setejercicio(ui->txtejercicio->text().toInt());
    oEmpresa.setdigitos_cuentas(ui->txtdigitos_cuentas->text().toInt());
    oEmpresa.setcodigo_cuenta_clientes(ui->txtcuentaCliente->text());
    oEmpresa.setcodigoCuentaProveedor(ui->txtcuenta_proveedores->text());
    oEmpresa.setcodigo_cuenta_acreedores(ui->txtcuenta_acreedores->text());
    oEmpresa.cuenta_venta_mercaderias = ui->txtCuenta_venta_mercaderias->text();
    oEmpresa.cuenta_venta_servicios = ui->txtCuenta_venta_servicios->text();
    oEmpresa.id_divisa = Configuracion_global->Devolver_id_moneda(ui->cboDivisas->currentText());
    oEmpresa.contabilidad = ui->chkContabilidad->isChecked();
    oEmpresa.consultas = ui->txtConsultas->value();

    oEmpresa.primer_dia_laborable = ui->cboPrimer_dia_laborable->currentText();
    oEmpresa.ultimo_dia_laborable = ui->cbo_ultimo_dia_semana->currentText();
    oEmpresa.horario_dia_normal = ui->txt_horario_dia_normal->text();
    oEmpresa.horario_primer_dia = ui->txt_horario_primer_dia->text();
    oEmpresa.horario_ultimo_dia = ui->txt_horario_ultimo_dia->text();

    oEmpresa.HostBD_contabilidad = ui->txtHost_contabilidad->text();
    oEmpresa.nombre_bd_contabilidad = ui->txtNombre_BDConta->text();
    oEmpresa.UsuarioBD_contabilidad = ui->txtUsuario_contabilidad->text();
    oEmpresa.ContrasenaBD_contabilidad = ui->txtpassword_bd_conta->text();
    oEmpresa.puertoDB_contabilidad = ui->txtPuertoBDConta->text();
    oEmpresa.driver_db_contabilidad = ui->txtcDriver->currentText();
    oEmpresa.RutaBD_Contabilidad_sqlite = ui->txtruta_bd_conta->text();
    oEmpresa.ticket_factura = ui->chk_ticket_factura->isChecked();
    oEmpresa.id_tarifa_predeterminada = Configuracion_global->Devolver_id_tarifa(ui->cboTarifa->currentText());
    oEmpresa.actualizar_divisas = ui->chk_upate_divisas->isChecked();
    oEmpresa.empresa_internacional = ui->cboGestionInternacional->isChecked();
    oEmpresa.empresa_medica = ui->dboEmpresaMedica->isChecked();
    oEmpresa.cuenta_iva_repercutido1 = ui->ivarepercutido1->text();
    oEmpresa.cuenta_iva_repercutido2 = ui->ivarepercutido2->text();
    oEmpresa.cuenta_iva_repercutido3 = ui->ivarepercutido3->text();
    oEmpresa.cuenta_iva_repercutido4 = ui->ivarepercutido4->text();
    oEmpresa.cuenta_iva_soportado1 = ui->ivasoportado1->text();
    oEmpresa.cuenta_iva_soportado2 = ui->ivasoportado2->text();
    oEmpresa.cuenta_iva_soportado3 = ui->ivasoportado3->text();
    oEmpresa.cuenta_iva_soportado4 = ui->ivasoportado4->text();
    oEmpresa.cuenta_iva_repercutido_re1 = ui->ivarepercutidore1->text();
    oEmpresa.cuenta_iva_repercutido_re2 = ui->ivarepercutidore2->text();
    oEmpresa.cuenta_iva_repercutido_re3 = ui->ivarepercutidore3->text();
    oEmpresa.cuenta_iva_repercutido_re4 = ui->ivarepercutidore4->text();
    oEmpresa.cuenta_iva_soportado_re1 = ui->ivasoportadore1->text();
    oEmpresa.cuenta_iva_soportado_re2 = ui->ivasoportadore2->text();
    oEmpresa.cuenta_iva_soportado_re3 = ui->ivasoportadore3->text();
    oEmpresa.cuenta_iva_soportado_re4 = ui->ivasoportadore4->text();
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

        oEmpresa.Anadir(ui->txtcodigo->text());
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

void FrmEmpresas::txtpoblacion_editingFinished()
{
    ui->txtpoblacion->setText(ui->txtpoblacion->text().toUpper());
    if (ui->txtcp->text().isEmpty() && !ui->txtpoblacion->text().isEmpty())
    {
       FrmBuscarPoblacion BuscarPoblacion;
       BuscarPoblacion.setpoblacion(ui->txtpoblacion->text(),1);
       if(BuscarPoblacion.exec())
       {
        //  BuscarPoblacion.setpoblacion(ui->txtcp->text(),0);
         int nid = BuscarPoblacion.Devolverid();
         if(nid > 0)
         {
             QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
             QString cid;
             cid = QString::number(nid);
             qPoblacion.prepare("select poblacion, CP, provincia from poblaciones where id = :cid");
             qPoblacion.bindValue(":cid",cid);
             if(!qPoblacion.exec())
             {
                 QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
             }
             else
             {
                 if (qPoblacion.next())
                 {
                     ui->txtpoblacion->setText(qPoblacion.value(0).toString());
                     ui->txtcp->setText(qPoblacion.value(1).toString());
                     ui->txtprovincia->setText(qPoblacion.value(2).toString());
                     ui->txtpais->setText("ESPAÑA");
                 }
             }
         }
       }
    }
}
void FrmEmpresas::txtcp_editingFinished()
{
    if (!ui->txtcp->text().isEmpty() && ui->txtpoblacion->text().isEmpty())
    {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setpoblacion(ui->txtcp->text(),0);
        if(BuscarPoblacion.exec())
        {

            int nid = BuscarPoblacion.Devolverid();
            if(nid > 0)
            {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cid;
                cid = QString::number(nid);
                qPoblacion.prepare("select poblacion, CP,provincia from poblaciones where id = :cid");
                qPoblacion.bindValue(":cid",cid);
                if(!qPoblacion.exec())
                {
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                }
                else
                {
                    if (qPoblacion.next())
                    {
                        ui->txtcp->setText(qPoblacion.value(1).toString());
                        ui->txtpoblacion->setText(qPoblacion.value(0).toString());
                        ui->txtprovincia->setText(qPoblacion.value(2).toString());
                        ui->txtpais->setText("ESPAÑA");
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
                ui->txtcodigo->setText(QString::number(last_id));
            }
            else
            {
                ui->txtcodigo->setText("1");
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
        QString qs = QString("CREATE DATABASE %1;").arg(ui->txtnombre_bd->text());
        if(query.exec(qs))
        {
            db.close();
            db.setDatabaseName(ui->txtnombre_bd->text());
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
        QString qs = QString("CREATE DATABASE %1;").arg(ui->txtnombre_bdMedic->text());
        if(query.exec(qs))
        {
            db.close();
            db.setDatabaseName(ui->txtnombre_bdMedic->text());
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
        query.prepare("DROP DATABASE "+oEmpresa.getnombre_bd());
        if(!query.exec())
        {
            qDebug() << query.lastQuery();
            qDebug() << query.lastError();
            valid = false;
        }
        query.prepare("DROP DATABASE "+oEmpresa.getnombre_bdMed());
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
    QFile::remove(oEmpresa.getruta_bd_sqlite());
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

void FrmEmpresas::on_addGrupo_clicked()
{
    addGroupFrom f(this);
    if(f.exec() == QDialog::Rejected)
        return;

    QString nombre = "";
    QString bd_name = QString("grupo%1").arg(nombre);
    QString host;
    QString user;
    QString pass;
    int port;
    QSqlQuery q(QSqlDatabase::database("Maya"));
    q.prepare("INSERT INTO `mayaglobal`.`grupos` "
              "(`nombre`, `bd_name`, `bd_driver`, `bd_ruta`, `bd_host`,"
              "`bd_user`, `bd_pass`, `bd_port`) "
              "VALUES "
              "(`:nombre`, `:bd_name`, `QMYSQL`, ` `, `:bd_host`,"
              "`:bd_user`, `:bd_pass`, `:bd_port`);"
              );
   /* q.bindValue(":nombre",x);
    q.bindValue(":bd_name",x);
    q.bindValue(":bd_host",x);
    q.bindValue(":bd_user",x);
    q.bindValue(":bd_pass",x);
    q.bindValue(":bd_port",x);*/
}
