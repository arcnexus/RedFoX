#include "frmproveedores.h"
#include "ui_frmproveedores.h"
#include "proveedor.h"
#include "configuracion.h"
#include "frmbuscarpoblacion.h"
#include <QDebug>

Proveedor *oProveedor = new Proveedor();

frmProveedores::frmProveedores(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmProveedores)
{
    ui->setupUi(this);
    oProveedor->setId(0);
    BloquearCampos();
    ui->txtdFechaAlta->setDate(QDate::currentDate());
    ui->txtdFechaUltimaCompra->setDate(QDate::currentDate());
    // cargar datos forma de pago.
    QSqlQueryModel *qmFormaPago = new QSqlQueryModel(this);
    qmFormaPago->setQuery("select cCodigo, cFormapago from FormPago",QSqlDatabase::database("empresa"));

    ui->txtcCodigoFormaPago->setModel(qmFormaPago);

    //-------------------------
    // CONEXIONES
    //-------------------------
    connect(ui->btnCerrar,SIGNAL(clicked()),this,SLOT(close()));

}

frmProveedores::~frmProveedores()
{
    delete ui;
}
void frmProveedores::LLenarCampos()
{
    Configuracion *oConf = new Configuracion();
    ui->txtcCodigo->setText(oProveedor->getcCodigo());
    ui->txtcProveedor->setText(oProveedor->getcProveedor());
    ui->txtcCif->setText(oProveedor->getcCif());
    ui->txtcDireccion1->setText(oProveedor->getcDireccion1());
    ui->txtcDireccion2->setText(oProveedor->getcDireccion2());
    ui->txtcCP->setText(oProveedor->getcCP());
    ui->txtcPoblacion->setText(oProveedor->getcPoblacion());
    ui->txtcProvincia->setText(oProveedor->getcProvincia());
    ui->txtcPais->setText(oProveedor->getcPais());
    ui->txtcTelefono1->setText(oProveedor->getcTelefono1());
    ui->txtcTelefono2->setText(oProveedor->getcTelefono2());
    ui->txtcTelefono3->setText(oProveedor->getcTelefono3());
    ui->txtcFax->setText(oProveedor->getcFax());
    ui->txtcMovil->setText(oProveedor->getcMovil());
    ui->txtcEmail->setText(oProveedor->getcEMail());
    ui->txtcWeb->setText(oProveedor->getcWeb());
    ui->txtcPersonaContacto->setText(oProveedor->getcPersonaContacto());
    ui->txtnDiaCobro->setText(QString::number(oProveedor->getnDiaCobro()));
    ui->txtcDireccionAlmacen->setText(oProveedor->getcDireccionAlmacen());
    ui->txtcCPAlmacen->setText(oProveedor->getcCPAlmacen());
    ui->txtcPoblacionAlmacen->setText(oProveedor->getcPoblacionAlmacen());
    ui->txtcProvinciaAmacen->setText(oProveedor->getcProvinciaAlmacen());
    ui->txtcPaisAlmacen->setText(oProveedor->getcPaisAlmacen());
    ui->txtcTelefonoAlmacen->setText(oProveedor->getcTelefonoAlmacen());
    ui->txtcFaxAlmacen->setText(oProveedor->getcFaxAlmacen());
    int nIndex = ui->txtcCodigoFormaPago->findText(oProveedor->getcCodigoFormaPago());
    if (nIndex !=-1)
        ui->txtcCodigoFormaPago->setCurrentIndex(nIndex);

    ui->txtdFechaUltimaCompra->setDate(oProveedor->getdFechaUltimaCompra());
    ui->txtrAcumuladoCompras->setText(oConf->FormatoNumerico(QString::number(oProveedor->getrAcumuladoCompras(),'f',2)));
    ui->txtcEntidadBancariaProveedor->setText(oProveedor->getcEntidadBancariaProveedor());
    ui->txtcOficinaBancariaProveedor->setText(oProveedor->getcOficinaBancariaProveedor());
    ui->txtcDCProveedor->setText(oProveedor->getcDCProveedor());
    ui->txtcCCProveedor->setText(oProveedor->getcCCProveedor());
    ui->txtcEntidadPagoProveedor->setText(oProveedor->getcEntidadPagoProveedor());
    ui->txtcOficinaPagoProveedor->setText(oProveedor->getcOficinaPagoProveedor());
    ui->txtcDCPagoProveedor->setText(oProveedor->getcDCPagoProveedor());
    ui->txtcCuentaPagoProveedor->setText(oProveedor->getcCuentaPagoProveedor());
    ui->txtcCuentaIvaSoportado->setText(oProveedor->getcCuentaIvaSoportado());
    ui->txtrRetencionIRPF->setText(oConf->FormatoNumerico(QString::number(oProveedor->getrRetencionIRPF(),'f',2)));
    ui->txtnTipoRetencionIRPF->setText(QString::number(oProveedor->getnTipoRetencion()));
    ui->txtcCuentaAplicacion->setText(oProveedor->getcCuentaAplicacion());
    ui->txtcCuentaPagos->setText(oProveedor->getcCuentaPagos());
    ui->txttComentarios->setText(oProveedor->gettComentarios());
    ui->txtnDto->setText(QString::number(oProveedor->getnDto()));
    ui->txtdFechaAlta->setDate(oProveedor->getdFechaAlta());
    ui->txtrDeudaMaxima->setText(oConf->FormatoNumerico(QString::number(oProveedor->getrDeudaMaxima(),'f',2)));
    ui->txtrDeudaActual->setText(oConf->FormatoNumerico(QString::number(oProveedor->getrDeudaActual(),'f',2)));
    if (oProveedor->getlRecargoEquivalencia()==1)
        ui->chklRecargoEquivalencia->setChecked(true);
    else
        ui->chklRecargoEquivalencia->setChecked(false);
    ui->txttTextoparaPedidos->setPlainText(oProveedor->gettTextoparaPedidos());

    QString cOk;
    if(!ui->txtcEntidadPagoProveedor->text().isEmpty() && !ui->txtcOficinaPagoProveedor->text().isEmpty() &&
            !ui->txtcDCPagoProveedor->text().isEmpty() && !ui->txtcCuentaPagoProveedor->text().isEmpty())
        cOk = oConf->ValidarCC(ui->txtcEntidadPagoProveedor->text(),ui->txtcOficinaPagoProveedor->text(),
                              ui->txtcDCPagoProveedor->text(),ui->txtcCuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
    if(!ui->txtcEntidadBancariaProveedor->text().isEmpty() && !ui->txtcOficinaBancariaProveedor->text().isEmpty() &&
            !ui->txtcDCProveedor->text().isEmpty() && !ui->txtcCCProveedor->text().isEmpty())
        cOk = oConf->ValidarCC(ui->txtcEntidadBancariaProveedor->text(),ui->txtcOficinaBancariaProveedor->text(),
                              ui->txtcDCProveedor->text(),ui->txtcCCProveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));

}

