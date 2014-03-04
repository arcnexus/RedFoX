#include "frmtransportistas.h"
#include "ui_frmtransportistas.h"
#include "../Busquedas/db_consulta_view.h"

FrmTransportistas::FrmTransportistas(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmTransportistas),
    menuButton(QIcon(":/Icons/PNG/transport.png"),tr("Transportistas"),this)
{
}

void FrmTransportistas::init_querys()
{
    model_busqueda->setQuery("select id, codigo,transportista from transportista",Configuracion_global->groupDB);
    pob_completer_model->setTable("municipios");
}

void FrmTransportistas::init()
{
    ui->setupUi(this);

    anadiendo = false;
    anadiendoContacto = false;
    id_contacto = 0;

    // Busqueda
    ui->stackedWidget->setCurrentIndex(0);
    model_busqueda = new QSqlQueryModel(this);
    model_contactos = new QSqlTableModel(this,Configuracion_global->groupDB);
    model_contactos->setTable("personascontactotransportista");

    ui->tablaBusqueda->setModel(model_busqueda);
    ui->tablaBusqueda->hideColumn(0);

    ui->listaContactos->setModel(model_contactos);
    ui->listaContactos->setModelColumn(1);

    ui->cbopais->setModel(Configuracion_global->paises_model);
    ui->cbopais->setModelColumn(1);
    Bloquear_campos(true);

    setUpBusqueda();

    pob_completer_model = new QSqlTableModel(this,QSqlDatabase::database("calles"));
    pob_completer_model->setTable("municipios");
    pob_completer = new QCompleter(pob_completer_model,this);
    pob_completer->setCaseSensitivity(Qt::CaseInsensitive);
    pob_completer->setCompletionColumn(3);
    ui->txtpoblacion->setCompleter(pob_completer);


    calle_completer_model = new QSqlTableModel(this,QSqlDatabase::database("calles"));
    calle_completer_model->setTable("calles");
    calle_completer = new QCompleter(calle_completer_model,this);
    calle_completer->setCaseSensitivity(Qt::CaseInsensitive);
    calle_completer->setCompletionColumn(2);
    ui->txtdireccion1->setCompleter(calle_completer);
    ui->txtdireccion2->setCompleter(calle_completer);
}

FrmTransportistas::~FrmTransportistas()
{
    delete ui;
}

void FrmTransportistas::Bloquear_campos(bool state)
{
    // -------------------------
    // campos transportista
    //--------------------------
    ui->txtCodigo->setReadOnly(state);
    ui->txtTransportista->setReadOnly(state);
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
    ui->btnBuscar->setEnabled(state);
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
}

void FrmTransportistas::cargar_en_objeto()
{
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

    int id_p = oTransportista.h_transportista.value("pais").toInt();
    for(auto i= 0; i< Configuracion_global->paises_model->rowCount();++i)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == id_p)
        {
            ui->cbopais->setCurrentIndex(i);
            break;
        }
    }
    ui->txttelefono1->setText(oTransportista.h_transportista.value("telefono1").toString());
    ui->txttelefono2->setText(oTransportista.h_transportista.value("telefono2").toString());
    ui->txtfax->setText(oTransportista.h_transportista.value("fax").toString());
    ui->txtmovil->setText(oTransportista.h_transportista.value("movil").toString());
    ui->txtemail->setText(oTransportista.h_transportista.value("email").toString());
    ui->txtweb->setText(oTransportista.h_transportista.value("web").toString());
    llenar_campos_contactoTransportista(oTransportista.h_transportista.value("id").toInt());
}

void FrmTransportistas::llenar_campos_contactoTransportista(int id)
{
    model_contactos->setFilter(QString("id_transportista = %1").arg(id));
    model_contactos->select();
}

bool FrmTransportistas::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Show && obj == this)
    {
        init_querys();
    }

    else if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj == ui->txtCodigo)
        {
            if(keyEvent->key() == Qt::Key_F1)
                consultar_proveedor();
        }
    }

    else if(event->type() == QEvent::Resize)
        _resizeBarraBusqueda(m_busqueda);

    return MayaModule::eventFilter(obj,event);
}

void FrmTransportistas::on_btnAnadir_clicked()
{
    Configuracion_global->groupDB.transaction();
    ui->stackedWidget->setCurrentIndex(1);
    Bloquear_campos(false);
    anadiendo = true;
    vaciarCampos();
    oTransportista.h_transportista.clear();
    ocultarBusqueda();
    llenar_campos_contactoTransportista(0);
    emit block();
    ui->txtCodigo->setFocus();
}

void FrmTransportistas::on_btnGuardar_clicked()
{
    if(ui->txtCodigo->text().isEmpty())
    {
        QMessageBox::warning(this,tr("ATENCIÓN:"),
                             tr("El campo de codigo no puede estar en blanco"),
                             tr("Aceptar"));
        ui->txtCodigo->setFocus();
        return;
    }
    cargar_en_objeto();
    if(anadiendo)
    {
        oTransportista.anadir();
        anadiendo = false;
    }
    else
    {

        oTransportista.guardar();
    }
    Configuracion_global->groupDB.commit();
    Bloquear_campos(true);
    emit unblock();
}

void FrmTransportistas::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void FrmTransportistas::on_btnEditar_clicked()
{
    if(ui->stackedWidget->currentIndex() == 0)
    {
        if(!ui->tablaBusqueda->currentIndex().isValid())
            return;
        QStringList condiciones,extras;
        condiciones << QString("id = %1").arg(model_busqueda->record(ui->tablaBusqueda->currentIndex().row()).value("id").toInt());
        extras << "";
        oTransportista.recuperar(condiciones,extras);
        llenar_campos();
    }
    Configuracion_global->groupDB.transaction();
    ui->stackedWidget->setCurrentIndex(1);
    emit block();
    Bloquear_campos(false);
}

