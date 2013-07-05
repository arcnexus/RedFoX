#include "frmproveedores.h"
#include "ui_frmproveedores.h"
#include "proveedor.h"

#include "../Busquedas/frmbuscarpoblacion.h"
#include "frmfacturasproveedor.h"
#include "frmpedidosproveedor.h"
#include "frmalbaranproveedor.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/datedelegate.h"
#include "../Busquedas/frmbuscarproveedor.h"
#include "../Auxiliares/frmaddentregascuenta.h"
#include "../Auxiliares/entregascuenta.h"


#ifdef WIN32
//#define and && 4005401548072

#endif
Proveedor *oProveedor = new Proveedor();

frmProveedores::frmProveedores(QWidget *parent) :
  MayaModule(module_zone(),module_name(),parent),
  ui(new Ui::frmProveedores),
  toolButton(tr("Proveedores"),":/Icons/PNG/proveedores_2.png",this),
  menuButton(QIcon(":/Icons/PNG/proveedores_2.png"),tr("Proveedores"),this),
  push(new QPushButton(QIcon(":/Icons/PNG/proveedores_2.png"),"",this))

{
    ui->setupUi(this);
    oProveedor->id = 0;
    BloquearCampos();
    ui->txtfecha_alta->setDate(QDate::currentDate());
    ui->txtfecha_ultima_compra->setDate(QDate::currentDate());
    //---------------------------
    // cargar datos forma de pago.
    //---------------------------
    QSqlQueryModel *qmFormaPago = new QSqlQueryModel(this);
    qmFormaPago->setQuery("select codigo, forma_pago from formpago",QSqlDatabase::database("Maya"));

    ui->txtcodigoFormaPago->setModel(qmFormaPago);
    oProveedor->idFormadePago = Configuracion_global->Devolver_id_forma_pago(ui->txtcodigoFormaPago->currentText());

    // ---------------
    // Cargar divisas
    //----------------
    QSqlQueryModel * modelDivisas = new QSqlQueryModel(this);
    modelDivisas->setQuery("select moneda from monedas",QSqlDatabase::database("Maya"));
    ui->cboDivisas->setModel(modelDivisas);

    // ----------------
    // Cargar Paises
    // ----------------
    QSqlQueryModel * modelPais = new QSqlQueryModel(this);
    modelPais->setQuery("select pais from paises",QSqlDatabase::database("Maya"));
    ui->txtpais->setModel(modelPais);
    oProveedor->id_pais = Configuracion_global->Devolver_id_pais(ui->txtpais->currentText());

    // ---------------------
    // Cargar Paises almacen
    // ---------------------
    QSqlQueryModel * modelPaisAlmacen = new QSqlQueryModel(this);
    modelPaisAlmacen->setQuery("select pais from paises",QSqlDatabase::database("Maya"));
    ui->txtpaisAlmacen->setModel(modelPaisAlmacen);
    oProveedor->id_pais_almacen = Configuracion_global->Devolver_id_pais(ui->txtpaisAlmacen->currentText());


    // -----------------------
    // CONTROLES
    // -----------------------
    ui->btnGuardarContacto->setVisible(false);

    // -----------------------
    // CONEXIONES
    //------------------------
    connect(ui->btnAnadir_persona_contacto,SIGNAL(clicked()),this,SLOT(nuevo_contacto()));
    ui->tablaContactos->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tablaPagos->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tablaContactos, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(menu_contactos(const QPoint&)));
    connect(ui->tablaPagos,SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(menu_deudas(const QPoint&)));
}

