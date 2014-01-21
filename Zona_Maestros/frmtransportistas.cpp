#include "frmtransportistas.h"
#include "ui_frmtransportistas.h"
#include "../Busquedas/db_consulta_view.h"

FrmTransportistas::FrmTransportistas(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmTransportistas),
    menuButton(QIcon(":/Icons/PNG/transport.png"),tr("Transportistas"),this)
{
    ui->setupUi(this);

    transportistas oTransportista(this);
    anadiendo = false;
    anadiendoContacto = false;
    id_contacto = 0;
    //ui->txtCodigo_proveedor->installEventFilter(this);
    //------------------
    // Busqueda
    //------------------
    ui->stackedWidget->setCurrentIndex(0);
    m = new QSqlQueryModel(this);
    m->setQuery("select id, codigo,transportista from transportista",Configuracion_global->groupDB);
    ui->tablaBusqueda->setModel(m);
    ui->tablaBusqueda->hideColumn(0);

    //---------------
    // Cargo combos
    //---------------
    QSqlQueryModel *paises = new QSqlQueryModel(this);
    paises->setQuery("select pais from paises order by pais",Configuracion_global->groupDB);
    ui->cbopais->setModel(paises);
    Bloquear_campos(true);

    //---------------
    //Llenar direcciones alternativas
    //---------------
    //    QSqlQueryModel *qModeldireccion = new QSqlQueryModel(this);
    //    qModeldireccion->setQuery("select nombre , telefono1 , telefono2 from personascontactotransportista where id_transportista = "+oTransportista.h_transportista.value("id").toString(),
    //                              Configuracion_global->groupDB);
    //    ui->tablaContactos->setModel(qModeldireccion);

    setUpBusqueda();
}

FrmTransportistas::~FrmTransportistas()
{
    delete ui;
}

void FrmTransportistas::on_btnAnadir_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    Bloquear_campos(false);
    anadiendo = true;
    vaciarCampos();

    oTransportista.h_transportista.clear();
    //oTransportista.anadir();
    ui->txtCodigo->setFocus();
}

