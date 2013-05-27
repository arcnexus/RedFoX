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


#ifdef WIN32
//#define and && 4005401548072

#endif
Proveedor *oProveedor = new Proveedor();

frmProveedores::frmProveedores(QWidget *parent) :
    QDialog(parent),
  ui(new Ui::frmProveedores)



{
    ui->setupUi(this);
    oProveedor->id = 0;
    BloquearCampos();
    ui->txtdFechaAlta->setDate(QDate::currentDate());
    ui->txtdFechaUltimaCompra->setDate(QDate::currentDate());
    //---------------------------
    // cargar datos forma de pago.
    //---------------------------
    QSqlQueryModel *qmFormaPago = new QSqlQueryModel(this);
    qmFormaPago->setQuery("select cCodigo, cFormapago from FormPago",QSqlDatabase::database("Maya"));

    ui->txtcCodigoFormaPago->setModel(qmFormaPago);
    oProveedor->idFormadePago = Configuracion_global->Devolver_id_forma_pago(ui->txtcCodigoFormaPago->currentText());


    // ----------------
    // Cargar Paises
    // ----------------
    QSqlQueryModel * modelPais = new QSqlQueryModel(this);
    modelPais->setQuery("select pais from paises",QSqlDatabase::database("Maya"));
    ui->txtcPais->setModel(modelPais);
    oProveedor->idpais = Configuracion_global->Devolver_id_pais(ui->txtcPais->currentText());

    // ---------------------
    // Cargar Paises almacen
    // ---------------------
    QSqlQueryModel * modelPaisAlmacen = new QSqlQueryModel(this);
    modelPaisAlmacen->setQuery("select pais from paises",QSqlDatabase::database("Maya"));
    ui->txtcPaisAlmacen->setModel(modelPaisAlmacen);
    oProveedor->idPaisAlmacen = Configuracion_global->Devolver_id_pais(ui->txtcPaisAlmacen->currentText());


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
    ui->lblCodigoProveedor->setText(oProveedor->cCodigo);
    ui->lblproveedor->setText(oProveedor->cProveedor);
    ui->txtcCodigo->setText(oProveedor->cCodigo);
    ui->txtcProveedor->setText(oProveedor->cProveedor);
    ui->txtcCif->setText(oProveedor->cCif);
    ui->txtcDireccion1->setText(oProveedor->cDireccion1);
    ui->txtcDireccion2->setText(oProveedor->cDireccion2);
    ui->txtcCP->setText(oProveedor->cCP);
    ui->txtcPoblacion->setText(oProveedor->cPoblacion);
    ui->txtcProvincia->setText(oProveedor->cProvincia);
    int index = ui->txtcPais->findText(oProveedor->cPais);
    ui->txtcPais->setCurrentIndex(index);
    ui->txtcTelefono1->setText(oProveedor->cTelefono1);
    ui->txtcTelefono2->setText(oProveedor->cTelefono2);
    ui->txtcTelefono3->setText(oProveedor->cTelefono3);
    ui->txtcFax->setText(oProveedor->cFax);
    ui->txtcMovil->setText(oProveedor->cMovil);
    ui->txtcEmail->setText(oProveedor->cEMail);
    ui->txtcWeb->setText(oProveedor->cWeb);
    ui->txtcPersonaContacto->setText(oProveedor->cPersonaContacto);
    ui->txtnDiaCobro->setText(QString::number(oProveedor->nDiaCobro));
    ui->txtcDireccionAlmacen->setText(oProveedor->cDireccionAlmacen);
    ui->txtcCPAlmacen->setText(oProveedor->cCPAlmacen);
    ui->txtcPoblacionAlmacen->setText(oProveedor->cPoblacionAlmacen);
    ui->txtcProvinciaAmacen->setText(oProveedor->cProvinciaAlmacen);
    index = ui->txtcPaisAlmacen->findText(oProveedor->cPaisAlmacen);
    ui->txtcPaisAlmacen->setCurrentIndex(index);
    ui->txtcTelefonoAlmacen->setText(oProveedor->cTelefonoAlmacen);
    ui->txtcFaxAlmacen->setText(oProveedor->cFaxAlmacen);

    int nIndex = ui->txtcCodigoFormaPago->findText(oProveedor->cCodigoFormaPago);
    if (nIndex !=-1)
    {
        ui->txtcCodigoFormaPago->setCurrentIndex(nIndex);
        ui->txtcFormaPago->setText(Configuracion_global->Devolver_forma_pago(oProveedor->idFormadePago));

    } else
    {
        ui->txtcFormaPago->setText("");
    }

    ui->txtdFechaUltimaCompra->setDate(oProveedor->dFechaUltimaCompra);
    ui->txtrAcumuladoCompras->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->rAcumuladoCompras,'f',2)));
    ui->txtcEntidadBancariaProveedor->setText(oProveedor->cEntidadBancariaProveedor);
    ui->txtcOficinaBancariaProveedor->setText(oProveedor->cOficinaBancariaProveedor);
    ui->txtcDCProveedor->setText(oProveedor->cDCProveedor);
    ui->txtcCCProveedor->setText(oProveedor->cCCProveedor);
    ui->txtcEntidadPagoProveedor->setText(oProveedor->cEntidadPagoProveedor);
    ui->txtcOficinaPagoProveedor->setText(oProveedor->cOficinaPagoProveedor);
    ui->txtcDCPagoProveedor->setText(oProveedor->cDCPagoProveedor);

    ui->txtrRetencionIRPF->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->rRetencionIRPF,'f',2)));
    ui->txtnTipoRetencionIRPF->setText(QString::number(oProveedor->nTipoRetencion));
    ui->txtcCuentaAplicacion->setText(oProveedor->cCuentaAplicacion);
    ui->txttComentarios->setText(oProveedor->tComentarios);
    ui->txtnDto->setText(QString::number(oProveedor->nDto));
    ui->txtdFechaAlta->setDate(oProveedor->dFechaAlta);
    ui->txtrDeudaMaxima->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->rDeudaMaxima,'f',2)));
    ui->txtrDeudaActual->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->rDeudaActual,'f',2)));
    if (oProveedor->lRecargoEquivalencia==1)
        ui->chklRecargoEquivalencia->setChecked(true);
    else
        ui->chklRecargoEquivalencia->setChecked(false);
    ui->txttTextoparaPedidos->setPlainText(oProveedor->tTextoparaPedidos);

    QString cOk;
    if(!ui->txtcEntidadPagoProveedor->text().isEmpty() && !ui->txtcOficinaPagoProveedor->text().isEmpty() &&
            !ui->txtcDCPagoProveedor->text().isEmpty() && !ui->txtcCuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtcEntidadPagoProveedor->text(),ui->txtcOficinaPagoProveedor->text(),
                                              ui->txtcDCPagoProveedor->text(),ui->txtcCuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
    if(!ui->txtcEntidadBancariaProveedor->text().isEmpty() && !ui->txtcOficinaBancariaProveedor->text().isEmpty() &&
            !ui->txtcDCProveedor->text().isEmpty() && !ui->txtcCCProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtcEntidadBancariaProveedor->text(),ui->txtcOficinaBancariaProveedor->text(),
                               ui->txtcDCProveedor->text(),ui->txtcCCProveedor->text());
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
    oProveedor->cCodigo = ui->txtcCodigo->text();
    oProveedor->cProveedor =ui->txtcProveedor->text();
    oProveedor->cCif = ui->txtcCif->text();
    oProveedor->cDireccion1 = ui->txtcDireccion1->text();
    oProveedor->cDireccion2 = ui->txtcDireccion2->text();
    oProveedor->cCP = ui->txtcCP->text();
    oProveedor->cPoblacion = ui->txtcPoblacion->text();
    oProveedor->cProvincia = ui->txtcProvincia->text();
    oProveedor->cPais = ui->txtcPais->currentText();
    oProveedor->cTelefono1 = ui->txtcTelefono1->text();
    oProveedor->cTelefono2 = ui->txtcTelefono2->text();
    oProveedor->cTelefono3 = ui->txtcTelefono3->text();
    oProveedor->cFax = ui->txtcFax->text();
    oProveedor->cMovil = ui->txtcMovil->text();
    oProveedor->cEMail = ui->txtcEmail->text();
    oProveedor->cWeb = ui->txtcWeb->text();
    oProveedor->cPersonaContacto = ui->txtcPersonaContacto->text();
    oProveedor->nDiaCobro = ui->txtnDiaCobro->text().toInt();
    oProveedor->cDireccionAlmacen = ui->txtcDireccionAlmacen->text();
    oProveedor->cCPAlmacen = ui->txtcCPAlmacen->text();
    oProveedor->cPoblacionAlmacen = ui->txtcPoblacionAlmacen->text();
    oProveedor->cProvinciaAlmacen = ui->txtcProvinciaAmacen->text();
    oProveedor->cPaisAlmacen = ui->txtcPaisAlmacen->currentText();
    oProveedor->cTelefonoAlmacen = ui->txtcTelefonoAlmacen->text();
    oProveedor->cFaxAlmacen = ui->txtcFaxAlmacen->text();
    oProveedor->cCodigoFormaPago = ui->txtcCodigoFormaPago->currentText();
    oProveedor->dFechaUltimaCompra = ui->txtdFechaUltimaCompra->date();
    oProveedor->rAcumuladoCompras = ui->txtrAcumuladoCompras->text().replace(".","").toDouble();
    oProveedor->cEntidadBancariaProveedor = ui->txtcEntidadBancariaProveedor->text();
    oProveedor->cOficinaBancariaProveedor = ui->txtcOficinaBancariaProveedor->text();
    oProveedor->cDCProveedor = ui->txtcDCProveedor->text();
    oProveedor->cCCProveedor = ui->txtcCCProveedor->text();
    oProveedor->cEntidadPagoProveedor = ui->txtcEntidadPagoProveedor->text();
    oProveedor->cOficinaPagoProveedor = ui->txtcOficinaPagoProveedor->text();
    oProveedor->cDCPagoProveedor = ui->txtcDCPagoProveedor->text();
    oProveedor->rRetencionIRPF = ui->txtrRetencionIRPF->text().replace(".","").toDouble();
    oProveedor->nTipoRetencion = ui->txtnTipoRetencionIRPF->text().toInt();
    oProveedor->cCuentaAplicacion = ui->txtcCuentaAplicacion->text();
    oProveedor->tComentarios = ui->txttComentarios->toPlainText();
    oProveedor->nDto = ui->txtnDto->text().replace(".","").toDouble();
    oProveedor->dFechaAlta = ui->txtdFechaAlta->date();
    oProveedor->rDeudaMaxima = ui->txtrDeudaMaxima->text().replace(".","").toDouble();
    oProveedor->rDeudaActual = ui->txtrDeudaActual->text().replace(".","").toDouble();
    oProveedor->lRecargoEquivalencia = ui->chklRecargoEquivalencia->isChecked();
    oProveedor->tTextoparaPedidos = ui->txttTextoparaPedidos->toPlainText();
    oProveedor->rEntregadoaCuenta = ui->txtrEntregadoaCuenta->text().replace(".","").toDouble();
    cargar_forma_pago(ui->txtcCodigoFormaPago->currentText());

}