frmProveedores::~frmProveedores()
{
    delete ui;
}
void frmProveedores::LLenarCampos()
{
    ui->lblcodigo_proveedor->setText(oProveedor->codigo);
    ui->lblproveedor->setText(oProveedor->proveedor);
    ui->txtcodigo->setText(oProveedor->codigo);
    ui->txtproveedor->setText(oProveedor->proveedor);
    ui->txtcif->setText(oProveedor->cif);
    ui->txtdireccion1->setText(oProveedor->direccion1);
    ui->txtdireccion2->setText(oProveedor->direccion2);
    ui->txtcp->setText(oProveedor->cp);
    ui->txtpoblacion->setText(oProveedor->poblacion);
    ui->txtprovincia->setText(oProveedor->provincia);
    int index = ui->txtpais->findText(oProveedor->pais);
    ui->txtpais->setCurrentIndex(index);
    ui->txttelefono1->setText(oProveedor->telefono1);
    ui->txttelefono2->setText(oProveedor->telefono2);
    ui->txttelefono3->setText(oProveedor->telefono3);
    ui->txtfax->setText(oProveedor->fax);
    ui->txtmovil->setText(oProveedor->movil);
    ui->txtemail->setText(oProveedor->email);
    ui->txtweb->setText(oProveedor->web);
    ui->txtpersona_contacto->setText(oProveedor->persona_contacto);
    ui->txtdia_cobro->setText(QString::number(oProveedor->dia_cobro));
    ui->txtdireccion_almacen->setText(oProveedor->direccion_almacen);
    ui->txtcp_almacen->setText(oProveedor->cp_almacen);
    ui->txtpoblacion_almacen->setText(oProveedor->poblacion_almacen);
    ui->txtprovinciaAmacen->setText(oProveedor->provincia_almacen);
    index = ui->txtpaisAlmacen->findText(oProveedor->paisAlmacen);
    ui->txtpaisAlmacen->setCurrentIndex(index);
    ui->txttelefono_almacen->setText(oProveedor->telefono_almacen);
    ui->txtfax_almacen->setText(oProveedor->fax_almacen);

    int nIndex = ui->txtcodigoFormaPago->findText(oProveedor->codigoFormaPago);
    if (nIndex !=-1)
    {
        ui->txtcodigoFormaPago->setCurrentIndex(nIndex);
        ui->txtforma_pago->setText(Configuracion_global->Devolver_forma_pago(oProveedor->idFormadePago));

    } else
    {
        ui->txtforma_pago->setText("");
    }

    ui->txtfecha_ultima_compra->setDate(oProveedor->fecha_ultima_compra);
    ui->txtacumulado_compras->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->acumulado_compras,'f',2)));
    ui->txtentidad_bancaria_proveedor->setText(oProveedor->entidad_bancaria_proveedor);
    ui->txtoficina_bancaria_proveedor->setText(oProveedor->oficina_bancaria_proveedor);
    ui->txtdc_proveedor->setText(oProveedor->dc_proveedor);
    ui->txtcc_proveedor->setText(oProveedor->cc_proveedor);
    ui->txtentidad_pago_proveedor->setText(oProveedor->entidad_pago_proveedor);
    ui->txtoficina_pago_proveedor->setText(oProveedor->oficina_pago_proveedor);
    ui->txtdc_pago_proveedor->setText(oProveedor->dc_pago_proveedor);
    ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->entregado_a_cuenta,'f',2)));
    ui->txtretencion_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->retencion_irpf,'f',2)));
    ui->txttipo_retencioirpf->setText(QString::number(oProveedor->tipo_retencion));
    ui->txtcuenta_aplicacion->setText(oProveedor->cuenta_aplicacion);
    ui->txtcomentarios->setText(oProveedor->comentarios);
    ui->txtdto->setText(QString::number(oProveedor->dto));
    ui->txtfecha_alta->setDate(oProveedor->fecha_alta);
    ui->txtdeuda_maxima->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->deuda_maxima,'f',2)));
    ui->txtdeuda_actual->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->deuda_actual,'f',2)));
    if (oProveedor->recargo_equivalencia==1)
        ui->chkrecargo_equivalencia->setChecked(true);
    else
        ui->chkrecargo_equivalencia->setChecked(false);
    ui->txttexto_para_pedidos->setPlainText(oProveedor->texto_para_pedidos);

    QString cOk;
    if(!ui->txtentidad_pago_proveedor->text().isEmpty() && !ui->txtoficina_pago_proveedor->text().isEmpty() &&
            !ui->txtdc_pago_proveedor->text().isEmpty() && !ui->txtcuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_pago_proveedor->text(),ui->txtoficina_pago_proveedor->text(),
                                              ui->txtdc_pago_proveedor->text(),ui->txtcuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
    if(!ui->txtentidad_bancaria_proveedor->text().isEmpty() && !ui->txtoficina_bancaria_proveedor->text().isEmpty() &&
            !ui->txtdc_proveedor->text().isEmpty() && !ui->txtcc_proveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_bancaria_proveedor->text(),ui->txtoficina_bancaria_proveedor->text(),
                               ui->txtdc_proveedor->text(),ui->txtcc_proveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));
    //----------------------
    //  Rellenar Historiales
    //----------------------

    historiales();

    // --------------------
    // Rellenar Acumulados
    //---------------------
    acumulados();

    //---------------------
    // Grafica
    //---------------------
    grafica();

    // -----------------------
    // Cargar Contactos
    // -----------------------
    contactos();




}

void frmProveedores::CargarCamposEnProveedor()
{
    oProveedor->codigo = ui->txtcodigo->text();
    oProveedor->proveedor =ui->txtproveedor->text();
    oProveedor->cif = ui->txtcif->text();
    oProveedor->direccion1 = ui->txtdireccion1->text();
    oProveedor->direccion2 = ui->txtdireccion2->text();
    oProveedor->cp = ui->txtcp->text();
    oProveedor->poblacion = ui->txtpoblacion->text();
    oProveedor->provincia = ui->txtprovincia->text();
    oProveedor->pais = ui->txtpais->currentText();
    oProveedor->telefono1 = ui->txttelefono1->text();
    oProveedor->telefono2 = ui->txttelefono2->text();
    oProveedor->telefono3 = ui->txttelefono3->text();
    oProveedor->fax = ui->txtfax->text();
    oProveedor->movil = ui->txtmovil->text();
    oProveedor->email = ui->txtemail->text();
    oProveedor->web = ui->txtweb->text();
    oProveedor->persona_contacto = ui->txtpersona_contacto->text();
    oProveedor->dia_cobro = ui->txtdia_cobro->text().toInt();
    oProveedor->direccion_almacen = ui->txtdireccion_almacen->text();
    oProveedor->cp_almacen = ui->txtcp_almacen->text();
    oProveedor->poblacion_almacen = ui->txtpoblacion_almacen->text();
    oProveedor->provincia_almacen = ui->txtprovinciaAmacen->text();
    oProveedor->paisAlmacen = ui->txtpaisAlmacen->currentText();
    oProveedor->telefono_almacen = ui->txttelefono_almacen->text();
    oProveedor->fax_almacen = ui->txtfax_almacen->text();
    oProveedor->codigoFormaPago = ui->txtcodigoFormaPago->currentText();
    oProveedor->fecha_ultima_compra = ui->txtfecha_ultima_compra->date();
    oProveedor->acumulado_compras = ui->txtacumulado_compras->text().replace(",",".").toDouble();
    oProveedor->entidad_bancaria_proveedor = ui->txtentidad_bancaria_proveedor->text();
    oProveedor->oficina_bancaria_proveedor = ui->txtoficina_bancaria_proveedor->text();
    oProveedor->dc_proveedor = ui->txtdc_proveedor->text();
    oProveedor->cc_proveedor = ui->txtcc_proveedor->text();
    oProveedor->entidad_pago_proveedor = ui->txtentidad_pago_proveedor->text();
    oProveedor->oficina_pago_proveedor = ui->txtoficina_pago_proveedor->text();
    oProveedor->dc_pago_proveedor = ui->txtdc_pago_proveedor->text();
    oProveedor->retencion_irpf = ui->txtretencion_irpf->text().replace(",",".").toDouble();
    oProveedor->tipo_retencion = ui->txttipo_retencioirpf->text().toInt();
    oProveedor->cuenta_aplicacion = ui->txtcuenta_aplicacion->text();
    oProveedor->comentarios = ui->txtcomentarios->toPlainText();
    oProveedor->dto = ui->txtdto->text().replace(",",".").toDouble();
    oProveedor->fecha_alta = ui->txtfecha_alta->date();
    oProveedor->deuda_maxima = ui->txtdeuda_maxima->text().replace(",",".").toDouble();
    oProveedor->deuda_actual = ui->txtdeuda_actual->text().replace(",",".").toDouble();
    oProveedor->recargo_equivalencia = ui->chkrecargo_equivalencia->isChecked();
    oProveedor->texto_para_pedidos = ui->txttexto_para_pedidos->toPlainText();
    cargar_forma_pago(ui->txtcodigoFormaPago->currentText());

}

