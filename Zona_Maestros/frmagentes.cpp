#include "frmagentes.h"
#include "ui_frmagentes.h"

frmAgentes::frmAgentes(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmAgentes),
    menuButton(QIcon(":/Icons/PNG/proveedores_2.png"),tr("Agentes"),this)
{
    ui->setupUi(this);
    ui->graficaEstadistica->setTipo(OpenChart::Lineas);
    init();
    init_querys();
}

frmAgentes::~frmAgentes()
{
    delete ui;
}

void frmAgentes::init()
{
    setUpBusqueda();

    model_busqueda = new QSqlTableModel(this,Configuracion_global->groupDB);
    model_busqueda->setTable("agentes");
    model_busqueda->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model_comisiones = new QSqlTableModel(this,Configuracion_global->groupDB);
    model_comisiones->setTable("agentes_comisiones");
    model_comisiones->setEditStrategy(QSqlTableModel::OnFieldChange);

    model_codTarifa = new QSqlTableModel(this,Configuracion_global->groupDB);
    model_codTarifa->setTable("codigotarifa");

    mapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    connect(&mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(mapperIndexChanged(int)));
    mapper.setModel(model_busqueda);

    mapper.addMapping(ui->txtcodigo_agente,1);
    mapper.addMapping(ui->txtnombre,2);
    mapper.addMapping(ui->txtcif_nif,3);
    mapper.addMapping(ui->txttelefono1,4);
    mapper.addMapping(ui->txtmovil,5);
    mapper.addMapping(ui->txtemail,6);
    mapper.addMapping(ui->txtObservaciones,7);

    model_empresas = new QSqlQueryModel(this);

    bloquear_campos(true);
    ui->stackedWidget->setCurrentIndex(0);
}

void frmAgentes::init_querys()
{
    model_busqueda->select();
    ui->tabla_busqueda->setModel(model_busqueda);
    ui->tabla_busqueda->setColumnHidden(0,true);    

    ui->tabla_comisiones->setModel(model_comisiones);
    ui->tabla_comisiones->setColumnHidden(0,true);
    ui->tabla_comisiones->setColumnHidden(1,true);
    ui->tabla_comisiones->setColumnHidden(2,true);

    model_codTarifa->select();
    ui->lista_tarifas->setModel(model_codTarifa);
    ui->lista_tarifas->setModelColumn(1);

    model_empresas->setQuery("SELECT nombre,nombre_bd FROM empresas;",Configuracion_global->groupDB);
    ui->cboEmpresas->setModel(model_empresas);
}

void frmAgentes::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);
    this->installEventFilter(this);

    QStringList orden;
    orden  <<  tr("Nombre Fiscal") ;
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(m_busqueda,SIGNAL(hideMe()),this,SLOT(ocultarBusqueda()));
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));
    connect(m_busqueda,SIGNAL(key_Down_Pressed()),ui->tabla_busqueda,SLOT(setFocus()));


    QPushButton* add = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir"),this);
    connect(add,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(add);

    QPushButton* edit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar"),this);
    connect(edit,SIGNAL(clicked()),this,SLOT(on_btnEditar_clicked()));
    m_busqueda->addWidget(edit);

    QPushButton* print = new QPushButton(QIcon(":/Icons/PNG/print2.png"),tr("Imprimir"),this);
   // connect(print,SIGNAL(clicked()),this,SLOT(on_btnEditar_2_clicked()));//TODO
    m_busqueda->addWidget(print);

    QPushButton* del = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    //connect(del,SIGNAL(clicked()),this,SLOT(on_btn_borrar_clicked()));//TODO
    m_busqueda->addWidget(del);
}

void frmAgentes::mostrarBusqueda()
{
    ui->stackedWidget->setCurrentIndex(0);
    _showBarraBusqueda(m_busqueda);
    m_busqueda->doFocustoText();

}

void frmAgentes::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}

