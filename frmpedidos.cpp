#include "frmpedidos.h"
#include "ui_frmpedidos.h"


#include "articulo.h"

#include "Busquedas/frmbuscarcliente.h"

FrmPedidos::FrmPedidos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmPedidos),
    helper(this)
{
    ui->setupUi(this);
    QSqlTableModel* modelo = new QSqlTableModel(ui->combo_pais,QSqlDatabase::database("empresa"));
    modelo->setTable("paises");
    modelo->select();
    ui->combo_pais->setModel(modelo);
    ui->combo_pais->setModelColumn(modelo->fieldIndex("pais"));
    // Pongo valores por defecto
    ui->lblFacturado->setVisible(false);
    ui->lblImpreso->setVisible(false);
    ui->lblNumFactura->setVisible(false);
    ui->txtcNumFra->setVisible(false);


    oPedido = new Pedidos();
    oCliente3 = new Cliente();

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
    connect(ui->chklRecargoEq,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

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

    aAlbaran_action = new QAction(tr("En albaran"),this);
    aFactura_action = new QAction(tr("En factura"),this);

    connect(aAlbaran_action,SIGNAL(triggered()),this,SLOT(convertir_enAlbaran()));
    connect(aFactura_action,SIGNAL(triggered()),this,SLOT(convertir_enFactura()));

    convertir_menu = new QMenu(this);
    convertir_menu->addAction(aAlbaran_action);
    convertir_menu->addAction(aFactura_action);

    ui->btn_convertir->setMenu(convertir_menu);
    BloquearCampos(true);
    if(oPedido->RecuperarPedido("Select * from ped_cli where nPedido > -1 order by nPedido  limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("Id_Cab = '%1'").arg(ui->txtnPedido->text());
        helper.fillTable("empresa","lin_ped",filter);
    }
}

FrmPedidos::~FrmPedidos()
{
    delete ui;
    delete oPedido;
    delete oCliente3;
}

void FrmPedidos::LLenarCampos()
{
    ui->txtcAlbaran->setText(QString::number(oPedido->nAlbaran));
    ui->txtnPedido->setText(QString::number(oPedido->nPedido));
    ui->txtdFecha->setDate(oPedido->dFecha);
    //oPedido->cPedido;
    //oPedido->iId_Cliente;
    ui->txtcCodigoCliente->setText(oPedido->cCodigoCliente);
    ui->txtcCliente->setText(oPedido->cCliente);
    ui->txtcDireccion->setText(oPedido->cDireccion);
    ui->txtcDireccion2->setText(oPedido->cDireccion2);
    ui->txtcPoblacion->setText(oPedido->cPoblacion);
    ui->txtcProvincia->setText(oPedido->cProvincia);
    ui->txtcCp->setText(oPedido->cCp);

    //oPedido->id_pais;

    ui->txtcCif->setText(oPedido->cCif);
    ui->chklRecargoEq->setChecked(oPedido->lRecargoEquivalencia==1);
    ui->txtrSubtotal->setText(QString::number(oPedido->rSubtotal));
    ui->txtrImporteDescuento->setText(QString::number(oPedido->rDto));
    //oPedido->nDto;
    ui->txtrBase1->setText(QString::number(oPedido->rBase1));
    ui->txtrBase2->setText(QString::number(oPedido->rBase2));
    ui->txtrBase3->setText(QString::number(oPedido->rBase3));
    ui->txtrBase4->setText(QString::number(oPedido->rBase4));
    ui->txtnPorcentajeIva1->setText(QString::number(oPedido->nPorcentajeIVA1));
    ui->txtnPorcentajeIva2->setText(QString::number(oPedido->nPorcentajeIVA2));
    ui->txtnPorcentajeIva3->setText(QString::number(oPedido->nPorcentajeIVA3));
    ui->txtnPorcentajeIva4->setText(QString::number(oPedido->nPorcentajeIVA4));
    ui->txtrIVA1->setText(QString::number(oPedido->rImporteIva1));
    ui->txtrIVA2->setText(QString::number(oPedido->rImporteIva2));
    ui->txtrIVA3->setText(QString::number(oPedido->rImporteIva3));
    ui->txtrIVA4->setText(QString::number(oPedido->rImporteIva4));
    ui->txtnRec1->setText(QString::number(oPedido->nPorcentajeRecargoEq1));
    ui->txtnRec2->setText(QString::number(oPedido->nPorcentajeRecargoEq2));
    ui->txtnRec3->setText(QString::number(oPedido->nPorcentajeRecargoEq3));
    ui->txtnRec4->setText(QString::number(oPedido->nPorcentajeRecargoEq4));
    ui->txtrRecargoEq1->setText(QString::number(oPedido->rImporteRecargoEq1));
    ui->txtrRecargoEq2->setText(QString::number(oPedido->rImporteRecargoEq2));
    ui->txtrRecargoEq3->setText(QString::number(oPedido->rImporteRecargoEq3));
    ui->txtrRecargoEq4->setText(QString::number(oPedido->rImporteRecargoEq4));
    ui->txtrTotal1->setText(QString::number(oPedido->rTotal1));
    ui->txtrTotal2->setText(QString::number(oPedido->rTotal2));
    ui->txtrTotal3->setText(QString::number(oPedido->rTotal3));
    ui->txtrTotal4->setText(QString::number(oPedido->rTotal4));
    ui->txtrBaseTotal_2->setText(QString::number(oPedido->rBaseTotal));
    ui->txtrTotalIVA_2->setText(QString::number(oPedido->rIvaTotal));
    ui->txtrTotalRecargoEq_2->setText(QString::number(oPedido->rRecargoEqTotal));
    ui->txtrTotal_2->setText(QString::number(oPedido->rTotalAlbaran));

    ui->lblImpreso->setVisible(oPedido->lImpreso == 1);

    ui->lblFacturado->setVisible(oPedido->lFacturado == 1);
    ui->txtdFechaFactura->setVisible(oPedido->lFacturado == 1);
    ui->txtcNumFra->setVisible(oPedido->lFacturado == 1);
    ui->txtcNumFra->setText(oPedido->cFactura);
    ui->txtdFechaFactura->setDate(oPedido->dFechaFactura);

    ui->txttComentario->setText(oPedido->tComentario);
    ui->txtrACuenta->setText(QString::number(oPedido->rACuenta));
    //oPedido->lTraspasadoAlb;
    //oPedido->lTraspasadoFac;
    ui->txtcDireccionEntrega->setText(oPedido->cDireccionEntrega);
    ui->txtcDireccionEntrega_2->setText(oPedido->cDireccionEntrega2);
    ui->txtcCpEntrega->setText(oPedido->cCPEntrega);
    ui->txtcPoblacionEntrega->setText(oPedido->cPoblacionEntrega);
    ui->txtcProvinciaEntrega->setText(oPedido->cProvinciaEntrega);
    ui->txtcPaisEntrega->setText(oPedido->cPaisEntrega);
    ui->chklEnviado->setChecked(oPedido->lEnviado==1);
    ui->chklCompleto->setChecked(oPedido->lCompleto==1);
    ui->chklEntregado->setChecked(oPedido->lEntregado==1);
    ui->txtdFechaLimiteEntrega->setDate(oPedido->dFechaLimiteEntrega);
    ui->txtrTotal->setText(QString::number(oPedido->rTotalPedido));
}

void FrmPedidos::LLenarCamposCliente()
{
    ui->txtcCodigoCliente->setText(oCliente3->getcCodigoCliente());
    ui->txtcCliente->setText(oCliente3->getcNombreFiscal());
    ui->txtcDireccion->setText(oCliente3->getcDireccion1());
    ui->txtcDireccion2->setText(oCliente3->getcDireccion2());
    ui->txtcCp->setText(oCliente3->getcCP());
    ui->txtcPoblacion->setText(oCliente3->getcPoblacion());
    ui->txtcProvincia->setText(oCliente3->getcProvincia());
    //ui->txtcPais->setText(oCliente3->getcPais());
    ui->txtcCif->setText(oCliente3->getcCifNif());
    if (oCliente3->getlIRPF()==1) {
        ui->chklRecargoEq->setChecked(true);
        oPedido->lRecargoEquivalencia = (1);
    } else {
        ui->chklRecargoEq->setChecked(false);
        oPedido->lRecargoEquivalencia = (0);
    }
}

void FrmPedidos::VaciarCampos()
{
    QDate dFecha;
    ui->txtcCodigoCliente->setText("");
    ui->txtcPedido->setText("");
    ui->txtdFecha->setDate(dFecha.currentDate());
    ui->txtdFechaFactura->setDate(dFecha.currentDate());
    ui->txtcCliente->setText("");
    ui->txtcDireccion->setText("");
    ui->txtcDireccion2->setText("");
    ui->txtcCp->setText("");
    ui->txtcPoblacion->setText("");
    ui->txtcProvincia->setText("");
    //ui->txtcPais->setText("");
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
    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    ui->txtnPorcentajeIva1->setText(QString::number(Configuracion_global->ivas[keys.at(0)].value("nIVA").toDouble()));
    ui->txtnPorcentajeIva2->setText(QString::number(Configuracion_global->ivas[keys.at(1)].value("nIVA").toDouble()));
    ui->txtnPorcentajeIva3->setText(QString::number(Configuracion_global->ivas[keys.at(2)].value("nIVA").toDouble()));
    ui->txtnPorcentajeIva4->setText(QString::number(Configuracion_global->ivas[keys.at(3)].value("nIVA").toDouble()));
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
    ui->txtnPedido->setReadOnly(true);

    helper.fillTable("empresa","lin_ped","Id_Cab = -1");
}

void FrmPedidos::BloquearCampos(bool state)
{
    helper.blockTable(state);
    ui->btnAnadirLinea->setEnabled(!state);
    ui->btn_borrarLinea->setEnabled(!state);
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(state);
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(!state);
        //qDebug() << lineEdit->objectName();
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
        //qDebug() << lineEdit->objectName();
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
    ui->txtnPedido->setReadOnly(true);
}

void FrmPedidos::LLenarPedido()
{
    oPedido->nAlbaran= ui->txtcAlbaran->text().toInt();
    oPedido->nPedido=ui->txtnPedido->text().toInt();
    oPedido->dFecha=ui->txtdFecha->date();
    //oPedido->cPedido;
    //oPedido->iId_Cliente;
    oPedido->cCodigoCliente=ui->txtcCodigoCliente->text();
    oPedido->cCliente=ui->txtcCliente->text();
    oPedido->cDireccion=ui->txtcDireccion->text();
    oPedido->cDireccion2=ui->txtcDireccion2->text();
    oPedido->cPoblacion=ui->txtcPoblacion->text();
    oPedido->cProvincia=ui->txtcProvincia->text();
    oPedido->cCp=ui->txtcCp->text();

    //oPedido->id_pais;
    int i = ui->combo_pais->findText(oPedido->cPais);
    ui->combo_pais->setCurrentIndex(i);

    oPedido->cCif=ui->txtcCif->text();
    oPedido->lRecargoEquivalencia=ui->chklRecargoEq->isChecked();
    oPedido->rSubtotal=ui->txtrSubtotal->text().toDouble();
    oPedido->rDto=ui->txtrImporteDescuento->text().toDouble();
    //oPedido->nDto;
    oPedido->rBase1=ui->txtrBase1->text().toDouble();
    oPedido->rBase2=ui->txtrBase2->text().toDouble();
    oPedido->rBase3=ui->txtrBase3->text().toDouble();
    oPedido->rBase4=ui->txtrBase4->text().toDouble();
    oPedido->nPorcentajeIVA1=ui->txtnPorcentajeIva1->text().toDouble();

    oPedido->nPorcentajeIVA2=ui->txtnPorcentajeIva2->text().toDouble();
    oPedido->nPorcentajeIVA3=ui->txtnPorcentajeIva3->text().toDouble();
    oPedido->nPorcentajeIVA4=ui->txtnPorcentajeIva4->text().toDouble();
    oPedido->rImporteIva1=ui->txtrIVA1->text().toDouble();
    oPedido->rImporteIva2=ui->txtrIVA2->text().toDouble();
    oPedido->rImporteIva3=ui->txtrIVA3->text().toDouble();
    oPedido->rImporteIva4=ui->txtrIVA4->text().toDouble();
    oPedido->nPorcentajeRecargoEq1=ui->txtnRec1->text().toDouble();
    oPedido->nPorcentajeRecargoEq2=ui->txtnRec2->text().toDouble();
    oPedido->nPorcentajeRecargoEq3=ui->txtnRec3->text().toDouble();
    oPedido->nPorcentajeRecargoEq4=ui->txtnRec4->text().toDouble();
    oPedido->rImporteRecargoEq1=ui->txtrRecargoEq1->text().toDouble();
    oPedido->rImporteRecargoEq2=ui->txtrRecargoEq2->text().toDouble();
    oPedido->rImporteRecargoEq3=ui->txtrRecargoEq3->text().toDouble();
    oPedido->rImporteRecargoEq4=ui->txtrRecargoEq4->text().toDouble();
    oPedido->rTotal1=ui->txtrTotal1->text().toDouble();
    oPedido->rTotal2=ui->txtrTotal2->text().toDouble();
    oPedido->rTotal3=ui->txtrTotal3->text().toDouble();
    oPedido->rTotal4=ui->txtrTotal4->text().toDouble();
    oPedido->rBaseTotal=ui->txtrBaseTotal_2->text().toDouble();
    oPedido->rIvaTotal=ui->txtrTotalIVA_2->text().toDouble();
    oPedido->rRecargoEqTotal=ui->txtrTotalRecargoEq_2->text().toDouble();
    oPedido->rTotalAlbaran=ui->txtrTotal_2->text().toDouble();

    oPedido->lImpreso = ui->lblImpreso->isVisible();

    oPedido->lFacturado=ui->lblFacturado->isVisible();
    oPedido->lFacturado=ui->txtdFechaFactura->isVisible();
    oPedido->lFacturado=ui->txtcNumFra->isVisible();
    oPedido->cFactura=ui->txtcNumFra->text().toDouble();
    oPedido->dFechaFactura=ui->txtdFechaFactura->date();

    oPedido->tComentario=ui->txttComentario->toPlainText();
    oPedido->rACuenta=ui->txtrACuenta->text().toDouble();
    //oPedido->lTraspasadoAlb;
    //oPedido->lTraspasadoFac;
    oPedido->cDireccionEntrega=ui->txtcDireccionEntrega->text();
    oPedido->cDireccionEntrega2=ui->txtcDireccionEntrega_2->text();
    oPedido->cCPEntrega=ui->txtcCpEntrega->text();
    oPedido->cPoblacionEntrega=ui->txtcPoblacionEntrega->text();
    oPedido->cProvinciaEntrega=ui->txtcProvinciaEntrega->text();
    oPedido->cPaisEntrega=ui->txtcPaisEntrega->text();
    oPedido->lEnviado=ui->chklEnviado->isChecked();
    oPedido->lCompleto=ui->chklCompleto->isChecked();
    oPedido->lEntregado=ui->chklEntregado->isChecked();
    oPedido->dFechaLimiteEntrega=ui->txtdFechaLimiteEntrega->date();
    oPedido->rTotalPedido=ui->txtrTotal->text().toDouble();;
}

void FrmPedidos::on_btnSiguiente_clicked()
{
    int nPedido = oPedido->nPedido;
    if(oPedido->RecuperarPedido("Select * from ped_cli where nPedido >'"+QString::number(nPedido)+"' order by nPedido  limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("Id_Cab = '%1'").arg(ui->txtnPedido->text());
        helper.fillTable("empresa","lin_ped",filter);
    }
}

void FrmPedidos::on_btnAnterior_clicked()
{
    int nPedido = oPedido->nPedido;
    if(oPedido->RecuperarPedido("Select * from ped_cli where nPedido <'"+QString::number(nPedido)+"' order by nPedido desc limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("Id_Cab = '%1'").arg(ui->txtnPedido->text());
        helper.fillTable("empresa","lin_ped",filter);
    }
}

void FrmPedidos::on_btnAnadir_clicked()
{
    VaciarCampos();
    BloquearCampos(false);
    ui->txtnPedido->setText(QString::number(oPedido->NuevoNumeroPedido()));
    ui->txtcCodigoCliente->setFocus();
    editando = false;
    emit block();
}



void FrmPedidos::on_btnEditar_clicked()
{
    if (!oPedido->lFacturado)
    {
        BloquearCampos(false);
        editando = true;
        emit block();
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Editar Pedido"),tr("No se puede editar un pedido que ha sido facturado, solo los NO facturados se pueden editar")+
                             tr("<p><b> Si necesita modificar algo genere una factura nueva y realice el abono correspondiente</b>")+
                                tr("y luego si es preciso realice un nuevo pedido y facture de nuevo "),tr("OK"));
    }
}

void FrmPedidos::on_btnGuardar_clicked()
{
    LLenarPedido();
    QSqlDatabase::database("empresa").transaction();
    bool succes = true;
    int iPed = ui->txtnPedido->text().toInt();
    if(editando)
    {
        succes &= oPedido->GuardarPedido(iPed);
        succes &= oPedido->BorrarLineas(iPed);
        succes &= helper.saveTable(iPed,"empresa","lin_ped");
    }
    else
    {
        succes &= oPedido->AnadirPedido();
        succes &= oPedido->GuardarPedido(iPed);
        succes &= helper.saveTable(iPed,"empresa","lin_ped");
    }

    if(succes)
    {
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
                              tr("Error al guardar el pedido.\n")+QSqlDatabase::database("empresa").lastError().text(),
                              tr("&Aceptar"));
        QSqlDatabase::database("empresa").rollback();
    }
}

void FrmPedidos::on_botBuscarCliente_clicked()
{
    FrmBuscarCliente BuscarClientes;
    BuscarClientes.exec();
    int nId = BuscarClientes.DevolverID();
    QString cId = QString::number(nId);
    oPedido->iId_Cliente = (cId.toInt());
    oCliente3->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
    LLenarCamposCliente();
}

void FrmPedidos::on_btnDeshacer_clicked()
{
    BloquearCampos(true);
    QString cId = (ui->txtnPedido->text());
    oPedido->RecuperarPedido("Select * from ped_cli where id ="+cId+" order by id limit 1 ");
    LLenarCampos();
    QString filter = QString("Id_Cab = '%1'").arg(ui->txtnPedido->text());
    helper.fillTable("empresa","lin_ped",filter);
    emit unblock();
}

void FrmPedidos::on_btn_borrar_clicked()
{
    if (QMessageBox::question(this,tr("Borrar"),
                              tr("Esta acción no se puede deshacer.\n¿Desea continuar?"),
                              tr("Cancelar"),
                               tr("&Continuar"))== QMessageBox::Accepted)
    {
        bool succes = true;
        QSqlDatabase::database("empresa").transaction();

        QSqlQuery q(QSqlDatabase::database("empresa"));
        q.prepare("DELETE FROM ped_cli WHERE nPedido = "+ui->txtnPedido->text());
        succes &= q.exec();
        succes &= oPedido->BorrarLineas(ui->txtnPedido->text().toInt());

        if(succes)
            succes &= QSqlDatabase::database("empresa").commit();

        if(succes)
            QMessageBox::information(this,tr("Borrado"),tr("Borrado con exito"));
        else
            QSqlDatabase::database("empresa").rollback();
    }
}

void FrmPedidos::totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda)
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

void FrmPedidos::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtrBase1->setText(QString::number(base));
    ui->txtrIVA1->setText(QString::number(iva));
    ui->txtrRecargoEq1->setText(QString::number(re));
    ui->txtrTotal1->setText(QString::number(total));
}

void FrmPedidos::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtrBase2->setText(QString::number(base));
    ui->txtrIVA2->setText(QString::number(iva));
    ui->txtrRecargoEq2->setText(QString::number(re));
    ui->txtrTotal2->setText(QString::number(total));
}

void FrmPedidos::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtrBase3->setText(QString::number(base));
    ui->txtrIVA4->setText(QString::number(iva));
    ui->txtrRecargoEq3->setText(QString::number(re));
    ui->txtrTotal3->setText(QString::number(total));
}

void FrmPedidos::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtrBase4->setText(QString::number(base));
    ui->txtrIVA4->setText(QString::number(iva));
    ui->txtrRecargoEq4->setText(QString::number(re));
    ui->txtrTotal4->setText(QString::number(total));
}

void FrmPedidos::convertir_enAlbaran()
{
    //TODO FrmPedidos::convertir_enAlbaran()
}

void FrmPedidos::convertir_enFactura()
{
    //TODO FrmPedidos::convertir_enFactura()
}
