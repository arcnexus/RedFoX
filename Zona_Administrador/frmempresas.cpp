#include "frmempresas.h"
#include "ui_frmempresas.h"

#include "../Zona_Administrador/frmconfigurar_terminal.h"
#include "../sqlcalls.h"

#include <QFileDialog>
FrmEmpresas::FrmEmpresas(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmEmpresas),
    menuButton(QIcon(":/Icons/PNG/empresa.png"),tr("Empresa"),this)
{
    ui->setupUi(this);
    on_btn_inicio_clicked();
    ui->cboSerie->addItem("A");
    getEmpresas();
}

FrmEmpresas::~FrmEmpresas()
{
    delete ui;
}

void FrmEmpresas:: CargarCamposEnEmpresa()
{
    oEmpresa.setcodigo(ui->txtcodigo->text());
    oEmpresa.setnombre(ui->txtEmpresa->text());
    oEmpresa.setruta_bd_sqlite(ui->txtRutaBd->text());

    oEmpresa.setnombre_bd(ui->txtnombre_bd->text());

    oEmpresa.setcDriverBD(ui->txtcDriver->currentText());

    oEmpresa.setnombre_bdMed(ui->txtnombre_bdMedic->text());

    oEmpresa.setcDriverBDMed(ui->txtcDriver->currentText());



    oEmpresa.setserie(ui->cboSerie->currentText());
    oEmpresa.setdireccion1(ui->txtdireccion1->text());
    oEmpresa.setcp(ui->txtcp->text());
    oEmpresa.setpoblacion(ui->txtpoblacion->text());
    oEmpresa.setprovincia(ui->txtprovincia->text());
   // oEmpresa.setpais(ui->txtpais->text());
    oEmpresa.settelefono1(ui->txttelefono1->text());
   // oEmpresa.settelefono2(ui->txtpais->text());
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
    oEmpresa.enlace_web = ui->chkEnlace_web->isChecked();
    oEmpresa.seguimiento = ui->chkSeguimiento->isChecked();
    oEmpresa.primer_dia_laborable = ui->cboPrimer_dia_laborable->currentText();
    oEmpresa.ultimo_dia_laborable = ui->cbo_ultimo_dia_semana->currentText();
    oEmpresa.horario_dia_normal = ui->txt_horario_dia_normal->text();
    oEmpresa.horario_primer_dia = ui->txt_horario_primer_dia->text();
    oEmpresa.horario_ultimo_dia = ui->txt_horario_ultimo_dia->text();
    oEmpresa.nombre_bd_contabilidad = ui->txtNombre_BDConta->text();
    oEmpresa.driver_db_contabilidad = ui->txtcDriver->currentText();
    oEmpresa.RutaBD_Contabilidad_sqlite = ui->txtruta_bd_conta->text();
    oEmpresa.ticket_factura = ui->chk_ticket_factura->isChecked();
    oEmpresa.id_tarifa_predeterminada = Configuracion_global->Devolver_id_tarifa(ui->cboTarifa->currentText());
    oEmpresa.actualizar_divisas = ui->chk_upate_divisas->isChecked();

    oEmpresa.empresa_medica = ui->chkMedica->isChecked();
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

    oEmpresa.irpf = ui->chkIRPF->isChecked();
    oEmpresa.porc_irpf = ui->spinPorc_irpf->value();
}