void frmProveedores::cargar_forma_pago(QString codigo)
{
    int id_forma_pago = Configuracion_global->Devolver_id_codigo_forma_pago(codigo);
    ui->txtcFormaPago->setText(Configuracion_global->Devolver_forma_pago(id_forma_pago));
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
    oProveedor->Recuperar("Select * from proveedores where id<"+QString::number(oProveedor->id)+" order by id desc",1);
    LLenarCampos();
}

void frmProveedores::on_btnGuardar_clicked()
{
    bool lGuardar = true;
    QString cTexto;
    cTexto = tr("Hay campos que debe rellenar para poder guardar")+"\n";
    if (oProveedor->idFormadePago == 0)
    {
        lGuardar = false;
        cTexto = cTexto + tr("Debe elegir una forma de pago")+"\n";
    }

    if (oProveedor->idpais == 0)
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
    ui->txtcCodigo->setFocus();
}

void frmProveedores::on_btnAnadir_clicked()
{
    oProveedor->Vaciar();
    oProveedor->Anadir();
    LLenarCampos();
    DesbloquerCampos();
    ui->txtcCodigo->setFocus();
}


void frmProveedores::on_txtcPoblacion_editingFinished()
{
    ui->txtcPoblacion->setText(ui->txtcPoblacion->text().toUpper());
    if (ui->txtcCP->text().isEmpty() && !ui->txtcPoblacion->text().isEmpty() && !ui->txtcCP->isReadOnly()) {
        FrmBuscarPoblacion BuscarPoblacion;

        //Configuracion_global->CargarDatos();
        BuscarPoblacion.setcPoblacion(ui->txtcPoblacion->text(),1);
        if(BuscarPoblacion.exec()) {
            //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
            int nId = BuscarPoblacion.DevolverID();

            if(nId > 0) {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cId;
                cId = QString::number(nId);
                qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
                qPoblacion.bindValue(":id",cId);
                if(!qPoblacion.exec()) {
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                } else {
                    if (qPoblacion.next()) {
                        ui->txtcPoblacion->setText(qPoblacion.value(0).toString());
                        ui->txtcCP->setText(qPoblacion.value(1).toString());
                        ui->txtcProvincia->setText(qPoblacion.value(2).toString());
                        int index = ui->txtcPais->findText(Configuracion_global->cPais);
                        ui->txtcPais->setCurrentIndex(index);
                    }
                }

            }
        }
    }
    ui->txtcTelefono1->setFocus();

}

