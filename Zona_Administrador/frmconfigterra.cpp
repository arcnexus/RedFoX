#include "frmconfigterra.h"
#include "ui_frmconfigterra.h"
#include <QtSql>
#include <QSqlDatabase>
#include "configuracion.h"


//Configuracion * Configuracion_global = 0;

frmConfigTerra::frmConfigTerra(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmConfigTerra)
{
    if(!Configuracion_global)
        Configuracion_global = new Configuracion;
    Configuracion_global->CargarDatosBD();
    Configuracion_global->Cargar_iva();
    Configuracion_global->Cargar_paises();
    ui->setupUi(this);
    QSqlQueryModel *qUsers = new QSqlQueryModel(this);
    qUsers->setQuery("select nombre from usuarios",QSqlDatabase::database("terra"));
    ui->cboUsuarioActivo->setModel(qUsers);


    ui->cboPaises->setModel(Configuracion_global->paises_model);
    ui->cboPaises->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    QSettings settings(qApp->applicationDirPath()+"/TerraConfig.ini", QSettings::IniFormat);
    ui->txtHost->setText(settings.value("cHostBDTerra").toString());
    ui->txtUsuario->setText(settings.value("cUserBDTerra").toString());
    ui->txtPassword->setText(settings.value("cPasswordBDTerra").toString());
    ui->txtPuerto->setText(settings.value("cPuertoDBTerra").toString());
    ui->txtDireccionBD->setText(settings.value("cRutaDBTerra").toString());

    int nindex = ui->cboPaises->findText(settings.value("cPais").toString());
    if (nindex > -1)
        ui->cboPaises->setCurrentIndex(nindex);


    ui->spinDigitosFactura->setValue(settings.value("nDigitosFactura").toInt());


    if(settings.value("lProfesional")==1)
        ui->chkProfesional->setChecked(true);
    else
        ui->chkProfesional->setChecked(false);

    ui->txtPorcIRPF->setText(settings.value("nIRPF").toString());

    ui->spinDigitosCuentaContable->setValue(settings.value("nDigitosCuentas").toInt());
    ui->txtCuentaClientes->setText(settings.value("cCuentaClientes").toString());
    ui->txtCuentaProveedores->setText(settings.value("cCuentaProveedores").toString());
    ui->txtCuentaAcreedores->setText(settings.value("cCuentaAcreedores").toString());
    ui->txtCuentaCobros->setText(settings.value("cuentacobros").toString());
    ui->txtCuentaPagos->setText(settings.value("cuentapagos").toString());
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
    if(settings.value("cDriverBDTerra").toString() =="QMYSQL")
        ui->radMysql->setChecked(true);
    else
        ui->radMysql->setChecked(false);

    if (settings.value("EnlacePrestaShop") == 1)
        ui->chkPrestaShop->setChecked(true);
    else
        ui->chkPrestaShop->setChecked(false);
    if(settings.value("medicum")==1)
        ui->chkmedicum->setChecked(true);
    else
        ui->chkmedicum->setChecked(false);
    if(settings.value("conta")==1)
        ui->chkconta->setChecked(true);
    else
        ui->chkconta->setChecked(false);
    if (settings.value("vad_alop")==1)
        ui->chkVademecum_alopatico->setChecked(true);
    else
        ui->chkVademecum_alopatico->setChecked(false);
    if (settings.value("vad_home")==1)
        ui->chkVademecum_homeopatia->setChecked(true);
    else
        ui->chkVademecum_homeopatia->setChecked(false);
    if (settings.value("vad_MTC")==1)
        ui->chkVademecum_MTC->setChecked(true);
    else
        ui->chkVademecum_MTC->setChecked(false);
    if(settings.value("vad_fito")==1)
        ui->chk_vademecum_fitoterapia->setChecked(true);
    else
        ui->chk_vademecum_fitoterapia->setChecked(false);

}

frmConfigTerra::~frmConfigTerra()
{
    delete ui;
}

void frmConfigTerra::configurar()
{


    QSettings settings(qApp->applicationDirPath()+"/TerraConfig.ini", QSettings::IniFormat);
    if(ui->radMysql->isChecked())
        settings.setValue("cDriverBDTerra","QMYSQL");
    else
        //settings.setValue("cRutaDBTerra",qApp->applicationDirPath()+"/DB/terra.sqlite");
        settings.setValue("cRutaDBTerra",ui->txtDireccionBD->text());
//    QDir directorioBd(qApp->applicationDirPath()+"/DB");
//    if(!directorioBd.exists())
//    {
//        QDir path(qApp->applicationDirPath());
//        path.mkdir(qApp->applicationDirPath()+"/DB");
//    }

//    QFile bd(qApp->applicationDirPath()+"/DB/terra.sqlite");
//    if(!bd.exists())
//    {
//       //TODO - Descargar terra.sqlite del servidor??
//    }

    settings.setValue("cHostBDTerra",ui->txtHost->text());
    settings.setValue("cUserBDTerra",ui->txtUsuario->text());
    settings.setValue("cPasswordBDTerra",ui->txtPassword->text());
    settings.setValue("cPais",ui->cboPaises->currentText());
    settings.setValue(("cPuertoDBTerra"),ui->txtPuerto->text());
    settings.setValue("nDigitosFactura",ui->spinDigitosFactura->value());
    if(ui->chkProfesional->isChecked())
        settings.setValue("lProfesional",1);
    else
        settings.setValue("lProfesional",0);

    settings.setValue("nIRPF",ui->txtPorcIRPF->text());
    if (ui->chkPrestaShop->isChecked())
        settings.setValue("EnlacePrestaShop",1);
    else
        settings.setValue("EnlacePrestaShop",0);
    settings.setValue("contrasenaactiva",ui->txtContrasenaActiva->text());
    settings.setValue("nDigitosCuentas",ui->spinDigitosCuentaContable->value());
    settings.setValue("cCuentaClientes",ui->txtCuentaClientes->text());
    settings.setValue("cCuentaProveedores",ui->txtCuentaProveedores->text());
    settings.setValue("cCuentaAcreedores",ui->txtCuentaAcreedores->text());
    settings.setValue("cuentacobros",ui->txtCuentaCobros->text());
    settings.setValue("cuentapagos",ui->txtCuentaPagos->text());
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


    accept();
}
