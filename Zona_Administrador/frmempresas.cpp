#include "frmempresas.h"
#include "ui_frmempresas.h"

#include "../Zona_Administrador/frmconfigurar_terminal.h"
#include "../sqlcalls.h"

FrmEmpresas::FrmEmpresas(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmEmpresas),
    menuButton(QIcon(":/Icons/PNG/empresa.png"),tr("Empresa"),this)
{
    ui->setupUi(this);
    QStyle *style = this ? this->style() : QApplication::style();
    QIcon tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, this);
    ui->blinkink->seticon(tmpIcon.pixmap(32,32));
    on_btn_inicio_clicked();

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
    oEmpresa.Email_contrasena  = ui->txtEmail_contrasena->text();
    oEmpresa.Email_imap = ui->txtEmail_imap->text();
    oEmpresa.Email_pop = ui->txtEmail_pop->text();
    oEmpresa.Email_smtp= ui->txtEmail_smtp->text();
    oEmpresa.Email_usuario = ui->txtEmail_usuario->text();
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
            QString query = QString(
                        "INSERT INTO `@grupo@`.`empresas` "
                        "(`codigo`, `nombre`, `digitos_factura`,`decimales_campos_totales`, `serie`, `nombre_bd`, `nombre_db_conta`,"
                        "`nombre_bd_medica`, `direccion`, `cp`, `poblacion`, `provincia`, `pais`,"
                        "`telefono1`, `telefono2`, `fax`, `mail`, `web`, `cif`, `inscripcion`,"
                        "`comentario_albaran`, `comentario_factura`, `comentario_ticket`,"
                        "`ejercicio`, `usar_irpf`, `codigo_cuenta_clientes`,"
                        "`codigo_cuenta_proveedores`, `codigo_cuenta_acreedores`,"
                        "`digitos_cuenta`, `clave1`, `clave2`, `medica`, `internacional`,"
                        "`auto_codigo`, `tamano_codigo`, `cuenta_cobros`, `cuenta_pagos`,"
                        "`id_divisa`, `enlace_web`, `contabilidad`, `consultas`, `primer_dia_laborable`,"
                        "`ultimo_dia_laborable`, `horario_primer_dia`, `horario_dia_normal`,"
                        "`horario_ultimo_dia`, `ticket_factura`, `margen`, `margen_minimo`, `seguimiento`,"
                        "`id_tarifa_predeterminada`, `actualizar_divisas`, `cuenta_ventas_mercaderias`,"
                        "`cuenta_ventas_servicios`, `cuenta_iva_soportado1`, `cuenta_iva_soportado2`,"
                        "`cuenta_iva_soportado3`, `cuenta_iva_soportado4`, `cuenta_iva_repercutido1`,"
                        "`cuenta_iva_repercutido2`, `cuenta_iva_repercutido3`, `cuenta_iva_repercutido4`,"
                        "`cuenta_iva_repercutido1_re`, `cuenta_iva_repercutido2_re`, `cuenta_iva_repercutido3_re`,"
                        "`cuenta_iva_repercutido4_re`, `cuenta_iva_soportado1_re`, `cuenta_iva_soportado2_re`,"
                        "`cuenta_iva_soportado3_re`, `cuenta_iva_soportado4_re`, `caducidad_vales`)"
                        "VALUES "
                        "(:codigo, :nombre, :digitos_factura,:decimales_campos_totales, :serie, :nombre_bd, :nombre_db_conta,"
                        ":nombre_bd_medica, :direccion, :cp, :poblacion, :provincia, :pais,"
                        ":telefono1, :telefono2, :fax, :mail, :web, :cif, :inscripcion,"
                        ":comentario_albaran, :comentario_factura, :comentario_ticket,"
                        ":ejercicio, :usar_irpf, :codigo_cuenta_clientes,"
                        ":codigo_cuenta_proveedores, :codigo_cuenta_acreedores,"
                        ":digitos_cuenta, :clave1, :clave2, :medica, :internacional,"
                        ":auto_codigo, :tamanocodigo, :cuenta_cobros, :cuenta_pagos,"
                        ":id_divisa, :enlaceweb, :contabilidad, :consultas, :primer_dia_laborable,"
                        ":ultimo_dia_laborable, :horario_primer_dia, :horario_dia_normal,"
                        ":horario_ultimo_dia, :ticket_factura, :margen, :margen_minimo, :seguimiento,"
                        ":id_tarifa_predeterminada, :actualizardivisas, :cuenta_ventas_mercaderias,"
                        ":cuenta_ventas_servicios, :cuenta_iva_soportado1, :cuenta_iva_soportado2,"
                        ":cuenta_iva_soportado3, :cuenta_iva_soportado4, :cuenta_iva_repercutido1,"
                        ":cuenta_iva_repercutido2, :cuenta_iva_repercutido3, :cuenta_iva_repercutido4,"
                        ":cuenta_iva_repercutido1_re, :cuenta_iva_repercutido2_re, :cuenta_iva_repercutido3_re,"
                        ":cuenta_iva_repercutido4_re, :cuenta_iva_soportado1_re, :cuenta_iva_soportado2_re,"
                        ":cuenta_iva_soportado3_re, :cuenta_iva_soportado4_re,:caducidad_vales);"
                        );
            query.replace("@grupo@",r.value("bd_name").toString());
            q.prepare(query);
            q.bindValue(":codigo",ui->txtcodigo->text());
            q.bindValue(":nombre",ui->txtEmpresa->text());
            q.bindValue(":digitos_factura",ui->txtndigitos_factura->text().toInt());
            q.bindValue(":decimales_campos_totales",ui->spinDecimales->value());
            q.bindValue(":serie",ui->txtserieFactura->text().toInt());
            q.bindValue(":nombre_bd",nEmpresa);
            q.bindValue(":nombre_db_conta",nConta);
            q.bindValue(":nombre_bd_medica",nMedic);
            q.bindValue(":direccion",ui->txtdireccion1->text());
            q.bindValue(":cp",ui->txtcp->text());
            q.bindValue(":poblacion",ui->txtpoblacion->text());
            q.bindValue(":provincia",ui->txtprovincia->text());
            q.bindValue(":pais",ui->txtpais->text());//TODO cambiar esto
            q.bindValue(":telefono1",ui->txttelefono1->text());
            q.bindValue(":telefono2",ui->txttelefono2->text());
            q.bindValue(":fax",ui->txtfax->text());
            q.bindValue(":mail",ui->txtcMail->text());
            q.bindValue(":web",ui->txtweb->text());
            q.bindValue(":cif",ui->txtcif->text());
            q.bindValue(":inscripcion",ui->txtcInscripcion->text());
            q.bindValue(":comentario_albaran",ui->txtcCometarioAlbaran->toPlainText());
            q.bindValue(":comentario_factura",ui->txtccomentario_factura->toPlainText());
            q.bindValue(":comentario_ticket",ui->txtccomentario_ticket->toPlainText());
            q.bindValue(":ejercicio",ui->txtejercicio->value());
            q.bindValue(":usar_irpf",0);//TODO irpf empresa
            q.bindValue(":codigo_cuenta_clientes",ui->txtcuentaCliente->text());
            q.bindValue(":codigo_cuenta_proveedores",ui->txtcuenta_proveedores->text());
            q.bindValue(":codigo_cuenta_acreedores",ui->txtcuenta_acreedores->text());
            q.bindValue(":digitos_cuenta",ui->txtdigitos_cuentas->value());
            q.bindValue(":clave1","");//TODO claves
            q.bindValue(":clave2","");
            q.bindValue(":medica",ui->chkMedica->isChecked());
            q.bindValue(":internacional",0);
            q.bindValue(":auto_codigo",ui->chkAutocodificiar->isChecked());
            q.bindValue(":tamanocodigo",ui->txttamano_codigoart->value());
            q.bindValue(":cuenta_cobros","");//TODO c cobros
            q.bindValue(":cuenta_pagos","");//TODO c pagos
            q.bindValue(":id_divisa",monedaEditModel->record(ui->cboDivisas->currentIndex()).value("id"));
            q.bindValue(":enlaceweb",0);//TODO Grrr
            q.bindValue(":contabilidad",0);
            q.bindValue(":consultas",ui->txtConsultas->value());
            q.bindValue(":primer_dia_laborable",ui->cboPrimer_dia_laborable->currentIndex());
            q.bindValue(":ultimo_dia_laborable",ui->cbo_ultimo_dia_semana->currentIndex());
            q.bindValue(":horario_primer_dia",ui->txt_horario_primer_dia->text());
            q.bindValue(":horario_dia_normal",ui->txt_horario_dia_normal->text());
            q.bindValue(":horario_ultimo_dia",ui->txt_horario_ultimo_dia->text());
            q.bindValue(":ticket_factura",ui->chk_ticket_factura->isChecked());
            q.bindValue(":margen",0);
            q.bindValue(":margen_minimo",0);
            q.bindValue(":seguimiento",0);
            q.bindValue(":id_tarifa_predeterminada",tarifaEditModel->record(ui->cboTarifa->currentIndex()).value("id"));
            q.bindValue(":actualizardivisas",ui->chk_upate_divisas->isChecked());
            q.bindValue(":cuenta_ventas_mercaderias",ui->txtCuenta_venta_mercaderias->text());
            q.bindValue(":cuenta_ventas_servicios",ui->txtCuenta_venta_servicios->text());
            q.bindValue(":cuenta_iva_soportado1",ui->ivasoportado1->text());
            q.bindValue(":cuenta_iva_soportado2",ui->ivasoportado2->text());
            q.bindValue(":cuenta_iva_soportado3",ui->ivasoportado3->text());
            q.bindValue(":cuenta_iva_soportado4",ui->ivasoportado4->text());
            q.bindValue(":cuenta_iva_repercutido1",ui->ivarepercutido1->text());
            q.bindValue(":cuenta_iva_repercutido2",ui->ivarepercutido2->text());
            q.bindValue(":cuenta_iva_repercutido3",ui->ivarepercutido3->text());
            q.bindValue(":cuenta_iva_repercutido4",ui->ivarepercutido4->text());
            q.bindValue(":cuenta_iva_repercutido1_re",ui->ivarepercutidore1->text());
            q.bindValue(":cuenta_iva_repercutido2_re",ui->ivarepercutidore2->text());
            q.bindValue(":cuenta_iva_repercutido3_re",ui->ivarepercutidore3->text());
            q.bindValue(":cuenta_iva_repercutido4_re",ui->ivarepercutidore4->text());
            q.bindValue(":cuenta_iva_soportado1_re",ui->ivasoportadore1->text());
            q.bindValue(":cuenta_iva_soportado2_re",ui->ivasoportadore2->text());
            q.bindValue(":cuenta_iva_soportado3_re",ui->ivasoportadore3->text());
            q.bindValue(":cuenta_iva_soportado4_re",ui->ivasoportadore4->text());
            q.bindValue(":caducidad_vales",ui->txtCaducidadvales->value());

            if(!q.exec())
                qDebug() << q.lastError().text();
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
    ui->txtndigitos_factura_3->setText(r.value("digitos_factura").toString());//->text().toInt());
    ui->txtserieFactura_3->setText(r.value("serie").toString());//text().toInt());
    ui->txtnombre_bd_3->setText(r.value("nombre_bd").toString());//nEmpresa);
    ui->txtNombre_BDConta_3->setText(r.value("nombre_db_conta").toString());//nConta);
    ui->txtnombre_bdMedic_3->setText(r.value("nombre_bd_medica").toString());//nMedic);
    ui->txtdireccion1_3->setText(r.value("direccion").toString());
    ui->txtcp_3->setText(r.value("cp").toString());
    ui->txtpoblacion_3->setText(r.value("poblacion").toString());
    ui->txtprovincia_3->setText(r.value("provincia").toString());
    ui->txtpais_3->setText(r.value("pais").toString());
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
    ui->txttamano_codigoart_3->setValue(r.value("tamanocodigo").toBool());
    ui->txtcuenta_cobros_3->setText(r.value("cuenta_cobros").toString());
    ui->txtcuenta_pagos_3->setText(r.value("cuenta_pagos").toString());

    ui->chkEnlace_web_3->setChecked(r.value("enlaceweb").toBool());
    ui->chkContabilidad_3->setChecked(r.value("contabilidad").toBool());
    //r.value("consultas"));//0) TODO - N.P.I de para que puse ese campo GRRRRR;
    ui->cboPrimer_dia_laborable_3->setCurrentIndex(r.value("primer_dia_laborable").toInt());
    ui->cbo_ultimo_dia_semana_3->setCurrentIndex(r.value("ultimo_dia_laborable").toInt());
    ui->txt_horario_primer_dia_3->setText(r.value("horario_primer_dia").toString());
    ui->txt_horario_dia_normal_3->setText(r.value("horario_dia_normal").toString());
    ui->txt_horario_ultimo_dia_3->setText(r.value("horario_ultimo_dia").toString());
    ui->chk_ticket_factura_3->setChecked(r.value("ticket_factura").toBool());
    ui->spinMargen_3->setValue(r.value("margen").toDouble());
    ui->spinMargen_minimo_3->setValue(r.value("margen_minimo").toDouble());
    ui->chkSeguimiento_3->setChecked(r.value("seguimiento").toDouble());

    ui->chk_upate_divisas_3->setChecked(r.value("actualizardivisas").toBool());
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
    ui->txtEmail_contrasena_3->setText(r.value("password_cuenta").toString());
    ui->txtEmail_imap_3->setText(r.value("cuenta_imap").toString());
    ui->txtEmail_pop_3->setText(r.value("cuenta_pop").toString());
    ui->txtEmail_smtp_3->setText(r.value("cuenta_smtp").toString());
    ui->txtEmail_usuario_3->setText(r.value("cuenta_mail").toString());
    ui->txtCaducidadvales_3->setValue(r.value("caducidad_vales").toInt());
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

    qDebug() << _targetGroupDbRecord.value("bd_pass").toString();
    if(_targetGroupDb.open())
    {
        tarifaEditModel = new QSqlQueryModel(this);
        monedaEditModel = new QSqlQueryModel(this);

        tarifaEditModel->setQuery("Select `id`,`descripcion` from codigotarifa",_targetGroupDb);
        monedaEditModel->setQuery("Select `id`,`moneda` from monedas",_targetGroupDb);

        ui->cboTarifa->setModel(tarifaEditModel);
        ui->cboTarifa->setModelColumn(1);

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
            error = _insertNivelAcesso(db, sError);

        QApplication::processEvents();
        if(!error)
            error = _insertPaises(db, sError);

        //TODO Poblaciones y Reports por defecto al crear grupo
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
    ui->frame->show();
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
bool FrmEmpresas::_insertNivelAcesso(QSqlDatabase db, QString& error)
{
    QSqlQuery q(db);
    bool e;
    e = q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (1,'Sin Acceso');");
    e &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (2,'Lectura parcial');");
    e &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (3,'Lectura total');");
    e &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (4,'Escritura parcial (editar)');");
    e &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (5,'Escritura parcial (añadir)');");
    e &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (6,'Escritural total');");
    e &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (7,'Administrador');");

    e &= q.exec("INSERT INTO `usuarios` (`nombre`, `contrasena`, `nivel_acceso`, `categoria`) VALUES ('admin', 'jGl25bVBBBW96Qi9Te4V37Fnqchz/Eu4qB9vKrRIqRg=', '99', 'ADMINISTRADOR');");
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
    ui->frame->show();
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

        tarifaEditModel->setQuery("Select `id`,`descripcion` from codigotarifa",_targetGroupDb);
        monedaEditModel->setQuery("Select `id`,`moneda` from monedas",_targetGroupDb);

        ui->cboTarifa->setModel(tarifaEditModel);
        ui->cboTarifa->setModelColumn(1);

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
    data["digitos_factura"]= ui->txtndigitos_factura_3->text();
    data["serie"]= ui->txtserieFactura_3->text();
    data["decimales_campos_totales"] = ui->spinDecimales_2->value();
    data["decimales"]=   ui->spinDecimales_precios_2->value();
    //data["mostrarsiempre"]=  ui->ch
    //data["ruta_bd_sqlite"]=  ;
    //data["ruta_db_conta"]=  ;
    //data["ruta_bd_medica_sqlite"]=  ;
    data["nombre_bd"]=  ui->txtnombre_bd_3->text();
    data["nombre_db_conta"]=  ui->txtNombre_BDConta_3->text();
    data["nombre_bd_medica"]=  ui->txtnombre_bdMedic_3->text();
    data["direccion"]=  ui->txtdireccion1_3->text();
    data["cp"]=  ui->txtcp_3->text();
    data["poblacion"]=  ui->txtpoblacion_3->text();
    data["provincia"]=  ui->txtprovincia_3->text();
    data["pais"]=  ui->txtpais_3->text();
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
    data["primer_dia_laborable"]=  ui->cboPrimer_dia_laborable->currentText();
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

    //data["importada_sp"]=  ; esto debe ser true si ha sido importada (se debería hacer desde el programa de importación.

    data["importe_cierre"]= ui->spinImporte_abertura->value();
    data["facturas_en_cierre"]= ui->checkMostrarfacturascierrecaja->isChecked();

    data["tpv_forzar_cantidad"]= ui->checktpv_forzar_cantidad->isChecked();
    data["caducidad_vales"]= ui->txtCaducidadvales_3->value();

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
    tarifaEditModel->deleteLater();
    monedaEditModel->deleteLater();

    ui->stackedWidget->setCurrentWidget(ui->main_page);    
}
