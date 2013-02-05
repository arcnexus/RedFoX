#include "frmcajaminuta.h"
#include "ui_frmcajaminuta.h"


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

    ticket.set_table(ui->lineas);
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
                         0,//FIXME rImporte en linea tpv?
                         21,//FIXME iva en linea tpv
                         ui->txtPorcDtoArticulo->text().toDouble(),
                         ui->txtSubtotalArticulo->text().toDouble(),
                         "",//FIXME cTipo en linea tpv
                         ui->txtSubtotalArticulo->text().toDouble(),
                         QDate::currentDate());
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
