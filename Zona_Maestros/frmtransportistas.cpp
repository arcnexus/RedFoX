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
    ui->txtCodigo_proveedor->installEventFilter(this);
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
    oTransportista.anadir();
    ui->txtCodigo->setFocus();
}

void FrmTransportistas::Bloquear_campos(bool state)
{
    // -------------------------
    // campos transportista
    //--------------------------
    ui->txtCodigo->setReadOnly(state);
    ui->txtTransportista->setReadOnly(state);
    ui->txtCodigo_proveedor->setReadOnly(state);
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
}

void FrmTransportistas::cargar_en_objeto()
{
    QString error;
    oTransportista.h_transportista["codigo"] = ui->txtCodigo->text();
    oTransportista.h_transportista["transportista"] = ui->txtTransportista->text();
    int id_proveedor = SqlCalls::SelectOneField("proveedores","id",QString("codigo = '%1'").arg(ui->txtCodigo_proveedor->text()),
                                                Configuracion_global->groupDB,error).toInt();

    oTransportista.h_transportista["id_proveedor"] = id_proveedor;
}

void FrmTransportistas::llenar_campos()
{
    //--------------------------
    // Definición transportista
    //--------------------------
    ui->txtCodigo->setText(oTransportista.h_transportista.value("codigo").toString());
    ui->txtTransportista->setText(oTransportista.h_transportista.value("transportista").toString());
    llenar_campos_proveedor();

}

void FrmTransportistas::llenar_campos_proveedor()
{
    //---------------------------------
    // Campos proveedor (solo lectura)
    //---------------------------------
    ui->txtCodigo_proveedor->setText(oTransportista.oProveedor.codigo);
    ui->txtProveedor->setText(oTransportista.oProveedor.proveedor);
    ui->txtcif->setText(oTransportista.oProveedor.cif);
    ui->txtdireccion1->setText(oTransportista.oProveedor.direccion1);
    ui->txtdireccion2->setText(oTransportista.oProveedor.direccion2);
    ui->txtcp->setText(oTransportista.oProveedor.cp);
    ui->txtpoblacion->setText(oTransportista.oProveedor.poblacion);
    ui->txtprovincia->setText(oTransportista.oProveedor.provincia);
    int index = ui->cbopais->findText(oTransportista.oProveedor.pais);
    ui->cbopais->setCurrentIndex(index);
    ui->txttelefono1->setText(oTransportista.oProveedor.telefono1);
    ui->txttelefono2->setText(oTransportista.oProveedor.telefono2);
    ui->txttelefono3->setText(oTransportista.oProveedor.telefono3);
    ui->txtfax->setText(oTransportista.oProveedor.fax);
    ui->txtmovil->setText(oTransportista.oProveedor.movil);
    ui->txtemail->setText(oTransportista.oProveedor.email);
    ui->txtweb->setText(oTransportista.oProveedor.web);
    ui->txtpersona_contacto->setText(oTransportista.oProveedor.persona_contacto);
    llenar_contactos(oTransportista.oProveedor.id);
}

bool FrmTransportistas::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj == ui->txtCodigo_proveedor)
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

    }
    Bloquear_campos(true);
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

        oTransportista.oProveedor.Recuperar(id_proveedor);
        llenar_campos_proveedor();
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


    QPushButton* add = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir forma de pago"),this);
    connect(add,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(add);

    QPushButton* edit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar forma de pago"),this);
    connect(edit,SIGNAL(clicked()),this,SLOT(on_btnEditar_2_clicked()));
    m_busqueda->addWidget(edit);

    QPushButton* print = new QPushButton(QIcon(":/Icons/PNG/print2.png"),tr("Imprimir forma de pago"),this);
   // connect(print,SIGNAL(clicked()),this,SLOT(on_btnEditar_2_clicked()));//TODO
    m_busqueda->addWidget(print);

    QPushButton* del = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar forma de pago"),this);
   // connect(del,SIGNAL(clicked()),this,SLOT(on_btnEditar_2_clicked()));//TODO
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

void FrmTransportistas::on_btnEditar_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
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
    Bloquear_campos(false);
    ui->stackedWidget->setCurrentIndex(1);
    ui->txtCodigo->setFocus();
}

void FrmTransportistas::on_tablaContactos_clicked(const QModelIndex &index)
{
    //QModelIndex index= ui->tablaContactos->currentIndex();
    int nid = ui->tablaContactos->model()->data(ui->tablaContactos->model()->index(index.row(),0),Qt::EditRole).toInt();
    this->id_contacto = nid;
    QSqlQuery queryContactos(Configuracion_global->groupDB);
    queryContactos.prepare("select * from personascontactoproveedor where id = :id");
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
