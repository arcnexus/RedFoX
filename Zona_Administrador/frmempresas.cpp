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
    QStyle *style = this ? this->style() : QApplication::style();
    QIcon tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, this);
    ui->blinkink->seticon(tmpIcon.pixmap(32,32));


    connect(ui->txtpoblacion,SIGNAL(editingFinished()),this,SLOT(txtpoblacion_editingFinished()));
    connect(ui->txtcp,SIGNAL(editingFinished()),this,SLOT(txtcp_editingFinished()));
    //------------------
    // LLeno divisas
    //------------------
    QSqlQueryModel *modelDivisas = new QSqlQueryModel(this);
    modelDivisas->setQuery("select moneda from monedas",Configuracion_global->groupDB);
    ui->cboDivisas->setModel(modelDivisas);

    //----------------
    // LLeno Tarifas
    //----------------
    QSqlQueryModel *modelTarifas = new QSqlQueryModel(this);
    modelTarifas->setQuery("select descripcion from codigotarifa",Configuracion_global->groupDB);
    ui->cboTarifa->setModel(modelTarifas);

    getEmpresas();
    showEmpresas();
    blockGUI(true,false);
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
    ui->txtnombre_bd->setText(oEmpresa.getnombre_bd());

    int indice=ui->txtcDriver->findText(oEmpresa.getcDriverBD());
    if(indice!=-1)
       ui->txtcDriver->setCurrentIndex(indice);

    ui->txtRutaBd->setText(oEmpresa.getruta_bd_sqlite());
    ui->txtnombre_bdMedic->setText(oEmpresa.getnombre_bdMed());


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


    ui->txtNombre_BDConta->setText(oEmpresa.nombre_bd_contabilidad);


    ui->txtruta_bd_conta->setText(oEmpresa.RutaBD_Contabilidad_sqlite);
    ui->chk_ticket_factura->setChecked(oEmpresa.ticket_factura);
    index = ui->cboTarifa->findText(Configuracion_global->Devolver_tarifa(oEmpresa.id_tarifa_predeterminada));
    ui->cboTarifa->setCurrentIndex(index);
    ui->chk_upate_divisas->setChecked(oEmpresa.actualizar_divisas);
    ui->chkMedica->setChecked(oEmpresa.empresa_medica);

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
             QSqlQuery qPoblacion(Configuracion_global->groupDB);
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
                QSqlQuery qPoblacion(Configuracion_global->groupDB);
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

void FrmEmpresas::on_addGrupo_clicked()
{
    addGroupFrom f(this);
    if(f.exec() == QDialog::Accepted)
    {
        getEmpresas();
        showEmpresas();
    }
}


void FrmEmpresas::showEmpresas()
{
    ui->treeEmpresas->clear();

    QHashIterator<QString, QPair<QList<empresa> , QSqlRecord> > it(_empresas);

    while(it.hasNext())
    {
        it.next();
        QString grupo_name = it.key();

        QTreeWidgetItem * root = new QTreeWidgetItem(ui->treeEmpresas);
        root->setText(0,grupo_name);
        QFont f("Arial");
        f.setBold(true);
        f.setItalic(true);
        f.setWeight(10);

        root->setFont(0,f);
        root->setTextColor(0,Qt::darkGreen);

        for(int i=0; i<it.value().first.size(); i++)
        {
            QTreeWidgetItem * item = new QTreeWidgetItem(root);
            item->setText(0,it.value().first.at(i).name);
        }
        ui->treeEmpresas->addTopLevelItem(root);
    }
    ui->treeEmpresas->expandAll();
}