void frmProveedores::cargar_forma_pago(QString codigo)
{
    int id_forma_pago = Configuracion_global->Devolver_id_codigo_forma_pago(codigo);
    ui->txtforma_pago->setText(Configuracion_global->Devolver_forma_pago(id_forma_pago));
    oProveedor->idFormadePago = id_forma_pago;
}
void frmProveedores::DesbloquerCampos()
{
    // LineEdit
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
    //    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    //    QSpinBox *SpinBox;
    //    foreach (SpinBox, SpinBoxList) {
    //        SpinBox->setReadOnly(false);
    //        //qDebug() << lineEdit->objectName();
    //    }
    // DoubleSpinBox
    //    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    //    QDoubleSpinBox *DSpinBox;
    //    foreach (DSpinBox, DSpinBoxList) {
    //        DSpinBox->setReadOnly(false);
    //        //qDebug() << lineEdit->objectName();
    //    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    ui->btnAnadir->setEnabled(false);
    ui->btnAnterior->setEnabled(false);
    ui->btnBorrar->setEnabled(false);
    ui->btnBuscar->setEnabled(false);
    ui->btnDeshacer->setEnabled(true);
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnSiguiente->setEnabled(false);
}

void frmProveedores::BloquearCampos()
{
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(true);
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(false);
    }
    //    // SpinBox
    //    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    //    QSpinBox *SpinBox;
    //    foreach (SpinBox, SpinBoxList) {
    //        SpinBox->setReadOnly(true);
    //    }
    //    // DoubleSpinBox
    //    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    //    QDoubleSpinBox *DSpinBox;
    //    foreach (DSpinBox, DSpinBoxList) {
    //        DSpinBox->setReadOnly(true);
    //    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(false);

    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(true);
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(false);
    }

    ui->btnAnadir->setEnabled(true);
    ui->btnAnterior->setEnabled(true);
    ui->btnBorrar->setEnabled(true);
    ui->btnBuscar->setEnabled(true);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);

}



void frmProveedores::on_btnSiguiente_clicked()
{
    oProveedor->Recuperar("Select * from proveedores where id>"+QString::number(oProveedor->id),1);
    LLenarCampos();
}

void frmProveedores::on_btnAnterior_clicked()
{
    oProveedor->Recuperar("Select * from proveedores where id<"+QString::number(oProveedor->id)+" order by id desc",2);
    LLenarCampos();
}

void frmProveedores::                          on_btnGuardar_clicked()
{
    bool lGuardar = true;
    QString cTexto;
    cTexto = tr("Hay campos que debe rellenar para poder guardar")+"\n";
    if (oProveedor->idFormadePago == 0)
    {
        lGuardar = false;
        cTexto = cTexto + tr("Debe elegir una forma de pago")+"\n";
    }

    if (oProveedor->id_pais == 0)
    {
        lGuardar = false;
        cTexto = cTexto + tr("Debe elegir el pais del Proveedor")+"\n";
    }
    if (lGuardar)
    {
        CargarCamposEnProveedor();
        oProveedor->Guardar();
        BloquearCampos();
    } else
    {
        QMessageBox::warning(this,tr("No se puede guardar, falta especificar datos"),cTexto,tr("Aceptar"));
    }
}

void frmProveedores::on_btnEditar_clicked()
{
    DesbloquerCampos();
    ui->txtcodigo->setFocus();
}

void frmProveedores::on_btnAnadir_clicked()
{
    oProveedor->Vaciar();
    oProveedor->Anadir();
    LLenarCampos();
    DesbloquerCampos();
    ui->txtcodigo->setFocus();
}


void frmProveedores::on_txtpoblacion_editingFinished()
{
    ui->txtpoblacion->setText(ui->txtpoblacion->text().toUpper());
    if (ui->txtcp->text().isEmpty() && !ui->txtpoblacion->text().isEmpty() && !ui->txtcp->isReadOnly()) {
        FrmBuscarPoblacion BuscarPoblacion;

        //Configuracion_global->CargarDatos();
        BuscarPoblacion.setpoblacion(ui->txtpoblacion->text(),1);
        if(BuscarPoblacion.exec()) {
            //  BuscarPoblacion.setpoblacion(ui->txtcp->text(),0);
            int nid = BuscarPoblacion.Devolverid();

            if(nid > 0) {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cid;
                cid = QString::number(nid);
                qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cid");
                qPoblacion.bindValue(":id",cid);
                if(!qPoblacion.exec()) {
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                } else {
                    if (qPoblacion.next()) {
                        ui->txtpoblacion->setText(qPoblacion.value(0).toString());
                        ui->txtcp->setText(qPoblacion.value(1).toString());
                        ui->txtprovincia->setText(qPoblacion.value(2).toString());
                        int index = ui->txtpais->findText(Configuracion_global->pais);
                        ui->txtpais->setCurrentIndex(index);
                    }
                }

            }
        }
    }
    ui->txttelefono1->setFocus();

}