void FrmTransportistas::Bloquear_campos(bool state)
{
    // -------------------------
    // campos transportista
    //--------------------------
    ui->txtCodigo->setReadOnly(state);
    ui->txtTransportista->setReadOnly(state);
    //ui->txtCodigo_proveedor->setReadOnly(state);
    //--------------------------
    // Botones
    //--------------------------
    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    ui->btnBorrar->setEnabled(state);
    ui->btnDeshacer->setEnabled(!state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->btnAnadirContacto->setEnabled(!state);
    ui->btnBorrarContacto->setEnabled(!state);
    ui->btnEditarContacto->setEnabled(!state);
    ui->pushButtonGuardarContacto->setEnabled(!state);
//    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
//    QLineEdit *lineEdit;
//    foreach (lineEdit, lineEditList) {
//        lineEdit->setReadOnly(state);
//    }
    ui->txtcif->setReadOnly(state);
    ui->txtdireccion1->setReadOnly(state);
    ui->txtdireccion2->setReadOnly(state);
    ui->txtcp->setReadOnly(state);
    ui->txtpoblacion->setReadOnly(state);
    ui->txtprovincia->setReadOnly(state);
    ui->txtfecha_alta->setReadOnly(state);
    ui->cbopais->setEnabled(!state);
    ui->txttelefono1->setReadOnly(state);
    ui->txttelefono2->setReadOnly(state);
    ui->txtmovil->setReadOnly(state);
    ui->txtfax->setReadOnly(state);
    ui->txtemail->setReadOnly(state);
    ui->txtweb->setReadOnly(state);
    ui->txtpersona_contacto->setReadOnly(state);





}

void FrmTransportistas::cargar_en_objeto()
{
    QString error;
    oTransportista.h_transportista["codigo"] = ui->txtCodigo->text();
    oTransportista.h_transportista["transportista"] = ui->txtTransportista->text();
    oTransportista.h_transportista["cif"] = ui->txtcif->text();
    oTransportista.h_transportista["fecha_alta"] = ui->txtfecha_alta->date().toString("yyyyMMdd");
    oTransportista.h_transportista["direccion1"] = ui->txtdireccion1->text();
    oTransportista.h_transportista["direccion2"] = ui->txtdireccion2->text();
    oTransportista.h_transportista["cp"] = ui->txtcp->text();
    oTransportista.h_transportista["poblacion"] = ui->txtpoblacion->text();
    oTransportista.h_transportista["provincia"] = ui->txtprovincia->text();
    oTransportista.h_transportista["pais"] = Configuracion_global->Devolver_id_pais(ui->cbopais->currentText());
    oTransportista.h_transportista["telefono1"] = ui->txttelefono1->text();
    oTransportista.h_transportista["telefono2"] = ui->txttelefono2->text();
    oTransportista.h_transportista["fax"] = ui->txtfax->text();
    oTransportista.h_transportista["movil"] = ui->txtmovil->text();
    oTransportista.h_transportista["email"] = ui->txtemail->text();
    oTransportista.h_transportista["web"] = ui->txtweb->text();
    oTransportista.h_transportista["contacto"] = ui->txtpersona_contacto->text();




    //int id_proveedor = SqlCalls::SelectOneField("proveedores","id",QString("codigo = '%1'").arg(ui->txtCodigo_proveedor->text()),Configuracion_global->groupDB,error).toInt();

    //oTransportista.h_transportista["id_proveedor"] = id_proveedor;
}

void FrmTransportistas::llenar_campos()
{
    //--------------------------
    // Definición transportista
    //--------------------------
    ui->txtCodigo->setText(oTransportista.h_transportista.value("codigo").toString());
    ui->txtTransportista->setText(oTransportista.h_transportista.value("transportista").toString());
    ui->txtcif->setText(oTransportista.h_transportista.value("cif").toString());
    ui->txtfecha_alta->setDate(oTransportista.h_transportista.value("fecha_alta").toDate());
    ui->txtdireccion1->setText(oTransportista.h_transportista.value("direccion1").toString());
    ui->txtdireccion2->setText(oTransportista.h_transportista.value("direccion2").toString());
    ui->txtcp->setText(oTransportista.h_transportista.value("cp").toString());
    ui->txtpoblacion->setText(oTransportista.h_transportista.value("poblacion").toString());
    ui->txtprovincia->setText(oTransportista.h_transportista.value("provincia").toString());
    int nindex = ui->cbopais->findText(oTransportista.RecuperarPais(oTransportista.h_transportista.value("pais").toInt()));
    if(nindex >-1)
        ui->cbopais->setCurrentIndex(nindex);
    ui->txttelefono1->setText(oTransportista.h_transportista.value("telefono1").toString());
    ui->txttelefono2->setText(oTransportista.h_transportista.value("telefono2").toString());
    ui->txtfax->setText(oTransportista.h_transportista.value("fax").toString());
    ui->txtmovil->setText(oTransportista.h_transportista.value("movil").toString());
    ui->txtemail->setText(oTransportista.h_transportista.value("email").toString());
    ui->txtweb->setText(oTransportista.h_transportista.value("web").toString());
    ui->txtpersona_contacto->setText(oTransportista.h_transportista.value("contacto").toString());



    llenar_campos_contactoTransportista(oTransportista.h_transportista.value("id").toInt());
    //qDebug() << oTransportista.h_transportista.value("id").toInt();
}

void FrmTransportistas::llenar_campos_contactoTransportista(int id)
{

    //---------------
    //Llenar direcciones alternativas
    //---------------
    QSqlQueryModel *qModeldireccion = new QSqlQueryModel(this);
    qModeldireccion->setQuery("select id , nombre , telefono1 , cargo_empresa from personascontactotransportista where id_transportista = "+QString::number(id),
                              Configuracion_global->groupDB);
    ui->tablaContactos->setModel(qModeldireccion);
    ui->tablaContactos->hideColumn(0);
//    QHeaderView *cabecera = new QHeaderView();
//    cabecera
//    ui->tablaContactos->setHorizontalHeader(cabecera);
    qDebug() << qModeldireccion->data(qModeldireccion->index(Qt::EditRole,0));
}

bool FrmTransportistas::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj == ui->txtCodigo)
        {
            if(keyEvent->key() == Qt::Key_F1)
                consultar_proveedor();
        }
    }
    if(event->type() == QEvent::Resize)
        _resizeBarraBusqueda(m_busqueda);
    return MayaModule::eventFilter(obj,event);
}

void FrmTransportistas::on_btnGuardar_clicked()
{
    if(anadiendo)
    {
        cargar_en_objeto();

        oTransportista.anadir();
        anadiendo = false;

    } else
    {
        cargar_en_objeto();
        oTransportista.guardar();
    }
    Bloquear_campos(true);
    emit unblock();
}

void FrmTransportistas::on_btnSiguiente_clicked()
{
    QStringList condiciones,extras;
    condiciones << QString("codigo >'%1'").arg(ui->txtCodigo->text().trimmed());
    extras << "order by codigo" <<"limit 0,1";
    oTransportista.recuperar(condiciones,extras);
    llenar_campos();
}