void FrmEmpresas::groupError(QString s)
{
    QMessageBox::critical(this,tr("Error al crear grupo nuevo"), s);
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

void FrmEmpresas::getEmpresas()
{
    _empresas.clear();
    QSqlQuery QryEmpresas(QSqlDatabase::database("Global"));

    QryEmpresas.prepare("Select * from mayaglobal.grupos");

    if(QryEmpresas.exec())
    {
        while (QryEmpresas.next())
        {
            ui->btn_add_nuevo->setEnabled(true);

            QSqlRecord rEmpresa = QryEmpresas.record();

            QString nombre = rEmpresa.value("nombre").toString();
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

            if(!db.open())
                continue;

            QString query = QString("SELECT * FROM %1.empresas;").arg(bd_name);
            QList<empresa> empresas;

            QSqlQuery q(db);
            q.exec(query);

            while(q.next())
            {
                empresa e;
                e.name = q.record().value("nombre").toString();
                e.record = q.record();
                empresas.append(e);
            }
            QPair<QList<empresa> , QSqlRecord> par;
            par.first = empresas;
            par.second = rEmpresa;
            _empresas.insert(nombre,par);
        }
    }
    ui->btn_edit_empresa->setEnabled(!_empresas.isEmpty());
}

void FrmEmpresas::_addEmpresa()
{
    QSqlRecord r = _empresas.value(_targetGroup).second;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(r.value("bd_host").toString());
    db.setUserName(r.value("bd_user").toString());
    db.setPassword(r.value("bd_pass").toString());
    db.setPort    (r.value("bd_port").toInt());
    if(db.open())
    {
        bool error = false;
        QProgressDialog d(this);
        d.setCancelButton(0);
        QSqlQuery q(db);
        d.show();        

        QString nEmpresa;
        if(ui->txtnombre_bd->text().isEmpty())
        {
            nEmpresa = QString("emp%1").arg(ui->txtEmpresa->text());
        }
        else
            nEmpresa = ui->txtnombre_bd->text();

        QString nConta;
        if(ui->txtNombre_BDConta->text().isEmpty())
        {
            nConta = QString("emp%1conta").arg(ui->txtEmpresa->text());
        }
        else
            nConta = ui->txtNombre_BDConta->text();

        QString nMedic;
        if(ui->txtnombre_bdMedic->text().isEmpty())
        {
            nMedic = QString("emp%1med").arg(ui->txtEmpresa->text());
        }
        else
            nMedic = ui->txtnombre_bdMedic->text();


        nEmpresa.replace(" ","");
        nConta.replace(" ","");
        nMedic.replace(" ","");

        QFile f(":/DB/DB/Empresa.sql");
        error = !f.open(QFile::ReadOnly);
        if(!error)
        {
            QString s = f.readAll();
            s.replace("@empresa@",nEmpresa);
            s.replace("\r\n"," ");

            QStringList querys = s.split(";",QString::SkipEmptyParts);

            d.setMaximum(querys.size());
            d.setValue(0);
            d.setLabelText(tr("Creando tablas"));

            for(int i = 0; i< querys.size();i++)
            {
                if(!(querys.at(i)== "\n") && !querys.at(i).isEmpty())
                {
                    if(!q.exec(querys.at(i)))
                    {
                        qDebug() << q.lastError().text();
                        qDebug() << querys.at(i);
                        error = true;
                        break;
                    }
                    d.setValue(i);
                    QApplication::processEvents();
                }
            }            
        }
        f.close();

        QFile f2(":/DB/DB/Conta.sql");
        error = !f2.open(QFile::ReadOnly);
        if(!error)
        {
            QString s = f2.readAll();
            s.replace("@conta@",nConta);
            s.replace("\r\n"," ");

            QStringList querys = s.split(";",QString::SkipEmptyParts);

            d.setValue(0);
            d.setMaximum(querys.size());
            d.setLabelText(tr("Creando tablas"));

            for(int i = 0; i< querys.size();i++)
            {
                if(!(querys.at(i)== "\n") && !querys.at(i).isEmpty())
                {
                    if(!q.exec(querys.at(i)))
                    {
                        qDebug() << q.lastError().text();
                        qDebug() << querys.at(i);
                        error = true;
                        break;
                    }
                    d.setValue(i);
                    QApplication::processEvents();
                }
            }
        }
        f2.close();

        QFile f3(":/DB/DB/Medica.sql");
        error = !f3.open(QFile::ReadOnly);
        if(!error && ui->chkMedica->isChecked())
        {
            QString s = f3.readAll();
            s.replace("@medica@",nConta);
            s.replace("\r\n"," ");

            QStringList querys = s.split(";",QString::SkipEmptyParts);

            d.setValue(0);
            d.setMaximum(querys.size());
            d.setLabelText(tr("Creando tablas"));

            for(int i = 0; i< querys.size();i++)
            {
                if(!(querys.at(i)== "\n") && !querys.at(i).isEmpty())
                {
                    if(!q.exec(querys.at(i)))
                    {
                        qDebug() << q.lastError().text();
                        qDebug() << querys.at(i);
                        error = true;
                        break;
                    }
                    d.setValue(i);
                    QApplication::processEvents();
                }
            }
        }
        f3.close();

        if(!error)
        {
            QHash<QString,QVariant> data;

            data["codigo"]= ui->txtcodigo->text();
            data["nombre"]= ui->txtEmpresa->text();
            data["digitos_factura"]= ui->spinDigitos->value();
            data["serie"]= ui->cboSerie->currentText();
            data["decimales_campos_totales"] = ui->spinDecimales_create->value();
            data["decimales"]=   ui->spinDecimales_precios_create->value();
            //data["mostrarsiempre"]=  ??????????
            //data["ruta_bd_sqlite"]=  //TODO reactivar en soporte SQLITE??
            //data["ruta_db_conta"]=  ;
            //data["ruta_bd_medica_sqlite"]=  ;
            data["nombre_bd"]=  nEmpresa;
            data["nombre_db_conta"]=  nConta;
            data["nombre_bd_medica"]=  nMedic;
            data["direccion"]=  ui->txtdireccion1->text();
            data["cp"]=  ui->txtcp->text();
            data["poblacion"]=  ui->txtpoblacion->text();
            data["provincia"]=  ui->txtprovincia->text();
            data["pais"]=  ui->cboPais_create->currentText();
            data["telefono1"]=  ui->txttelefono1->text();
            data["telefono2"]=  ui->txttelefono2->text();
            data["fax"]=  ui->txtfax->text();
            data["mail"]=  ui->txtcMail->text();
            data["web"]=  ui->txtweb->text();
            data["cif"]=  ui->txtcif->text();
            data["inscripcion"]=  ui->txtcInscripcion->text();
            data["comentario_albaran"]=  ui->txtcCometarioAlbaran->toPlainText();
            data["comentario_factura"]=  ui->txtcCometarioAlbaran->toPlainText();
            data["comentario_ticket"]=  ui->txtccomentario_ticket->toPlainText();
            data["ejercicio"]=  ui->txtejercicio->value();
            data["usar_irpf"]=  ui->chkIRPF->isChecked();
            data["porc_irpf"]=  ui->spinPorc_irpf->value();
            data["codigo_cuenta_clientes"]=  ui->txtcuentaCliente->text();
            data["codigo_cuenta_proveedores"]=  ui->txtcuenta_proveedores->text();
            data["codigo_cuenta_acreedores"]=  ui->txtcuenta_acreedores->text();
            data["digitos_cuenta"]=  ui->txtdigitos_cuentas->text();
            data["clave1"]=  ui->txtclave1->text();
            data["clave2"]=  ui->txtclave2->text();
            data["medica"]=  ui->chkMedica->isChecked();
            data["internacional"]=  ui->chkInternacional->isChecked();
            data["auto_codigo"]=  ui->chkAutocodificiar->isChecked();
            data["tamano_codigo"]=  ui->txttamano_codigoart->value();
            data["cuenta_cobros"]=  ui->txtcuenta_cobros->text();
            data["cuenta_pagos"]=  ui->txtcuenta_pagos->text();
            data["id_divisa"]=  monedaEditModel->record(ui->cboDivisas->currentIndex()).value("id");
            data["enlace_web"]=  ui->chkEnlace_web->isChecked();
            data["contabilidad"]=  ui->chkContabilidad->isChecked();
            data["consultas"]=  ui->txtConsultas->value();
            data["primer_dia_laborable"]=  ui->cboPrimer_dia_laborable->currentText();
            data["ultimo_dia_laborable"]=  ui->cbo_ultimo_dia_semana->currentText();
            data["horario_primer_dia"]=  ui->txt_horario_primer_dia->text();
            data["horario_dia_normal"]=  ui->txt_horario_dia_normal->text();
            data["horario_ultimo_dia"]=  ui->txt_horario_ultimo_dia->text();
            data["ticket_factura"]=  ui->chk_ticket_factura->isChecked();
            data["margen"]=  ui->spinMargen->value();
            data["margen_minimo"]=  ui->spinMargen_minimo->value();
            data["seguimiento"]=  ui->chkSeguimiento->isChecked();
            data["id_tarifa_predeterminada"]=  tarifaEditModel->record(ui->cboTarifa->currentIndex()).value("id");
            data["actualizar_divisas"]=  ui->chk_upate_divisas->isChecked();
            data["cuenta_ventas_mercaderias"]=  ui->txtCuenta_venta_mercaderias->text();
            data["cuenta_ventas_servicios"]=  ui->txtCuenta_venta_servicios->text();
            data["cuenta_iva_soportado1"]=  ui->ivasoportado1->text();
            data["cuenta_iva_soportado2"]=  ui->ivasoportado2->text();
            data["cuenta_iva_soportado3"]=  ui->ivasoportado3->text();
            data["cuenta_iva_soportado4"]=  ui->ivasoportado4->text();
            data["cuenta_iva_repercutido1"]=  ui->ivarepercutido1->text();
            data["cuenta_iva_repercutido2"]=  ui->ivarepercutido2->text();
            data["cuenta_iva_repercutido3"]=  ui->ivarepercutido3->text();
            data["cuenta_iva_repercutido4"]=  ui->ivarepercutido4->text();
            data["cuenta_iva_repercutido1_re"]=  ui->ivarepercutidore1->text();
            data["cuenta_iva_repercutido2_re"]=  ui->ivarepercutidore2->text();
            data["cuenta_iva_repercutido3_re"]=  ui->ivarepercutidore3->text();
            data["cuenta_iva_repercutido4_re"]=  ui->ivarepercutidore4->text();
            data["cuenta_iva_soportado1_re"]=  ui->ivasoportadore1->text();
            data["cuenta_iva_soportado2_re"]=  ui->ivasoportadore2->text();
            data["cuenta_iva_soportado3_re"]=  ui->ivasoportadore3->text();
            data["cuenta_iva_soportado4_re"]=  ui->ivasoportadore4->text();
            data["importe_cierre"]= ui->spinImporte_abertura_create->value();
            data["facturas_en_cierre"]= ui->checkMostrarfacturascierrecaja_create->isChecked();
            data["tpv_forzar_cantidad"]= ui->checktpv_forzar_cantidad_create->isChecked();
            data["caducidad_vales"]= ui->txtCaducidadvales->value();

            QByteArray bArray;
            QBuffer buffer( &bArray );
            buffer.open( QIODevice::WriteOnly );
            ui->label_logo_create->pixmap()->save( &buffer, "JPG" );
            QByteArray b_64 = bArray.toBase64();
            data["logo"] = b_64;
            QString error;

            if(SqlCalls::SqlInsert(data,QString("`%1`.`empresas`").arg(r.value("bd_name").toString()),db,error) == -1)
                qDebug() << error;
        }
    }
    else
        qDebug() << db.lastError().text();
}

void FrmEmpresas::_llenarCampos(QSqlRecord r)
{
    tarifaEditModel = new QSqlQueryModel(this);
    monedaEditModel = new QSqlQueryModel(this);

    tarifaEditModel->setQuery("Select `id`,`descripcion` from codigotarifa",_targetGroupDb);
    monedaEditModel->setQuery("Select `id`,`moneda` from monedas",_targetGroupDb);

    ui->cboTarifa_3->setModel(tarifaEditModel);
    ui->cboTarifa_3->setModelColumn(1);

    ui->cboDivisas_3->setModel(monedaEditModel);
    ui->cboDivisas_3->setModelColumn(1);

    for(auto i = 0; i< tarifaEditModel->rowCount(); i++)
    {
        if(tarifaEditModel->record(i).value("id") == r.value("id_tarifa_predeterminada"))
        {
            ui->cboTarifa_3->setCurrentText(tarifaEditModel->record(i).value("descripcion").toString());
            break;
        }
    }

    for(auto i = 0; i< monedaEditModel->rowCount(); i++)
    {
        if(monedaEditModel->record(i).value("id") == r.value("id_divisa"))
        {
            ui->cboDivisas_3->setCurrentText(monedaEditModel->record(i).value("moneda").toString());
            break;
        }
    }

    ui->spinDecimales_2->setValue(r.value("decimales_campos_totales").toInt());
    ui->txtcodigo_2->setText(r.value("codigo").toString());
    ui->txtEmpresa_2->setText(r.value("nombre").toString());//
    ui->spinDigitos_edit->setValue(r.value("digitos_factura").toInt());//->text().toInt());
    ui->cboSerie_Edit->setCurrentText(r.value("serie").toString());//text().toInt());
    ui->txtnombre_bd_3->setText(r.value("nombre_bd").toString());//nEmpresa);
    ui->txtNombre_BDConta_3->setText(r.value("nombre_db_conta").toString());//nConta);
    ui->txtnombre_bdMedic_3->setText(r.value("nombre_bd_medica").toString());//nMedic);
    ui->txtdireccion1_3->setText(r.value("direccion").toString());
    ui->txtcp_3->setText(r.value("cp").toString());
    ui->txtpoblacion_3->setText(r.value("poblacion").toString());
    ui->txtprovincia_3->setText(r.value("provincia").toString());
    ui->cboPais_edit->setCurrentText(r.value("pais").toString());
    ui->txttelefono1_3->setText(r.value("telefono1").toString());
    ui->txttelefono2_3->setText(r.value("telefono2").toString());
    ui->txtfax_3->setText(r.value("fax").toString());
    ui->txtcMail_3->setText(r.value("mail").toString());
    ui->txtweb_3->setText(r.value("web").toString());
    ui->txtcif_3->setText(r.value("cif").toString());
    ui->txtcInscripcion_3->setText(r.value("inscripcion").toString());
    ui->txtcCometarioAlbaran_3->setText(r.value("comentario_albaran").toString());
    ui->txtccomentario_factura_3->setText(r.value("comentario_factura").toString());
    ui->txtccomentario_ticket_3->setText(r.value("comentario_ticket").toString());
    ui->txtejercicio_2->setValue(r.value("ejercicio").toInt());
    ui->chkIRPF_3->setChecked(r.value("usar_irpf").toBool());
    ui->spinPorc_irpf_3->setValue(r.value("porc_irpf").toFloat());
    ui->txtcuentaCliente_3->setText(r.value("codigo_cuenta_clientes").toString());
    ui->txtcuenta_proveedores_3->setText(r.value("codigo_cuenta_proveedores").toString());
    ui->txtcuenta_acreedores_3->setText(r.value("codigo_cuenta_acreedores").toString());
    ui->txtdigitos_cuentas_3->setValue(r.value("digitos_cuenta").toInt());
    ui->txtclave1_3->setText(r.value("clave1").toString());
    ui->txtclave2_3->setText(r.value("clave2").toString());
    ui->chkMedica_3->setChecked(r.value("medica").toBool());
    ui->chkInternacional_3->setChecked(r.value("internacional").toBool());
    ui->chkAutocodificiar_3->setChecked(r.value("auto_codigo").toBool());
    ui->txttamano_codigoart_3->setValue(r.value("tamano_codigo").toBool());
    ui->txtcuenta_cobros_3->setText(r.value("cuenta_cobros").toString());
    ui->txtcuenta_pagos_3->setText(r.value("cuenta_pagos").toString());

    ui->chkEnlace_web_3->setChecked(r.value("enlaceweb").toBool());
    ui->chkContabilidad_3->setChecked(r.value("contabilidad").toBool());
    ui->txtConsultas_3->setValue(r.value("consultas").toInt());
    ui->cboPrimer_dia_laborable_3->setCurrentIndex(r.value("primer_dia_laborable").toInt());
    ui->cbo_ultimo_dia_semana_3->setCurrentIndex(r.value("ultimo_dia_laborable").toInt());
    ui->txt_horario_primer_dia_3->setText(r.value("horario_primer_dia").toString());
    ui->txt_horario_dia_normal_3->setText(r.value("horario_dia_normal").toString());
    ui->txt_horario_ultimo_dia_3->setText(r.value("horario_ultimo_dia").toString());
    ui->chk_ticket_factura_3->setChecked(r.value("ticket_factura").toBool());
    ui->spinMargen_3->setValue(r.value("margen").toDouble());
    ui->spinMargen_minimo_3->setValue(r.value("margen_minimo").toDouble());
    ui->chkSeguimiento_3->setChecked(r.value("seguimiento").toDouble());

    ui->chk_upate_divisas_3->setChecked(r.value("actualizar_divisas").toBool());
    ui->txtCuenta_venta_mercaderias_3->setText(r.value("cuenta_ventas_mercaderias").toString());
    ui->txtCuenta_venta_servicios_3->setText(r.value("cuenta_ventas_servicios").toString());
    ui->ivasoportado1_3->setText(r.value("cuenta_iva_soportado1").toString());
    ui->ivasoportado2_3->setText(r.value("cuenta_iva_soportado2").toString());
    ui->ivasoportado3_3->setText(r.value("cuenta_iva_soportado3").toString());
    ui->ivasoportado4_3->setText(r.value("cuenta_iva_soportado4").toString());
    ui->ivarepercutido1_3->setText(r.value("cuenta_iva_repercutido1").toString());
    ui->ivarepercutido2_3->setText(r.value("cuenta_iva_repercutido2").toString());
    ui->ivarepercutido3_3->setText(r.value("cuenta_iva_repercutido3").toString());
    ui->ivarepercutido4_3->setText(r.value("cuenta_iva_repercutido4").toString());
    ui->ivarepercutidore1_3->setText(r.value("cuenta_iva_repercutido1_re").toString());
    ui->ivarepercutidore2_3->setText(r.value("cuenta_iva_repercutido2_re").toString());
    ui->ivarepercutidore3_3->setText(r.value("cuenta_iva_repercutido3_re").toString());
    ui->ivarepercutidore4_3->setText(r.value("cuenta_iva_repercutido4_re").toString());
    ui->ivasoportadore1_3->setText(r.value("cuenta_iva_soportado1_re").toString());
    ui->ivasoportadore2_3->setText(r.value("cuenta_iva_soportado2_re").toString());
    ui->ivasoportadore3_3->setText(r.value("cuenta_iva_soportado3_re").toString());
    ui->ivasoportadore4_3->setText(r.value("cuenta_iva_soportado4_re").toString());

    ui->txtCaducidadvales_3->setValue(r.value("caducidad_vales").toInt());
    ui->cboSerie_Edit->setCurrentText(r.value("serie").toString());

    ui->spinDecimales_precios_2->setValue(r.value("decimales").toInt());
    ui->spinImporte_abertura->setValue(r.value("importe_cierre").toDouble());
    ui->checkMostrarfacturascierrecaja->setChecked(r.value("facturas_en_cierre").toBool());
    ui->checktpv_forzar_cantidad->setChecked(r.value("tpv_forzar_cantidad").toBool());

    QByteArray bArray = r.value("logo").toByteArray();
    QByteArray b_64 = QByteArray::fromBase64(bArray);
    QImage im = QImage::fromData(b_64);
    QPixmap p = QPixmap::fromImage(im);
    ui->label_logo_edit->setMaximumSize(p.size());
    ui->label_logo_edit->setPixmap(p);
}

void FrmEmpresas::on_btnConfigTerminal_clicked()
{
    frmConfigurar_terminal terminal;
    terminal.exec();
}

void FrmEmpresas::on_btn_add_empresa_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->create_page_1);
}

