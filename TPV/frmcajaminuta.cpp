#include "frmcajaminuta.h"
#include "ui_frmcajaminuta.h"
#include "../articulo.h"

/*Pendiente de cobro:
background-color: rgb(192, 0, 0);
color: rgb(255, 255, 255);
*/

/*Cobrado:
background-color: rgb(0, 192, 0);
color: rgb(255, 255, 255);
*/
FrmCajaMinuta::FrmCajaMinuta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmCajaMinuta),
    ticket(this)
{
    ui->setupUi(this);
    ui->txtcCodigoArticulo->installEventFilter(this);
    ui->txtcCantidadArticulo->installEventFilter(this);
    ui->txtDescripcionArticulo->installEventFilter(this);
    ui->txtDescripcionArticulo->installEventFilter(this);

    ui->txtPorcIVAArticulo->setModel(Configuracion_global->iva_model);
    ui->txtPorcIVAArticulo->setModelColumn(Configuracion_global->iva_model->fieldIndex("cTipo"));
    ticket.set_table(ui->lineas);
    ui->txtcCodigoArticulo->setFocus();
}

FrmCajaMinuta::~FrmCajaMinuta()
{
    delete ui;
}

bool FrmCajaMinuta::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(target == ui->txtcCodigoArticulo)
        {
            keys_onCodigo(keyEvent->key());
        }
        else if(target == ui->txtcCantidadArticulo)
        {

        }
        else if(target == ui->txtDescripcionArticulo)
        {

        }
        else if(target == ui->txtPVPArticulo)
        {

        }
    }
    return false;
}

void FrmCajaMinuta::keys_onCodigo(int key)
{
    if(key == Qt::Key_Return)
    {
        rellenarArticulo(ui->txtcCodigoArticulo->text());

        ticket.add_linea(ui->txtcCodigoArticulo->text(),
                         ui->txtDescripcionArticulo->text(),
                         ui->txtPVPArticulo->text().toDouble(),
                         ui->txtcCantidadArticulo->text().toInt(),
                         ui->txtPVPArticulo->text().toDouble() * ui->txtcCantidadArticulo->text().toInt(),
                         Configuracion_global->ivas[ui->txtPorcIVAArticulo->currentText()].value("nIVA").toDouble(),
                         ui->txtPorcDtoArticulo->text().toDouble(),
                         ui->txttotalArticulo->text().toDouble(),
                         ui->txtSubtotalArticulo->text().toDouble(),
                         QDate::currentDate());

        ui->txtcCantidadArticulo->setValue(0);
    }
}

void FrmCajaMinuta::keys_onDescripcion(int key)
{
}

void FrmCajaMinuta::keys_onPvp(int key)
{
}

void FrmCajaMinuta::rellenarArticulo(QString cCodigo)
{
    Articulo art(this);
    art.Recuperar(QString("SELECT * FROM articulos WHERE cCodigo = '%1'").arg(cCodigo));
    ui->txtDescripcionArticulo->setText(art.cDescripcion);
    ui->txtPVPArticulo->setValue(art.rTarifa1);//FIXME que precio uso??
    if(ui->txtcCantidadArticulo->value()==0)
        ui->txtcCantidadArticulo->setValue(1);

    double iva = 0;
    double re = 0;
    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    for (int i=0;i<keys.size();i++)
    {
        if(Configuracion_global->ivas[keys.at(i)].value("id").toInt() == art.nTipoIva)
        {
            iva = Configuracion_global->ivas[keys.at(i)].value("nIVA").toDouble();
            re = Configuracion_global->ivas[keys.at(i)].value("nRegargoEquivalencia").toDouble();
            int index = ui->txtPorcIVAArticulo->findText(keys.at(i));
            ui->txtPorcIVAArticulo->setCurrentIndex(index);
            break;
        }
    }
    ui->txtPorcDtoArticulo->setValue(art.rDto);//FIXME dto ??
    double subtotal = ui->txtPVPArticulo->value() *  ui->txtcCantidadArticulo->value();

    subtotal -= ui->txtDtoArticulo->value();
    double dtoperc = ui->txtPorcDtoArticulo->value();
    dtoperc =  1 - (dtoperc / 100);
    iva = 1 + (iva /100);
    double total = subtotal * dtoperc * iva;

    if(ui->chklRecargoEq->isChecked())
    {
        re = 1 + (re/100);
        total = total * re;
    }

    ui->txttotalArticulo->setText(QString::number(total));
    ui->txtSubtotalArticulo->setText(QString::number(subtotal));
}

void FrmCajaMinuta::on_btnBuscarArt_clicked()
{
    Db_table_View searcher(this);
    searcher.set_db("empresa");
    searcher.set_table("articulos");
    searcher.set_selection("cCodigo");
    searcher.setWindowTitle(tr("Articulos"));
    QStringList headers;
    headers << tr("Codigo")<< "1" << "2" << tr("Descripción");
    searcher.set_table_headers(headers);

    searcher.set_columnHide(0);
    searcher.set_columnHide(2);
    searcher.set_columnHide(3);

    for(int i = 5; i<50;i++)
        searcher.set_columnHide(i);

    searcher.exec();
    ui->txtcCodigoArticulo->setText(searcher.selected_value);
}
