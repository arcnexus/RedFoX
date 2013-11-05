#include "frmexcepciones.h"
#include "ui_frmexcepciones.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Auxiliares/frmnuevosavisos.h"
#include "../Auxiliares/avisos.h"

FrmExcepciones::FrmExcepciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmExcepciones)
{
    ui->setupUi(this);
    this->id = 0;
    this->id_cliente = 0;
    this->id_familia = 0;
    this->id_familia_cliente = 0;
    this->id_subfamilia_cliente = 0;
    this->id_proveedor = 0;
    this->id_agente = 0;
    this->id_articulo = 0;
    m = new QSqlQueryModel(this);
    m->setQuery("select id,");
    ui->txt_codigo_articulo->installEventFilter(this);
    ui->txt_codigo_cliente->installEventFilter(this);
    ui->txt_codigo_familia->installEventFilter(this);
    ui->txt_codigo_proveedor->installEventFilter(this);
    ui->txt_codigo_agente->installEventFilter(this);
    ui->txt_codigo_familia_cliente->installEventFilter(this);
    ui->txtcodigo_sub_familia_cliente->installEventFilter(this);

    setControlsReadOnly(true);

    //------------------------
    // mostrar tabla
    //------------------------
    modelo = new QSqlQueryModel(this);
    modelo->setQuery("select id, descripcion from articulos_excepciones",Configuracion_global->empresaDB);
    ui->tabla->setModel(modelo);
    ui->tabla->setColumnHidden(0,true);
    modelo->setHeaderData(1,Qt::Horizontal,tr("Descripción"));

}

FrmExcepciones::~FrmExcepciones()
{
    delete ui;
}

void FrmExcepciones::cargar_articulo(int id_art)
{
    this->id_articulo = id_art;
    QMap <int,QSqlRecord>  r;
    QString error;
    QStringList filter;
    filter << QString("id =%1").arg(id_art) << "tarifa = 1";
    r = SqlCalls::SelectRecord("vistaart_tarifa",filter,Configuracion_global->groupDB,error);
    QMapIterator<int, QSqlRecord> i(r);
    while (i.hasNext())
    {
        i.next();
        ui->txt_codigo_articulo->setText(i.value().value("codigo").toString());
        ui->txt_descripcion_articulo->setText(i.value().value("descripcion").toString());
        ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(QString::number(i.value().value("pvp_con_iva").toDouble(),
                                                                                  'f',Configuracion_global->decimales)));

    }
    ui->txtDescripcion->setText(ui->txt_descripcion_articulo->text());
    modelo->setQuery(QString("select id, descripcion from articulos_excepciones where id_articulo = %1").arg(this->id_articulo),
                    Configuracion_global->empresaDB);

}

void FrmExcepciones::cargar_cliente(int id_cliente)
{
    int tarifa;
    QMap <int,QSqlRecord>  rc;
    QString error;
    QStringList filter;
    filter << QString("id =%1").arg(id_cliente);
    rc = SqlCalls::SelectRecord("clientes",filter,Configuracion_global->groupDB,error);
    QMapIterator<int, QSqlRecord> ic(rc);
    while (ic.hasNext())
    {
        ic.next();
        //clear_contra();
        this->id_cliente = id_cliente;
        ui->txt_codigo_cliente->setText(ic.value().value("codigo_cliente").toString());
        ui->txt_nombre_cliente->setText(ic.value().value("nombre_fiscal").toString());
        tarifa = ic.value().value("tarifa_cliente").toInt();
    }

    QMap <int,QSqlRecord>  ra;
    filter << QString("codigo =%1").arg(ui->txt_codigo_articulo->text()) << QString("tarifa = %1").arg(tarifa);
    ra = SqlCalls::SelectRecord("vistaart_tarifa",filter,Configuracion_global->groupDB,error);
    QMapIterator<int, QSqlRecord> ia(ra);
    while (ia.hasNext())
    {
        ia.next();
        ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(
                                QString::number(ia.value().value("pvp_con_iva").toDouble(),'f',Configuracion_global->decimales)));

    }
    ui->txtDescripcion->setText(ui->txt_descripcion_articulo->text()+" / "+ui->txt_nombre_cliente->text());
}

