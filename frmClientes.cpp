#include "frmClientes.h"
#include "ui_frmClientes.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QErrorMessage>
#include<QSqlError>
#include "cliente.h"
#include<QDate>
#include <QSqlQueryModel>
#include <QDebug>
#include "frmbuscarcliente.h"
#include "frmbuscarpoblacion.h"


Cliente *oCliente = new Cliente();
frmClientes::frmClientes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmClientes)
{
    ui->setupUi(this);
    dbCliente = QSqlDatabase::addDatabase("QSQLITE","clientes");

    /* esto es para mysql y postgres
    dbCliente.setDatabaseName("emp0999");
    dbCliente.setHostName("localhost");
    dbCliente.open("root","PatataBullida_99"); */

    dbCliente.setDatabaseName("/home/arcnexus/Informatica-Intelligent/programacio/Terra/DB/emp0999.sqlite");
    dbCliente.open();
    if (dbCliente.lastError().isValid())
        {
            QMessageBox::critical(0, "error:", dbCliente.lastError().text());

        }

    // Rellenar formas de pago
    modelFP = new QSqlQueryModel();
    modelFP->setQuery("Select cFormaPago,id from FormPago",QSqlDatabase::database("clientes"));


    ui->cbocFormaPago->setModel(modelFP);
    //ui->tableView_5->setModel(model);

    // Rellenar Tarifas:
    ui->cbonTarifaCliente->addItem("A");
    ui->cbonTarifaCliente->addItem("B");
    ui->cbonTarifaCliente->addItem("C");

    //oCliente->Recuperar("Select * from clientes");
    //LLenarCampos();
    bloquearCampos();
    this->Altas = false;

}