void FrmEmpresas::on_btn_crear_nuevo_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->create_page_newgroup);
}

void FrmEmpresas::on_btn_add_nuevo_clicked()
{
    ui->group_list->clear();
    QHashIterator<QString, QPair<QList<empresa> , QSqlRecord> > it(_empresas);

    while(it.hasNext())
    {
        it.next();
        QString grupo_name = it.key();
        QListWidgetItem * item = new QListWidgetItem(grupo_name,ui->group_list);
    }

    ui->stackedWidget->setCurrentWidget(ui->create_page_add);
}

void FrmEmpresas::on_btn_inicio_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->main_page);
}

void FrmEmpresas::on_btn_guardar_nuevo_clicked()
{
    //Crear empresa @targetGroup
    _addEmpresa();

    _targetGroupDb.close();
    QSqlDatabase::removeDatabase("save_empresa");
    tarifaEditModel->deleteLater();
    monedaEditModel->deleteLater();
    paisesEditModel->deleteLater();

    ui->stackedWidget->setCurrentWidget(ui->main_page);
}

void FrmEmpresas::on_btn_crearGrupo_clicked()
{
    QProgressDialog d(this);

    d.setValue(0);
    d.setMinimum(0);
    d.setMaximum(0);
    d.show();
    QApplication::processEvents();
    createGroup();

    QSqlRecord _targetGroupDbRecord = _empresas.value(_targetGroup).second;
    _targetGroupDb = QSqlDatabase::addDatabase("QMYSQL","save_empresa");
    _targetGroupDb.setHostName(_targetGroupDbRecord.value("bd_host").toString());
    _targetGroupDb.setUserName(_targetGroupDbRecord.value("bd_user").toString());
    _targetGroupDb.setPassword(_targetGroupDbRecord.value("bd_pass").toString());
    _targetGroupDb.setPort(_targetGroupDbRecord.value("bd_port").toInt());
    _targetGroupDb.setDatabaseName(_targetGroupDbRecord.value("bd_name").toString());

    if(_targetGroupDb.open())
    {
        tarifaEditModel = new QSqlQueryModel(this);
        monedaEditModel = new QSqlQueryModel(this);
        paisesEditModel = new QSqlQueryModel(this);

        tarifaEditModel->setQuery("Select `id`,`descripcion` from codigotarifa",_targetGroupDb);
        monedaEditModel->setQuery("Select `id`,`moneda` from monedas",_targetGroupDb);
        paisesEditModel->setQuery("Select pais from paises",_targetGroupDb);

        ui->cboTarifa->setModel(tarifaEditModel);
        ui->cboTarifa->setModelColumn(1);
        ui->cboPais_create->setModel(paisesEditModel);

        ui->cboDivisas->setModel(monedaEditModel);
        ui->cboDivisas->setModelColumn(1);

        ui->stackedWidget->setCurrentWidget(ui->create_page_empresa);
    }
    else
        qDebug() << _targetGroupDb.lastError().text();
}