void FrmTransportistas::on_btnBorrar_clicked()
{
    if(ui->stackedWidget->currentIndex() == 0)
    {
        if(!ui->tablaBusqueda->currentIndex().isValid())
            return;
        oTransportista.h_transportista["id"] = model_busqueda->record(ui->tablaBusqueda->currentIndex().row()).value("id").toInt();
    }
    if(oTransportista.h_transportista["id"].isValid())
    {
        oTransportista.BorrarTransportista(oTransportista.h_transportista["id"].toInt());
        vaciarCampos();
    }
}

void FrmTransportistas::on_btnDeshacer_clicked()
{
    Configuracion_global->groupDB.rollback();
    Bloquear_campos(true);
    llenar_campos();
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

void FrmTransportistas::on_btnAnterior_clicked()
{
    QStringList condiciones,extras;
    condiciones << QString("codigo <'%1'").arg(ui->txtCodigo->text().trimmed());
    extras << "order by codigo desc" <<"limit 0,1";
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

void FrmTransportistas::on_tablaBusqueda_doubleClicked(const QModelIndex &index)
{
    int id = ui->tablaBusqueda->model()->data(ui->tablaBusqueda->model()->index(index.row(),0),Qt::EditRole).toInt();

    QStringList filtro,extras;
    filtro << QString("id = %1").arg(id);
    extras << "";
    oTransportista.recuperar(filtro,extras);
    llenar_campos();
    Bloquear_campos(true);
    ocultarBusqueda();
    ui->stackedWidget->setCurrentIndex(1);
    ui->txtCodigo->setFocus();
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
    ui->stackedWidget->setCurrentIndex(0);
    QHash <QString,QString> h;
    h[tr("Código")] = "codigo";
    h[tr("Descripción")] = "transportista";
    QString order = h.value(orden);
    if(modo == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    model_busqueda->setQuery("SELECT id , codigo, transportista "
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
    ui->txtfecha_alta->setDate(QDate::currentDate());
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
    if(ui->listaContactos->currentIndex().isValid())
    {
        bloquearCamposContacto(false);
        ui->txtNombre->setFocus();
    }
    else{
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
    if(anadiendoContacto)
    {
        if (oTransportista.h_transportista["id"].toInt() == 0) {
            QMessageBox::warning(qApp->activeWindow(),tr("Crear Contacto"),tr("Debe guardar el transportista antes \n de crear contactos"),
                                               tr("Aceptar"));
            return;
        }
        id_alternativa = SqlCalls::SqlInsert(d,"personascontactotransportista",Configuracion_global->groupDB, error);

    }
    else
    {
        id_alternativa = SqlCalls::SqlUpdate(d,"personascontactotransportista",Configuracion_global->groupDB,
                                             QString("id=").append(QString::number(id_contacto)),error);

    }
    if(id_alternativa == -1)
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir/Guardar dirección"),
                             tr("Ocurrió un error al guardar los datos del contacto: %1").arg(error),
                             tr("Aceptar"));
    }
    else
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Persona de contacto de transportista\nañadida con éxito"));
    }
    anadiendoContacto = false;
    bloquearCamposContacto(true);
    llenar_campos_contactoTransportista(oTransportista.h_transportista.value("id").toInt());
}

void FrmTransportistas::on_listaContactos_clicked(const QModelIndex &index)
{
    QSqlRecord r = model_contactos->record(index.row());
    ui->txtNombre->setText(r.value("nombre").toString());
    ui->txtCargo->setText(r.value("cargo_empresa").toString());
    ui->txtDescripcionT1->setText(r.value("desc_telefono1").toString());
    ui->txtDescripcionT2->setText(r.value("desc_telefono2").toString());
    ui->txtDescripcionT3->setText(r.value("desc_telefono3").toString());
    ui->txtDescripcionM1->setText(r.value("desc_movil1").toString());
    ui->txtDescripcionM2->setText(r.value("desc_movil2").toString());
    ui->txtTelefono1->setText(r.value("telefono1").toString());
    ui->txtTelefono2->setText(r.value("telefono2").toString());
    ui->txtTelefono3->setText(r.value("telefono3").toString());
    ui->txtMovil1->setText(r.value("movil").toString());
    ui->txtMovil2->setText(r.value("movil2").toString());
    ui->txtCargo->setText(r.value("cargo_empresa").toString());
    ui->txtemail_contacto->setText(r.value("email").toString());
}

void FrmTransportistas::on_txtcp_editingFinished()
{
    if(!QSqlDatabase::database("calles").isOpen())
        return;
    QString cp = QString("CodPostal = '%1'").arg(ui->txtcp->text());
    pob_completer_model->setFilter(cp);
    pob_completer_model->select();

    calle_completer_model->setFilter(cp);
    calle_completer_model->select();

    if(pob_completer_model->rowCount() > 0)
    {
        ui->txtpoblacion->setText(pob_completer_model->record(0).value("Municipio").toString());
        ui->txtprovincia->setText(pob_completer_model->record(0).value("CodProv").toString());
        ui->cbopais->setCurrentText("España");

        if(pob_completer_model->rowCount() > 1){
            ui->txtpoblacion->setText("");
            ui->txtpoblacion->setFocus();
        }
        else
            ui->txtdireccion1->setFocus();
    }
}

void FrmTransportistas::on_txtdireccion2_editingFinished()
{
    if(!ui->txtprovincia->text().isEmpty())
        ui->txttelefono1->setFocus();
}
