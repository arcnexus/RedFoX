#include "frmexcepciones.h"
#include "ui_frmexcepciones.h"
#include "../Busquedas/db_consulta_view.h"

FrmExcepciones::FrmExcepciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmExcepciones)
{
    ui->setupUi(this);
    m = new QSqlQueryModel(this);
    m->setQuery("select id,");
    ui->txt_codigo_articulo->installEventFilter(this);
    ui->txt_codigo_cliente->installEventFilter(this);
    setControlsReadOnly(true);
}

FrmExcepciones::~FrmExcepciones()
{
    delete ui;
}

void FrmExcepciones::cargar_articulo(int id_art)
{
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
}

void FrmExcepciones::buscar_id_cliente(QString cod_cli)
{
    QString error;
    int id_cliente = SqlCalls::SelectOneField("clientes","id",QString("codigo_cliente = %1").arg(cod_cli),
                                              Configuracion_global->groupDB,error).toInt();
    cargar_cliente(id_cliente);
}

void FrmExcepciones::buscar_id_articulo(QString cod_art)
{
    QString error;
    int id_art = SqlCalls::SelectOneField("articulos","id",QString("codigo = %1").arg(cod_art),
                                          Configuracion_global->groupDB,error).toInt();
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
    ui->txt_pvp_fijo->setText("0,00");
    ui->txt_variacion_dto->setText("0");
    ui->txt_variacion_porc->setText("0");
    ui->txt_variacion_porc_dto->setText("0");
    ui->txt_variacion_pvp->setText("0,00");
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
    ui->radArticulo->setFocus();


}
