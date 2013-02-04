#include "frmalbaran.h"
#include "ui_frmalbaran.h"
#include "albaran.h"
#include "Zona_Pacientes/cliente.h"
//
#include "articulo.h"

Albaran *oAlbaran = new Albaran();
Cliente *oCliente2 = new Cliente();

FrmAlbaran::FrmAlbaran(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAlbaran),
    helper(this)
{
    ui->setupUi(this);
    // Pongo valores por defecto
    ui->lblFacturado->setVisible(false);
    ui->lblImpreso->setVisible(false);
    ui->lblNumFactura->setVisible(false);
    ui->txtcNumFra->setVisible(false);
    // valores edicion
    //ui->txtcCodigoArticulo->setFocus();

    BloquearCampos(true);

    Db_table_View* searcher = new Db_table_View(this);
    searcher->set_db("empresa");
    searcher->set_table("articulos");
    searcher->set_selection("cCodigo");
    searcher->setWindowTitle(tr("Articulos"));

    QStringList headers;
    headers << tr("Codigo")<< "1" << "2" << tr("Descripción");
    searcher->set_table_headers(headers);

    searcher->set_columnHide(0);
    searcher->set_columnHide(2);
    searcher->set_columnHide(3);

    for(int i = 5; i<50;i++)
        searcher->set_columnHide(i);

    helper.set_Searcher(searcher);
    helper.set_Tipo(false);
    helper.help_table(ui->Lineas);

    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));

    connect(&helper,SIGNAL(totalChanged(double,double,double,double,double,double,QString)),
            this,SLOT(totalChanged(double,double,double,double,double,double,QString)));

    connect(&helper,SIGNAL(desglose1Changed(double,double,double,double)),
            this,SLOT(desglose1Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose2Changed(double,double,double,double)),
            this,SLOT(desglose2Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose3Changed(double,double,double,double)),
            this,SLOT(desglose3Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose4Changed(double,double,double,double)),
            this,SLOT(desglose4Changed(double,double,double,double)));

    connect(ui->chklRecargoEq,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

    if(oAlbaran->RecuperarAlbaran("Select * from cab_alb where nAlbaran > -1 order by nAlbaran  limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("Id_Cab = '%1'").arg(ui->txtcAlbaran->text());
        helper.fillTable("empresa","lin_alb",filter);
    }
}

FrmAlbaran::~FrmAlbaran()
{
    delete ui;
}


void FrmAlbaran::LLenarCampos() {
    int lEstado;
    ui->txtcCodigoCliente->setText(oAlbaran->cCodigoCliente);
    ui->txtcAlbaran->setText(QString::number(oAlbaran->nAlbaran));
    ui->txtdFecha->setDate(oAlbaran->dFecha);
    ui->txtdFechaFactura->setDate(oAlbaran->dFechaFactura);
    ui->txtcCliente->setText(oAlbaran->cCliente);
    ui->txtcDireccion->setText(oAlbaran->cDireccion);
    ui->txtcDireccion2->setText(oAlbaran->cDireccion2);
    ui->txtcCp->setText(oAlbaran->cCp);
    ui->txtcPoblacion->setText(oAlbaran->cPoblacion);
    ui->txtcProvincia->setText(oAlbaran->cProvincia);
    ui->txtcPais->setText(oAlbaran->cPais);
    ui->txtcCif->setText(oAlbaran->cCif);
    ui->txtrSubtotal->setText(Configuracion_global->FormatoNumerico( QString::number(oAlbaran->rSubtotal,'f',2)));
    ui->txtrImporteDescuento->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rDto,'f',2)));
    ui->txtrBase->setText(Configuracion_global->FormatoNumerico(QString::number( oAlbaran->rBaseTotal,'f',2)));
    ui->txtrBaseTotal_2->setText(Configuracion_global->FormatoNumerico(QString::number( oAlbaran->rBaseTotal,'f',2)));

    ui->txtrImporteIva->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rIvaTotal,'f',2)));
    ui->txtrTotal->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rTotalAlbaran,'f',2)));
    ui->txtrTotal_2->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rTotalAlbaran,'f',2)));
    ui->txtrTotalRecargoEq_2->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rRecargoEqTotal,'f',2)));
    lEstado = oAlbaran->lImpreso;
    if((lEstado == 1)) {
        ui->lblImpreso->setVisible(true);
    } else {
        ui->lblImpreso->setVisible(false);
    }
    lEstado = oAlbaran->lFacturado;
    if ((lEstado == 1)) {
        ui->lblFacturado->setVisible(true);
        ui->txtdFechaFactura->setVisible(true);
        ui->txtcNumFra->setVisible(true);
    } else {
        ui->lblFacturado->setVisible(false);
        ui->txtdFechaFactura->setVisible(false);
        ui->txtcNumFra->setVisible(false);

    }
    ui->txttComentario->setText(oAlbaran->tComentario);
    ui->txtrBase1->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rBase1,'f',2)));
    ui->txtrBase2->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rBase2,'f',2)));
    ui->txtrBase3->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rBase3,'f',2)));
    ui->txtrBase4->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rBase4,'f',2)));
    ui->txtnPorcentajeIva1->setText(QString::number(oAlbaran->nPorcentajeIVA1));
    ui->txtnPorcentajeIva2->setText(QString::number(oAlbaran->nPorcentajeIVA2));
    ui->txtnPorcentajeIva3->setText(QString::number(oAlbaran->nPorcentajeIVA3));
    ui->txtnPorcentajeIva4->setText(QString::number(oAlbaran->nPorcentajeIVA4));
    ui->txtrIVA1->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rImporteIva1,'f',2)));
    ui->txtrIVA2->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rImporteIva2,'f',2)));
    ui->txtrIVA3->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rImporteIva3,'f',2)));
    ui->txtrIVA4->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rImporteIva4,'f',2)));
    ui->txtrTotal1->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rTotal1,'f',2)));
    ui->txtrTotal2->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rTotal2,'f',2)));
    ui->txtrTotal3->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rTotal3,'f',2)));
    ui->txtrTotal4->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rTotal4,'f',2)));
    ui->txtnRec1->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->nPorcentajeRecargoEq1,'f',2)));
    ui->txtnRec2->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->nPorcentajeRecargoEq2,'f',2)));
    ui->txtnRec3->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->nPorcentajeRecargoEq3,'f',2)));
    ui->txtnRec4->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->nPorcentajeRecargoEq4,'f',2)));
    ui->txtrRecargoEq1->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rImporteRecargoEq1,'f',2)));
    ui->txtrRecargoEq2->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rImporteRecargoEq2,'f',2)));
    ui->txtrRecargoEq3->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rImporteRecargoEq3,'f',2)));
    ui->txtrRecargoEq4->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rImporteRecargoEq4,'f',2)));
    ui->txtrTotalRecargoEq->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rRecargoEqTotal,'f',2)));
    ui->txtrTotalIVA_2->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbaran->rIvaTotal,'f',2)));
    ui->txtcPedidoCliente->setText(oAlbaran->cPedidoCli);
    if(oAlbaran->lRecargoEquivalencia==1)
        ui->chklRecargoEq->setChecked(true);
    else
        ui->chklRecargoEq->setChecked(false);

    QString filter = QString("Id_Cab = '%1'").arg(ui->txtcAlbaran->text());
    helper.fillTable("empresa","lin_alb",filter);
}
void FrmAlbaran::LLenarCamposCliente()
{
    ui->txtcCodigoCliente->setText(oCliente2->getcCodigoCliente());
    ui->txtcCliente->setText(oCliente2->getcNombreFiscal());
    ui->txtcDireccion->setText(oCliente2->getcDireccion1());
    ui->txtcDireccion2->setText(oCliente2->getcDireccion2());
    ui->txtcCp->setText(oCliente2->getcCP());
    ui->txtcPoblacion->setText(oCliente2->getcPoblacion());
    ui->txtcProvincia->setText(oCliente2->getcProvincia());
    ui->txtcPais->setText(oCliente2->getcPais());
    ui->txtcCif->setText(oCliente2->getcCifNif());
    if (oCliente2->getlIRPF()==1) {
        ui->chklRecargoEq->setChecked(true);
        oAlbaran->lRecargoEquivalencia = (1);
    } else {
        ui->chklRecargoEq->setChecked(false);
        oAlbaran->lRecargoEquivalencia = (0);
    }
}