void FrmEmpresas::createGroup()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ui->txtHost->text());
    db.setUserName(ui->txtUser->text());
    db.setPassword(ui->txtPass->text());
    db.setPort(ui->spinPort->value());
    bool error = !db.open();

    db.transaction();

    if(!error)
        error = _createTables(db);

    QString grupo = "Grupo";
    QString sError;
    if(!error)
    {
        grupo.append(ui->txtNombreGrupo->text());

        db.close();
        db.setHostName(ui->txtHost->text());
        db.setUserName(ui->txtUser->text());
        db.setPassword(ui->txtPass->text());
        db.setPort(ui->spinPort->value());
        db.setDatabaseName(grupo);
        error = !db.open();

        QApplication::processEvents();
        if(!error)
            error = _insertMonedas(db, sError);

        QApplication::processEvents();
        if(!error)
            error = _insertIVA(db, sError);

        QApplication::processEvents();
        if(!error)
            error = _insertPaises(db, sError);

        //TODO Reports por defecto al crear grupo
    }

    if(!error)
    {
        db.commit();
        _insertNewGroup(grupo);
        _targetGroup = ui->txtNombreGrupo->text();
    }
    else
    {
        db.rollback();
    }
    emit endGroup(!error);
}

//Funciones Creacion de Grupo

