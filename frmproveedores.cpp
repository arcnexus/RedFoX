#include "frmproveedores.h"
#include "ui_frmproveedores.h"
#include "proveedor.h"

Proveedor *oProveedor = new Proveedor();

frmProveedores::frmProveedores(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmProveedores)
{
    ui->setupUi(this);
}

frmProveedores::~frmProveedores()
{
    delete ui;
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
    // SpinBox
    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    QSpinBox *SpinBox;
    foreach (SpinBox, SpinBoxList) {
        SpinBox->setReadOnly(true);
    }
    // DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(true);
    }
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

void frmProveedores::LLenarCampos()
{
    ui->txtcCodigo->setText(oProveedor->getcCodigo());
}

void frmProveedores::CargarCamposEnProveedor()
{
}