void frmProveedores::CargarCamposEnProveedor()
{
    oProveedor->setcCodigo(ui->txtcCodigo->text());
    oProveedor->setcProveedor(ui->txtcProveedor->text());
    oProveedor->setcCif(ui->txtcCif->text());
    oProveedor->setcDireccion1(ui->txtcDireccion1->text());
    oProveedor->setcDireccion2(ui->txtcDireccion2->text());
    oProveedor->setcCP(ui->txtcCP->text());
    oProveedor->setcPoblacion(ui->txtcPoblacion->text());
    oProveedor->setcProvincia(ui->txtcProvincia->text());
    oProveedor->setcPais(ui->txtcPais->text());
    oProveedor->setcTelefono1(ui->txtcTelefono1->text());
    oProveedor->setcTelefono2(ui->txtcTelefono2->text());
    oProveedor->setcTelefono3(ui->txtcTelefono3->text());
    oProveedor->setcFax(ui->txtcFax->text());
    oProveedor->setcMovil(ui->txtcMovil->text());
    oProveedor->setcEMail(ui->txtcEmail->text());
    oProveedor->setcWeb(ui->txtcWeb->text());
    oProveedor->setcPersonaContacto(ui->txtcPersonaContacto->text());
    oProveedor->setnDiaCobro(ui->txtnDiaCobro->text().toInt());
    oProveedor->setcDireccionAlmacen(ui->txtcDireccionAlmacen->text());
    oProveedor->setcCPAlmacen(ui->txtcCPAlmacen->text());
    oProveedor->setcPoblacionAlmacen(ui->txtcPoblacionAlmacen->text());
    oProveedor->setcProvinciaAlmacen(ui->txtcProvinciaAmacen->text());
    oProveedor->setcPaisAlmacen(ui->txtcPaisAlmacen->text());
    oProveedor->setcTelefonoAlmacen(ui->txtcTelefonoAlmacen->text());
    oProveedor->setcFaxAlmacen(ui->txtcFaxAlmacen->text());
    oProveedor->setcCodigoFormaPago(ui->txtcCodigoFormaPago->currentText());
    oProveedor->setdFechaUltimaCompra(ui->txtdFechaUltimaCompra->date());
    oProveedor->setrAcumuladoCompras(ui->txtrAcumuladoCompras->text().replace(".","").toDouble());
    oProveedor->setcEntidadBancariaProveedor(ui->txtcEntidadBancariaProveedor->text());
    oProveedor->setcOficinaBancariaProveedor(ui->txtcOficinaBancariaProveedor->text());
    oProveedor->setcDCProveedor(ui->txtcDCProveedor->text());
    oProveedor->setcCCProveedor(ui->txtcCCProveedor->text());
    oProveedor->setcEntidadPagoPro(ui->txtcEntidadPagoProveedor->text());
    oProveedor->setcOficinaPagoProveedor(ui->txtcOficinaPagoProveedor->text());
    oProveedor->setcDCPagoProveedor(ui->txtcDCPagoProveedor->text());
    oProveedor->setcCuentaPagoProveedor(ui->txtcCuentaPagoProveedor->text());
    oProveedor->setcCuentaIvaSoportado(ui->txtcCuentaIvaSoportado->text());
    oProveedor->setrRetencion(ui->txtrRetencionIRPF->text().replace(".","").toDouble());
    oProveedor->setnTipoRetencion(ui->txtnTipoRetencionIRPF->text().toInt());
    oProveedor->setcCuentaAplicacion(ui->txtcCuentaAplicacion->text());
    oProveedor->setcCuentaPagos(ui->txtcCuentaPagos->text());
    oProveedor->settComentarios(ui->txttComentarios->toPlainText());
    oProveedor->setnDto(ui->txtnDto->text().replace(".","").toDouble());
    oProveedor->setdFechaAlta(ui->txtdFechaAlta->date());
    oProveedor->setrDeudaMaxima(ui->txtrDeudaMaxima->text().replace(".","").toDouble());
    oProveedor->setrDeudaActual(ui->txtrDeudaActual->text().replace(".","").toDouble());
    oProveedor->setlRecargoEquivalencia(ui->chklRecargoEquivalencia->isChecked());
    oProveedor->settTextoparaPedidos(ui->txttTextoparaPedidos->toPlainText());
    oProveedor->setrEntregadoaCuenta(ui->txtrEntregadoaCuenta->text().replace(".","").toDouble());

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
    ui->btnCerrar->setEnabled(false);
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
    ui->btnCerrar->setEnabled(true);
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
    CargarCamposEnProveedor();
    oProveedor->Guardar();
    BloquearCampos();
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
    if (ui->txtcCP->text().isEmpty() and !ui->txtcPoblacion->text().isEmpty() and !ui->txtcCP->isReadOnly()) {
        FrmBuscarPoblacion BuscarPoblacion;
        Configuracion *oConfig = new Configuracion();
        oConfig->CargarDatos();
        BuscarPoblacion.setcPoblacion(ui->txtcPoblacion->text(),1);
       if(BuscarPoblacion.exec()) {
        //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
           int nId = BuscarPoblacion.DevolverID();

         if(nId > 0) {
               QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
               QString cId;
               cId = QString::number(nId);
               qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
               qPoblacion.bindValue(":id",cId);
               if(!qPoblacion.exec()) {
                   QMessageBox::critical(NULL,tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
               } else {
                   if (qPoblacion.next()) {
                        ui->txtcPoblacion->setText(qPoblacion.value(0).toString());
                        ui->txtcCP->setText(qPoblacion.value(1).toString());
                        ui->txtcProvincia->setText(qPoblacion.value(2).toString());
                        ui->txtcPais->setText(oConfig->cPais);
                   }
               }

           }
       }
    }
    ui->txtcTelefono1->setFocus();

}

void frmProveedores::on_txtcCP_editingFinished()
{
    if (!ui->txtcCP->text().isEmpty() and ui->txtcPoblacion->text().isEmpty() and !ui->txtcCP->isReadOnly()){
        FrmBuscarPoblacion BuscarPoblacion;
        Configuracion *oConfig = new Configuracion();
        oConfig->CargarDatos();
        BuscarPoblacion.setcPoblacion(ui->txtcCP->text(),0);
       if(BuscarPoblacion.exec()) {
           int nId = BuscarPoblacion.DevolverID();
         if(nId > 0) {
               QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
               QString cId;
               cId = QString::number(nId);
               qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
               qPoblacion.bindValue(":id",cId);
               if(!qPoblacion.exec()) {
                   QMessageBox::critical(NULL,tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
               } else {
                   if (qPoblacion.next()) {
                        ui->txtcPoblacion->setText(qPoblacion.value(0).toString());
                        ui->txtcProvincia->setText(qPoblacion.value(2).toString());
                        ui->txtcPais->setText(oConfig->cPais);
                   }
               }

           }
    delete oConfig;
       }
      }
    ui->txtcTelefono1->setFocus();
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
    Configuracion *oConfig = new Configuracion();
    QString cOk;
    if(!ui->txtcEntidadBancariaProveedor->text().isEmpty() && !ui->txtcOficinaBancariaProveedor->text().isEmpty() &&
            !ui->txtcDCProveedor->text().isEmpty() && !ui->txtcCCProveedor->text().isEmpty())
        cOk = oConfig->ValidarCC(ui->txtcEntidadBancariaProveedor->text(),ui->txtcOficinaBancariaProveedor->text(),
                              ui->txtcDCProveedor->text(),ui->txtcCCProveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));
    delete oConfig;
}

void frmProveedores::on_txtcOficinaBancariaProveedor_editingFinished()
{
    Configuracion *oConfig = new Configuracion();
    QString cOk;
    if(!ui->txtcEntidadBancariaProveedor->text().isEmpty() && !ui->txtcOficinaBancariaProveedor->text().isEmpty() &&
            !ui->txtcDCProveedor->text().isEmpty() && !ui->txtcCCProveedor->text().isEmpty())
        cOk = oConfig->ValidarCC(ui->txtcEntidadBancariaProveedor->text(),ui->txtcOficinaBancariaProveedor->text(),
                              ui->txtcDCProveedor->text(),ui->txtcCCProveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));
    delete oConfig;
}