bool FrmEmpresas::_createTables(QSqlDatabase db)
{
    bool error = false;
    QFile f(":/DB/DB/Grupo.sql");
    f.open(QFile::ReadOnly);
    QString s = f.readAll();
    f.close();
    QString grupo = "Grupo";
    grupo.append(ui->txtNombreGrupo->text());
    s.replace("@grupo@",grupo);
    s.replace("\r\n"," ");
    QStringList querys = s.split(";",QString::SkipEmptyParts);
    int i;
    QSqlQuery q(db);
    for(i = 0; i< querys.size();i++)
    {
        if(querys.at(i) == "\n")
            continue;
        if(!q.exec(querys.at(i)))
        {
            qDebug() << q.lastError().text();
            qDebug() << querys.at(i);
            error = true;
            break;
        }
        QApplication::processEvents();
    }
    return error;
}

bool FrmEmpresas::_insertMonedas(QSqlDatabase db, QString& error)
{
    QFile f(":/Icons/divisas.txt");
    if(f.open((QIODevice::ReadOnly | QIODevice::Text)))
    {
        QTextStream in(&f);
        in.setCodec("UTF-8");
        QString all = in.readAll();
        all.remove("\r");
        all.remove("\n");
        QStringList blocks = all.split("},",QString::SkipEmptyParts);
        foreach(const QString &moneda , blocks)
        {
            QStringList subBlocks = moneda.split("\",",QString::SkipEmptyParts);
            QString nombre_corto = subBlocks.at(0).mid(subBlocks.at(0).lastIndexOf("\"")+1);
            QString nombre = subBlocks.at(1).mid(subBlocks.at(1).lastIndexOf("\"")+1);
            QString simbol = subBlocks.at(4).mid(subBlocks.at(4).lastIndexOf("\"")+1);
            simbol.remove("}");
            simbol = simbol.trimmed();
            QHash<QString,QVariant> _data;
            _data["moneda"] = nombre;
            _data["nombre_corto"] = nombre_corto;
            _data["simbolo"] = simbol;
            int id = SqlCalls::SqlInsert(_data,"monedas",db,error);
            if(id < 0)
                return true;
        }
    }
    return false;
}