void frmAgentes::filter_table(QString texto, QString orden, QString modo)
{
    if(ui->stackedWidget->currentIndex() ==1)
        ui->stackedWidget->setCurrentIndex(0);
    if(modo == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    QString cSQL = "nombre like '%"+texto.trimmed()+"%' order by nombre "+modo;
    model_busqueda->setFilter(cSQL);
    model_busqueda->select();

    ui->tabla_busqueda->selectRow(0);
}

void frmAgentes::on_btnAnadir_clicked()
{
    if(model_busqueda->insertRow(model_busqueda->rowCount()))
    {
        mapper.toLast();
        bloquear_campos(false);
        editando = false;
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
         QMessageBox::critical(this,tr("Error al guardar agente"),model_busqueda->lastError().text());
}

void frmAgentes::on_btnSiguiente_clicked()
{
    if(!model_busqueda->filter().isEmpty())
    {
        int id = model_busqueda->record(mapper.currentIndex()).value("id").toInt();
        model_busqueda->setFilter("");
        model_busqueda->select();
        for(auto i=0;i<model_busqueda->rowCount();i++)
        {
            if(model_busqueda->record(i).value("id").toInt() == id)
            {
                mapper.setCurrentIndex(i);
                break;
            }
        }
    }
    ui->tabwidget->setCurrentIndex(0);
    mapper.toNext();
}

void frmAgentes::on_btnAnterior_clicked()
{
    if(!model_busqueda->filter().isEmpty())
    {
        int id = model_busqueda->record(mapper.currentIndex()).value("id").toInt();
        model_busqueda->setFilter("");
        model_busqueda->select();
        for(auto i=0;i<model_busqueda->rowCount();i++)
        {
            if(model_busqueda->record(i).value("id").toInt() == id)
            {
                mapper.setCurrentIndex(i);
                break;
            }
        }
    }
    ui->tabwidget->setCurrentIndex(0);
    mapper.toPrevious();    
}

void frmAgentes::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    mostrarBusqueda();
}

void frmAgentes::on_btnEditar_clicked()
{
    if(sender()!= ui->btnEditar)
    {
        if(!ui->tabla_busqueda->currentIndex().isValid())
            return;
        on_tabla_busqueda_doubleClicked(ui->tabla_busqueda->currentIndex());
    }
    bloquear_campos(false);
    editando = true;
}

void frmAgentes::on_btnGuardar_clicked()
{
   bool b = mapper.submit();
   b &= model_busqueda->submitAll();

   if(!b)
       QMessageBox::critical(this,tr("Error al guardar agente"),model_busqueda->lastError().text());
   else if(!editando)
       mapper.toLast();
   bloquear_campos(true);
}

void frmAgentes::on_btnDeshacer_clicked()
{
    model_busqueda->revertAll();
    mapper.revert();
    bloquear_campos(true);
}

void frmAgentes::on_btnListados_clicked()
{
    //TODO frmAgentes::on_btnListados_clicked()
}

void frmAgentes::on_btnBorrar_clicked()
{
    auto index = -1;
    if(ui->stackedWidget->currentIndex() == 0)
    {
        if(!ui->tabla_busqueda->currentIndex().isValid())
            return;
        index = ui->tabla_busqueda->currentIndex().row();
    }
    else
        index = mapper.currentIndex();
    if(QMessageBox::question(this,tr("¿Borrar agente?")
                             ,tr("¿Realmente desea borrar el agente?\nEsta accion no se puede deshacer...")
                             ,tr("No"),tr("Sí")) == QMessageBox::Accepted)
    {
        if(model_busqueda->removeRow(index))
        {
            if(model_busqueda->submitAll())
            {
                TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Agente borrado con éxito"));
                model_busqueda->setFilter("");
                model_busqueda->select();
                mapper.toFirst();
            }
            else
                QMessageBox::critical(this,tr("Error al borrar agente"),model_busqueda->lastError().text());
        }
        else
            QMessageBox::critical(this,tr("Error al borrar agente"),model_busqueda->lastError().text());
    }
}

void frmAgentes::bloquear_campos(bool state)
{
    ui->txtcodigo_agente->setReadOnly(state);
    ui->txtnombre->setReadOnly(state);
    ui->txtcif_nif->setReadOnly(state);
    ui->txttelefono1->setReadOnly(state);
    ui->txtmovil->setReadOnly(state);
    ui->txtemail->setReadOnly(state);
    ui->txtObservaciones->setReadOnly(state);

    ui->btnGuardar->setEnabled(!state);
    ui->btnDeshacer->setEnabled(!state);

    ui->btnAnterior->setEnabled(state);
    ui->btnSiguiente->setEnabled(state);
    ui->btnEditar->setEnabled(state);
    ui->btnBorrar->setEnabled(state);
    ui->btnListados->setEnabled(state);

    ui->btn_addComision->setEnabled(!state);
    ui->btn_removeComision->setEnabled(!state);

    if(state)
    {
        ui->tabla_comisiones->setEditTriggers(QTableView::NoEditTriggers);
        emit unblock();
    }
    else
    {
        ui->tabla_comisiones->setEditTriggers(QTableView::AllEditTriggers);
        emit block();
    }

    m_busqueda->block(!state);
}

bool frmAgentes::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Show && obj == this)
    {
        init_querys();
    }

    if(event->type() == QEvent::Resize)
        _resizeBarraBusqueda(m_busqueda);

    return MayaModule::eventFilter(obj,event);
}

void frmAgentes::on_tabla_busqueda_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    mapper.setCurrentIndex(index.row());
    ui->tabwidget->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(1);    
    ocultarBusqueda();
}

void frmAgentes::mapperIndexChanged(int i)
{
    ui->btnAnterior->setEnabled(i>0);
    ui->btnSiguiente->setEnabled(i<model_busqueda->rowCount()-1);
}