void frmProveedores::on_txtcCP_editingFinished()
{
    if (!ui->txtcCP->text().isEmpty() && ui->txtcPoblacion->text().isEmpty() && !ui->txtcCP->isReadOnly()){
        FrmBuscarPoblacion BuscarPoblacion;

        BuscarPoblacion.setcPoblacion(ui->txtcCP->text(),0);
        if(BuscarPoblacion.exec()) {
            int nId = BuscarPoblacion.DevolverID();
            if(nId > 0) {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cId;
                cId = QString::number(nId);
                qPoblacion.prepare("select  poblacion,  cp, provincia from poblaciones where id = :cId");
                qPoblacion.bindValue(":id",cId);
                if(!qPoblacion.exec()) {
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                } else {
                    if (qPoblacion.next()) {
                        ui->txtcPoblacion->setText(qPoblacion.record().value("poblacion").toString());
                        ui->txtcProvincia->setText(qPoblacion.record().value("provincia").toString());
                        int index = ui->txtcPais->findText(Configuracion_global->cPais);
                        ui->txtcPais->setCurrentIndex(index);
                        ui->txtcTelefono1->setFocus();
                    }
                }

            }
        }
    }

}






void frmProveedores::on_btnBorrar_clicked()
{
    int ndev = QMessageBox::question(NULL,tr("Gestión de proveedores"),tr("Está seguro/a de borrar el proveedor?\n"
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

void frmProveedores::on_txtcEntidadBancariaProveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtcEntidadBancariaProveedor->text().isEmpty() && !ui->txtcOficinaBancariaProveedor->text().isEmpty() &&
            !ui->txtcDCProveedor->text().isEmpty() && !ui->txtcCCProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtcEntidadBancariaProveedor->text(),ui->txtcOficinaBancariaProveedor->text(),
                                 ui->txtcDCProveedor->text(),ui->txtcCCProveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));
}