bool FrmEmpresas::_insertIVA(QSqlDatabase db, QString& error)
{
    QSqlQuery q(db);
    bool e;
    e = q.exec("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, recargo_equivalencia) VALUES ('base1', 'NORMAL'       ,    '21%', 21.00, 5.20);");
    e &= q.exec("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, recargo_equivalencia) VALUES ('base2', 'Reducido'     ,    '10%', 10.00, 1.40);");
    e &= q.exec("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, recargo_equivalencia) VALUES ('base3', 'Superreducido',     '4%',  4.00, 0.50);");
    e &= q.exec("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, recargo_equivalencia) VALUES ('base4', 'Exento'       , 'Exento',  0.00, 0.00);");
    if(!e)
        error = q.lastError().text();
    return !e;
}

bool FrmEmpresas::_insertPaises(QSqlDatabase db, QString& error)
{
    bool e = false;
    QFile f(":/DB/DB/Paises.sql");
    f.open(QFile::ReadOnly);
    QString s = f.readAll();
    f.close();
    QString grupo = "Grupo";
    grupo.append(ui->txtNombreGrupo->text());
    s.replace("@grupo@",grupo);
    s.replace("\r\n"," ");
    QStringList querys = s.split(";",QString::SkipEmptyParts);
    int i;
    QSqlQuery q(db);
    for(i = 0; i< querys.size();i++)
    {
        if(querys.at(i) == "\n")
            continue;
        if(!q.exec(querys.at(i)))
        {
            qDebug() << q.lastError().text();
            qDebug() << querys.at(i);
            error = q.lastError().text();
            e = true;
            break;
        }
    }
    return e;
}

void FrmEmpresas::_insertNewGroup(QString grupo)
{
    QSqlQuery q2(Configuracion_global->groupDB);

    q2.prepare("INSERT INTO `mayaglobal`.`grupos` "
               "(`nombre`, `bd_name`, `bd_driver`, "
               "`bd_host`, `bd_user`, `bd_pass`, `bd_port`) "
               "VALUES "
               "(:nombre, :bd_name, :bd_driver, "
               ":bd_host, :bd_user, :bd_pass, :bd_port) ");

    q2.bindValue(":nombre",ui->txtNombreGrupo->text());
    q2.bindValue(":bd_name",grupo);
    q2.bindValue(":bd_driver","QMYSQL");//NOTE cambiar para multi driver
    q2.bindValue(":bd_host",ui->txtHost->text());
    q2.bindValue(":bd_user",ui->txtUser->text());
    q2.bindValue(":bd_pass",ui->txtPass->text());
    q2.bindValue(":bd_port",ui->spinPort->value());

    if(!q2.exec())
        emit groupError(q2.lastError().text());
    else
        getEmpresas();
}
//Fin funciones de grupo

void FrmEmpresas::on_btn_add_empresaGrupo_clicked()
{
    if(ui->group_list->selectedItems().size() == 0)
        return;
    _targetGroup = ui->group_list->selectedItems().at(0)->text();

    QSqlRecord _targetGroupDbRecord = _empresas.value(_targetGroup).second;
    _targetGroupDb = QSqlDatabase::addDatabase("QMYSQL","save_empresa");
    _targetGroupDb.setHostName(_targetGroupDbRecord.value("bd_host").toString());
    _targetGroupDb.setUserName(_targetGroupDbRecord.value("bd_user").toString());
    _targetGroupDb.setPassword(_targetGroupDbRecord.value("bd_pass").toString());
    _targetGroupDb.setPort(_targetGroupDbRecord.value("bd_port").toInt());
    _targetGroupDb.setDatabaseName(_targetGroupDbRecord.value("bd_name").toString());

    if(_targetGroupDb.open())
    {
        tarifaEditModel = new QSqlQueryModel(this);
        monedaEditModel = new QSqlQueryModel(this);
        paisesEditModel = new QSqlQueryModel(this);

        tarifaEditModel->setQuery("Select `id`,`descripcion` from codigotarifa",_targetGroupDb);
        monedaEditModel->setQuery("Select `id`,`moneda` from monedas",_targetGroupDb);                
        paisesEditModel->setQuery("Select pais from paises",_targetGroupDb);

        ui->cboTarifa->setModel(tarifaEditModel);
        ui->cboTarifa->setModelColumn(1);
        ui->cboPais_create->setModel(paisesEditModel);

        ui->cboDivisas->setModel(monedaEditModel);
        ui->cboDivisas->setModelColumn(1);

        ui->stackedWidget->setCurrentWidget(ui->create_page_empresa);
    }
}