void FrmExcepciones::cargar_familia(int id_familia)
{
    QString error;
    QMap <int,QSqlRecord> f;
    f= SqlCalls::SelectRecord("familias",QString("id=%1").arg(id_familia),Configuracion_global->groupDB,error);
    if(error.isEmpty())
    {
        this->id_familia = id_familia;
        ui->txt_codigo_familia->setText(f.value(id_familia).value("codigo").toString());
        ui->txtDescFamilia->setText(f.value(id_familia).value("familia").toString());
        if(!ui->txt_codigo_familia->text().isEmpty())
            ui->radFamilia->setChecked(true);
        if(ui->txtDescripcion->text().isEmpty())
                ui->txtDescripcion->setText(ui->txtDescFamilia->text());
    }
}

void FrmExcepciones::cargar_proveedor(int id_proveedor)
{
    QString error;
    QMap <int, QSqlRecord> p;
    p = SqlCalls::SelectRecord("proveedores",QString("id=%1").arg(id_proveedor),Configuracion_global->groupDB, error);
    if(error.isEmpty())
    {
        //clear_contra();
        this->id_proveedor = id_proveedor;
        ui->txt_codigo_proveedor->setText(p.value(id_proveedor).value("codigo").toString());
        ui->txt_nombre_proveedor->setText(p.value(id_proveedor).value("proveedor").toString());
        if(!ui->txt_codigo_proveedor->text().isEmpty())
            ui->radProveedor->setChecked(true);
    }
}

void FrmExcepciones::cargar_agente(int id_agente)
{
    QString error;
    QMap <int, QSqlRecord> a;
    a = SqlCalls::SelectRecord("agentes",QString("id=%1").arg(id_agente),Configuracion_global->groupDB, error);
    if(error.isEmpty())
    {
        //clear_contra();
        this->id_agente = id_agente;
        ui->txt_codigo_agente->setText(a.value(id_agente).value("codigo").toString());
        ui->txt_nombre_agente->setText(a.value(id_agente).value("nombre").toString());
        if(!ui->txt_codigo_agente->text().isEmpty())
            ui->radAgente->setChecked(true);
    }
}

void FrmExcepciones::cargar_familia_cliente(int id_fam)
{
    QString error;
    QMap <int, QSqlRecord> a;
    a = SqlCalls::SelectRecord("maestro_familia_cliente",QString("id=%1").arg(id_fam),Configuracion_global->groupDB, error);
    if(error.isEmpty())
    {
        //clear_contra();
        this->id_familia_cliente = id_fam;
        ui->txt_codigo_familia_cliente->setText(a.value(id_fam).value("id").toString());
        ui->txtDescripcion_familia_cliente->setText(a.value(id_fam).value("descripcion").toString());
        if(!ui->txt_codigo_familia_cliente->text().isEmpty())
            ui->radFamilia_cliente->setChecked(true);
    }
}

void FrmExcepciones::cargar_subfamilia_cliente(int id_subfam)
{
    QString error;
    QMap <int, QSqlRecord> a;
    a = SqlCalls::SelectRecord("maestro_subfamilia_cliente",QString("id=%1").arg(id_subfam),Configuracion_global->groupDB, error);
    if(error.isEmpty())
    {
        QString cod_fam_cli,fam_cli;
        this->id_subfamilia_cliente = id_subfam;
        cod_fam_cli = ui->txt_codigo_familia_cliente->text();
        fam_cli = ui->txtDescripcion_familia_cliente->text();
        //clear_contra();
        ui->txtcodigo_sub_familia_cliente->setText(a.value(id_subfam).value("id").toString());
        ui->txtDescripcion_subfamilia->setText(a.value(id_subfam).value("descripcion").toString());
        ui->txt_codigo_familia_cliente->setText(cod_fam_cli);
        ui->txtDescripcion_familia_cliente->setText(fam_cli);
        if(!ui->txtcodigo_sub_familia_cliente->text().isEmpty())
            ui->chkSubfamilia_cliente->setChecked(true);
    }
}

