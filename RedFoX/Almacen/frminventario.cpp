#include "frminventario.h"
#include "ui_frminventario.h"

frmInventario::frmInventario(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmInventario),
  menuButton(QIcon(":/Icons/PNG/inventario.png"),tr("Inventario"),this),
  shortCut(new QPushButton(QIcon(":/Icons/PNG/inventario.png"),"",this))

{
    ui->setupUi(this);
    ui->btnSincronizar->setVisible(false);
    // ------------------------
    // Tabla
    // ------------------------
    m = new QSqlTableModel(this,Configuracion_global->groupDB);
    m->setTable("inventario");
    m->setSort(1,Qt::AscendingOrder);
    m->select();
    ui->tabla->setModel(m);
    formato_tabla(m);
    //---------------------
    // combo indices
    //---------------------
    QStringList orders;
    orders << tr("código") <<tr("código Barras") <<tr("referencia proveedor") <<tr("descripción");
    ui->cboOrder->addItems(orders);
    shortCut->setStyleSheet("background-color: rgb(133, 170, 142)");

    connect(m,&QSqlTableModel::dataChanged,[this](const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles ){
       ui->txtBuscar->setText("");
       ui->txtBuscar->setFocus();
    });
}

frmInventario::~frmInventario()
{
    delete ui;
}

void frmInventario::formato_tabla(QSqlTableModel *modelo)
{
    QStringList headers;
    QVariantList sizes;

    headers << "id" << tr("código") <<tr("C.Barras") <<tr("R. Fabricante") << tr("Descripción") << tr("Stock Real");
    headers <<"Stock Almacen";
    sizes << 0 << 120 << 120 <<120 <<500 << 120 <<120;
    for(int i = 0; i < headers.length();i++)
    {
        ui->tabla->setColumnWidth(i,sizes.at(i).toInt());
        modelo->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
    //ui->tabla->setColumnHidden(6,true);
}

void frmInventario::on_txtBuscar_textEdited(const QString &arg1)
{
    QHash <QString,QString> h;
    h["código"] = "codigo";
    h["código Barras"] = "codigo_barras";
    h["referencia proveedor"] = "referencia_proveedor";
    h["descripción"] = "descripcion";

    QString indice = h.value(ui->cboOrder->currentText());
    m->setFilter(indice +" like '%"+arg1.trimmed()+"%'");
    m->select();
    formato_tabla(m);
}

void frmInventario::on_btnSincronizar_clicked()
{
//    QSqlQuery stock(Configuracion_global->groupDB);
//    QString cSql = "update articulos set stock_fisico_almacen = stock_real where id >0;";
//    if(stock.exec(cSql))
//        TimedMessageBox::Box(this,tr("Se han sincronizado los stocks"));
}

void frmInventario::on_btnBuscar_clicked()
{
    ui->txtBuscar->setFocus();
}

void frmInventario::on_btnImprimir_clicked()
{
    FrmDialogoImprimir dlg_print(this);
    dlg_print.set_preview(false);
    if(dlg_print.exec() == dlg_print.Accepted)
    {

        int valor = dlg_print.get_option();
        QMap <QString,QString> parametros_sql;
        parametros_sql["General.articulos"]= "id > 0 order by codigo";
        QString report = "inventario_valorado";


        QMap <QString,QString> parametros;
        parametros["fecha"] = QDate::currentDate().toString("dd/MM/yyyy");
//        QSqlQuery queryValor(Configuracion_global->groupDB);
//        if(queryValor.exec("select sum(valor_stock) as valor from articulos where id >0"))
//            queryValor.next();
//        else
//            QMessageBox::warning(this,tr("Inventario"),
//                                 tr("No se pudo calcular el valor del stock: %1").arg(queryValor.lastError().text()));
//        double nValor = queryValor.value(0).toDouble();
//        QString cValor = QString::number(nValor,'f',Configuracion_global->decimales_campos_totales);
//        parametros["valor"] = Configuracion_global->toFormatoMoneda(cValor);
        switch (valor) {
        case 1: // Impresora
            Configuracion::ImprimirDirecto(report,parametros_sql,parametros);
            break;
        case 2: // email
            // TODO - enviar pdf por mail
            break;
        case 3: // PDF
            Configuracion::ImprimirPDF(report,parametros_sql,parametros);
            break;
        case 4: //preview
            Configuracion::ImprimirPreview(report,parametros_sql,parametros);
            break;
        default:
            break;
        }


    }
}
