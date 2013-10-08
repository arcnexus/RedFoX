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
    ui->txtDescripcion->setText(ui->txt_descripcion_articulo->text()+" / "+ui->txt_nombre_cliente->text());
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
        return false;
    }
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

void FrmExcepciones::cargar_excepcion(int id)
{
    QMap <int,QSqlRecord> ex;
    QString error;
    ex = SqlCalls::SelectRecord("articulos_excepciones",QString("id=%1").arg(id),Configuracion_global->empresaDB,error);
    if(error.isEmpty())
    {
        cargar_articulo(ex.value(id).value("id_articulo").toInt());
        cargar_cliente(ex.value(id).value("id_cliente").toInt());
        ui->txtDescripcion->setText(ex.value(id).value("descripcion").toString());
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
    cargar_articulo(this->id_articulo);
    cargar_cliente(this->id_cliente);
    this->adding = true;
}

void FrmExcepciones::on_btnEditar_clicked()
{
    setControlsReadOnly(false);
    setButtonsEditMode(true);

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
    h["importe_porc_aumento"] = ui->txt_importe_porc_aumento->text().toFloat();
    h["importe_moneda_aumento"] = Configuracion_global->MonedatoDouble(ui->txt_importe_moneda_aumento->text());
    h["importe_fijo"] = Configuracion_global->MonedatoDouble(ui->txt_importe_fijo->text());
    h["dto_aumento_fijo"] = ui->txt_dto_aumento_fijo->text().toFloat();
    h["dto_aumento_porc"] = ui->txt_dto_aumento_porc->text().toFloat();
    h["dto_fijo"] = ui->txt_dto_fijo->text().toFloat();
    h["fecha_inicio"] = ui->txtDesde->date();
    h["fecha_final"] = ui->txtHasta->date();

    QString error;
    if(this->adding)
    {
        this->adding = false;
        int new_id = SqlCalls::SqlInsert(h,"articulos_excepciones",Configuracion_global->empresaDB,error);
        if(new_id > -1)
            TimedMessageBox *t = new TimedMessageBox(this,tr("Se añadió la nueva excepción."));
        else
            QMessageBox::warning(this,tr("Articulos excepciones"),tr("Ocurrió un error al insertar: %1").arg(error));

    } else
    {
        bool success = SqlCalls::SqlUpdate(h,"articulos_excepciones",Configuracion_global->empresaDB,
                                           QString("id=%1").arg(this->id),error);
        if(!success)
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
    frna.setHora(QTime(9,0));
    frna.setText(tr("Inicio excepcion: %1").arg(ui->txtDescripcion->text()));
    if(frna.exec() == QDialog::Accepted)
    {
        h_aviso["fecha_hora_aviso"] = QDateTime(frna.getFecha(),frna.getHora());
        h_aviso["aviso"] = frna.getText();
        h_aviso["id_usuario_destino"] = frna.get_id_usuario_destino();
        h_aviso["id_usuario_origen"] = Configuracion_global->id_usuario_activo;
        h_aviso["id_empresa"] = Configuracion_global->idEmpresa;

        int new_id = oAviso.add_notice(h_aviso);
        if(new_id == -1)
            QMessageBox::warning(this,tr("Gestión de excepciones"),
                                 tr("Atención: Ocurrió un error al insertar el aviso de inicio: %1").arg(error));
    }
}