void frmProveedores::on_txtcp_editingFinished()
{
    if (!ui->txtcp->text().isEmpty() && ui->txtpoblacion->text().isEmpty() && !ui->txtcp->isReadOnly()){
        FrmBuscarPoblacion BuscarPoblacion;

        BuscarPoblacion.setpoblacion(ui->txtcp->text(),0);
        if(BuscarPoblacion.exec()) {
            int nid = BuscarPoblacion.Devolverid();
            if(nid > 0) {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cid;
                cid = QString::number(nid);
                qPoblacion.prepare("select  poblacion,  cp, provincia from poblaciones where id = :cid");
                qPoblacion.bindValue(":id",cid);
                if(!qPoblacion.exec()) {
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                } else {
                    if (qPoblacion.next()) {
                        ui->txtpoblacion->setText(qPoblacion.record().value("poblacion").toString());
                        ui->txtprovincia->setText(qPoblacion.record().value("provincia").toString());
                        int index = ui->txtpais->findText(Configuracion_global->pais);
                        ui->txtpais->setCurrentIndex(index);
                        ui->txttelefono1->setFocus();
                    }
                }

            }
        }
    }

}






void frmProveedores::on_btnBorrar_clicked()
{
    int ndev = QMessageBox::question(this,tr("Gestión de proveedores"),tr("Está seguro/a de borrar el proveedor?\n"
                                                                          "se borrarán todos los historiales de ese proveedor.\n"
                                                                          " Esta opción no se puede deshacer"),
                                     tr("Borrar"),tr("Cancelar Borrado"));
    if(ndev ==0) {
        oProveedor->Borrar(oProveedor->id);
        oProveedor->Vaciar();
        LLenarCampos();
    }
}

void frmProveedores::on_btnDeshacer_clicked()
{
    int ndev = QMessageBox::question(NULL,tr("Gestión de proveedores"),tr("Se perderán los cambios realizados en la ficha\n"
                                                                          " Esta opción no se puede deshacer\n\n"
                                                                          "¿ Desea anular los cambios o continuar la edición?"),
                                     tr("Continuar Edición"),tr("Anular cambios"));
    if(ndev==1) {
        oProveedor->Recuperar("Select * from proveedores where id = "+QString::number(oProveedor->id));
        LLenarCampos();
        BloquearCampos();
    }
}

void frmProveedores::on_txtentidad_bancaria_proveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtentidad_bancaria_proveedor->text().isEmpty() && !ui->txtoficina_bancaria_proveedor->text().isEmpty() &&
            !ui->txtdc_proveedor->text().isEmpty() && !ui->txtcc_proveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_bancaria_proveedor->text(),ui->txtoficina_bancaria_proveedor->text(),
                                 ui->txtdc_proveedor->text(),ui->txtcc_proveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));
}

void frmProveedores::on_txtoficina_bancaria_proveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtentidad_bancaria_proveedor->text().isEmpty() && !ui->txtoficina_bancaria_proveedor->text().isEmpty() &&
            !ui->txtdc_proveedor->text().isEmpty() && !ui->txtcc_proveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_bancaria_proveedor->text(),ui->txtoficina_bancaria_proveedor->text(),
                                 ui->txtdc_proveedor->text(),ui->txtcc_proveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtdc_proveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtentidad_bancaria_proveedor->text().isEmpty() && !ui->txtoficina_bancaria_proveedor->text().isEmpty() &&
            !ui->txtdc_proveedor->text().isEmpty() && !ui->txtcc_proveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_bancaria_proveedor->text(),ui->txtoficina_bancaria_proveedor->text(),
                                 ui->txtdc_proveedor->text(),ui->txtcc_proveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));
}