void FrmExcepciones::buscar_id_cliente(QString cod_cli)
{
    QString error;
    int id_clien = SqlCalls::SelectOneField("clientes","id",QString("codigo_cliente = %1").arg(cod_cli),
                                              Configuracion_global->groupDB,error).toInt();
    this->id_cliente = id_clien;
    cargar_cliente(id_clien);
}

void FrmExcepciones::buscar_id_articulo(QString cod_art)
{
    QString error;
    int id_art = SqlCalls::SelectOneField("articulos","id",QString("codigo = %1").arg(cod_art),
                                          Configuracion_global->groupDB,error).toInt();
    this->id_articulo = id_art;
    modelo->setQuery(QString("select id, descripcion from articulos_excepciones where id_articulo = %1").arg(this->id_articulo),
                    Configuracion_global->empresaDB);
    cargar_articulo(id_art);
}

void FrmExcepciones::buscar_id_familia(QString cod_fam)
{
    QString error;
    int id_fam = SqlCalls::SelectOneField("familias","id",QString("codigo = %1").arg(cod_fam),
                                          Configuracion_global->groupDB,error).toInt();
    this->id_familia = id_fam;
    modelo->setQuery(QString("select id, descripcion from articulos_excepciones where id_familia = %1").arg(this->id_familia),
                    Configuracion_global->empresaDB);
    cargar_familia(id_fam);
}

void FrmExcepciones::buscar_id_proveedor(QString cod_prov)
{
    QString error;
    int id_prov = SqlCalls::SelectOneField("proveedores","id",QString("codigo = %1").arg(cod_prov),
                                          Configuracion_global->groupDB,error).toInt();
    this->id_proveedor = id_prov;
    cargar_proveedor(id_prov);
}

void FrmExcepciones::buscar_id_agente(QString cod_agen)
{
    QString error;
    int id_agen = SqlCalls::SelectOneField("agentes","id",QString("codigo = %1").arg(cod_agen),
                                          Configuracion_global->groupDB,error).toInt();
    this->id_agente = id_agen;
    cargar_proveedor(id_agen);
}

bool FrmExcepciones::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj == ui->txt_codigo_articulo)
        {
            if(keyEvent->key() == Qt::Key_F1)
                consultar_articulo();
        }
        if(obj == ui->txt_codigo_cliente)
        {
            if(keyEvent->key() == Qt::Key_F1)
                consultar_cliente();
        }
        if (obj == ui->txt_codigo_familia)
        {
            if(keyEvent->key() == Qt::Key_F1)
                consultar_familia();
        }
        if(obj == ui->txt_codigo_proveedor)
        {
            if(keyEvent->key() == Qt::Key_F1)
                consultar_proveedor();
        }
        if(obj == ui->txt_codigo_agente)
        {
            if(keyEvent->key() == Qt::Key_F1)
                consultar_agente();
        }
        if(obj == ui->txt_codigo_familia_cliente)
        {
            if(keyEvent->key() == Qt::Key_F1)
                consultar_familia_cliente();
        }
        if(obj == ui->txtcodigo_sub_familia_cliente)
        {
            if(keyEvent->key() == Qt::Key_F1)
                consultar_subfamilia_cliente();
        }
        return false;
    }
    return false;
}

void FrmExcepciones::consultar_articulo()
{
    db_consulta_view consulta;
    QStringList campos;
    campos << "descripcion" << "codigo" <<"codigo_barras" <<"codigo_fabricante"  << "pvp_con_iva";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Artículos");
    consulta.set_db("Maya");
    consulta.set_SQL("select id, codigo,codigo_barras,codigo_fabricante,descripcion,pvp_con_iva from vistaart_tarifa");
    QStringList cabecera;
    QVariantList tamanos;
    QVariantList moneda;
    moneda << 5;
    cabecera  << tr("Código") <<tr("Barras") << tr("Ref. Fabric.") << tr("Descripcion") << tr("PVP+IVA");
    tamanos <<"0" << "100"  <<"100" << "100" << "300" <<"120";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_delegate_monetary(moneda);
    consulta.set_titulo("Busqueda de Artículos");
    if(consulta.exec())
    {
        int id_articulo = consulta.get_id();

        cargar_articulo(id_articulo);
    }
}