void FrmAlbaran::VaciarCampos() {
    QDate dFecha;
    ui->txtcCodigoCliente->setText("");
    ui->txtcAlbaran->setText("");
    ui->txtdFecha->setDate(dFecha.currentDate());
    ui->txtdFechaFactura->setDate(dFecha.currentDate());
    ui->txtcCliente->setText("");
    ui->txtcDireccion->setText("");
    ui->txtcDireccion2->setText("");
    ui->txtcCp->setText("");
    ui->txtcPoblacion->setText("");
    ui->txtcProvincia->setText("");
    ui->txtcPais->setText("");
    ui->txtcCif->setText("");
    ui->txtrSubtotal->setText(0);
    ui->txtrImporteDescuento->setText("0,00");
    ui->txtrBase->setText("0,00");
    ui->txtrImporteIva->setText("0,00");
    ui->txtrTotal->setText("0,00");
    ui->lblImpreso->setVisible(false);
    ui->lblFacturado->setVisible(false);
    ui->lblNumFactura->setVisible(false);
    ui->txtcNumFra->setVisible(false);
    ui->txttComentario->setText("");
    ui->txtrBase1->setText(0);
    ui->txtrBase2->setText(0);
    ui->txtrBase3->setText(0);
    ui->txtrBase4->setText(0);
    Configuracion_global->CargarDatos();
    ui->txtnPorcentajeIva1->setText(QString::number(Configuracion_global->nIVA1));
    ui->txtnPorcentajeIva2->setText(QString::number(Configuracion_global->nIVA2));
    ui->txtnPorcentajeIva3->setText(QString::number(Configuracion_global->nIVA3));
    ui->txtnPorcentajeIva4->setText(QString::number(Configuracion_global->nIVA4));
    ui->txtnRec1->clear();
    ui->txtnRec2->clear();
    ui->txtnRec3->clear();
    ui->txtnRec4->clear();
    ui->txtrIVA1->setText(0);
    ui->txtrIVA2->setText(0);
    ui->txtrIVA3->setText(0);
    ui->txtrIVA4->setText(0);
    ui->txtrTotal1->setText(0);
    ui->txtrTotal2->setText(0);
    ui->txtrTotal3->setText(0);
    ui->txtrTotal4->setText(0);
    ui->txtrRecargoEq1->setText(0);
    ui->txtrRecargoEq2->setText(0);
    ui->txtrRecargoEq3->setText(0);
    ui->txtrRecargoEq4->setText(0);
    ui->txtrTotalRecargoEq->setText(0);
    ui->txtrACuenta->setText("0,00");
    ui->txtcPedidoCliente->setText("");
    ui->txtrTotalIVA_2->setText("0,00");
    ui->txtrBaseTotal_2->setText("0,00");
    ui->txtrTotalRecargoEq_2->setText("0,00");
    ui->txtrTotal_2->setText("0,00");
    ui->txtrSubtotal->setText("0,00");

    helper.fillTable("empresa","lin_alb","Id_Cab = -1");
}

