#include "frmconfigmaya.h"
#include "ui_frmconfigmaya.h"
#include <QtSql>
#include <QSqlDatabase>
#include "../configuracion.h"


//Configuracion * Configuracion_global = 0;

frmConfigmaya::frmConfigmaya(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmConfigmaya),
    toolButton(tr("Configuracion\nGeneral"),":/Icons/PNG/Config.png",this),
    menuButton(QIcon(":/Icons/PNG/Config.png"),tr("Configuracion General"),this)
{
    Configuracion_global->Cargar_iva();
    //Configuracion_global->Cargar_paises();
    ui->setupUi(this);
    QSqlQueryModel *qUsers = new QSqlQueryModel(this);
    qUsers->setQuery("select nombre from usuarios",Configuracion_global->groupDB);
    ui->cboUsuarioActivo->setModel(qUsers);


    ui->cboPaises->setModel(Configuracion_global->paises_model);
   // ui->cboPaises->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    QFile f(qApp->applicationDirPath()+"/MayaConfig.ini");
    if(!f.exists())
        return;
    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
    ui->txtHost->setText(settings.value("cHostBDMaya").toString());
    ui->txtUsuario->setText(Configuracion::DeCrypt(settings.value("cUserBDMaya").toString()));
    ui->txtPassword->setText(Configuracion::DeCrypt(settings.value("cPasswordBDMaya").toString()));
    ui->txtPuerto->setText(settings.value("cPuertoDBMaya").toString());
    ui->txtdireccionBD->setText(settings.value("cRutaDBMaya").toString());


    ui->txtHostWeb->setText(settings.value("hostTiendaWeb").toString());
    ui->txtUsuarioDBweb->setText(Configuracion::DeCrypt(settings.value("usuarioTiendaWeb").toString()));
    ui->txtPasswordDBWeb->setText(Configuracion::DeCrypt(settings.value("Pass_web").toString()));
    ui->txtPuerto_DBWeb->setText(settings.value("puertoTiendaWeb").toString());
    ui->txtnombre_bdWeb->setText((settings.value("nombre_bdTiendaWeb").toString()));

    int nindex = ui->cboPaises->findText(settings.value("pais").toString());
    if (nindex > -1)
        ui->cboPaises->setCurrentIndex(nindex);


    ui->spindigitos_factura->setValue(settings.value("ndigitos_factura").toInt());


    if(settings.value("lProfesional")==1)
        ui->chkProfesional->setChecked(true);
    else
        ui->chkProfesional->setChecked(false);

    ui->txtPorcIRPF->setText(settings.value("irpf").toString());

    ui->spidigitos_cuentaContable->setValue(settings.value("digitos_cuentas").toInt());
    ui->txtCuentaClientes->setText(settings.value("cuenta_clientes").toString());
    ui->txtCuentaProveedores->setText(settings.value("cuenta_proveedores").toString());
    ui->txtCuentaAcreedores->setText(settings.value("cuenta_acreedores").toString());
    ui->txtcuenta_cobros->setText(settings.value("cuenta_cobros").toString());
    ui->txtcuenta_pagos->setText(settings.value("cuenta_pagos").toString());
    nindex = ui->cboUsuarioActivo->findText(settings.value("cUsuarioActivo").toString());
    if (nindex >-1)
        ui->cboUsuarioActivo->setCurrentIndex(nindex);
    ui->txtContrasenaActiva->setText(settings.value("contrasenaactiva").toString());
    ui->txtclaveV1_1->setText(settings.value("Clave1").toString());
    ui->txtclaveV1_2->setText(settings.value("Clave2").toString());
    ui->txtclaveV2_1->setText(settings.value("Clave3").toString());
    ui->txtclaveV2_2->setText(settings.value("Clave4").toString());
    ui->txtclaveV3_1->setText(settings.value("Clave5").toString());
    ui->txtclaveV3_2->setText(settings.value("Clave6").toString());
    ui->txtclaveV4_1->setText(settings.value("Clave7").toString());
    ui->txtclaveV4_2->setText(settings.value("Clave8").toString());
    ui->txtHostBD_MediTec->setText(settings.value("HostDB_MediTec").toString());
    ui->txtnombre_bd_MediTec->setText(settings.value("NombreDB_MediTec").toString());
    ui->txtUsuarioDB_MediTec->setText(settings.value("UsuarioDB_MediTec").toString());
    ui->txtPasswordDB_MediTec->setText(settings.value("PasswordDB_MediTec").toString());
    ui->txtPuerto_DB_MediTec->setText(settings.value("PuertoDB_MediTec").toString());
    if(settings.value("cDriverBDMaya").toString() =="QMYSQL")
        ui->radMysql->setChecked(true);
    else
        ui->radMysql->setChecked(false);

    ui->chkPrestaShop->setChecked(settings.value("enlace_web").toBool());

    ui->chkmedicum->setChecked(settings.value("medicum").toBool());
    ui->chkconta->setChecked(settings.value("conta").toBool());
    ui->chkVademecum_alopatico->setChecked(settings.value("vad_alop").toBool());
    ui->chkVademecum_homeopatia->setChecked(settings.value("vad_home").toBool());
    ui->chkVademecum_MTC->setChecked(settings.value("vad_MTC").toBool());
    ui->chk_vademecum_fitoterapia->setChecked(settings.value("vad_fito").toBool());
    ui->txtnombre_cliente->setText("");
}