void FrmEmpresas::on_btn_edit_empresa_clicked()
{
    ui->listWidgetEmpresas->clear();
    ui->listWidgetGrupos->clear();

    QHashIterator<QString, QPair<QList<empresa> , QSqlRecord> > it(_empresas);

    while(it.hasNext())
    {
        it.next();
        QString grupo_name = it.key();
        QListWidgetItem * item = new QListWidgetItem(grupo_name,ui->listWidgetGrupos);
    }
    if(ui->listWidgetGrupos->count())
        ui->listWidgetGrupos->setCurrentRow(0);

    ui->stackedWidget->setCurrentWidget(ui->edit_page_select);
}

void FrmEmpresas::on_listWidgetGrupos_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(!current)
        return;
    ui->listWidgetEmpresas->clear();
    QList<empresa> e_list = _empresas.value(current->text()).first;
    foreach (const empresa e, e_list) {
        QListWidgetItem * item = new QListWidgetItem(e.name,ui->listWidgetEmpresas);
    }
    if(ui->listWidgetEmpresas->count())
        ui->listWidgetEmpresas->setCurrentRow(0);
}

void FrmEmpresas::on_btnEditaEmpresa_clicked()
{
    if(!ui->listWidgetEmpresas->currentItem() || !ui->listWidgetGrupos->currentItem())
        return;
    QList<empresa> e_list = _empresas.value(ui->listWidgetGrupos->currentItem()->text()).first;
    QString current = ui->listWidgetEmpresas->currentItem()->text();
    foreach (const empresa e, e_list) {
        if(e.name == current)
        {
            QSqlRecord _targetGroupDbRecord = _empresas.value(ui->listWidgetGrupos->currentItem()->text()).second;
            _targetGroupDb = QSqlDatabase::addDatabase("QMYSQL","save_empresa");
            _targetGroupDb.setHostName(_targetGroupDbRecord.value("bd_host").toString());
            _targetGroupDb.setUserName(_targetGroupDbRecord.value("bd_user").toString());
            _targetGroupDb.setPassword(_targetGroupDbRecord.value("bd_pass").toString());
            _targetGroupDb.setPort(_targetGroupDbRecord.value("bd_port").toInt());
            _targetGroupDb.setDatabaseName(_targetGroupDbRecord.value("bd_name").toString());
            if(_targetGroupDb.open())
            {
                seriesEditModel = new QSqlQueryModel(this);
                paisesEditModel = new QSqlQueryModel(this);

                paisesEditModel->setQuery("Select pais from paises",_targetGroupDb);

                QSqlDatabase d = _targetGroupDb.cloneDatabase(_targetGroupDb,"series_db");
                d.setDatabaseName(e.record.value("nombre_bd").toString());
                d.open();

                seriesEditModel->setQuery("Select `serie` from series",d);

                ui->cboSerie_Edit->setModel(seriesEditModel);
                ui->cboPais_edit->setModel(paisesEditModel);

                _idEmpresa = e.record.value("id").toInt();
                _llenarCampos(e.record);
                break;
            }
        }
    }
    ui->stackedWidget->setCurrentWidget(ui->edit_page_empresa);
}