void FrmExcepciones::consultar_cliente()
{
    db_consulta_view consulta;
    QStringList campos;
    campos << "nombre_fiscal" <<"codigo_cliente" << "cif_nif" <<"telefono1" <<"direccion1"  << "poblacion";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Clientes");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,codigo_cliente,nombre_fiscal,telefono1,poblacion from clientes");
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Código") <<tr("Cliente") << tr("CIF/NIF/NIE") << tr("telefono1") << tr("Poblacion");
    tamanos <<"0" << "100"  <<"300" << "100" << "100" <<"100";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);

    consulta.set_titulo("Busqueda de Clientes");
    if(consulta.exec())
    {
        int id_cliente = consulta.get_id();

        cargar_cliente(id_cliente);
    }
}

void FrmExcepciones::consultar_familia()
{
    db_consulta_view consulta;
    QStringList campos;
    campos << "codigo" <<"familia";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Familias");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,codigo,familia from familias");
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Código") <<tr("Familia");
    tamanos <<"0" << "100"  <<"300";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);

    consulta.set_titulo("Busqueda de Familias");
    if(consulta.exec())
    {
        int id_familia = consulta.get_id();

        cargar_familia(id_familia);
    }
}

void FrmExcepciones::consultar_proveedor()
{
    db_consulta_view consulta;
    QStringList campos;
    campos << "codigo" <<"proveedor" <<"cif";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("proveedores");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,codigo,proveedor,cif from proveedores");
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Código") <<tr("Proveedor") << tr("CIF");
    tamanos <<"0" << "100"  <<"300" << "120";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);

    consulta.set_titulo("Busqueda de Proveedores");
    if(consulta.exec())
    {
        int id_proveedor = consulta.get_id();

        cargar_proveedor(id_proveedor);
    }
}

void FrmExcepciones::consultar_agente()
{
    db_consulta_view consulta;
    QStringList campos;
    campos << "codigo" <<"nombre" <<"dni";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Agentes");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,codigo,nombre,dni from agentes");
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Código") <<tr("Agente") <<tr("DNI/NIE");
    tamanos <<"0" << "100"  <<"300" <<"120";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);

    consulta.set_titulo("Busqueda de Agentes");
    if(consulta.exec())
    {
        int id_agente = consulta.get_id();

        cargar_agente(id_agente);
    }

}

void FrmExcepciones::consultar_familia_cliente()
{
    db_consulta_view consulta;
    QStringList campos;
    campos << "descripcion";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Grupo clientes");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,descripcion from maestro_familia_cliente;");
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Descripción");
    tamanos <<"0" << "450";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);

    consulta.set_titulo("Busqueda de Grupos de Cliente");
    if(consulta.exec())
    {
        int id_fam_cli = consulta.get_id();

        cargar_familia_cliente(id_fam_cli);
    }
}

void FrmExcepciones::consultar_subfamilia_cliente()
{
    db_consulta_view consulta;
    QStringList campos;
    campos << "descripcion";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Subgrupo clientes");
    consulta.set_db("Maya");
    consulta.set_SQL(QString("select id,descripcion from maestro_subfamilia_cliente where id_maestro_familia_cliente = %1;").arg(
                         this->id_familia_cliente));
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Descripción");
    tamanos <<"0" << "450";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);

    consulta.set_titulo("Busqueda de Subgrupos de Cliente");
    if(consulta.exec())
    {
        int id_subfam_cli = consulta.get_id();

        cargar_subfamilia_cliente(id_subfam_cli);
    }
}