void FrmTransportistas::consultar_proveedor()
{
    db_consulta_view consulta;
    QStringList campos;
    campos << "proveedor" <<"codigo" << "cif" << "poblacion";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Proveedores");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,codigo,proveedor,poblacion from proveedores");
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Código") <<tr("Proveedor") << tr("cif/nif") << tr("Poblacion");
    tamanos <<"0" << "100"  <<"300" << "100" <<"100";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);

    consulta.set_titulo("Busqueda de Proveedores");
    if(consulta.exec())
    {
        int id_proveedor = consulta.get_id();

        //oTransportista.oProveedor.Recuperar(id_proveedor);
        //llenar_campos_transportista();
    }
}

void FrmTransportistas::llenar_contactos(int id_proveedor)
{
    QSqlQueryModel *modelContactos= new QSqlQueryModel(this);
    modelContactos->setQuery("select id,id_proveedor,cargo_empresa, nombre, desc_telefono1,telefono1,desc_telefono2,telefono2,"
                             "desc_telefono3, telefono3, desc_movil1,movil1,desc_movil2,movil2 "
                             "from personascontactoproveedor where id_proveedor ="+QString::number(id_proveedor),
                             Configuracion_global->groupDB);

    ui->tablaContactos->setModel(modelContactos);

    modelContactos->setHeaderData(0,Qt::Horizontal,tr("id"));
    modelContactos->setHeaderData(1,Qt::Horizontal,tr("id_proveedor"));
    modelContactos->setHeaderData(2,Qt::Horizontal,tr("CARGO"));
    modelContactos->setHeaderData(3,Qt::Horizontal,tr("NOMBRE"));
    modelContactos->setHeaderData(4,Qt::Horizontal,tr("DESC.T1"));
    modelContactos->setHeaderData(5,Qt::Horizontal,tr("TEL1"));
    modelContactos->setHeaderData(6,Qt::Horizontal,tr("DESC.T2"));
    modelContactos->setHeaderData(7,Qt::Horizontal,tr("TEL2"));
    modelContactos->setHeaderData(8,Qt::Horizontal,tr("DESC.T3"));
    modelContactos->setHeaderData(9,Qt::Horizontal,tr("TEL3"));
    modelContactos->setHeaderData(10,Qt::Horizontal,tr("DESC.M1"));
    modelContactos->setHeaderData(11,Qt::Horizontal,tr("Mov.1"));
    modelContactos->setHeaderData(12,Qt::Horizontal,tr("DESC.M2"));
    modelContactos->setHeaderData(13,Qt::Horizontal,tr("Mov.2"));

    ui->tablaContactos->setColumnHidden(0,true);
    ui->tablaContactos->setColumnHidden(1,true);

}

void FrmTransportistas::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);
    this->installEventFilter(this);

    QStringList orden;
    orden  <<  tr("Código") << tr("Descripción");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(this,&MayaModule::hideBusqueda,this,&FrmTransportistas::ocultarBusqueda);
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));


    QPushButton* add = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir transportista"),this);
    connect(add,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(add);

    QPushButton* edit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar transportista"),this);
    connect(edit,SIGNAL(clicked()),this,SLOT(on_btnEditar_clicked()));
    m_busqueda->addWidget(edit);

    QPushButton* print = new QPushButton(QIcon(":/Icons/PNG/print2.png"),tr("Imprimir transportista"),this);
   // connect(print,SIGNAL(clicked()),this,SLOT(on_btnEditar_2_clicked()));//TODO
    m_busqueda->addWidget(print);

    QPushButton* del = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar transportista"),this);
    connect(del,SIGNAL(clicked()),this,SLOT(on_btnBorrar_clicked()));
    m_busqueda->addWidget(del);
}

void FrmTransportistas::on_btnAnterior_clicked()
{
    QStringList condiciones,extras;
    condiciones << QString("codigo <'%1'").arg(ui->txtCodigo->text().trimmed());
    extras << "order by codigo desc" <<"limit 0,1";
    oTransportista.recuperar(condiciones,extras);
    llenar_campos();
}

void FrmTransportistas::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void FrmTransportistas::on_btnEditar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    emit block();
    Bloquear_campos(false);



}