void frmProveedores::on_txtcc_proveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtentidad_bancaria_proveedor->text().isEmpty() && !ui->txtoficina_bancaria_proveedor->text().isEmpty() &&
            !ui->txtdc_proveedor->text().isEmpty() && !ui->txtcc_proveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_bancaria_proveedor->text(),ui->txtoficina_bancaria_proveedor->text(),
                                 ui->txtdc_proveedor->text(),ui->txtcc_proveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtentidad_pago_proveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtentidad_pago_proveedor->text().isEmpty() && !ui->txtoficina_pago_proveedor->text().isEmpty() &&
            !ui->txtdc_pago_proveedor->text().isEmpty() && !ui->txtcuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_pago_proveedor->text(),ui->txtoficina_pago_proveedor->text(),
                                 ui->txtdc_pago_proveedor->text(),ui->txtcuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtoficina_pago_proveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtentidad_pago_proveedor->text().isEmpty() && !ui->txtoficina_pago_proveedor->text().isEmpty() &&
            !ui->txtdc_pago_proveedor->text().isEmpty() && !ui->txtcuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_pago_proveedor->text(),ui->txtoficina_pago_proveedor->text(),
                                 ui->txtdc_pago_proveedor->text(),ui->txtcuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtdc_pago_proveedor_editingFinished()
{
    QString cOk;
    if(!ui->txtentidad_pago_proveedor->text().isEmpty() && !ui->txtoficina_pago_proveedor->text().isEmpty() &&
            !ui->txtdc_pago_proveedor->text().isEmpty() && !ui->txtcuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_pago_proveedor->text(),ui->txtoficina_pago_proveedor->text(),
                                 ui->txtdc_pago_proveedor->text(),ui->txtcuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
}

void frmProveedores::on_txtcuentaPagoProveedor_editingFinished()
{
    QString cOk;
    if(!ui->txtentidad_pago_proveedor->text().isEmpty() && !ui->txtoficina_pago_proveedor->text().isEmpty() &&
            !ui->txtdc_pago_proveedor->text().isEmpty() && !ui->txtcuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_pago_proveedor->text(),ui->txtoficina_pago_proveedor->text(),
                                 ui->txtdc_pago_proveedor->text(),ui->txtcuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
}

void frmProveedores::on_txtcp_almacen_editingFinished()
{
    if (!ui->txtcp_almacen->isReadOnly()) {
        if (!ui->txtcp_almacen->text().isEmpty() && ui->txtpoblacion_almacen->text().isEmpty()) {
            FrmBuscarPoblacion BuscarPoblacion;

            //Configuracion_global->CargarDatos();
            BuscarPoblacion.setpoblacion(ui->txtcp_almacen->text(),0);
            if(BuscarPoblacion.exec()) {
                int nid = BuscarPoblacion.Devolverid();
                if(nid > 0) {
                    QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                    QString cid;
                    cid = QString::number(nid);
                    qPoblacion.prepare("select poblacion, cp, provincia from poblaciones where id = :cid");
                    qPoblacion.bindValue(":id",cid);
                    if(!qPoblacion.exec()) {
                        QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                    } else {
                        if (qPoblacion.next()) {
                            ui->txtpoblacion_almacen->setText(qPoblacion.value(0).toString());
                            ui->txtprovinciaAmacen->setText(qPoblacion.value(2).toString());
                            int index = ui->txtpaisAlmacen->findText(Configuracion_global->pais);
                            ui->txtpaisAlmacen->setCurrentIndex(index);
                        }
                    }

                }
            }
        }
        ui->txttelefono_almacen->setFocus();
    }
}

void frmProveedores::on_txtpais_currentIndexChanged(const QString &arg1)
{
    QString pais = arg1;
   oProveedor->id_pais = Configuracion_global->Devolver_id_pais(pais);
}


void frmProveedores::on_txtcodigoFormaPago_currentIndexChanged(const QString &arg1)
{
    QString codigo = arg1;
    oProveedor->idFormadePago = Configuracion_global->Devolver_id_codigo_forma_pago(codigo);
}

void frmProveedores::on_btnNuevaFactura_clicked()
{
    FrmFacturasProveedor frmFactura(this,true);
    frmFactura.setWindowState(Qt::WindowMaximized);
    frmFactura.llenarProveedor(oProveedor->id);
    frmFactura.exec();
}

void frmProveedores::on_btnNuevoAlbaran_clicked()
{
    FrmAlbaranProveedor frmAlbaran(this,true);
    frmAlbaran.setWindowState(Qt::WindowMaximized);
    frmAlbaran.llenarProveedor(oProveedor->id);
    frmAlbaran.exec();
}

void frmProveedores::on_btnNuevoPedido_clicked()
{
    FrmPedidosProveedor frmPedidos(this,true);
    frmPedidos.setWindowState(Qt::WindowMaximized);
    frmPedidos.llenarProveedor(oProveedor->id,true);
    frmPedidos.exec();
}

void frmProveedores::pagar_deuda()
{
    if(QMessageBox::question(this,tr("Gestión de Proveedores"),
                             tr("¿Cobrar la deuda?"),
                             tr("Cancelar"),tr("Cobrar"))==QMessageBox::Accepted)
    {

    }
}

void frmProveedores::pagar_fraccion()
{

}

void frmProveedores::ver_asiento()
{

}

void frmProveedores::historiales()
{
    // -------------------------
    // Cargar historial Pedidos
    //--------------------------
    QSqlQueryModel *modelPedidos = new QSqlQueryModel(this);
    modelPedidos->setQuery("select id,pedido,fecha,total,recibido_completo from ped_pro where id_proveedor ="+
                           QString::number(oProveedor->id)+" order by pedido desc",QSqlDatabase::database("empresa"));
    ui->tablaColumnasPedidos->setModel(modelPedidos);
    ui->tablaColumnasPedidos->setColumnHidden(0,true);
    ui->tablaColumnasPedidos->setColumnWidth(1,120);
    ui->tablaColumnasPedidos->setColumnWidth(1,120);
    ui->tablaColumnasPedidos->setColumnWidth(3,120);
    modelPedidos->setHeaderData(1,Qt::Horizontal,"PEDidO");
    ui->tablaColumnasPedidos->setItemDelegateForColumn(2, new DateDelegate);
    modelPedidos->setHeaderData(2,Qt::Horizontal,"Fecha");
    ui->tablaColumnasPedidos->setItemDelegateForColumn(3,new MonetaryDelegate);
    modelPedidos->setHeaderData(3,Qt::Horizontal,"Total");
    modelPedidos->setHeaderData(4,Qt::Horizontal,"C.");


    // -------------------------
    // Cargar Historial Facturas
    //--------------------------
    QSqlQueryModel *modelFacturas = new QSqlQueryModel(this);
    modelFacturas->setQuery("select id,factura,fecha,pedido,total_base,total_iva,total_retencion,total_recargo,total,importe_deuda_pendiente"
                            " from fac_pro where id_proveedor = "+QString::number(oProveedor->id)+
                            " order by factura desc",QSqlDatabase::database("empresa"));
    ui->tablacolumnasFacturas->setModel(modelFacturas);
    ui->tablacolumnasFacturas->setColumnHidden(0,true);
    modelFacturas->setHeaderData(1,Qt::Horizontal,"N.Factura");
    modelFacturas->setHeaderData(2,Qt::Horizontal,"Fecha");
    modelFacturas->setHeaderData(3,Qt::Horizontal,"Pedido");
    modelFacturas->setHeaderData(4,Qt::Horizontal,"Base IMP.");
    modelFacturas->setHeaderData(5,Qt::Horizontal,"Tot.IVA");
    modelFacturas->setHeaderData(6,Qt::Horizontal,"Ret IRPF");
    modelFacturas->setHeaderData(7,Qt::Horizontal,"Rec. Eq.");
    modelFacturas->setHeaderData(8,Qt::Horizontal,"TOTAL");
    modelFacturas->setHeaderData(9,Qt::Horizontal,"Pendiente");

    ui->tablacolumnasFacturas->setItemDelegateForColumn(2,new DateDelegate);
    ui->tablacolumnasFacturas->setItemDelegateForColumn(4,new MonetaryDelegate);
    ui->tablacolumnasFacturas->setItemDelegateForColumn(5,new MonetaryDelegate);
    ui->tablacolumnasFacturas->setItemDelegateForColumn(6,new MonetaryDelegate);
    ui->tablacolumnasFacturas->setItemDelegateForColumn(7,new MonetaryDelegate);
    ui->tablacolumnasFacturas->setItemDelegateForColumn(8,new MonetaryDelegate);
    ui->tablacolumnasFacturas->setItemDelegateForColumn(9,new MonetaryDelegate);


    // --------------------------
    // Cargar Historial Albaranes
    //---------------------------
    QSqlQueryModel *modelAlbaranes = new QSqlQueryModel(this);
    modelAlbaranes->setQuery("select id,albaran,fecha,base_total,iva_total,"
                             " total from alb_pro where id_proveedor = "+QString::number(oProveedor->id)+
                             " order by albaran desc ",QSqlDatabase::database("empresa"));
    ui->tablaColumnasAlbaran->setModel(modelAlbaranes);
    ui->tablaColumnasAlbaran->setColumnHidden(0,true);
    modelAlbaranes->setHeaderData(1,Qt::Horizontal,"N.Albarán");
    modelAlbaranes->setHeaderData(2,Qt::Horizontal,"Fecha");
    modelAlbaranes->setHeaderData(3,Qt::Horizontal,"Pedido");
    modelAlbaranes->setHeaderData(4,Qt::Horizontal,"Base IMP.");
    modelAlbaranes->setHeaderData(5,Qt::Horizontal,"Tot.IVA");
    modelAlbaranes->setHeaderData(6,Qt::Horizontal,"Ret IRPF");
    modelAlbaranes->setHeaderData(7,Qt::Horizontal,"Rec. Eq.");
    modelAlbaranes->setHeaderData(8,Qt::Horizontal,"TOTAL");
    modelAlbaranes->setHeaderData(9,Qt::Horizontal,"Pendiente");

    ui->tablaColumnasAlbaran->setItemDelegateForColumn(2,new DateDelegate);
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(4,new MonetaryDelegate);
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(5,new MonetaryDelegate);
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(6,new MonetaryDelegate);
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(7,new MonetaryDelegate);
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(8,new MonetaryDelegate);
    ui->tablaColumnasAlbaran->setItemDelegateForColumn(9,new MonetaryDelegate);

    //---------------------
    // Artículos proveedor
    //---------------------
    QSqlQueryModel *modelArticulo = new QSqlQueryModel(this);
    modelArticulo->setQuery("select codigo,codigo_barras, descripcion,coste "
                           "from articulos where id_proveedor =" +QString::number(oProveedor->id),
                           QSqlDatabase::database("Maya"));


    ui->tablaArticulos->setModel(modelArticulo);
    ui->tablaArticulos->setColumnWidth(2,200);
    ui->tablaArticulos->setItemDelegateForColumn(3,new MonetaryDelegate);

    //----------------------
    // Deudas a Proveedores
    //----------------------
    QSqlQueryModel *modeloDeudas = new QSqlQueryModel(this);
    modeloDeudas->setQuery("select id,documento,fecha_deuda,vencimiento,importe_deuda,pagado,pendiente,pago_por,"
                           "numero_tarjeta_cuenta,asiento_numero from deudas_proveedores where id_proveedor = "+
                           QString::number(oProveedor->id)+ " order by fecha_deuda desc",QSqlDatabase::database("empresa"));
    ui->tablaPagos->setModel(modeloDeudas);
    ui->tablaPagos->setColumnHidden(0,true);
    ui->tablaPagos->setColumnWidth(1,80);
    ui->tablaPagos->setColumnWidth(2,70);
    ui->tablaPagos->setColumnWidth(3,70);
    ui->tablaPagos->setColumnWidth(4,65);
    ui->tablaPagos->setColumnWidth(5,65);
    ui->tablaPagos->setColumnWidth(6,65);
    ui->tablaPagos->setColumnWidth(8,200);
    ui->tablaPagos->setColumnWidth(9,200);
    ui->tablaPagos->setItemDelegateForColumn(2,new DateDelegate);
    ui->tablaPagos->setItemDelegateForColumn(3,new DateDelegate);
    ui->tablaPagos->setItemDelegateForColumn(4,new MonetaryDelegate);
    ui->tablaPagos->setItemDelegateForColumn(5,new MonetaryDelegate);
    ui->tablaPagos->setItemDelegateForColumn(6, new MonetaryDelegate);
    modeloDeudas->setHeaderData(1,Qt::Horizontal,tr("Documento"));
    modeloDeudas->setHeaderData(2,Qt::Horizontal,tr("F. Deuda"));
    modeloDeudas->setHeaderData(3,Qt::Horizontal,tr("Vto"));
    modeloDeudas->setHeaderData(4,Qt::Horizontal,tr("Importe"));
    modeloDeudas->setHeaderData(5,Qt::Horizontal,tr("Pagado"));
    modeloDeudas->setHeaderData(6,Qt::Horizontal,tr("Pendiente"));
    modeloDeudas->setHeaderData(7,Qt::Horizontal,tr("Pago por...."));
    modeloDeudas->setHeaderData(8,Qt::Horizontal,tr("N.Tarjeta/Cuenta"));
    modeloDeudas->setHeaderData(9,Qt::Horizontal,tr("Asiento numero"));

    //----------------------
    // Asientos contables
    //----------------------
    QSqlQueryModel *modelAsientos = new QSqlQueryModel(this);

    modelAsientos->setQuery("select id,asiento,fecha_asiento,cuenta_d,descripcion_d, importe_d,cuenta_h,descripcion_h,importe_h "
                            "from diario where cta_principal = '"+oProveedor->cuenta_aplicacion +
                            "' order by asiento + ' '+pos_en_asiento",
                            QSqlDatabase::database("dbconta"));

    ui->tablaAsientos->setModel(modelAsientos);
    ui->tablaAsientos->setColumnHidden(0,true);

    ui->tablaAsientos->setColumnWidth(1,55);
    ui->tablaAsientos->setColumnWidth(2,63);
    ui->tablaAsientos->setItemDelegateForColumn(2, new DateDelegate);
    ui->tablaAsientos->setColumnWidth(3,55);
    ui->tablaAsientos->setColumnWidth(4,75);
    ui->tablaAsientos->setColumnWidth(5,60);
    ui->tablaAsientos->setItemDelegateForColumn(5, new MonetaryDelegate);
    ui->tablaAsientos->setColumnWidth(6,55);
    ui->tablaAsientos->setColumnWidth(7,75);
    ui->tablaAsientos->setColumnWidth(8,60);
    ui->tablaAsientos->setItemDelegateForColumn(8, new MonetaryDelegate);

    //------------------------------
    // Historial entregas a cuenta
    //------------------------------
    QSqlQueryModel * modelEntregas = new QSqlQueryModel(this);
    modelEntregas->setQuery("select id,fecha_entrega,concepto,importe,disponible from proveedor_a_cuenta where id_proveedor = "+QString::number(oProveedor->id),
                            QSqlDatabase::database("Maya"));
    ui->tabla_entregas->setModel(modelEntregas);
    ui->tabla_entregas->setColumnHidden(0,true);
    ui->tabla_entregas->setColumnWidth(1,100);
    ui->tabla_entregas->setItemDelegateForColumn(1,new DateDelegate);
    ui->tabla_entregas->setColumnWidth(2,250);
    ui->tabla_entregas->setColumnWidth(3,80);
    ui->tabla_entregas->setItemDelegateForColumn(3,new MonetaryDelegate);
    ui->tabla_entregas->setColumnWidth(4,80);
    ui->tabla_entregas->setItemDelegateForColumn(4,new MonetaryDelegate);
    QStringList cHeader;
    cHeader << tr("Fecha") <<tr("concepto") << tr("importe") <<tr("disponible");
    for(int pos = 0; pos < cHeader.size();pos++)
    {
        modelEntregas->setHeaderData(pos,Qt::Horizontal,cHeader.at(pos));
    }



}

void frmProveedores::acumulados()
{
    //----------------------
    // Acumulados
    //----------------------
    ui->txtEnero->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->enero,'f',2)));
    ui->txtFebrero->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->febrero,'f',2)));
    ui->txtMarzo->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->marzo,'f',2)));
    ui->txtAbril->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->abril,'f',2)));
    ui->txtMayo->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->mayo,'f',2)));
    ui->txtJunio->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->junio,'f',2)));
    ui->txtjulio->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->julio,'f',2)));
    ui->txtAgosto->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->agosto,'f',2)));
    ui->txtSeptiembre->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->septiembre,'f',2)));
    ui->txtOctubre->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->octubre,'f',2)));
    ui->txtNoviembre->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->noviembre,'f',2)));
    ui->txtDiciembre->setText(Configuracion_global->toFormatoMoneda(QString::number(oProveedor->diciembre,'f',2)));

}