void FrmExcepciones::clear()
{
    ui->txtDesde->setDate(QDate::currentDate());
    ui->txtHasta->setDate(QDate::currentDate().addYears(1));
    ui->txtDescFamilia->clear();
    ui->txtPVP->setText("0,00");
    ui->txt_codigo_agente->clear();
    ui->txt_codigo_articulo->clear();
    ui->txt_codigo_cliente->clear();
    ui->txt_codigo_familia->clear();
    ui->txt_codigo_proveedor->clear();
    ui->txt_codigo_familia_cliente->clear();
    ui->txtcodigo_sub_familia_cliente->clear();
    ui->txtDescripcion_familia_cliente->clear();
    ui->txtDescripcion_subfamilia->clear();
    ui->txt_descripcion_articulo->clear();
    ui->txt_dto_fijo->setText(0);
    ui->txt_nombre_agente->clear();
    ui->txt_nombre_proveedor->clear();
    ui->txt_importe_fijo->setText("0,00");
    ui->txt_importe_porc_aumento->setText("0");
    ui->txt_dto_aumento_porc->setText("0");
    ui->txt_dto_aumento_fijo->setText("0");
    ui->txt_importe_moneda_aumento->setText("0,00");
}

void FrmExcepciones::clear_contra()
{
    ui->txt_codigo_familia_cliente->clear();
    ui->txt_codigo_proveedor->clear();
    ui->txt_codigo_agente->clear();
    ui->txtcodigo_sub_familia_cliente->clear();
    ui->txt_codigo_cliente->clear();

    ui->txtDescripcion_familia_cliente->clear();
    ui->txt_nombre_proveedor->clear();
    ui->txt_nombre_agente->clear();
    ui->txtDescripcion_subfamilia->clear();
    ui->txt_nombre_cliente->clear();
}

void FrmExcepciones::cargar_excepcion(int id)
{
    QMap <int,QSqlRecord> ex;
    QString error;
    ex = SqlCalls::SelectRecord("articulos_excepciones",QString("id=%1").arg(id),Configuracion_global->empresaDB,error);
    if(error.isEmpty())
    {
        this->id_aviso_ini = ex.value(id).value("id_aviso_ini").toInt();
        this->id_aviso_fin = ex.value(id).value("id_aviso_fin").toInt();
        this->id = id;
        this->id_agente = ex.value(id).value("id_agente").toInt();
        this->id_articulo = ex.value(id).value("id_articulo").toInt();
        cargar_articulo(this->id_articulo);
        this->id_cliente = ex.value(id).value("id_cliente").toInt();
        cargar_cliente(this->id_cliente);
        this->id_familia = ex.value(id).value("id_familia").toInt();
        cargar_familia(this->id_familia);
        this->id_aviso_ini = ex.value(id).value("id_aviso_ini").toInt();
        this->id_aviso_fin = ex.value(id).value("id_aviso_fin").toInt();
        this->id_familia_cliente = ex.value(id).value("id_familia_cliente").toInt();
        cargar_familia_cliente(this->id_familia_cliente);
        this->id_subfamilia_cliente = ex.value(id).value("id_subfamilia_cliente").toInt();
        cargar_subfamilia_cliente(this->id_subfamilia_cliente);
        this->id_proveedor = ex.value(id).value("id_proveedor").toInt();
        cargar_proveedor(this->id_proveedor);
        ui->txtDesde->setDate(ex.value(id).value("fecha_inicio").toDate());
        ui->txtHasta->setDate(ex.value(id).value("fecha_final").toDate());
        QStringList clausulas;
        clausulas << QString("id =%1").arg(this->id_articulo);
        clausulas << QString("tarifa = %1").arg(Configuracion_global->id_tarifa_predeterminada);
        double pvp = SqlCalls::SelectOneField("vistaart_tarifas","pvp_con_iva",clausulas,Configuracion_global->groupDB,error).toDouble();
        ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(QString::number(pvp,'f',
                                                                                  Configuracion_global->decimales_campos_totales)));
        ui->txtDescripcion->setText(ex.value(id).value("descripcion").toString());
        ui->txt_importe_fijo->setText(Configuracion_global->toFormatoMoneda(
                                          QString::number(ex.value(id).value("importe_fijo").toDouble(),'f',
                                                          Configuracion_global->decimales)));
        ui->txt_importe_moneda_aumento->setText(
                    Configuracion_global->toFormatoMoneda(QString::number(ex.value(id).value("importe_moneda_aumento").toDouble(),
                                                                          'f',Configuracion_global->decimales)));
        ui->txt_importe_porc_aumento->setText(QString::number(ex.value(id).value("importe_porc_aumento").toFloat(),'f',2));
        ui->txt_dto_fijo->setText(QString::number(ex.value(id).value("dto_fijo").toFloat(),'f',2));
        ui->txt_dto_aumento_fijo->setText(QString::number(ex.value(id).value("dto_aumento_fijo").toFloat(),'f',2));
        ui->txt_dto_aumento_porc->setText(QString::number(ex.value(id).value("dto_aumento_porc").toFloat(),'f',2));

    } else
    {
        QMessageBox::warning(this,tr("Gestión de Excepciones"),tr("Atención: Ocurrió un error al recuperar: %1").arg(error),
                tr("Aceptar"));
    }
}