void FrmAlbaran::BloquearCampos(bool state)
{
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(state);
        //qDebug() << lineEdit->objectName();
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(!state);
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!state);
    }
    // QTextEdit
    QList<QTextEdit *> textEditList = this->findChildren<QTextEdit *>();
    QTextEdit *textEdit;
    foreach (textEdit,textEditList) {
        textEdit->setReadOnly(state);
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!state);
    }

    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    ui->btnBuscar->setEnabled(state);
    ui->btnDeshacer->setEnabled(!state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->botBuscarCliente->setEnabled(!state);
    ui->btnFacturar->setEnabled(!state);
    ui->txtcAlbaran->setReadOnly(true);
    ui->btnAnadirLinea->setEnabled(!state);
    ui->btn_borrarLinea->setEnabled(!state);

    helper.blockTable(state);
}

void FrmAlbaran::LLenarAlbaran()
{
    oAlbaran->cCodigoCliente= (ui->txtcCodigoCliente->text());
    oAlbaran->nAlbaran= (ui->txtcAlbaran->text().toInt());
    oAlbaran->dFecha= (ui->txtdFecha->date());
    oAlbaran->dFechaFactura= (ui->txtdFechaFactura->date());
    oAlbaran->cCliente= (ui->txtcCliente->text());
    oAlbaran->cDireccion= (ui->txtcDireccion->text());
    oAlbaran->cDireccion2= (ui->txtcDireccion2->text());
    oAlbaran->cCp= (ui->txtcCp->text());
    oAlbaran->cPoblacion= (ui->txtcPoblacion->text());
    oAlbaran->cProvincia= (ui->txtcProvincia->text());
    oAlbaran->cPais= (ui->txtcPais->text());
    oAlbaran->cCif= (ui->txtcCif->text());
    oAlbaran->rSubtotal= (ui->txtrSubtotal->text().replace(".","").toDouble());
    oAlbaran->rDto= (ui->txtrImporteDescuento->text().replace(".","").toDouble());
    oAlbaran->rBaseTotal= (ui->txtrBase->text().replace(".","").toDouble());
    oAlbaran->rIvaTotal= (ui->txtrImporteIva->text().replace(".","").toDouble());
    oAlbaran->rTotalAlbaran= (ui->txtrTotal->text().replace(".","").toDouble());
    oAlbaran->tComentario= (ui->txttComentario->toPlainText());
    oAlbaran->rBase1= (ui->txtrBase1->text().replace(".","").toDouble());
    oAlbaran->rBase2= (ui->txtrBase2->text().replace(".","").toDouble());
    oAlbaran->rBase3= (ui->txtrBase3->text().replace(".","").toDouble());
    oAlbaran->rBase4= (ui->txtrBase4->text().replace(".","").toDouble());
    oAlbaran->nPorcentajeIVA1= (ui->txtnPorcentajeIva1->text().replace(".","").toDouble());
    oAlbaran->nPorcentajeIVA2= (ui->txtnPorcentajeIva2->text().replace(".","").toDouble());
    oAlbaran->nPorcentajeIVA3= (ui->txtnPorcentajeIva3->text().replace(".","").toDouble());
    oAlbaran->nPorcentajeIVA4= (ui->txtnPorcentajeIva4->text().replace(".","").toDouble());
    oAlbaran->rImporteIva1= (ui->txtrIVA1->text().replace(".","").toDouble());
    oAlbaran->rImporteIva2= (ui->txtrIVA2->text().replace(".","").toDouble());
    oAlbaran->rImporteIva3= (ui->txtrIVA3->text().replace(".","").toDouble());
    oAlbaran->rImporteIva4= (ui->txtrIVA4->text().replace(".","").toDouble());
    oAlbaran->rTotal1= (ui->txtrTotal1->text().replace(".","").toDouble());
    oAlbaran->rTotal2= (ui->txtrTotal2->text().replace(".","").toDouble());
    oAlbaran->rTotal3= (ui->txtrTotal3->text().replace(".","").toDouble());
    oAlbaran->rTotal4= (ui->txtrTotal4->text().replace(".","").toDouble());
    oAlbaran->nPorcentajeRecargoEq1= (ui->txtnRec1->text().replace(".","").toDouble());
    oAlbaran->nPorcentajeRecargoEq2= (ui->txtnRec2->text().replace(".","").toDouble());
    oAlbaran->nPorcentajeRecargoEq3= (ui->txtnRec3->text().replace(".","").toDouble());
    oAlbaran->nPorcentajeRecargoEq4= (ui->txtnRec4->text().replace(".","").toDouble());
    oAlbaran->rImporteRecargoEq1= (ui->txtrRecargoEq1->text().replace(".","").toDouble());
    oAlbaran->rImporteRecargoEq2= (ui->txtrRecargoEq2->text().replace(".","").toDouble());
    oAlbaran->rImporteRecargoEq3= (ui->txtrRecargoEq3->text().replace(".","").toDouble());
    oAlbaran->rImporteRecargoEq4= (ui->txtrRecargoEq4->text().replace(".","").toDouble());
    oAlbaran->rRecargoEqTotal= (ui->txtrTotalRecargoEq->text().replace(".","").toDouble());
    if (ui->chklRecargoEq->isChecked())
        oAlbaran->lRecargoEquivalencia = (1);
    else
        oAlbaran->lRecargoEquivalencia = (0);

    oAlbaran->cPedidoCli= (ui->txtcPedidoCliente->text());
}

