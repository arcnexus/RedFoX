#include "frmproveedores.h"
#include "ui_frmproveedores.h"
#include "proveedor.h"

#include "Busquedas/frmbuscarpoblacion.h"


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
    // cargar datos forma de pago.
    QSqlQueryModel *qmFormaPago = new QSqlQueryModel(this);
    qmFormaPago->setQuery("select cCodigo, cFormapago from FormPago",QSqlDatabase::database("empresa"));

    ui->txtcCodigoFormaPago->setModel(qmFormaPago);
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
    ui->txtcPais->setText(oProveedor->cPais);
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
    ui->txtcPaisAlmacen->setText(oProveedor->cPaisAlmacen);
    ui->txtcTelefonoAlmacen->setText(oProveedor->cTelefonoAlmacen);
    ui->txtcFaxAlmacen->setText(oProveedor->cFaxAlmacen);
    int nIndex = ui->txtcCodigoFormaPago->findText(oProveedor->cCodigoFormaPago);
    if (nIndex !=-1)
        ui->txtcCodigoFormaPago->setCurrentIndex(nIndex);

    ui->txtdFechaUltimaCompra->setDate(oProveedor->dFechaUltimaCompra);
    ui->txtrAcumuladoCompras->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->rAcumuladoCompras,'f',2)));
    ui->txtcEntidadBancariaProveedor->setText(oProveedor->cEntidadBancariaProveedor);
    ui->txtcOficinaBancariaProveedor->setText(oProveedor->cOficinaBancariaProveedor);
    ui->txtcDCProveedor->setText(oProveedor->cDCProveedor);
    ui->txtcCCProveedor->setText(oProveedor->cCCProveedor);
    ui->txtcEntidadPagoProveedor->setText(oProveedor->cEntidadPagoProveedor);
    ui->txtcOficinaPagoProveedor->setText(oProveedor->cOficinaPagoProveedor);
    ui->txtcDCPagoProveedor->setText(oProveedor->cDCPagoProveedor);
    ui->txtcCuentaPagoProveedor->setText(oProveedor->cCuentaPagoProveedor);
    ui->txtcCuentaIvaSoportado->setText(oProveedor->cCuentaIvaSoportado);
    ui->txtrRetencionIRPF->setText(Configuracion_global->FormatoNumerico(QString::number(oProveedor->rRetencionIRPF,'f',2)));
    ui->txtnTipoRetencionIRPF->setText(QString::number(oProveedor->nTipoRetencion));
    ui->txtcCuentaAplicacion->setText(oProveedor->cCuentaAplicacion);
    ui->txtcCuentaPagos->setText(oProveedor->cCuentaPagos);
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
    oProveedor->cPais = ui->txtcPais->text();
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
    oProveedor->cPaisAlmacen = ui->txtcPaisAlmacen->text();
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
    oProveedor->cCuentaPagoProveedor = ui->txtcCuentaPagoProveedor->text();
    oProveedor->cCuentaIvaSoportado = ui->txtcCuentaIvaSoportado->text();
    oProveedor->rRetencionIRPF = ui->txtrRetencionIRPF->text().replace(".","").toDouble();
    oProveedor->nTipoRetencion = ui->txtnTipoRetencionIRPF->text().toInt();
    oProveedor->cCuentaAplicacion = ui->txtcCuentaAplicacion->text();
    oProveedor->cCuentaPagos = ui->txtcCuentaPagos->text();
    oProveedor->tComentarios = ui->txttComentarios->toPlainText();
    oProveedor->nDto = ui->txtnDto->text().replace(".","").toDouble();
    oProveedor->dFechaAlta = ui->txtdFechaAlta->date();
    oProveedor->rDeudaMaxima = ui->txtrDeudaMaxima->text().replace(".","").toDouble();
    oProveedor->rDeudaActual = ui->txtrDeudaActual->text().replace(".","").toDouble();
    oProveedor->lRecargoEquivalencia = ui->chklRecargoEquivalencia->isChecked();
    oProveedor->tTextoparaPedidos = ui->txttTextoparaPedidos->toPlainText();
    oProveedor->rEntregadoaCuenta = ui->txtrEntregadoaCuenta->text().replace(".","").toDouble();

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
                        ui->txtcPais->setText(Configuracion_global->cPais);
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
                qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
                qPoblacion.bindValue(":id",cId);
                if(!qPoblacion.exec()) {
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                } else {
                    if (qPoblacion.next()) {
                        ui->txtcPoblacion->setText(qPoblacion.value(0).toString());
                        ui->txtcProvincia->setText(qPoblacion.value(2).toString());
                        ui->txtcPais->setText(Configuracion_global->cPais);
                    }
                }

            }
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
                    qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
                    qPoblacion.bindValue(":id",cId);
                    if(!qPoblacion.exec()) {
                        QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                    } else {
                        if (qPoblacion.next()) {
                            ui->txtcPoblacionAlmacen->setText(qPoblacion.value(0).toString());
                            ui->txtcProvinciaAmacen->setText(qPoblacion.value(2).toString());
                            ui->txtcPaisAlmacen->setText(Configuracion_global->cPais);
                        }
                    }

                }
            }
        }
        ui->txtcTelefonoAlmacen->setFocus();
    }
}