frmConfigmaya::~frmConfigmaya()
{
    delete ui;
}

void frmConfigmaya::configurar()
{


    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
    if(ui->radMysql->isChecked())
        settings.setValue("cDriverBDMaya","QMYSQL");
    else
        settings.setValue("cRutaDBMaya",ui->txtdireccionBD->text());

    settings.setValue("cHostBDMaya",ui->txtHost->text());
    settings.setValue("cUserBDMaya",Configuracion::Crypt(ui->txtUsuario->text()));
    settings.setValue("cPasswordBDMaya",Configuracion::Crypt(ui->txtPassword->text()));
    settings.setValue("pais",ui->cboPaises->currentText());
    settings.setValue(("global_port"),ui->txtPuerto->text());
    settings.setValue("ndigitos_factura",ui->spindigitos_factura->value());
    if(ui->chkProfesional->isChecked())
        settings.setValue("lProfesional",1);
    else
        settings.setValue("lProfesional",0);

    settings.setValue("irpf",ui->txtPorcIRPF->text());
    settings.setValue("contrasenaactiva",ui->txtContrasenaActiva->text());
    settings.setValue("digitos_cuentas",ui->spidigitos_cuentaContable->value());
    settings.setValue("cuenta_clientes",ui->txtCuentaClientes->text());
    settings.setValue("cuenta_proveedores",ui->txtCuentaProveedores->text());
    settings.setValue("cuenta_acreedores",ui->txtCuentaAcreedores->text());
    settings.setValue("cuenta_cobros",ui->txtcuenta_cobros->text());
    settings.setValue("cuenta_pagos",ui->txtcuenta_pagos->text());
    settings.setValue("cUsuarioActivo",ui->cboUsuarioActivo->currentText());
    settings.setValue("Clave1",ui->txtclaveV1_1->text());
    settings.setValue("Clave2",ui->txtclaveV1_2->text());
    settings.setValue("Clave3",ui->txtclaveV2_1->text());
    settings.setValue("Clave4",ui->txtclaveV2_2->text());
    settings.setValue("Clave5",ui->txtclaveV3_1->text());
    settings.setValue("Clave6",ui->txtclaveV3_2->text());
    settings.setValue("Clave7",ui->txtclaveV4_1->text());
    settings.setValue("Clave8",ui->txtclaveV4_2->text());
    if(ui->chkmedicum->isChecked())
        settings.setValue("medicum",1);
    else
        settings.setValue("medicum",0);
    if(ui->chkconta->isChecked())
        settings.setValue("conta",1);
    else
        settings.setValue("conta",0);

    if(ui->chkVademecum_alopatico->isChecked())
        settings.setValue("vad_alop",1);
    else
        settings.setValue("vad_alop",0);
   if (ui->chkVademecum_homeopatia->isChecked())
       settings.setValue("vad_home",1);
   else
       settings.setValue("vad_home",0);
   if (ui->chkVademecum_MTC->isChecked())
       settings.setValue("vad_MTC",1);
   else
       settings.setValue("vad_MTC",0);
   if (ui->chk_vademecum_fitoterapia->isChecked())
       settings.setValue("vad_fito",1);
   else
       settings.setValue("vad_fito",0);
   settings.setValue("hostTiendaWeb", ui->txtHostWeb->text());
   settings.setValue("usuarioTiendaWeb",Configuracion::Crypt(ui->txtUsuarioDBweb->text()));
   settings.setValue("Pass_web",Configuracion::Crypt(ui->txtPasswordDBWeb->text()));
   settings.setValue("puertoTiendaWeb",ui->txtPuerto_DBWeb->text());
   settings.setValue("nombre_bdTiendaWeb",ui->txtnombre_bdWeb->text());
   settings.setValue("enlace_web",ui->chkPrestaShop->isChecked());


   settings.setValue("HostDB_MediTec",ui->txtHostBD_MediTec->text());
   settings.setValue("NombreDB_MediTec",ui->txtnombre_bd_MediTec->text());
   settings.setValue("UsuarioDB_MediTec",ui->txtUsuarioDB_MediTec->text());
   settings.setValue("PasswordDB_MediTec",ui->txtPasswordDB_MediTec->text());
   settings.setValue("PuertoDB_MediTec",ui->txtPuerto_DB_MediTec->text());

    accept();
}