void frmProveedores::grafica()
{
    ui->Grafica->Clear();

    ui->Grafica->addItem(tr("Ene"),oProveedor->enero);
    ui->Grafica->addItem(tr("Feb"),oProveedor->febrero);
    ui->Grafica->addItem(tr("Mar"),oProveedor->marzo);
    ui->Grafica->addItem(tr("Abr"),oProveedor->abril);
    ui->Grafica->addItem(tr("May"),oProveedor->mayo);
    ui->Grafica->addItem(tr("Jun"),oProveedor->junio);
    ui->Grafica->addItem(tr("Jul"),oProveedor->julio);
    ui->Grafica->addItem(tr("Ago"),oProveedor->agosto);
    ui->Grafica->addItem(tr("Sep"),oProveedor->septiembre);
    ui->Grafica->addItem(tr("Oct"),oProveedor->octubre);
    ui->Grafica->addItem(tr("Nov"),oProveedor->noviembre);
    ui->Grafica->addItem(tr("Dic"),oProveedor->diciembre);
}

void frmProveedores::contactos()
{
    QSqlQueryModel *modelContactos= new QSqlQueryModel(this);
    modelContactos->setQuery("select id,id_proveedor,cargo_empresa, nombre, desc_telefono1,telefono1,desc_telefono2,telefono2,"
                             "desc_telefono3, telefono3, desc_movil1,movil1,desc_movil2,movil2 "
                             "from personascontactoproveedor where id_proveedor ="+QString::number(oProveedor->id),
                             QSqlDatabase::database("Maya"));

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

void frmProveedores::menu_contactos(const QPoint &position)
{
   QPoint globalPos = ui->tablaContactos->mapToGlobal(position);
   QMenu myMenu;
   QAction *actionEditar = new QAction(tr("Editar Contacto"),this);
   QAction *actionBorrar = new QAction(tr("Borrar contacto"),this);
   myMenu.addAction(actionEditar);
   myMenu.addAction(actionBorrar);

       connect(actionEditar, SIGNAL(triggered()), this, SLOT(editar_contacto()));
       connect(actionBorrar,SIGNAL(triggered()),this,SLOT(borrar_contacto()));

    myMenu.exec(globalPos);


}

void frmProveedores::menu_deudas(const QPoint &position)
{
    QPoint globalPos = ui->tablaPagos->mapToGlobal(position);
    QMenu myMenu;
    QAction *actionPagar = new QAction(tr("Pagar vencimiento"),this);
    myMenu.addAction(actionPagar);
    QAction *actionFraccion = new QAction(tr("Pago parcial"),this);
    myMenu.addAction(actionFraccion);
    QAction *actionAsiento = new QAction(tr("Ver asiento contable"),this);
    myMenu.addAction(actionAsiento);

        connect(actionPagar, SIGNAL(triggered()), this, SLOT(pagar_deuda()));
        connect(actionFraccion,SIGNAL(triggered()),this,SLOT(pagar_fraccion()));
        connect(actionAsiento,SIGNAL(triggered()),this,SLOT(ver_asiento()));


     myMenu.exec(globalPos);
}

void frmProveedores::nuevo_contacto()
{
    oProveedor->anadir_persona_contacto(oProveedor->id,ui->txtNombre->text(),ui->txtDescripcionT1->text(),
                                   ui->txtTelefono1->text(),ui->txtDescripcionT2->text(),ui->txtTelefono2->text(),
                                   ui->txtDescripcionT3->text(),ui->txtTelefono3->text(),ui->txtDescripcionM1->text(),
                                   ui->txtMovil1->text(),ui->txtDescripcionM2->text(),ui->txtMovil2->text(),ui->txtCargo->text(),
                                        ui->txtemail_contacto->text());
    ui->txtDescripcionM1->setText("");
    ui->txtDescripcionM2->setText("");
    ui->txtDescripcionT1->setText("");
    ui->txtDescripcionT2->setText("");
    ui->txtDescripcionT3->setText("");
    ui->txtMovil1->setText("");
    ui->txtMovil2->setText("");
    ui->txtNombre->setText("");
    ui->txtTelefono1->setText("");
    ui->txtTelefono2->setText("");
    ui->txtTelefono3->setText("");
    ui->txtCargo->setText("");
    ui->txtemail_contacto->setText("");
    ui->txtNombre->setFocus();

    contactos();
}

void frmProveedores::guardar_contacto()
{
    oProveedor->guardar_persona_contacto(this->id_contacto,ui->txtNombre->text(),ui->txtDescripcionT1->text(),
                                   ui->txtTelefono1->text(),ui->txtDescripcionT2->text(),ui->txtTelefono2->text(),
                                   ui->txtDescripcionT3->text(),ui->txtTelefono3->text(),ui->txtDescripcionM1->text(),
                                   ui->txtMovil1->text(),ui->txtDescripcionM2->text(),ui->txtMovil2->text(),ui->txtCargo->text(),
                                   ui->txtemail_contacto->text());
    ui->txtDescripcionM1->setText("");
    ui->txtDescripcionM2->setText("");
    ui->txtDescripcionT1->setText("");
    ui->txtDescripcionT2->setText("");
    ui->txtDescripcionT3->setText("");
    ui->txtMovil1->setText("");
    ui->txtMovil2->setText("");
    ui->txtNombre->setText("");
    ui->txtTelefono1->setText("");
    ui->txtTelefono2->setText("");
    ui->txtTelefono3->setText("");
    ui->txtCargo->setText("");
    ui->txtemail_contacto->setText("");
    ui->txtNombre->setFocus();

    contactos();
    ui->btnGuardarContacto->setVisible(false);
    ui->txtDescripcionM1->setReadOnly(true);
    ui->txtDescripcionM2->setReadOnly(true);
    ui->txtDescripcionT1->setReadOnly(true);
    ui->txtDescripcionT2->setReadOnly(true);
    ui->txtDescripcionT3->setReadOnly(true);
    ui->txtMovil1->setReadOnly(true);
    ui->txtMovil2->setReadOnly(true);
    ui->txtNombre->setReadOnly(true);
    ui->txtTelefono1->setReadOnly(true);
    ui->txtTelefono2->setReadOnly(true);
    ui->txtTelefono3->setReadOnly(true);
    ui->txtCargo->setReadOnly(true);
    ui->txtemail->setReadOnly(true);
}

void frmProveedores::editar_contacto()

{
    QModelIndex index= ui->tablaContactos->currentIndex();
    int nid = ui->tablaContactos->model()->data(ui->tablaContactos->model()->index(index.row(),0),Qt::EditRole).toInt();
    this->id_contacto = nid;
    QSqlQuery queryContactos(QSqlDatabase::database("Maya"));
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

        ui->btnGuardarContacto->setVisible("true");
        ui->txtDescripcionM1->setReadOnly(false);
        ui->txtDescripcionM2->setReadOnly(false);
        ui->txtDescripcionT1->setReadOnly(false);
        ui->txtDescripcionT2->setReadOnly(false);
        ui->txtDescripcionT3->setReadOnly(false);
        ui->txtMovil1->setReadOnly(false);
        ui->txtMovil2->setReadOnly(false);
        ui->txtNombre->setReadOnly(false);
        ui->txtTelefono1->setReadOnly(false);
        ui->txtTelefono2->setReadOnly(false);
        ui->txtTelefono3->setReadOnly(false);
        ui->txtCargo->setReadOnly(false);
        ui->txtemail_contacto->setReadOnly(false);
        ui->txtNombre->setFocus();
    }
}

