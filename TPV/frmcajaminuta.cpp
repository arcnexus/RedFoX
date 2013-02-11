#include "frmcajaminuta.h"
#include "ui_frmcajaminuta.h"
#include "../Almacen/articulo.h"
#include "../Auxiliares/spinboxdelegate.h"

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
    linea_row = 0;
    linea_column  =0;
    this->setFocusPolicy(Qt::ClickFocus);

    ui->setupUi(this);
    connect(ui->lineas,SIGNAL(itemSelectionChanged()),this,SLOT(linea_itemSelectionChanged()));
    //ui->txtPVPArticulo->setValidator(Configuracion_global->validator_cantidad);
    connect(ui->txtPVPArticulo,SIGNAL(editingFinished()),Configuracion_global,SLOT(format_text()));
    QList<QWidget*> childs = this->findChildren<QWidget*>();
    QWidget* wid;
    foreach (wid, childs) {
        wid->installEventFilter(this);
    }

    ui->lineas->setItemDelegateForColumn(2,new SpinBoxDelegate(ui->lineas,false,-99999999999999999.99,99999999999999999.99));
    ui->lineas->setItemDelegateForColumn(2,new SpinBoxDelegate(ui->lineas,true,-99999999999999999.99,99999999999999999.99));
    ui->txtPorcIVAArticulo->setModel(Configuracion_global->iva_model);
    ui->txtPorcIVAArticulo->setModelColumn(Configuracion_global->iva_model->fieldIndex("cTipo"));
    ticket.set_table(ui->lineas);
    ui->txtcCodigoArticulo->setFocus();

    bloquearCaja(true);
}

FrmCajaMinuta::~FrmCajaMinuta()
{
    delete ui;
}

bool FrmCajaMinuta::eventFilter(QObject *target, QEvent *event)
{
    QList<QWidget*> childs = ui->lineas->findChildren<QWidget*>();
    QWidget* wid;
    foreach (wid, childs) {
        wid->installEventFilter(this);
    }
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        if(key==Qt::Key_F1)
        {
            ui->txtcCantidadArticulo->setFocus();
            ui->txtcCantidadArticulo->findChildren<QLineEdit*>().at(0)->setText("");
        }
        else if(key==Qt::Key_F2)
        {
            ui->txtDescripcionArticulo->setFocus();
            ui->txtDescripcionArticulo->setText("");
        }
        else if(key==Qt::Key_F3)
        {
            ui->txtPVPArticulo->setFocus();
            ui->txtPVPArticulo->setText("");
        }
        else if(key==Qt::Key_F4)
        {
            ui->txtPorcDtoArticulo->setFocus();
            ui->txtPorcDtoArticulo->findChildren<QLineEdit*>().at(0)->setText("");
        }
        else if(key==Qt::Key_F5)
        {
            ui->txtDtoArticulo->setFocus();
            ui->txtDtoArticulo->findChildren<QLineEdit*>().at(0)->setText("");
        }
        else if(key==Qt::Key_F6)
        {
            ui->txtPorcIVAArticulo->setFocus();
        }
        else if(key==Qt::Key_F7)
        {
            if(ui->lineas->rowCount() != 0)
            {
                int row = ui->lineas->rowCount() -1;
                ui->lineas->clearSelection();
                ui->lineas->setCurrentCell(row,2,QItemSelectionModel::Select);
                ui->lineas->setFocus();
            }
        }


        else if(target == ui->txtcCodigoArticulo)
        {
            if(keys_onCodigo(key))
                return true;
        }
        else if(target == ui->lineas)
        {
            if(keys_lineas(key))
                return true;
        }
        else if(((key==Qt::Key_Down)||(key==Qt::Key_Up)||(key==Qt::Key_Left)||(key==Qt::Key_Right)) && ui->lineas->hasFocus())
        {
            switch (key)
            {
            case Qt::Key_Down:
                if(linea_row == ui->lineas->rowCount()-1)
                    linea_row++;
                break;
            case Qt::Key_Up:
                if(linea_row != 0)
                    linea_row--;
                break;
            case Qt::Key_Left:
                if(linea_column != 0)
                    linea_column--;
                break;
            case Qt::Key_Right:
                if(linea_column != ui->lineas->columnCount()-1)
                    linea_column++;
                break;
            }
            ui->lineas->setCurrentCell(linea_row,linea_column,QItemSelectionModel::Select);
            return true;

        }
        if((key==Qt::Key_Return)||(key==Qt::Key_Enter))
            ui->txtcCodigoArticulo->setFocus();
    }
    return false;
}