void frmConfigmaya::on_btnAnadir_cuentas_clicked()
{
    QSqlQuery q_clientes(Configuracion_global->groupDB);
    QSqlQuery q_cuentas(Configuracion_global->contaDB) ;
    bool registros = true;
    int id = 0;
   while (registros) {
       if(!q_clientes.exec("select * from clientes where id >"+QString::number(id)+" limit 0,1"))
            qDebug () << q_clientes.lastError().text();
        if(q_clientes.next())
        {

            ui->txtnombre_cliente->setText("Validando...: " + q_clientes.record().value("nombre_fiscal").toString());
            QApplication::processEvents();
            if(q_cuentas.exec("select codigo_cta from plan_general where codigo_cta ='"+q_clientes.record().value("codigo_cliente").toString()+"' limit 0,1"))
            {
                qDebug() <<q_cuentas.lastQuery();
                id++;
                if (!q_cuentas.next())
                {
                    // ----------------------------------
                    // Si no existe la cuenta la creamos
                    // ----------------------------------
                    q_cuentas.prepare("INSERT INTO plan_general (codigo_cta,descripcion,activo,codigo_balance,"
                                      "cif_nif,direccion,"
                                      "cp,poblacion,provincia,pais)"
                                      "VALUES (:codigo_cta,:descripcion,:activo,:codigo_balance,"
                                      ":cif_nif,:direccion,:cp,:poblacion,"
                                      ":provincia,pais);");

                    q_cuentas.bindValue(":codigo_cta",q_clientes.record().value("codigo_cliente").toString());
                    q_cuentas.bindValue(":descripcion",q_clientes.record().value("nombre_fiscal").toString());
                    q_cuentas.bindValue(":activo",true);
                    q_cuentas.bindValue(":codigo_balance","ABIII1");
                    q_cuentas.bindValue(":cif_nif",q_clientes.record().value("cif_nif").toString());
                    q_cuentas.bindValue(":direccion",q_clientes.record().value("direccion1").toString());
                    q_cuentas.bindValue(":cp",q_clientes.record().value("cp").toString());
                    q_cuentas.bindValue(":poblacion",q_clientes.record().value("poblacion").toString());
                    q_cuentas.bindValue(":provincia",q_clientes.record().value("provincia").toString());
                    q_cuentas.bindValue(":pais",q_clientes.record().value("pais").toString());
                    if(!q_cuentas.exec())
                        QMessageBox::warning(this,tr("Configuración contable de Maya"),
                                             tr("No se puede crear la cuenta : %1").arg(q_cuentas.lastError().text()));


                }
            } else
                qDebug() << q_cuentas.lastError().text();
        }else
        {
            registros = false;
        }
    }
}