void FrmEmpresas::on_btn_guardar_edit_clicked()
{
    QHash<QString,QVariant> data;

    data["codigo"]= ui->txtcodigo_2->text();
    data["nombre"]= ui->txtEmpresa_2->text();
    data["digitos_factura"]= ui->spinDigitos_edit->value();
    data["serie"]= ui->cboSerie_Edit->currentText();
    data["decimales_campos_totales"] = ui->spinDecimales_2->value();
    data["decimales"]=   ui->spinDecimales_precios_2->value();
    //data["mostrarsiempre"]=  ??????????
    //data["ruta_bd_sqlite"]=  //TODO reactivar en soporte SQLITE??
    //data["ruta_db_conta"]=  ;
    //data["ruta_bd_medica_sqlite"]=  ;
    data["nombre_bd"]=  ui->txtnombre_bd_3->text();
    data["nombre_db_conta"]=  ui->txtNombre_BDConta_3->text();
    data["nombre_bd_medica"]=  ui->txtnombre_bdMedic_3->text();
    data["direccion"]=  ui->txtdireccion1_3->text();
    data["cp"]=  ui->txtcp_3->text();
    data["poblacion"]=  ui->txtpoblacion_3->text();
    data["provincia"]=  ui->txtprovincia_3->text();
    data["pais"]=  ui->cboPais_edit->currentText();
    data["telefono1"]=  ui->txttelefono1_3->text();
    data["telefono2"]=  ui->txttelefono2_3->text();
    data["fax"]=  ui->txtfax_3->text();
    data["mail"]=  ui->txtcMail_3->text();
    data["web"]=  ui->txtweb_3->text();
    data["cif"]=  ui->txtcif_3->text();
    data["inscripcion"]=  ui->txtcInscripcion_3->text();
    data["comentario_albaran"]=  ui->txtcCometarioAlbaran_3->toPlainText();
    data["comentario_factura"]=  ui->txtcCometarioAlbaran_3->toPlainText();
    data["comentario_ticket"]=  ui->txtccomentario_ticket_3->toPlainText();
    data["ejercicio"]=  ui->txtejercicio_2->value();
    data["usar_irpf"]=  ui->chkIRPF_3->isChecked();
    data["porc_irpf"]=  ui->spinPorc_irpf_3->value();
    data["codigo_cuenta_clientes"]=  ui->txtcuentaCliente_3->text();
    data["codigo_cuenta_proveedores"]=  ui->txtcuenta_proveedores_3->text();
    data["codigo_cuenta_acreedores"]=  ui->txtcuenta_acreedores_3->text();
    data["digitos_cuenta"]=  ui->txtdigitos_cuentas_3->text();
    data["clave1"]=  ui->txtclave1_3->text();
    data["clave2"]=  ui->txtclave2_3->text();
    data["medica"]=  ui->chkMedica_3->isChecked();
    data["internacional"]=  ui->chkInternacional_3->isChecked();
    data["auto_codigo"]=  ui->chkAutocodificiar_3->isChecked();
    data["tamano_codigo"]=  ui->txttamano_codigoart_3->value();
    data["cuenta_cobros"]=  ui->txtcuenta_cobros_3->text();
    data["cuenta_pagos"]=  ui->txtcuenta_pagos_3->text();   
    data["id_divisa"]=  monedaEditModel->record(ui->cboDivisas_3->currentIndex()).value("id");
    data["enlace_web"]=  ui->chkEnlace_web_3->isChecked();
    data["contabilidad"]=  ui->chkContabilidad_3->isChecked();
    data["consultas"]=  ui->txtConsultas_3->value();
    data["primer_dia_laborable"]=  ui->cboPrimer_dia_laborable_3->currentText();
    data["ultimo_dia_laborable"]=  ui->cbo_ultimo_dia_semana_3->currentText();
    data["horario_primer_dia"]=  ui->txt_horario_primer_dia_3->text();
    data["horario_dia_normal"]=  ui->txt_horario_dia_normal_3->text();
    data["horario_ultimo_dia"]=  ui->txt_horario_ultimo_dia_3->text();
    data["ticket_factura"]=  ui->chk_ticket_factura_3->isChecked();
    data["margen"]=  ui->spinMargen_3->value();
    data["margen_minimo"]=  ui->spinMargen_minimo_3->value();
    data["seguimiento"]=  ui->chkSeguimiento_3->isChecked();
    data["id_tarifa_predeterminada"]=  tarifaEditModel->record(ui->cboTarifa_3->currentIndex()).value("id");
    data["actualizar_divisas"]=  ui->chk_upate_divisas_3->isChecked();
    data["cuenta_ventas_mercaderias"]=  ui->txtCuenta_venta_mercaderias_3->text();
    data["cuenta_ventas_servicios"]=  ui->txtCuenta_venta_servicios_3->text();
    data["cuenta_iva_soportado1"]=  ui->ivasoportado1_3->text();
    data["cuenta_iva_soportado2"]=  ui->ivasoportado2_3->text();
    data["cuenta_iva_soportado3"]=  ui->ivasoportado3_3->text();
    data["cuenta_iva_soportado4"]=  ui->ivasoportado4_3->text();
    data["cuenta_iva_repercutido1"]=  ui->ivarepercutido1_3->text();
    data["cuenta_iva_repercutido2"]=  ui->ivarepercutido2_3->text();
    data["cuenta_iva_repercutido3"]=  ui->ivarepercutido3_3->text();
    data["cuenta_iva_repercutido4"]=  ui->ivarepercutido4_3->text();
    data["cuenta_iva_repercutido1_re"]=  ui->ivarepercutidore1_3->text();
    data["cuenta_iva_repercutido2_re"]=  ui->ivarepercutidore2_3->text();
    data["cuenta_iva_repercutido3_re"]=  ui->ivarepercutidore3_3->text();
    data["cuenta_iva_repercutido4_re"]=  ui->ivarepercutidore4_3->text();
    data["cuenta_iva_soportado1_re"]=  ui->ivasoportadore1_3->text();
    data["cuenta_iva_soportado2_re"]=  ui->ivasoportadore2_3->text();
    data["cuenta_iva_soportado3_re"]=  ui->ivasoportadore3_3->text();
    data["cuenta_iva_soportado4_re"]=  ui->ivasoportadore4_3->text();    
    data["importe_cierre"]= ui->spinImporte_abertura->value();
    data["facturas_en_cierre"]= ui->checkMostrarfacturascierrecaja->isChecked();
    data["tpv_forzar_cantidad"]= ui->checktpv_forzar_cantidad->isChecked();
    data["caducidad_vales"]= ui->txtCaducidadvales_3->value();

    QByteArray bArray;
    QBuffer buffer( &bArray );
    buffer.open( QIODevice::WriteOnly );
    ui->label_logo_edit->pixmap()->save( &buffer, "JPG" );
    QByteArray b_64 = bArray.toBase64();
    data["logo"] = b_64;

    QString error;

    if(_targetGroupDb.isOpen())
    {
        if(SqlCalls::SqlUpdate(data,"empresas",_targetGroupDb,QString("id = %1").arg(_idEmpresa),error))
            TimedMessageBox *m = new TimedMessageBox(this,tr("Las modificaciones han sido guardadas"));
        else
            QMessageBox::warning(this,tr("Gestión de empresas"),tr("No se han podido guardar los datos de la empresa:\n %1").arg(error),
                                 tr("Aceptar"));
    }
    _targetGroupDb.close();
    QSqlDatabase::removeDatabase("save_empresa");
    QSqlDatabase::database("series_db").close();
    QSqlDatabase::removeDatabase("series_db");
    tarifaEditModel->deleteLater();
    monedaEditModel->deleteLater();
    seriesEditModel->deleteLater();
    paisesEditModel->deleteLater();

    ui->stackedWidget->setCurrentWidget(ui->main_page);    
}

void FrmEmpresas::on_btnCambiarLogo_create_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Imagen de logotipo"),QString(),tr("Imagen (*.jpg)"));
    if(!filename.isEmpty())
    {
        QPixmap p(filename);
        ui->label_logo_create->setMaximumSize(p.size());
        ui->label_logo_create->setPixmap(p);
    }
}

void FrmEmpresas::on_btnCambiarLogo_edit_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Imagen de logotipo"),QString(),tr("Imagen (*.jpg)"));
    if(!filename.isEmpty())
    {
        QPixmap p(filename);
        ui->label_logo_edit->setMaximumSize(p.size());
        ui->label_logo_edit->setPixmap(p);
    }
}