void frmProveedores::borrar_contacto()
{
    QModelIndex index= ui->tablaContactos->currentIndex();
    int nid = ui->tablaContactos->model()->data(ui->tablaContactos->model()->index(index.row(),0),Qt::EditRole).toInt();
    this->id_contacto = nid;
    QSqlQuery queryContactos(QSqlDatabase::database("Maya"));
    queryContactos.prepare("delete from personascontactoproveedor where id = :id");
    queryContactos.bindValue(":id",this->id_contacto);
    if (!queryContactos.exec())
    {
        QMessageBox::warning(this,tr("ATENCIÓN:"),
                             tr("Ocurrió un error al borrar el contacto: %1").arg(queryContactos.lastError().text()),
                             tr("Aceptar"));
    } else
    {
        contactos();
    }
}

void frmProveedores::on_btnBuscar_clicked()
{
    FrmBuscarProveedor frmBuscar(this);
    if(frmBuscar.exec() == QDialog::Accepted)
    {
        int id_pro = frmBuscar.nidProv;
        oProveedor->Recuperar("select * from proveedores where id = "+QString::number(id_pro));
        this->LLenarCampos();
    }
}

void frmProveedores::on_btnAnadirEntrega_clicked()
{
    frmAddEntregasCuenta frmEntregas(this);
    if(frmEntregas.exec() == QDialog::Accepted){
        EntregasCuenta oEntrega(this);
        if(!oEntrega.Anadir(2,oProveedor->id,frmEntregas.importe,frmEntregas.fecha,frmEntregas.concepto))
            QMessageBox::warning(this,tr("Gestión de proveedores"),
                                 tr("Falló el insertar una nueva entrega a cuenta"),
                                 tr("Aceptar"));
        else
            historiales();
    }

}

void frmProveedores::on_txtcif_editingFinished()
{
    blockSignals(true);
    if (ui->txtcif->text().length() == 8)
    {
        QString cif = Configuracion::letraDNI(ui->txtcif->text());
        ui->txtcif->setText(cif.toUpper());
    }
    blockSignals(false);
}