void frmProveedores::on_txtcOficinaBancariaProveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtcEntidadBancariaProveedor->text().isEmpty() && !ui->txtcOficinaBancariaProveedor->text().isEmpty() &&
            !ui->txtcDCProveedor->text().isEmpty() && !ui->txtcCCProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtcEntidadBancariaProveedor->text(),ui->txtcOficinaBancariaProveedor->text(),
                                 ui->txtcDCProveedor->text(),ui->txtcCCProveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtcDCProveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtcEntidadBancariaProveedor->text().isEmpty() && !ui->txtcOficinaBancariaProveedor->text().isEmpty() &&
            !ui->txtcDCProveedor->text().isEmpty() && !ui->txtcCCProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtcEntidadBancariaProveedor->text(),ui->txtcOficinaBancariaProveedor->text(),
                                 ui->txtcDCProveedor->text(),ui->txtcCCProveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));
}



void frmProveedores::on_txtcCCProveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtcEntidadBancariaProveedor->text().isEmpty() && !ui->txtcOficinaBancariaProveedor->text().isEmpty() &&
            !ui->txtcDCProveedor->text().isEmpty() && !ui->txtcCCProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtcEntidadBancariaProveedor->text(),ui->txtcOficinaBancariaProveedor->text(),
                                 ui->txtcDCProveedor->text(),ui->txtcCCProveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtcEntidadPagoProveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtcEntidadPagoProveedor->text().isEmpty() && !ui->txtcOficinaPagoProveedor->text().isEmpty() &&
            !ui->txtcDCPagoProveedor->text().isEmpty() && !ui->txtcCuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtcEntidadPagoProveedor->text(),ui->txtcOficinaPagoProveedor->text(),
                                 ui->txtcDCPagoProveedor->text(),ui->txtcCuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtcOficinaPagoProveedor_editingFinished()
{

    QString cOk;
    if(!ui->txtcEntidadPagoProveedor->text().isEmpty() && !ui->txtcOficinaPagoProveedor->text().isEmpty() &&
            !ui->txtcDCPagoProveedor->text().isEmpty() && !ui->txtcCuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtcEntidadPagoProveedor->text(),ui->txtcOficinaPagoProveedor->text(),
                                 ui->txtcDCPagoProveedor->text(),ui->txtcCuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::on_txtcDCPagoProveedor_editingFinished()
{
    QString cOk;
    if(!ui->txtcEntidadPagoProveedor->text().isEmpty() && !ui->txtcOficinaPagoProveedor->text().isEmpty() &&
            !ui->txtcDCPagoProveedor->text().isEmpty() && !ui->txtcCuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtcEntidadPagoProveedor->text(),ui->txtcOficinaPagoProveedor->text(),
                                 ui->txtcDCPagoProveedor->text(),ui->txtcCuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
}

void frmProveedores::on_txtcCuentaPagoProveedor_editingFinished()
{
    QString cOk;
    if(!ui->txtcEntidadPagoProveedor->text().isEmpty() && !ui->txtcOficinaPagoProveedor->text().isEmpty() &&
            !ui->txtcDCPagoProveedor->text().isEmpty() && !ui->txtcCuentaPagoProveedor->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtcEntidadPagoProveedor->text(),ui->txtcOficinaPagoProveedor->text(),
                                 ui->txtcDCPagoProveedor->text(),ui->txtcCuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
}

void frmProveedores::on_txtcCPAlmacen_editingFinished()
{
    if (!ui->txtcCPAlmacen->isReadOnly()) {
        if (!ui->txtcCPAlmacen->text().isEmpty() && ui->txtcPoblacionAlmacen->text().isEmpty()) {
            FrmBuscarPoblacion BuscarPoblacion;

            //Configuracion_global->CargarDatos();
            BuscarPoblacion.setcPoblacion(ui->txtcCPAlmacen->text(),0);
            if(BuscarPoblacion.exec()) {
                int nId = BuscarPoblacion.DevolverID();
                if(nId > 0) {
                    QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                    QString cId;
                    cId = QString::number(nId);
                    qPoblacion.prepare("select poblacion, cp, provincia from poblaciones where id = :cId");
                    qPoblacion.bindValue(":id",cId);
                    if(!qPoblacion.exec()) {
                        QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                    } else {
                        if (qPoblacion.next()) {
                            ui->txtcPoblacionAlmacen->setText(qPoblacion.value(0).toString());
                            ui->txtcProvinciaAmacen->setText(qPoblacion.value(2).toString());
                            int index = ui->txtcPaisAlmacen->findText(Configuracion_global->cPais);
                            ui->txtcPaisAlmacen->setCurrentIndex(index);
                        }
                    }

                }
            }
        }
        ui->txtcTelefonoAlmacen->setFocus();
    }
}

void frmProveedores::on_txtcPais_currentIndexChanged(const QString &arg1)
{
    QString cPais = arg1;
   oProveedor->idpais = Configuracion_global->Devolver_id_pais(cPais);
}


void frmProveedores::on_txtcCodigoFormaPago_currentIndexChanged(const QString &arg1)
{
    QString cCodigo = arg1;
    oProveedor->idFormadePago = Configuracion_global->Devolver_id_codigo_forma_pago(cCodigo);
}


void frmProveedores::on_btnNuevaFactura_clicked()
{
    QMessageBox::information(this,tr("Nueva Factura"),
                             tr("Para volver a proveedores cierre la ventana\n desde el boton X de la ventana de Facturas"),
                             tr("Aceptar"));
    FrmFacturasProveedor frmFactura(this);
    frmFactura.setWindowState(Qt::WindowMaximized);
    frmFactura.exec();
}

void frmProveedores::on_btnNuevoAlbaran_clicked()
{
    QMessageBox::information(this,tr("Nuevo Albarán"),
                             tr("Para volver a proveedores cierre la ventana\n desde el boton X de la ventana de Albaranes"),
                             tr("Aceptar"));
    FrmAlbaranProveedor frmAlbaran(this);
    frmAlbaran.setWindowState(Qt::WindowMaximized);
    frmAlbaran.exec();
}

void frmProveedores::on_btnNuevoPedido_clicked()
{
    QMessageBox::information(this,tr("Nuevo Pedido"),
                             tr("Para volver a proveedores cierre la ventana\n desde el boton X de la ventana de Pedidos"),
                             tr("Aceptar"));
    FrmPedidosProveedor frmPedidos(this);
    frmPedidos.setWindowState(Qt::WindowMaximized);
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
    modelPedidos->setQuery("select id,nPedido,dFecha,rTotal,lRecibidoCompleto from ped_pro where id_Proveedor ="+
                           QString::number(oProveedor->id)+" order by nPedido desc",QSqlDatabase::database("empresa"));
    ui->tablaColumnasPedidos->setModel(modelPedidos);
    ui->tablaColumnasPedidos->setColumnHidden(0,true);
    ui->tablaColumnasPedidos->setColumnWidth(1,120);
    ui->tablaColumnasPedidos->setColumnWidth(1,120);
    ui->tablaColumnasPedidos->setColumnWidth(3,120);
    modelPedidos->setHeaderData(1,Qt::Horizontal,"PEDIDO");
    ui->tablaColumnasPedidos->setItemDelegateForColumn(2, new DateDelegate);
    modelPedidos->setHeaderData(2,Qt::Horizontal,"Fecha");
    ui->tablaColumnasPedidos->setItemDelegateForColumn(3,new MonetaryDelegate);
    modelPedidos->setHeaderData(3,Qt::Horizontal,"Total");
    modelPedidos->setHeaderData(4,Qt::Horizontal,"C.");


    // -------------------------
    // Cargar Historial Facturas
    //--------------------------
    QSqlQueryModel *modelFacturas = new QSqlQueryModel(this);
    modelFacturas->setQuery("select id,cFactura,dFecha,cPedido,rTotalBase,rTotalIva,rTotalRetencion,rTotalRecargoEq,rTotal,rImporteDeudaPendiente"
                            " from fac_pro where id_Proveedor = "+QString::number(oProveedor->id)+
                            " order by cFactura desc",QSqlDatabase::database("empresa"));
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
    modelAlbaranes->setQuery("select id,cAlbaran,dFecha,rBaseTotal,rIvaTotal,"
                             " rTotal from alb_pro where id_Proveedor = "+QString::number(oProveedor->id)+
                             " order by cAlbaran desc ",QSqlDatabase::database("empresa"));
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
    modelArticulo->setQuery("select cCodigo,cCodigoBarras, cDescripcion,rCoste "
                           "from articulos where id_Proveedor =" +QString::number(oProveedor->id),
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

    modelAsientos->setQuery("select id,asiento,fechaAsiento,cuentaD,descripcionD, importeD,cuentaH,descripcionH,importeH "
                            "from diario where cta_principal = '"+oProveedor->cCuentaAplicacion +
                            "' order by asiento + ' '+posenasiento",
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


}

void frmProveedores::acumulados()
{
    //----------------------
    // Acumulados
    //----------------------
    ui->txtEnero->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->enero,'f',2)));
    ui->txtFebrero->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->febrero,'f',2)));
    ui->txtMarzo->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->marzo,'f',2)));
    ui->txtAbril->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->abril,'f',2)));
    ui->txtMayo->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->mayo,'f',2)));
    ui->txtJunio->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->junio,'f',2)));
    ui->txtjulio->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->julio,'f',2)));
    ui->txtAgosto->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->agosto,'f',2)));
    ui->txtSeptiembre->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->septiembre,'f',2)));
    ui->txtOctubre->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->octubre,'f',2)));
    ui->txtNoviembre->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->noviembre,'f',2)));
    ui->txtDiciembre->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->diciembre,'f',2)));

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
    modelContactos->setQuery("select id,idproveedor,cargo_empresa, nombre, desctelefono1,telefono1,desctelefono2,telefono2,"
                             "desctelefono3, telefono3, descmovil1,movil1,descmovil2,movil2 "
                             "from personascontactoproveedor where idproveedor ="+QString::number(oProveedor->id),
                             QSqlDatabase::database("Maya"));

    ui->tablaContactos->setModel(modelContactos);

    modelContactos->setHeaderData(0,Qt::Horizontal,tr("ID"));
    modelContactos->setHeaderData(1,Qt::Horizontal,tr("IDPROVEEDOR"));
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
   myMenu.addAction(actionEditar);

       connect(actionEditar, SIGNAL(triggered()), this, SLOT(editar_contacto()));

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
                                   ui->txtMovil1->text(),ui->txtDescripcionM2->text(),ui->txtMovil2->text(),ui->txtCargo->text());
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
    ui->txtNombre->setFocus();

    contactos();
}