frmClientes::~frmClientes()
{
    delete ui;
    delete oCliente;
    dbCliente.close();
}
void frmClientes::LLenarCampos() {
    ui->txtcCodigoCliente->setText(oCliente->getcCodigoCliente());
    ui->txtPrimerApellido->setText(oCliente->getcApellido1());
    ui->txtSegundoApellido->setText(oCliente->getcApellido2());
    ui->txtcNombre->setText(oCliente->getcNombre());
    ui->txtcNombreFiscal->setText(oCliente->getcNombreFiscal());
    ui->txtcNombreComercial->setText(oCliente->getcNombreComercial());
    ui->txtcPersonaContacto->setText(oCliente->getcPersonaContacto());
    ui->txtcCifNif->setText(oCliente->getcCifNif());
    ui->txtcDireccion1->setText(oCliente->getcDireccion1());
    ui->txtcDireccion2->setText(oCliente->getcDireccion2());
    ui->txtcCp->setText(oCliente->getcCP());
    ui->txtcPoblacion->setText(oCliente->getcPoblacion());
    ui->txtcProvincia->setText(oCliente->getcProvincia());
    ui->txtcPais->setText(oCliente->getcPais());
    ui->txtcTelefono1->setText(oCliente->getcTelefono1());
    ui->txtcTelefono2->setText(oCliente->getcTelefono2());
    ui->txtcFax->setText(oCliente->getcFax());
    ui->txtcMovil->setText(oCliente->getcMovil());
    ui->txtcEMail->setText(oCliente->getcEmail());
    ui->txtcWeb->setText(oCliente->getcWeb());
    ui->txtcDireccionFactura1->setText(oCliente->getcDireccionFactura1());
    ui->txtcDireccionFactura2->setText(oCliente->getcDireccionFactura2());
    ui->txtcCPFactura->setText(oCliente->getcCPFactura());
    ui->txtcPoblacionFactura->setText(oCliente->getcPoblacionFactura());
    ui->txtcProvinciaFactura->setText(oCliente->getcProvinciaFactura());
    ui->txtcPaisFactura->setText(oCliente->getcPaisFactura());
    ui->txtcDireccionAlmacen1->setText(oCliente->getcDireccionAlmacen());
    ui->txtcDireccionAlmacen2->setText(oCliente->getcDireccionAlmacen2());
    ui->txtcCpPoblacionAlmacen->setText(oCliente->getcCPAlmacen());
    ui->txtcPoblacionAlmacen->setText(oCliente->getcPoblacionAlmacen());
    ui->txtcProvinciaAlmacen->setText(oCliente->getcProvinciaAlmacen());
    ui->txtcPaisAlmacen->setText(oCliente->getcPaisAlmacen());
    ui->txtdFechaAlta->setDate(oCliente->getdFechaAlta());
    ui->txtdFechaUltimaCompra->setDate(oCliente->getdFechaUltimaCompra());
    ui->txtrImporteAcumulado->setValue(oCliente->getrAcumuladoVentas());
    ui->txtrVentasEjercicio->setValue(oCliente->getrVentasEjercicio());
    ui->txtrRiesgoPermitido->setValue(oCliente->getrRiesgoMaximo());
    ui->txtrDeudaActual->setValue(oCliente->getrDeudaActual());
    ui->txttComentarios->setText(oCliente->gettComentarios());
    if(oCliente->islBloqueado()) {
        ui->chklBloqueoCliente->setChecked(true);
    } else {
        ui->chklBloqueoCliente->setChecked(false);
    }
    ui->txttComentarioBloqueo->setText(oCliente->gettComentarioBloqueo());
    ui->txtnPorcDtoCliente->setValue(oCliente->getnPorcDtoCliente());
    if(oCliente->islRecargoEquivalencia()) {
        ui->chklRecargoEquivalencia->setChecked(true);
    } else {
        ui->chklRecargoEquivalencia->setChecked(false);
    }
    ui->txtcCuentaContable->setText(oCliente->getcCuentaContable());
    ui->txtcCuentaIvaRepercutido->setText(oCliente->getcCuentaIvaRepercutido());
    ui->txtcCuentaDeudas->setText(oCliente->getcCuentaDeudas());
    ui->txtcCuentaCobros->setText(oCliente->getcCuentaCobros());
    int indice=ui->cbocFormaPago->findText(oCliente->getcFormaPago());
    if(indice!=-1)
     ui->cbocFormaPago->setCurrentIndex(indice);

    //ui->cbocFormaPago->setItemText();
    ui->txtnDiaPago1->setValue(oCliente->getnDiaPago1());
    ui->txtnDiaPago2->setValue(oCliente->getnDiaPago2());
   // ui->cbonTarifaCliente->lineEdit()->setText(oCliente->getnTarifaCliente());
    ui->txtrImporteACuenta->setValue(oCliente->getrImporteACuenta());
    ui->txtrVales->setValue(oCliente->getrVales());
    ui->txtcEntidadBancaria->setText(oCliente->getcEntidadBancaria());
    ui->txtcOficinaBancaria->setText(oCliente->getcOficinaBancaria());
    ui->txtcDc->setText(oCliente->getcDC());
    ui->txtcCuentaCorriente->setText(oCliente->getcCuentaCorriente());
    ui->txtdFechaNacimiento->setDate(oCliente->getdFechaNacimiento());
    ui->txtcAccesoWeb->setText(oCliente->getcAccesoWeb());
    ui->txtcPasswordWeb->setText(oCliente->getcPasswordweb());


}
void frmClientes::VaciarCampos() {
    QDate dFecha;

    ui->txtcCodigoCliente->setText("");
    ui->txtPrimerApellido->setText("");
    ui->txtSegundoApellido->setText("");
    ui->txtcNombre->setText("");
    ui->txtcNombreFiscal->setText("");
    ui->txtcNombreComercial->setText("");
    ui->txtcPersonaContacto->setText("");
    ui->txtcCifNif->setText("");
    ui->txtcDireccion1->setText("");
    ui->txtcDireccion2->setText("");
    ui->txtcPoblacion->setText("");
    ui->txtcProvincia->setText("");
    ui->txtcPais->setText("");
    ui->txtcTelefono1->setText("");
    ui->txtcTelefono2->setText("");
    ui->txtcFax->setText("");
    ui->txtcMovil->setText("");
    ui->txtcEMail->setText("");
    ui->txtcWeb->setText("");
    ui->txtcDireccionFactura1->setText("");
    ui->txtcDireccionFactura2->setText("");
    ui->txtcCPFactura->setText("");
    ui->txtcPoblacionFactura->setText("");
    ui->txtcProvinciaFactura->setText("");
    ui->txtcPaisFactura->setText("");
    ui->txtcDireccionAlmacen1->setText("");
    ui->txtcDireccionAlmacen2->setText("");
    ui->txtcCpPoblacionAlmacen->setText("");
    ui->txtcPoblacionAlmacen->setText("");
    ui->txtcProvinciaAlmacen->setText("");
    ui->txtcPaisAlmacen->setText("");
    ui->txtdFechaAlta->setDate(dFecha.currentDate());
    ui->txtdFechaUltimaCompra->setDate(dFecha.currentDate());
    ui->txtrImporteAcumulado->setValue(0);
    ui->txtrVentasEjercicio->setValue(0);
    ui->txtrRiesgoPermitido->setValue(0);
    ui->txtrDeudaActual->setValue(0);
    ui->txttComentarios->setText("");
    ui->chklBloqueoCliente->setChecked(false);
    ui->txttComentarioBloqueo->setText( "");
    ui->txtnPorcDtoCliente->setValue(0);
    ui->chklRecargoEquivalencia->setChecked(false);
    ui->txtcCuentaContable->setText("");
    ui->txtcCuentaIvaRepercutido->setText("");
    ui->txtcCuentaDeudas->setText("");
    ui->txtcCuentaCobros->setText("");


    //ui->cbocFormaPago->setItemText();
    ui->txtnDiaPago1->setValue(0);
    ui->txtnDiaPago2->setValue(0);
   // ui->cbonTarifaCliente->lineEdit()->setText(oCliente->getnTarifaCliente());
    ui->txtrImporteACuenta->setValue(0);
    ui->txtrVales->setValue(0);
    ui->txtcEntidadBancaria->setText("");
    ui->txtcOficinaBancaria->setText("");
    ui->txtcDc->setText("");
    ui->txtcCuentaCorriente->setText("");
    ui->txtdFechaNacimiento->setDate(dFecha.currentDate());
    ui->txtcAccesoWeb->setText("");
    ui->txtcPasswordWeb->setText("");
    ui->txtPrimerApellido->setFocus();

}