void FrmExcepciones::setControlsReadOnly(bool state)
{
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(state);
    }
    ui->txtDesde->setReadOnly(state);
    ui->txtHasta->setReadOnly(state);
    ui->txtDescripcion->setReadOnly(true);

}

void FrmExcepciones::setButtonsEditMode(bool state)
{
    ui->btnAnadir->setEnabled(!state);
    ui->btnEditar->setEnabled(!state);
    ui->btnBorrar->setEnabled(!state);
    ui->btnGuardar->setEnabled(state);
    ui->btnDeshacer->setEnabled(state);
}

void FrmExcepciones::on_txt_codigo_articulo_editingFinished()
{
    blockSignals(true);
    buscar_id_articulo(ui->txt_codigo_articulo->text());
    blockSignals(false);
}



void FrmExcepciones::on_btnAnadir_clicked()
{
    clear();
    setControlsReadOnly(false);
    setButtonsEditMode(true);
    ui->btnGenerarAvisoInicio->setEnabled(true);
    ui->btnGenerarAvisoFin->setEnabled(true);
    ui->radArticulo->setFocus();
    QHash <QString, QVariant> h;
    QString error;
    h["id_articulo"] = this->id_articulo;
    h["id_cliente"] = this->id_cliente;
    h["id_familia"] = this->id_familia;
    int new_id = SqlCalls::SqlInsert(h,"articulos_excepciones",Configuracion_global->empresaDB,error);
    if(new_id > -1)
    {
        cargar_articulo(this->id_articulo);
        cargar_cliente(this->id_cliente);
        this->adding = true;
        this->id = new_id;
    } else
        QMessageBox::warning(this,tr("Gestión de Excepciones"),
                             tr("Atención: Ocurrió un error al crear una excepción: %1").arg(error),tr("Aceptar"));

}

void FrmExcepciones::on_btnEditar_clicked()
{
    setControlsReadOnly(false);
    setButtonsEditMode(true);
    if(this->id_aviso_ini <1)
        ui->btnGenerarAvisoInicio->setEnabled(true);

    else
        ui->btnGenerarAvisoInicio->setEnabled(false);

    if(this->id_aviso_fin<1)
        ui->btnGenerarAvisoFin->setEnabled(true);

    else
        ui->btnGenerarAvisoFin->setEnabled(false);

}