void FrmTransportistas::on_tablaBusqueda_doubleClicked(const QModelIndex &index)
{
//    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tablaBusqueda->model());
//    int id = Configuracion_global->devolver_id_tabla(model,index);

    int id = ui->tablaBusqueda->model()->data(ui->tablaBusqueda->model()->index(index.row(),0),Qt::EditRole).toInt();

    QStringList filtro,extras;
    filtro << QString("id = %1").arg(id);
    extras << "";
    oTransportista.recuperar(filtro,extras);
    llenar_campos();
    Bloquear_campos(true);
    ui->stackedWidget->setCurrentIndex(1);
    ui->txtCodigo->setFocus();
}

void FrmTransportistas::on_tablaContactos_clicked(const QModelIndex &index)
{
    //QModelIndex index= ui->tablaContactos->currentIndex();
    int nid = ui->tablaContactos->model()->data(ui->tablaContactos->model()->index(index.row(),0),Qt::EditRole).toInt();
    this->id_contacto = nid;
    QSqlQuery queryContactos(Configuracion_global->groupDB);
    queryContactos.prepare("select * from personascontactotransportista where id = :id");
    queryContactos.bindValue(":id",this->id_contacto);
    if (!queryContactos.exec())
    {
        QMessageBox::warning(this,tr("ATENCIÓN:"),
                             tr("Ocurrió un error al recuperar los datos: %1").arg(queryContactos.lastError().text()),
                             tr("Aceptar"));
    } else
    {
        queryContactos.next();
        ui->txtNombre->setText(queryContactos.record().value("nombre").toString());
        ui->txtCargo->setText(queryContactos.record().value("cargo_empresa").toString());
        ui->txtDescripcionT1->setText(queryContactos.record().value("desc_telefono1").toString());
        ui->txtDescripcionT2->setText(queryContactos.record().value("desc_telefono2").toString());
        ui->txtDescripcionT3->setText(queryContactos.record().value("desc_telefono3").toString());
        ui->txtDescripcionM1->setText(queryContactos.record().value("desc_movil1").toString());
        ui->txtDescripcionM2->setText(queryContactos.record().value("desc_movil2").toString());
        ui->txtTelefono1->setText(queryContactos.record().value("telefono1").toString());
        ui->txtTelefono2->setText(queryContactos.record().value("telefono2").toString());
        ui->txtTelefono3->setText(queryContactos.record().value("telefono3").toString());
        ui->txtMovil1->setText(queryContactos.record().value("movil1").toString());
        ui->txtMovil2->setText(queryContactos.record().value("movil2").toString());
        ui->txtCargo->setText(queryContactos.record().value("cargo_empresa").toString());
        ui->txtemail_contacto->setText(queryContactos.record().value("email").toString());
    }
}

void FrmTransportistas::mostrarBusqueda()
{
    _showBarraBusqueda(m_busqueda);
}

void FrmTransportistas::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}

void FrmTransportistas::filter_table(QString texto, QString orden, QString modo)
{
    QHash <QString,QString> h;
    h[tr("Código")] = "codigo";
    h[tr("Descripción")] = "transportista";
    QString order = h.value(orden);
    if(modo == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    m->setQuery("SELECT id , codigo, transportista "
                "FROM transportista WHERE "+order+" LIKE '%"+texto.trimmed()+
                "%' ORDER BY "+order +" "+modo,Configuracion_global->groupDB);
}

void FrmTransportistas::vaciarCampos()
{
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->clear();
    }
}

void FrmTransportistas::on_txtCodigo_editingFinished()
{
    if(ui->txtCodigo->text().isEmpty()){
        QMessageBox::warning(this,tr("ATENCIÓN:"),
                             tr("El campo de codigo no puede estar en blanco"),
                             tr("Aceptar"));
        ui->txtCodigo->setFocus();
    }
}

void FrmTransportistas::on_btnBorrar_clicked()
{
    if(oTransportista.h_transportista["id"].isValid()){
        oTransportista.BorrarTransportista(oTransportista.h_transportista["id"].toInt());
        vaciarCampos();
    }
}

void FrmTransportistas::on_btnAnadirContacto_clicked()
{
    anadiendoContacto = true;
    ui->txtNombre->clear();
    ui->txtCargo->clear();
    ui->txtDescripcionT1->clear();
    ui->txtDescripcionT2->clear();
    ui->txtDescripcionT3->clear();
    ui->txtDescripcionM1->clear();
    ui->txtDescripcionM2->clear();
    ui->txtTelefono1->clear();
    ui->txtTelefono2->clear();
    ui->txtTelefono3->clear();
    ui->txtMovil1->clear();
    ui->txtMovil2->clear();
    ui->txtCargo->clear();
    ui->txtemail_contacto->clear();
    bloquearCamposContacto(false);
    ui->txtNombre->setFocus();
}