void FrmAlbaran::on_btnSiguiente_clicked()
{
    int nAlbaran = oAlbaran->nAlbaran;
    if(oAlbaran->RecuperarAlbaran("Select * from cab_alb where nAlbaran >'"+QString::number(nAlbaran)+"' order by nAlbaran  limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("Id_Cab = '%1'").arg(ui->txtcAlbaran->text());
        helper.fillTable("empresa","lin_alb",filter);
    }
}

void FrmAlbaran::on_btnAnterior_clicked()
{
    int nAlbaran = oAlbaran->nAlbaran;
    if(oAlbaran->RecuperarAlbaran("Select * from cab_alb where nAlbaran <'"+QString::number(nAlbaran)+"' order by nAlbaran desc limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("Id_Cab = '%1'").arg(ui->txtcAlbaran->text());
        helper.fillTable("empresa","lin_alb",filter);
    }
}

void FrmAlbaran::on_btnAnadir_clicked()
{
    in_edit = false;
    VaciarCampos();
    LLenarAlbaran();
    BloquearCampos(false);
    ui->txtcAlbaran->setText(QString::number(oAlbaran->NuevoNumeroAlbaran()));
    ui->txtcCodigoCliente->setFocus();
    emit block();
}

void FrmAlbaran::on_botBuscarCliente_clicked()
{
    FrmBuscarCliente BuscarClientes;
    BuscarClientes.exec();
    int nId = BuscarClientes.DevolverID();
    QString cId = QString::number(nId);
    oAlbaran->iId_Cliente=(cId.toInt());
    oCliente2->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
    LLenarCamposCliente();
}



void FrmAlbaran::on_btnEditar_clicked()
{
    in_edit = true;
    if (!oAlbaran->lFacturado == 1)
    {
            BloquearCampos(false);
            emit block();
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Editar Albarán"),tr("No se puede editar un albarán que ha sido facturado, solo los NO facturados se pueden editar")+
                             tr("<p><b> Si necesita modificar algo genere una factura nueva y realice el abono correspondiente</b>")+
                                tr("y luego si es preciso realice un nuevo albarán y facture de nuevo "),tr("OK"));
    }

}

void FrmAlbaran::on_btnGuardar_clicked()
{        
    QSqlDatabase::database("empresa").transaction();
    LLenarAlbaran();
    bool succes = true;
    if(in_edit)
    {
        succes &= oAlbaran->GuardarAlbaran(ui->txtcAlbaran->text().toInt());
        succes &= oAlbaran->borrarLineas(ui->txtcAlbaran->text().toInt()) ;
        succes &= helper.saveTable(ui->txtcAlbaran->text().toInt(),"empresa","lin_alb");
    }
    else
    {
        succes &= oAlbaran->AnadirAlbaran();
        succes &= helper.saveTable(ui->txtcAlbaran->text().toInt(),"empresa","lin_alb");
    }
    if(succes)
    {
        LLenarCampos();
        succes = QSqlDatabase::database("empresa").commit();
    }
    if(succes)
    {
        QMessageBox::information(this,tr("Guardado"),tr("Guardado con éxito"),tr("&Aceptar"));
        BloquearCampos(true);
        emit unblock();
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),
                              tr("Error al guardar el albaran.\n")+QSqlDatabase::database("empresa").lastError().text(),
                              tr("&Aceptar"));
        QSqlDatabase::database("empresa").rollback();
    }

}