bool FrmCajaMinuta::keys_onCodigo(int key)
{
    if((key==Qt::Key_Return)||(key==Qt::Key_Enter)|| key==Qt::Key_Tab)
    {
        if(rellenarArticulo(ui->txtcCodigoArticulo->text()))
        {
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
        }
        ui->txtcCantidadArticulo->setValue(0);
        ui->txtcCodigoArticulo->setText("");
        ui->txtDescripcionArticulo->setText("");
        ui->txtPVPArticulo->setText("");
        ui->txtPorcDtoArticulo->setValue(0);
        ui->txtDtoArticulo->setValue(0);
        ui->txttotalArticulo->setText("");
        ui->txtSubtotalArticulo->setText("");
        ui->txtcCodigoArticulo->setFocus();
        return true;
    }
    return false;
}

bool FrmCajaMinuta::keys_lineas(int key)
{
    if(caja_abierta)
    {
        if((key==Qt::Key_Return)||(key==Qt::Key_Enter))
            return false;
        if((key==Qt::Key_Down)||(key==Qt::Key_Up)||(key==Qt::Key_Left)||(key==Qt::Key_Right))
            return false;
        else
        {
            if((key == Qt::Key_Delete)||(key == Qt::Key_Backspace))
            {
                if(ui->lineas->rowCount() != 0)
                {
                    ticket.remove_linea(ui->lineas->currentRow());
                }
            }
            else if(key== Qt::Key_Plus)
            {
                if(ui->lineas->currentColumn() == 2)
                {
                    int value = ui->lineas->currentItem()->text().toInt();
                    ui->lineas->currentItem()->setText(QString::number(++value));
                }
            }
            else if(key== Qt::Key_Minus)
            {
                if(ui->lineas->currentColumn() == 2)
                {
                    int value = ui->lineas->currentItem()->text().toInt();
                    ui->lineas->currentItem()->setText(QString::number(--value));
                }
            }
            return true;
        }
    }
    return true;
}

bool FrmCajaMinuta::rellenarArticulo(QString cCodigo)
{
    Articulo art(this);
    if(art.Recuperar(QString("SELECT * FROM articulos WHERE cCodigo = '%1'").arg(cCodigo)))
    {
        ui->txtDescripcionArticulo->setText(art.cDescripcion);
        ui->txtPVPArticulo->setText(QString::number(art.pvp));
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
        double subtotal = ui->txtPVPArticulo->text().toDouble() *  ui->txtcCantidadArticulo->value();

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
        return true;
    }
    return false;
}

void FrmCajaMinuta::on_btnBuscarArt_clicked()
{
    Db_table_View searcher(this);
    searcher.set_db("terra");
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

void FrmCajaMinuta::linea_itemSelectionChanged()
{
    linea_row = ui->lineas->currentRow();
    linea_column = ui->lineas->currentColumn();
}

void FrmCajaMinuta::focusInEvent(QFocusEvent *e)
{
    Q_UNUSED(e);
    ui->txtcCodigoArticulo->setFocus();
    ui->lineas->clearSelection();
}

void FrmCajaMinuta::on_btn_abrirCerrarCaja_clicked()
{
    if(ui->btn_abrirCerrarCaja->text() == tr("Abrir Caja"))
    {
        ui->btn_abrirCerrarCaja->setText(tr("Cerrar caja"));
        ui->btn_abrirCerrarCaja->setIcon(QIcon(":/Icons/PNG/cierrecaja.png"));
        bloquearCaja(false);
        emit block();
    }
    else
    {
        ui->btn_abrirCerrarCaja->setText(tr("Abrir Caja"));
        ui->btn_abrirCerrarCaja->setIcon(QIcon(":/Icons/PNG/abrecaja.png"));
        //TODO Control de cierre de caja
        bloquearCaja(true);
        emit unblock();
    }

}

void FrmCajaMinuta::bloquearCaja(bool state)
{
    caja_abierta = !state;
    if(state)
        ui->lineas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    else
        ui->lineas->setEditTriggers(QAbstractItemView::AllEditTriggers);

    ui->btnCambioUsuario->setEnabled(!state);
    ui->btnCobrarTicket->setEnabled(!state);
    ui->btnPonerEspera->setEnabled(!state);
    ui->btnRecuperarEspera->setEnabled(!state);
    ui->btnVerAgenda->setEnabled(!state);
    ui->btnNuevaVisita->setEnabled(!state);
    ui->btnImprimirInforme->setEnabled(!state);
    ui->botBuscarCliente->setEnabled(!state);

    ui->frame_3->setEnabled(!state);

    ui->chklRecargoEq->setEnabled(!state);

    QList<QLineEdit *> lineEditList = ui->tabWidget->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(state);
    }
    lineEditList = ui->tab_desglose->findChildren<QLineEdit *>();
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(true);
    }
    lineEditList = ui->tab_detalle->findChildren<QLineEdit *>();
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(true);
    }
}