void frmClientes::LLenarCliente() {
    oCliente->setcCodigoCliente(ui->txtcCodigoCliente->text());
    oCliente->setcApellido1(ui->txtPrimerApellido->text());
    oCliente->setcApellido2(ui->txtSegundoApellido->text());
    oCliente->setcNombre(ui->txtcNombre->text());
    oCliente->setcNombreFiscal(ui->txtcNombreFiscal->text());
    oCliente->setcNombreComercial(ui->txtcNombreComercial->text());
    oCliente->setcPersonaContacto(ui->txtcPersonaContacto->text());
    oCliente->setcCifNif(ui->txtcCifNif->text());
    oCliente->setcDireccion1(ui->txtcDireccion1->text());
    oCliente->setcDireccion2(ui->txtcDireccion2->text());
    oCliente->setcCp(ui->txtcCp->text());
    oCliente->setcPoblacion(ui->txtcPoblacion->text());
    oCliente->setcProvincia(ui->txtcProvincia->text());
    oCliente->setcPais(ui->txtcPais->text());
    oCliente->setcTelefono1(ui->txtcTelefono1->text());
    oCliente->setcTelefono2(ui->txtcTelefono2->text());
    oCliente->setcFax(ui->txtcFax->text());
    oCliente->setcMovil(ui->txtcMovil->text());
    oCliente->setcEmail(ui->txtcEMail->text());
    oCliente->setcWeb(ui->txtcWeb->text());
    oCliente->setcDireccionFactura1(ui->txtcDireccionFactura1->text());
    oCliente->setcDireccionFactura2(ui->txtcDireccionFactura2->text());
    oCliente->setcCPFactura(ui->txtcCPFactura->text());
    oCliente->setcPoblacionFactura(ui->txtcPoblacionFactura->text());
    oCliente->setcProvinciaFactura(ui->txtcProvinciaFactura->text());
    oCliente->setcPaisFactura(ui->txtcPaisFactura->text());
    oCliente->setcDireccionAlmacen1(ui->txtcDireccionAlmacen1->text());
    oCliente->setcDireccionAlmacen2(ui->txtcDireccionAlmacen2->text());
    oCliente->setcCPAlmacen(ui->txtcCpPoblacionAlmacen->text());
    oCliente->setcPoblacionAlmacen(ui->txtcPoblacionAlmacen->text());
    oCliente->setcProvinciaAlmacen(ui->txtcProvinciaAlmacen->text());
    oCliente->setcPaisAlmacen(ui->txtcPaisAlmacen->text());
    oCliente->setdFechaAlta(ui->txtdFechaAlta->date());
    oCliente->setdFechaUltimaCompra(ui->txtdFechaUltimaCompra->date());
    oCliente->setrAcumuladoVentas(ui->txtrImporteAcumulado->value());
    oCliente->setrVentasEjercicio(ui->txtrVentasEjercicio->value());
    oCliente->setrRiesgoMaximo(ui->txtrRiesgoPermitido->value());
    oCliente->setrDeudaActual(ui->txtrDeudaActual->value());
    oCliente->settComentarios(ui->txttComentarios->toPlainText());
    oCliente->setisBloqueado(ui->chklBloqueoCliente->isChecked());
    oCliente->settComentarioBloqueo(ui->txttComentarioBloqueo->toPlainText());
    oCliente->setnPorcDtoCliente(ui->txtnPorcDtoCliente->value());
    oCliente->setislRecargoEquivalencia(ui->chklRecargoEquivalencia->isChecked());
    oCliente->setcCuentaContable(ui->txtcCuentaContable->text());
    oCliente->setcCuentaIvaRepercutido(ui->txtcCuentaIvaRepercutido->text());
    oCliente->setcCuentaDeudas(ui->txtcCuentaDeudas->text());
    oCliente->setcCuentaCobros(ui->txtcCuentaCobros->text());
    oCliente->setcFormaPago(ui->cbocFormaPago->currentText());
    oCliente->setnDiaPago1(ui->txtnDiaPago1->value());
    oCliente->setnDiaPago2(ui->txtnDiaPago2->value());
    oCliente->setnTarifaCliente(ui->cbonTarifaCliente->currentText().toDouble());
    oCliente->setrImporteaCuenta(ui->txtrImporteACuenta->value());
    oCliente->setrVales( ui->txtrVales->value());
    oCliente->setcEntidadBancaria(ui->txtcEntidadBancaria->text());
    oCliente->setcOficinaBancaria(ui->txtcOficinaBancaria->text());
    oCliente->setcDC(ui->txtcDc->text());
    oCliente->setcCuentaCorriente(ui->txtcCuentaCorriente->text());
    oCliente->setdFechaNacimiento(ui->txtdFechaNacimiento->date());
    oCliente->setrImportePendiente(ui->txtrImporteAcumulado->value());
    oCliente->setcAccesoWeb(ui->txtcAccesoWeb->text());
    oCliente->setcPasswordWeb(ui->txtcPasswordWeb->text());
}