void FrmAlbaran::on_btnDeshacer_clicked()
{
    BloquearCampos(true);
    QString cId = QString::number(oAlbaran->id);
    oAlbaran->RecuperarAlbaran("Select * from cab_alb where id ="+cId+" order by id limit 1 ");
    LLenarCampos();
    emit unblock();
}

void FrmAlbaran::on_pushButton_clicked()
{
    if (QMessageBox::question(this,tr("Borrar"),
                              tr("Esta acción no se puede deshacer.\n¿Desea continuar?"),
                              tr("Cancelar"),
                               tr("&Continuar"))== QMessageBox::Accepted)
    {
        bool succes = true;
        QSqlDatabase::database("empresa").transaction();

        QSqlQuery q(QSqlDatabase::database("empresa"));
        q.prepare("DELETE FROM cab_alb WHERE nAlbaran = "+ui->txtcAlbaran->text());
        succes &= q.exec();
        succes &= oAlbaran->borrarLineas(ui->txtcAlbaran->text().toInt());

        if(succes)
            succes &= QSqlDatabase::database("empresa").commit();

        if(succes)
            QMessageBox::information(this,tr("Borrado"),tr("Borrado con exito"));
        else
            QSqlDatabase::database("empresa").rollback();
    }
}

void FrmAlbaran::totalChanged(double base, double dto, double subTotal, double iva, double re, double total, QString moneda)
{
    ui->txtrBase->setText(QString::number(base)+moneda);
    ui->txtrImporteDescuento->setText(QString::number(dto)+moneda);
    ui->txtrSubtotal->setText(QString::number(subTotal)+moneda);
    ui->txtrImporteIva->setText(QString::number(iva)+moneda);
    ui->txtrTotalRecargoEq->setText(QString::number(re)+moneda);
    ui->txtrTotal->setText(QString::number(total)+moneda);
    ui->lbl_total->setText(QString::number(total)+moneda);

    ui->txtrBaseTotal_2->setText(QString::number(base)+moneda);
    ui->txtrTotalIVA_2->setText(QString::number(iva)+moneda);
    ui->txtrTotalRecargoEq_2->setText(QString::number(re)+moneda);
    ui->txtrTotal_2->setText(QString::number(total)+moneda);
}

void FrmAlbaran::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtrBase1->setText(QString::number(base));
    ui->txtrIVA1->setText(QString::number(iva));
    ui->txtrRecargoEq1->setText(QString::number(re));
    ui->txtrTotal1->setText(QString::number(total));
}

void FrmAlbaran::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtrBase2->setText(QString::number(base));
    ui->txtrIVA2->setText(QString::number(iva));
    ui->txtrRecargoEq2->setText(QString::number(re));
    ui->txtrTotal2->setText(QString::number(total));
}

void FrmAlbaran::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtrBase3->setText(QString::number(base));
    ui->txtrIVA3->setText(QString::number(iva));
    ui->txtrRecargoEq3->setText(QString::number(re));
    ui->txtrTotal3->setText(QString::number(total));
}

void FrmAlbaran::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtrBase4->setText(QString::number(base));
    ui->txtrIVA4->setText(QString::number(iva));
    ui->txtrRecargoEq4->setText(QString::number(re));
    ui->txtrTotal4->setText(QString::number(total));
}


