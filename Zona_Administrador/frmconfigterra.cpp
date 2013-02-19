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

    ui->cboPaises->setModel(Configuracion_global->paises_model);
    ui->cboPaises->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));


    QSettings settings("terraproject", "terra");
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
    ui->txtUsuario->setText(settings.value("cUsuarioActivo").toString());
    ui->txtclaveV1_1->setText(settings.value("Clave1").toString());
    ui->txtclaveV1_2->setText(settings.value("Clave2").toString());
}

frmConfigTerra::~frmConfigTerra()
{
    delete ui;
}

void frmConfigTerra::configurar()
{
    //NOTE - Hacer esto por defecto la primera vez que se ejecute Terra?

    QSettings settings("terraproject", "terra");
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
    settings.setValue("nDigitosFactura",5);
    settings.setValue("nIVA1",21);
    settings.setValue("nIVA2",10);
    settings.setValue("nIVA3",4);
    settings.setValue("nIVA4",0);
    settings.setValue("nRE1",5.2);
    settings.setValue("nRE2",1.4);
    settings.setValue("nRE3",0.5);
    settings.setValue("nRE4",0);
    settings.setValue("lProfesional",1);
    settings.setValue("nIRPF",21);

    settings.setValue("nDigitosCuentas",0);
    settings.setValue("cCuentaClientes","430");
    settings.setValue("cCuentaProveedores","400");
    settings.setValue("cCuentaAcreedores","410");
    settings.setValue("cUsuarioActivo","");
    settings.setValue("nNivelAcceso",0);
    settings.setValue("cCategoria","");
    settings.setValue("Clave1","");
    settings.setValue("Clave2","");
}