void frmClientes::on_btnSiguiente_clicked()
{

    QString cId = QString::number(oCliente->getId());
    oCliente->Recuperar("Select * from clientes where id >"+cId+" order by id limit 0,1 ");
    LLenarCampos();

}

void frmClientes::on_btnAnterior_clicked()
{
    QString cId = QString::number(oCliente->getId());
    oCliente->Recuperar("Select * from clientes where id <"+cId+" order by id desc limit 0,1 ");
    LLenarCampos();
}

void frmClientes::on_btnGuardar_clicked()
{

    LLenarCliente();
    if(this->Altas) {
       this->Altas = false;
       oCliente->Anadir();
    } else {

        oCliente->Guardar();
    }
    bloquearCampos();
}

void frmClientes::on_btnAnadir_clicked()
{
    desbloquearCampos();

   VaciarCampos();
   this->Altas = true;

}

void frmClientes::on_txtPrimerApellido_lostFocus()
{
    ui->txtPrimerApellido->setText(ui->txtPrimerApellido->text().toUpper());
    if(!ui->txtPrimerApellido->text().isEmpty() and !ui->txtSegundoApellido->text().isEmpty() and !ui->txtcNombre->text().isEmpty()) {
    ui->txtcNombreFiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtcNombre->text()); }
}