void FrmEmpresas::getEmpresas()
{
    _empresas.clear();
    QScopedPointer<QSqlQuery>QryEmpresas(new QSqlQuery(QSqlDatabase::database("Global")));
    //QSqlQuery *QryEmpresas = new QSqlQuery(Configuracion_global->groupDB);

    QryEmpresas->prepare("Select * from grupos");

    if(QryEmpresas->exec())
    {
        while (QryEmpresas->next())
        {
            QSqlRecord rEmpresa = QryEmpresas->record();

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
}

void FrmEmpresas::_addEmpresa()
{
    if(ui->treeEmpresas->selectedItems().isEmpty())
        return;
    QTreeWidgetItem * item;

    if(ui->treeEmpresas->selectedItems().at(0)->childCount()==0)
    {
        item = ui->treeEmpresas->selectedItems().at(0)->parent();
        if(!item)
            item = ui->treeEmpresas->selectedItems().at(0);
    }
    else
        item = ui->treeEmpresas->selectedItems().at(0);

    QSqlRecord r = _empresas.value(item->text(0)).second;

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


        QFile f(":/Icons/DB/DBEmpresa.sql");
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
                if(!querys.at(i).isEmpty())
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

        if(!error)
        {
            QString query = QString(
                        "INSERT INTO `@grupo@`.`empresas` "
                        "(`codigo`, `nombre`, `digitos_factura`, `serie`, `nombre_bd`, `nombre_db_conta`,"
                        "`nombre_bd_medica`, `direccion`, `cp`, `poblacion`, `provincia`, `pais`,"
                        "`telefono1`, `telefono2`, `fax`, `mail`, `web`, `cif`, `inscripcion`,"
                        "`comentario_albaran`, `comentario_factura`, `comentario_ticket`,"
                        "`ejercicio`, `usar_irpf`, `codigo_cuenta_clientes`,"
                        "`codigo_cuenta_proveedores`, `codigo_cuenta_acreedores`,"
                        "`digitos_cuenta`, `clave1`, `clave2`, `medica`, `internacional`,"
                        "`auto_codigo`, `tamanocodigo`, `cuenta_cobros`, `cuenta_pagos`,"
                        "`id_divisa`, `enlaceweb`, `contabilidad`, `consultas`, `primer_dia_laborable`,"
                        "`ultimo_dia_laborable`, `horario_primer_dia`, `horario_dia_normal`,"
                        "`horario_ultimo_dia`, `ticket_factura`, `margen`, `margen_minimo`, `seguimiento`,"
                        "`id_tarifa_predeterminada`, `actualizardivisas`, `cuenta_ventas_mercaderias`,"
                        "`cuenta_ventas_servicios`, `cuenta_iva_soportado1`, `cuenta_iva_soportado2`,"
                        "`cuenta_iva_soportado3`, `cuenta_iva_soportado4`, `cuenta_iva_repercutido1`,"
                        "`cuenta_iva_repercutido2`, `cuenta_iva_repercutido3`, `cuenta_iva_repercutido4`,"
                        "`cuenta_iva_repercutido1_re`, `cuenta_iva_repercutido2_re`, `cuenta_iva_repercutido3_re`,"
                        "`cuenta_iva_repercutido4_re`, `cuenta_iva_soportado1_re`, `cuenta_iva_soportado2_re`,"
                        "`cuenta_iva_soportado3_re`, `cuenta_iva_soportado4_re`)"
                        "VALUES "
                        "(:codigo, :nombre, :digitos_factura, :serie, :nombre_bd, :nombre_db_conta,"
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
                        ":cuenta_iva_soportado3_re, :cuenta_iva_soportado4_re);"
                        );
            query.replace("@grupo@",r.value("bd_name").toString());
            q.prepare(query);
            q.bindValue(":codigo",ui->txtcodigo->text());
            q.bindValue(":nombre",ui->txtEmpresa->text());
            q.bindValue(":digitos_factura",ui->txtndigitos_factura->text().toInt());
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
            q.bindValue(":id_divisa",0);//TODO divisa
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
            q.bindValue(":id_tarifa_predeterminada",0);
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

            if(!q.exec())
                qDebug() << q.lastError().text();
        }
    }
    else
        qDebug() << db.lastError().text();
}

void FrmEmpresas::_llenarCampos(QSqlRecord r)
{
   // oEmpresa.id = r.value("id").toInt();
//    oEmpresa.Recuperar("select * from empresas where id = "+QString::number(oEmpresa.id));
//    LLenarCampos();
    ui->txtcodigo->setText(r.value("codigo").toString());
    ui->txtEmpresa->setText(r.value("nombre").toString());//
    ui->txtndigitos_factura->setText(r.value("digitos_factura").toString());//->text().toInt());
    ui->txtserieFactura->setText(r.value("serie").toString());//text().toInt());
    ui->txtnombre_bd->setText(r.value("nombre_bd").toString());//nEmpresa);
    ui->txtNombre_BDConta->setText(r.value("nombre_db_conta").toString());//nConta);
    ui->txtnombre_bdMedic->setText(r.value("nombre_bd_medica").toString());//nMedic);
    ui->txtdireccion1->setText(r.value("direccion").toString());
    ui->txtcp->setText(r.value("cp").toString());
    ui->txtpoblacion->setText(r.value("poblacion").toString());
    ui->txtprovincia->setText(r.value("provincia").toString());
    ui->txtpais->setText(r.value("pais").toString());
    ui->txttelefono1->setText(r.value("telefono1").toString());
    ui->txttelefono2->setText(r.value("telefono2").toString());
    ui->txtfax->setText(r.value("fax").toString());
    ui->txtcMail->setText(r.value("mail").toString());
    ui->txtweb->setText(r.value("web").toString());
    ui->txtcif->setText(r.value("cif").toString());
    ui->txtcInscripcion->setText(r.value("inscripcion").toString());
    ui->txtcCometarioAlbaran->setText(r.value("comentario_albaran").toString());
    ui->txtccomentario_factura->setText(r.value("comentario_factura").toString());
    ui->txtccomentario_ticket->setText(r.value("comentario_ticket").toString());
    ui->txtejercicio->setValue(r.value("ejercicio").toInt());
    ui->chkIRPF->setChecked(r.value("usar_irpf").toBool());
    ui->txtcuentaCliente->setText(r.value("codigo_cuenta_clientes").toString());
    ui->txtcuenta_proveedores->setText(r.value("codigo_cuenta_proveedores").toString());
    ui->txtcuenta_acreedores->setText(r.value("codigo_cuenta_acreedores").toString());
    ui->txtdigitos_cuentas->setValue(r.value("digitos_cuenta").toInt());
    ui->txtclave1->setText(r.value("clave1").toString());
    ui->txtclave2->setText(r.value("clave2").toString());
    ui->chkMedica->setChecked(r.value("medica").toBool());
    ui->chkInternacional->setChecked(r.value("internacional").toBool());
    ui->chkAutocodificiar->setChecked(r.value("auto_codigo").toBool());
    ui->txttamano_codigoart->setValue(r.value("tamanocodigo").toBool());
    ui->txtcuenta_cobros->setText(r.value("cuenta_cobros").toString());
    ui->txtcuenta_pagos->setText(r.value("cuenta_pagos").toString());
    int index = ui->cboDivisas->findText(Configuracion_global->Devolver_moneda(r.value("id_divisa").toInt()));
    ui->cboDivisas->setCurrentIndex(index);
    ui->chkEnlace_web->setChecked(r.value("enlaceweb").toBool());
    ui->chkContabilidad->setChecked(r.value("contabilidad").toBool());
    //r.value("consultas"));//0) TODO - N.P.I de para que puse ese campo GRRRRR;
    ui->cboPrimer_dia_laborable->setCurrentIndex(r.value("primer_dia_laborable").toInt());
    ui->cbo_ultimo_dia_semana->setCurrentIndex(r.value("ultimo_dia_laborable").toInt());
    ui->txt_horario_primer_dia->setText(r.value("horario_primer_dia").toString());
    ui->txt_horario_dia_normal->setText(r.value("horario_dia_normal").toString());
    ui->txt_horario_ultimo_dia->setText(r.value("horario_ultimo_dia").toString());
    ui->chk_ticket_factura->setText(r.value("ticket_factura").toString());
    ui->spinMargen->setValue(r.value("margen").toDouble());
    ui->spinMargen_minimo->setValue(r.value("margen_minimo").toDouble());
    ui->chkSeguimiento->setChecked(r.value("seguimiento").toDouble());
    index = ui->cboTarifa->findText(Configuracion_global->Devolver_tarifa(r.value("id_tarifa_predeterminada").toInt()));
    ui->cboTarifa->setCurrentIndex(index);
    ui->chk_upate_divisas->setChecked(r.value("actualizardivisas").toBool());
    ui->txtCuenta_venta_mercaderias->setText(r.value("cuenta_ventas_mercaderias").toString());
    ui->txtCuenta_venta_servicios->setText(r.value("cuenta_ventas_servicios").toString());
    ui->ivasoportado1->setText(r.value("cuenta_iva_soportado1").toString());
    ui->ivasoportado2->setText(r.value("cuenta_iva_soportado2").toString());
    ui->ivasoportado3->setText(r.value("cuenta_iva_soportado3").toString());
    ui->ivasoportado4->setText(r.value("cuenta_iva_soportado4").toString());
    ui->ivarepercutido1->setText(r.value("cuenta_iva_repercutido1").toString());
    ui->ivarepercutido2->setText(r.value("cuenta_iva_repercutido2").toString());
    ui->ivarepercutido3->setText(r.value("cuenta_iva_repercutido3").toString());
    ui->ivarepercutido4->setText(r.value("cuenta_iva_repercutido4").toString());
    ui->ivarepercutidore1->setText(r.value("cuenta_iva_repercutido1_re").toString());
    ui->ivarepercutidore2->setText(r.value("cuenta_iva_repercutido2_re").toString());
    ui->ivarepercutidore3->setText(r.value("cuenta_iva_repercutido3_re").toString());
    ui->ivarepercutidore4->setText(r.value("cuenta_iva_repercutido4_re").toString());
    ui->ivasoportadore1->setText(r.value("cuenta_iva_soportado1_re").toString());
    ui->ivasoportadore2->setText(r.value("cuenta_iva_soportado2_re").toString());
    ui->ivasoportadore3->setText(r.value("cuenta_iva_soportado3_re").toString());
    ui->ivasoportadore4->setText(r.value("cuenta_iva_soportado4_re").toString());
}

void FrmEmpresas::blockGUI(bool block, bool limpia)
{
    QList<QLineEdit*> l = this->findChildren<QLineEdit*>();
    QList<QLineEdit*>::Iterator it;
    for(it=l.begin();it!=l.end();++it)
    {
        if(limpia)
            (*it)->clear();
        (*it)->setReadOnly(block);
    }
    QList<QTextEdit*> l2 = this->findChildren<QTextEdit*>();
    QList<QTextEdit*>::Iterator it2;
    for(it2=l2.begin();it2!=l2.end();++it2)
    {
        if(limpia)
            (*it2)->clear();
        (*it2)->setReadOnly(block);
    }
}

void FrmEmpresas::on_btnAddEmpresa_clicked()
{
    if(ui->treeEmpresas->selectedItems().isEmpty())
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Seleccione un grupo donde crear la empresa"));
        return;
    }
    if(ui->btnAddEmpresa->text()== tr("Añadir Empresa"))
    {
        ui->btnAddEmpresa->setText(tr("Guardar"));
        blockGUI(false, true);
        ui->addGrupo->setEnabled(false);
        ui->btnBorrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
    }
    else
    {
        ui->btnAddEmpresa->setText(tr("Añadir Empresa"));
        _addEmpresa();
        getEmpresas();
        showEmpresas();
        blockGUI(true, false);
        ui->addGrupo->setEnabled(true);
        ui->btnBorrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
    }
}

void FrmEmpresas::on_treeEmpresas_itemSelectionChanged()
{
    if(ui->treeEmpresas->selectedItems().isEmpty())
        return;
    if(!ui->treeEmpresas->selectedItems().at(0)->childCount()==0)
        return;

    QTreeWidgetItem * p = ui->treeEmpresas->selectedItems().at(0)->parent();
    if(!p)
        return;

    QTreeWidgetItem * item = ui->treeEmpresas->selectedItems().at(0);

    QPair<QList<empresa> , QSqlRecord> x = _empresas.value(p->text(0));
    QList<empresa>::Iterator it;
    for(it=x.first.begin();it!=x.first.end();++it)
    {
        if( (*it).name == item->text(0) )
        {
            _llenarCampos((*it).record);
        }
    }
}

void FrmEmpresas::on_btnEditar_clicked()
{

    if(ui->btnEditar->text() == tr("Editar"))
    {
        ui->btnEditar->setText(tr("Guardar"));
        blockGUI(false,false);
        ui->txtdireccion1->setFocus();
    } else
    {
        CargarCamposEnEmpresa();
        blockGUI(true,false);
        ui->btnEditar->setText(tr("Editar"));
        oEmpresa.Guardar();
    }
}