void FrmTransportistas::on_btnEditarContacto_clicked()
{
    if(id_contacto > 0){
        bloquearCamposContacto(false);
    ui->txtNombre->setFocus();
    }else{
        QMessageBox::warning(qApp->activeWindow(),tr("Editar contacto"),
                                 tr("Tiene que seleccionar un contacto para editar"),
                                 tr("Aceptar"));

    }
}

void FrmTransportistas::on_btnBorrarContacto_clicked()
{
    if(QMessageBox::question(qApp->activeWindow(),tr("Borrar Contacto"),
                          tr("Está apunto de borrar un contacto\n¿Desea continuar?"),
                          tr("No"),tr("Si")) == QMessageBox::Accepted)
    {
        if(id_contacto != 0){
            QSqlQuery qTransportista(Configuracion_global->groupDB);
        qTransportista.prepare("DELETE FROM personascontactotransportista WHERE id =:id");
        qTransportista.bindValue(":id",id_contacto);
        if (!qTransportista.exec()) {
            QMessageBox::warning(qApp->activeWindow(),tr("Borrar Cliente"),tr("Falló el borrado de tipos de cliente"),
                                 tr("Aceptar"));
        }
        }else{QMessageBox::warning(qApp->activeWindow(),tr("Borrar Contacto"),tr("Falló el borrado del contacto"),
                                   tr("Aceptar"));
}
}
    bloquearCamposContacto(true);
    llenar_campos_contactoTransportista(oTransportista.h_transportista.value("id").toInt());
}


void FrmTransportistas::bloquearCamposContacto(bool state)
{
    ui->txtNombre->setReadOnly(state);
    ui->txtCargo->setReadOnly(state);
    ui->txtDescripcionT1->setReadOnly(state);
    ui->txtDescripcionT2->setReadOnly(state);
    ui->txtDescripcionT3->setReadOnly(state);
    ui->txtDescripcionM1->setReadOnly(state);
    ui->txtDescripcionM2->setReadOnly(state);
    ui->txtTelefono1->setReadOnly(state);
    ui->txtTelefono2->setReadOnly(state);
    ui->txtTelefono3->setReadOnly(state);
    ui->txtMovil1->setReadOnly(state);
    ui->txtMovil2->setReadOnly(state);
    ui->txtCargo->setReadOnly(state);
    ui->txtemail_contacto->setReadOnly(state);
}


void FrmTransportistas::on_pushButtonGuardarContacto_clicked()
{
    QHash <QString, QVariant> d;
    QString error;
    int id_alternativa;

    //d["id"] = id_contacto;
    d["nombre"]= ui->txtNombre->text();
    d["telefono1"] = ui->txtTelefono1->text();
    d["telefono2"] = ui->txtTelefono2->text();
    d["telefono3"] = ui->txtTelefono3->text();
    d["movil"] = ui->txtMovil1->text();
    d["movil2"] = ui->txtMovil2->text();
    d["id_transportista"] = oTransportista.h_transportista["id"].toInt();
    d["desc_telefono1"] = ui->txtDescripcionT1->text();
    d["desc_telefono2"] = ui->txtDescripcionT2->text();
    d["desc_telefono3"] = ui->txtDescripcionT3->text();
    d["desc_movil1"] = ui->txtDescripcionM1->text();
    d["desc_movil2"] = ui->txtDescripcionM2->text();
    d["cargo_empresa"] = ui->txtCargo->text();
    d["email"] = ui->txtemail_contacto->text();
    if(anadiendoContacto){
        if (oTransportista.h_transportista["id"].toInt() == 0) {
            QMessageBox::warning(qApp->activeWindow(),tr("Crear Contacto"),tr("Debe guardar el transportista antes \n de crear contactos"),
                                               tr("Aceptar"));
            return;
        }
        id_alternativa = SqlCalls::SqlInsert(d,"personascontactotransportista",Configuracion_global->groupDB, error);

    }else{
        id_alternativa = SqlCalls::SqlUpdate(d,"personascontactotransportista",Configuracion_global->groupDB,
                                             QString("id=").append(QString::number(id_contacto)),error);

    }
    if(id_alternativa == -1){
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir/Guardar dirección"),
                             tr("Ocurrió un error al guardar los datos del contacto: %1").arg(error),
                             tr("Aceptar"));
    }
    anadiendoContacto = false;
    bloquearCamposContacto(true);
    llenar_campos_contactoTransportista(oTransportista.h_transportista.value("id").toInt());

}