void frmAgentes::on_btn_addComision_clicked()
{
    if(!ui->lista_tarifas->currentIndex().isValid())
        return;
    int row = model_comisiones->rowCount();
    model_comisiones->insertRow(row);
    model_comisiones->setData(model_comisiones->index(row,1),model_busqueda->record(mapper.currentIndex()).value("id").toInt());
    model_comisiones->setData(model_comisiones->index(row,2),model_codTarifa->record(ui->lista_tarifas->currentIndex().row()).value("id").toInt());
    model_comisiones->setData(model_comisiones->index(row,3),ui->spin_desde->value());
    model_comisiones->setData(model_comisiones->index(row,4),ui->spin_hasta->value());
    model_comisiones->setData(model_comisiones->index(row,5),ui->spin_comision->value());
    model_comisiones->submitAll();
}

void frmAgentes::on_btn_removeComision_clicked()
{
    if(ui->tabla_comisiones->currentIndex().isValid())
        if(model_comisiones->removeRow(ui->tabla_comisiones->currentIndex().row()))
            if(model_comisiones->submitAll())
                model_comisiones->select();
}

void frmAgentes::on_tabwidget_currentChanged(int index)
{
    if(index == 1)
    {
        model_codTarifa->select();

        ui->lista_tarifas->setCurrentIndex(model_codTarifa->index(0,0));
        model_comisiones->setFilter(QString("id_agente = %1 and id_tarifa = %2")
                                    .arg(model_busqueda->record(mapper.currentIndex()).value("id").toInt())
                                    .arg(model_codTarifa->record(0).value("id").toInt()));
        model_comisiones->select();

        model_comisiones->setHeaderData(3,Qt::Horizontal,tr("Ventas desde"));
        model_comisiones->setHeaderData(4,Qt::Horizontal,tr("Ventas hasta"));
        model_comisiones->setHeaderData(5,Qt::Horizontal,tr("Comisión (%)"));

        int siz = (ui->tabla_comisiones->width() -ui->tabla_comisiones->verticalHeader()->width()) / 3;

        ui->tabla_comisiones->setColumnWidth(3,siz);
        ui->tabla_comisiones->setColumnWidth(4,siz);
    }
    if(index == 2)
        on_cboEmpresas_currentIndexChanged(ui->cboEmpresas->currentIndex());
}

void frmAgentes::on_lista_tarifas_clicked(const QModelIndex &index)
{
    model_comisiones->setFilter(QString("id_agente = %1 and id_tarifa = %2")
                                .arg(model_busqueda->record(mapper.currentIndex()).value("id").toInt())
                                .arg(model_codTarifa->record(index.row()).value("id").toInt()));
    model_comisiones->select();
}

void frmAgentes::on_cboEmpresas_currentIndexChanged(int index)
{
    int id_agente = model_busqueda->record(mapper.currentIndex()).value("id").toInt();
    if(id_agente == 0)
        return;

    QString bd_name = model_empresas->record(index).value("nombre_bd").toString();
    QSqlDatabase db = QSqlDatabase::cloneDatabase(Configuracion_global->groupDB,"estadistica_emp");

    db.setDatabaseName(bd_name);
    if(!db.open())
    {
        QMessageBox::critical(this,tr("Error al abrir base de datos"),db.lastError().text());
        QSqlDatabase::removeDatabase("estadistica_emp");
        return;
    }
    QString base = "SELECT sum(total) FROM `cab_fac` where id_agente = %1 and fecha > '2014-%2-01' and fecha < '2014-%2-31';";

    QList<QLineEdit*> _lines;
    _lines << ui->txtEnero << ui->txtFebrero << ui->txtMarzo << ui->txtAbril << ui->txtMayo << ui->txtJunio;
    _lines << ui->txtjulio << ui->txtAgosto << ui->txtSeptiembre << ui->txtOctubre << ui->txtNoviembre << ui->txtDiciembre;

    QStringList stops;
    stops << tr("ENERO")<< tr("FEBRERO")<< tr("MARZO")<< tr("ABRIL")<< tr("MAYO")<< tr("JUNIO");
    stops << tr("JULIO")<< tr("AGOSTO")<< tr("SEPTIEMBRE")<< tr("OCTUBRE")<< tr("NOVIEMBRE")<< tr("DICIEMBRE");

    ui->graficaEstadistica->Clear();
    ui->graficaEstadistica->addLineaStops(stops);
    QVector<float>v;
    for(int i = 0; i<12; i++)
        v.append(0);

    QSqlQuery q(db);
    for(int i = 1; i<13; i++)
    {
        if(q.exec(base.arg(id_agente).arg(i,2,10,QLatin1Char('0'))) && q.next())
        {
            _lines.at(i-1)->setText(Configuracion_global->toFormatoMoneda(q.record().value(0).toString()));
            v[i-1] = q.record().value(0).toFloat();
        }
    }
    ui->graficaEstadistica->addItem(tr("Ventas"),v,QColor::fromRgb(150,150,150));
    ui->graficaEstadistica->repaint();
    db.close();
    QSqlDatabase::removeDatabase("estadistica_emp");
}