void frmClientes::on_txtSegundoApellido_lostFocus()
{
    ui->txtSegundoApellido->setText(ui->txtSegundoApellido->text().toUpper());
    if(!ui->txtPrimerApellido->text().isEmpty() and !ui->txtSegundoApellido->text().isEmpty() and !ui->txtcNombre->text().isEmpty()) {
    ui->txtcNombreFiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtcNombre->text()); }

}

void frmClientes::on_txtcNombre_lostFocus()
{
    ui->txtcNombre->setText(ui->txtcNombre->text().toUpper());
    if(!ui->txtPrimerApellido->text().isEmpty() and !ui->txtSegundoApellido->text().isEmpty() and !ui->txtcNombre->text().isEmpty()) {
    ui->txtcNombreFiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtcNombre->text()); }

}

void frmClientes::on_txtcPoblacion_lostFocus()
{
    ui->txtcPoblacion->setText(ui->txtcPoblacion->text().toUpper());
}

void frmClientes::on_txtcProvincia_lostFocus()
{
    ui->txtcProvincia->setText(ui->txtcProvincia->text().toUpper());
}

void frmClientes::on_txtcCifNif_lostFocus()
{
    ui->txtcCifNif->setText(ui->txtcCifNif->text().toUpper());
}

void frmClientes::on_btnEditar_clicked()
{
        desbloquearCampos();
        ui->txtcCodigoCliente->setFocus();


}
void frmClientes::bloquearCampos() {

    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    QSpinBox *SpinBox;
    foreach (SpinBox, SpinBoxList) {
        SpinBox->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(false);
        //qDebug() << lineEdit->objectName();
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
void frmClientes::desbloquearCampos() {
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
    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    QSpinBox *SpinBox;
    foreach (SpinBox, SpinBoxList) {
        SpinBox->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
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

void frmClientes::on_btnDeshacer_clicked()
{
    QString cId = QString::number(oCliente->getId());
    oCliente->Recuperar("Select * from clientes where id ="+cId+" order by id limit 0,1 ");
    LLenarCampos();
    bloquearCampos();
}

void frmClientes::on_btnBorrar_clicked()
{
    int id_Cliente = oCliente->getId();
    id_Cliente --;

    oCliente->Borrar(oCliente->getId());
    VaciarCampos();

}

void frmClientes::on_btnBuscar_clicked()
{
    FrmBuscarCliente BuscarClientes;
    BuscarClientes.exec();
    int nId = BuscarClientes.DevolverID();
    qDebug() << nId;
    QString cId = QString::number(nId);

    oCliente->Recuperar("Select * from clientes where id ="+cId+" order by id limit 0,1 ");
    LLenarCampos();
}

void frmClientes::on_txtcCp_lostFocus()
    {
    if (!ui->txtcCp->text().isEmpty() and ui->txtcPoblacion->text().isEmpty()) {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
       if(BuscarPoblacion.exec()) {
        //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
           int nId = BuscarPoblacion.DevolverID();
           qDebug() <<nId;
         if(nId > 0) {
               QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
               QString cId;
               cId = QString::number(nId);
               qPoblacion.prepare("select col_3 as poblacion, col_4 as CP from poblaciones where col_1 = '"+cId+"'");
               //qPoblacion.bindValue(":id",nId);
               if(!qPoblacion.exec()) {
                   qDebug() << qPoblacion.lastQuery();
                   qDebug() << qPoblacion.value(0).toString();
                   QMessageBox::critical(NULL,tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
               } else {
                   qDebug() << qPoblacion.lastQuery();
                   if (qPoblacion.next()) {
                                        qDebug() << qPoblacion.value(0).toString();
                        ui->txtcPoblacion->setText(qPoblacion.value(0).toString());
                   }
               }

           }
       }
      // BuscarPoblacion.close();
    }

    }