void frmProveedores::on_txtcDCProveedor_editingFinished()
{
    Configuracion *oConfig = new  Configuracion();
    QString cOk;
    if(!ui->txtcEntidadBancariaProveedor->text().isEmpty() && !ui->txtcOficinaBancariaProveedor->text().isEmpty() &&
            !ui->txtcDCProveedor->text().isEmpty() && !ui->txtcCCProveedor->text().isEmpty())
        cOk = oConfig->ValidarCC(ui->txtcEntidadBancariaProveedor->text(),ui->txtcOficinaBancariaProveedor->text(),
                              ui->txtcDCProveedor->text(),ui->txtcCCProveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));
    delete oConfig;
}



void frmProveedores::on_txtcCCProveedor_editingFinished()
{
    Configuracion *oConfig = new Configuracion();
    QString cOk;
    if(!ui->txtcEntidadBancariaProveedor->text().isEmpty() && !ui->txtcOficinaBancariaProveedor->text().isEmpty() &&
            !ui->txtcDCProveedor->text().isEmpty() && !ui->txtcCCProveedor->text().isEmpty())
        cOk = oConfig->ValidarCC(ui->txtcEntidadBancariaProveedor->text(),ui->txtcOficinaBancariaProveedor->text(),
                              ui->txtcDCProveedor->text(),ui->txtcCCProveedor->text());
    if(cOk == "1")
        ui->lblCuenta1Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblCuenta1Valida->setText(tr("La cuenta no es válida"));
    delete oConfig;
}