void FrmExcepciones::on_btnGuardar_clicked()
{
    setControlsReadOnly(true);
    setButtonsEditMode(false);
    QHash <QString, QVariant> h;
    h["id_articulo"] = this->id_articulo;
    h["descripcion"] = ui->txtDescripcion->text();
    h["id_familia"] = this->id_familia;
    h["id_cliente"] = this->id_cliente;
    h["id_familia_cliente"] = this->id_familia_cliente;
    h["id_subfamilia_cliente"] = this->id_subfamilia_cliente;
    h["id_proveedor"] = this->id_proveedor;
    h["id_agente"] = this->id_agente;
    h["id_aviso_ini"] = this->id_aviso_ini;
    h["id_aviso_fin"] = this->id_aviso_fin;
    h["importe_porc_aumento"] = ui->txt_importe_porc_aumento->text().toFloat();
    h["importe_moneda_aumento"] = Configuracion_global->MonedatoDouble(ui->txt_importe_moneda_aumento->text());
    h["importe_fijo"] = Configuracion_global->MonedatoDouble(ui->txt_importe_fijo->text());
    h["dto_aumento_fijo"] = ui->txt_dto_aumento_fijo->text().toFloat();
    h["dto_aumento_porc"] = ui->txt_dto_aumento_porc->text().toFloat();
    h["dto_fijo"] = ui->txt_dto_fijo->text().toFloat();
    h["fecha_inicio"] = ui->txtDesde->date();
    h["fecha_final"] = ui->txtHasta->date();

    QString error;
    bool success = SqlCalls::SqlUpdate(h,"articulos_excepciones",Configuracion_global->empresaDB,
                                           QString("id=%1").arg(this->id),error);
    if(!success)
    {
        if(this->adding)
        {
            success = SqlCalls::SqlDelete("articulos_excepciones", Configuracion_global->empresaDB,QString("id = %1").arg(this->id),error);
            if(!success)
                QMessageBox::warning(this, tr("Gestión de excepciones"),
                                     tr("Ocurrió un error al insertar: %1").arg(error),tr("Aceptar"));
        }
            QMessageBox::warning(this,tr("Gestión de excepciones"),tr("Ocurrió un error al actualizar: %1").arg(error));

    }
    modelo->setQuery(QString("select id, descripcion from articulos_excepciones where id_articulo = %1").arg(this->id_articulo),
                    Configuracion_global->empresaDB);

}

void FrmExcepciones::on_btnDeshacer_clicked()
{
    setControlsReadOnly(true);
    setButtonsEditMode(false);
    QModelIndex index = ui->tabla->currentIndex();
    int id = ui->tabla->model()->data(ui->tabla->model()->index(index.row(),0)).toInt();
            //int id = ui->tabla->model()->data(ui->tabla->model()->index(index.row(),0),Qt::EditRole).toInt();
    QMap <int, QSqlRecord> exc;
    ui->txtDesde->setDate(exc.value(id).value("fecha_inicio").toDate());
    ui->txtDescripcion->setText(exc.value(id).value("descripcion").toString());
}

void FrmExcepciones::on_tabla_clicked(const QModelIndex &index)
{
    this->id = ui->tabla->model()->data(ui->tabla->model()->index(index.row(),0)).toInt();
    cargar_excepcion(this->id);

}

void FrmExcepciones::on_btnGenerarAvisoInicio_clicked()
{
    QHash<QString,QVariant> h_aviso;
    frmNuevosAvisos frna(this);
    Avisos oAviso(this);
    QString error;
    frna.setFecha(ui->txtDesde->date());
    frna.setHora(QTime(0,1));
    frna.setText(tr("Inicio excepcion: %1").arg(ui->txtDescripcion->text()));
    if(frna.exec() == QDialog::Accepted)
    {
        h_aviso["id_tipoaviso"] = frna.get_id_tipoaviso();
        h_aviso["fecha_hora_aviso"] = QDateTime(frna.getFecha(),frna.getHora());
        h_aviso["aviso"] = frna.getText();
        h_aviso["id_usuario_destino"] = frna.get_id_usuario_destino();
        h_aviso["id_usuario_origen"] = Configuracion_global->id_usuario_activo;
        h_aviso["id_empresa"] = Configuracion_global->idEmpresa;

        int new_id = oAviso.add_notice(h_aviso);
        this->id_aviso_ini = new_id;
        if(new_id == -1)
            QMessageBox::warning(this,tr("Gestión de excepciones"),
                                 tr("Atención: Ocurrió un error al insertar el aviso de inicio: %1").arg(error));
       else{
            TimedMessageBox *m = new TimedMessageBox(this,tr("Se ha creado un nuevo aviso"));
            ui->btnGenerarAvisoInicio->setEnabled(false);
        }
    }

}