void frmProveedores::guardar_contacto()
{
    oProveedor->guardar_persona_contacto(this->id_contacto,ui->txtNombre->text(),ui->txtDescripcionT1->text(),
                                   ui->txtTelefono1->text(),ui->txtDescripcionT2->text(),ui->txtTelefono2->text(),
                                   ui->txtDescripcionT3->text(),ui->txtTelefono3->text(),ui->txtDescripcionM1->text(),
                                   ui->txtMovil1->text(),ui->txtDescripcionM2->text(),ui->txtMovil2->text(),ui->txtCargo->text());
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
}

void frmProveedores::editar_contacto()

{
    QModelIndex index= ui->tablaContactos->currentIndex();
    int nId = ui->tablaContactos->model()->data(ui->tablaContactos->model()->index(index.row(),0),Qt::EditRole).toInt();
    this->id_contacto = nId;
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
        ui->txtDescripcionT1->setText(queryContactos.record().value("desctelefono1").toString());
        ui->txtDescripcionT2->setText(queryContactos.record().value("desctelefono2").toString());
        ui->txtDescripcionT3->setText(queryContactos.record().value("desctelefono3").toString());
        ui->txtDescripcionM1->setText(queryContactos.record().value("descmovil1").toString());
        ui->txtDescripcionM2->setText(queryContactos.record().value("descmovil2").toString());
        ui->txtcTelefono1->setText(queryContactos.record().value("telefono1").toString());
        ui->txtcTelefono2->setText(queryContactos.record().value("telefono2").toString());
        ui->txtcTelefono3->setText(queryContactos.record().value("telefono3").toString());
        ui->txtMovil1->setText(queryContactos.record().value("movil1").toString());
        ui->txtMovil2->setText(queryContactos.record().value("movil2").toString());

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
        ui->txtNombre->setFocus();
    }
}

void frmProveedores::borrar_contacto()
{
}

void frmProveedores::on_btnBuscar_clicked()
{
    FrmBuscarProveedor frmBuscar(this);
    if(frmBuscar.exec() == QDialog::Accepted)
    {
        int id_pro = frmBuscar.nIdProv;
        oProveedor->Recuperar("select * from proveedores where id = "+QString::number(id_pro));
        this->LLenarCampos();
    }
}