void frmProveedores::on_txtcEntidadPagoProveedor_editingFinished()
{
    Configuracion *oConfig = new Configuracion();
    QString cOk;
    if(!ui->txtcEntidadPagoProveedor->text().isEmpty() && !ui->txtcOficinaPagoProveedor->text().isEmpty() &&
            !ui->txtcDCPagoProveedor->text().isEmpty() && !ui->txtcCuentaPagoProveedor->text().isEmpty())
        cOk = oConfig->ValidarCC(ui->txtcEntidadPagoProveedor->text(),ui->txtcOficinaPagoProveedor->text(),
                              ui->txtcDCPagoProveedor->text(),ui->txtcCuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
    delete oConfig;
}

void frmProveedores::on_txtcOficinaPagoProveedor_editingFinished()
{
    Configuracion *oConfig = new Configuracion();
    QString cOk;
    if(!ui->txtcEntidadPagoProveedor->text().isEmpty() && !ui->txtcOficinaPagoProveedor->text().isEmpty() &&
            !ui->txtcDCPagoProveedor->text().isEmpty() && !ui->txtcCuentaPagoProveedor->text().isEmpty())
        cOk = oConfig->ValidarCC(ui->txtcEntidadPagoProveedor->text(),ui->txtcOficinaPagoProveedor->text(),
                              ui->txtcDCPagoProveedor->text(),ui->txtcCuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
    delete oConfig;
}

void frmProveedores::on_txtcDCPagoProveedor_editingFinished()
{
    Configuracion *oConfig = new Configuracion();
    QString cOk;
    if(!ui->txtcEntidadPagoProveedor->text().isEmpty() && !ui->txtcOficinaPagoProveedor->text().isEmpty() &&
            !ui->txtcDCPagoProveedor->text().isEmpty() && !ui->txtcCuentaPagoProveedor->text().isEmpty())
        cOk = oConfig->ValidarCC(ui->txtcEntidadPagoProveedor->text(),ui->txtcOficinaPagoProveedor->text(),
                              ui->txtcDCPagoProveedor->text(),ui->txtcCuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
    delete oConfig;
}

void frmProveedores::on_txtcCuentaPagoProveedor_editingFinished()
{
    Configuracion *oConfig = new Configuracion();
    QString cOk;
    if(!ui->txtcEntidadPagoProveedor->text().isEmpty() && !ui->txtcOficinaPagoProveedor->text().isEmpty() &&
            !ui->txtcDCPagoProveedor->text().isEmpty() && !ui->txtcCuentaPagoProveedor->text().isEmpty())
        cOk = oConfig->ValidarCC(ui->txtcEntidadPagoProveedor->text(),ui->txtcOficinaPagoProveedor->text(),
                              ui->txtcDCPagoProveedor->text(),ui->txtcCuentaPagoProveedor->text());
    if(cOk == "1")
        ui->lblcuenta2Valida->setText(tr("La cuenta es válida"));
    else
        ui->lblcuenta2Valida->setText(tr("La cuenta no es válida"));
    delete oConfig;
}

void frmProveedores::on_txtcCPAlmacen_editingFinished()
{
    if (!ui->txtcCPAlmacen->isReadOnly()) {
        if (!ui->txtcCPAlmacen->text().isEmpty() and ui->txtcPoblacionAlmacen->text().isEmpty()) {
            FrmBuscarPoblacion BuscarPoblacion;
            Configuracion *oConfig = new Configuracion();
            oConfig->CargarDatos();
            BuscarPoblacion.setcPoblacion(ui->txtcCPAlmacen->text(),0);
           if(BuscarPoblacion.exec()) {
               int nId = BuscarPoblacion.DevolverID();
             if(nId > 0) {
                   QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
                   QString cId;
                   cId = QString::number(nId);
                   qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
                   qPoblacion.bindValue(":id",cId);
                   if(!qPoblacion.exec()) {
                       QMessageBox::critical(NULL,tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                   } else {
                       if (qPoblacion.next()) {
                            ui->txtcPoblacionAlmacen->setText(qPoblacion.value(0).toString());
                            ui->txtcProvinciaAmacen->setText(qPoblacion.value(2).toString());
                            ui->txtcPaisAlmacen->setText(oConfig->cPais);
                       }
                   }

               }
        delete oConfig;
           }
        }
        ui->txtcTelefonoAlmacen->setFocus();
    }
}