void FrmExcepciones::on_btnGenerarAvisoFin_clicked()
{
    QHash<QString,QVariant> h_aviso;
    frmNuevosAvisos frna(this);
    Avisos oAviso(this);
    QString error;
    frna.setFecha(ui->txtHasta->date());
    frna.setHora(QTime(24,0));
    frna.setText(tr("Atención: Finaliza excepcion: %1").arg(ui->txtDescripcion->text()));
    if(frna.exec() == QDialog::Accepted)
    {
        h_aviso["id_tipoaviso"] = frna.get_id_tipoaviso();
        h_aviso["fecha_hora_aviso"] = QDateTime(frna.getFecha(),frna.getHora());
        h_aviso["aviso"] = frna.getText();
        h_aviso["id_usuario_destino"] = frna.get_id_usuario_destino();
        h_aviso["id_usuario_origen"] = Configuracion_global->id_usuario_activo;
        h_aviso["id_empresa"] = Configuracion_global->idEmpresa;

        int new_id = oAviso.add_notice(h_aviso);
        this->id_aviso_fin = new_id;
        if(new_id == -1)
            QMessageBox::warning(this,tr("Gestión de excepciones"),
                                 tr("Atención: Ocurrió un error al insertar el aviso de finalización: %1").arg(error));
       else{
            TimedMessageBox *m = new TimedMessageBox(this,tr("Se ha creado un nuevo aviso"));
            ui->btnGenerarAvisoFin->setEnabled(false);
        }
    }
}

void FrmExcepciones::on_txt_codigo_familia_editingFinished()
{
    blockSignals(true);
    buscar_id_familia(ui->txt_codigo_familia->text());
    blockSignals(false);
}

void FrmExcepciones::on_txt_codigo_cliente_editingFinished()
{
    blockSignals(true);
    buscar_id_cliente(ui->txt_codigo_cliente->text());
    blockSignals(false);
}

void FrmExcepciones::on_txt_codigo_proveedor_editingFinished()
{
    blockSignals(true);
    buscar_id_proveedor(ui->txt_codigo_proveedor->text());
    blockSignals(false);
}

void FrmExcepciones::on_txt_codigo_agente_editingFinished()
{
    blockSignals(true);
    buscar_id_agente(ui->txt_codigo_agente->text());
    blockSignals(false);
}

void FrmExcepciones::on_txtDesde_editingFinished()
{
    if(ui->btnGuardar->isEnabled())
    {
        //--------------------------------------------------------------------
        // si se creo aviso y se cambia la fecha se modifica también el aviso
        //--------------------------------------------------------------------
        if(this->id_aviso_ini >-1)
        {
            QHash <QString,QVariant> avi;
            avi["fecha_hora_aviso"] = QDateTime(ui->txtDesde->date(),QTime(0,1));
            QString error;
            bool success = SqlCalls::SqlUpdate( avi,"avisos",Configuracion_global->groupDB,QString("id=%1").arg(this->id_aviso_ini),
                                                error);
            if(!success)
                QMessageBox::warning(this,tr("Gestión de excepciones"),tr("Ocurrió un error al actualizar aviso inicio excepción: %1").arg(error),
                                     tr("Aceptar"));
            else
                TimedMessageBox *t = new TimedMessageBox(this,tr("Se actualizó la fecha del aviso"));
         }
    }
}

void FrmExcepciones::on_txtHasta_editingFinished()
{
    if(ui->btnGuardar->isEnabled())
    {
        if(this->id_aviso_fin >-1)
        {
            QHash <QString,QVariant> avi;
            avi["fecha_hora_aviso"] = QDateTime(ui->txtHasta->date(),QTime(23,59));
            QString error;
            bool success = SqlCalls::SqlUpdate( avi,"avisos",Configuracion_global->groupDB,QString("id=%1").arg(this->id_aviso_fin),
                                                error);
            if(!success)
                QMessageBox::warning(this,tr("Gestión de excepciones"),tr("Ocurrió un error al actualizar aviso fin de excepción: %1").arg(error),
                                     tr("Aceptar"));
            else
                TimedMessageBox *t = new TimedMessageBox(this,tr("Se actualizó la fecha del aviso"));
        }
    }
}
