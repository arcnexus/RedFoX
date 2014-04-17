#include "dlgdivisionesalmacen.h"
#include "ui_dlgdivisionesalmacen.h"

DlgDivisionesAlmacen::DlgDivisionesAlmacen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDivisionesAlmacen)
{
    ui->setupUi(this);

    model_secciones   = new QSqlTableModel(this,Configuracion_global->groupDB);
    model_familias    = new QSqlTableModel(this,Configuracion_global->groupDB);
    model_subfamilias = new QSqlTableModel(this,Configuracion_global->groupDB);

    model_secciones->setTable("secciones");
    model_familias->setTable("familias");
    model_subfamilias->setTable("subfamilias");

    model_secciones->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_familias->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_subfamilias->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model_secciones->select();

    ui->listSecciones->setModel(model_secciones);
    ui->listFamilias->setModel(model_familias);
    ui->listSubfamilias->setModel(model_subfamilias);

    ui->listSecciones->setModelColumn(model_secciones->fieldIndex("seccion"));
    ui->listFamilias->setModelColumn(model_familias->fieldIndex("familia"));
    ui->listSubfamilias->setModelColumn(model_subfamilias->fieldIndex("sub_familia"));

    editando= false;
}

DlgDivisionesAlmacen::~DlgDivisionesAlmacen()
{
    delete ui;
}

void DlgDivisionesAlmacen::on_listSecciones_clicked(const QModelIndex &index)
{
    QString filter = QString("id_seccion = %1").arg(model_secciones->record(index.row()).value("id").toInt());
    model_familias->setFilter(filter);
    model_familias->select();

    model_subfamilias->setFilter("id < 0");
    model_subfamilias->select();

    ui->txtcodigo->setText(model_secciones->record(index.row()).value("codigo").toString());
    ui->txtnombre->setText(model_secciones->record(index.row()).value("seccion").toString());

    load_imagen(model_secciones, index.row());
    set_treeLabel(Seccion);
}

void DlgDivisionesAlmacen::on_listFamilias_clicked(const QModelIndex &index)
{
    QString filter = QString("id_familia = %1").arg(model_familias->record(index.row()).value("id").toInt());
    model_subfamilias->setFilter(filter);
    model_subfamilias->select();

    ui->txtcodigo->setText(model_familias->record(index.row()).value("codigo").toString());
    ui->txtnombre->setText(model_familias->record(index.row()).value("familia").toString());

    load_imagen(model_familias, index.row());
    set_treeLabel(Familia);
}

void DlgDivisionesAlmacen::on_listSubfamilias_clicked(const QModelIndex &index)
{
    ui->txtcodigo->setText(model_subfamilias->record(index.row()).value("codigo").toString());
    ui->txtnombre->setText(model_subfamilias->record(index.row()).value("sub_familia").toString());

    load_imagen(model_subfamilias, index.row());
    set_treeLabel(SubFamilia);
}

void DlgDivisionesAlmacen::set_treeLabel(editState e)
{
    QString tree;
    switch (e) {
    case DlgDivisionesAlmacen::Seccion:
        tree= QString("%1");
        ui->btnAddFamily->setEnabled(true);
        ui->btnAddSub->setEnabled(false);
        break;
    case DlgDivisionesAlmacen::Familia:
        tree = QString("%1 > %2");
        ui->btnAddFamily->setEnabled(true);
        ui->btnAddSub->setEnabled(true);
        break;
    case DlgDivisionesAlmacen::SubFamilia:
        tree = QString("%1 > %2 > %3");
        break;
    }

    tree = tree
            .arg(ui->listSecciones->currentIndex().data().toString())
            .arg(ui->listFamilias->currentIndex().data().toString())
            .arg(ui->listSubfamilias->currentIndex().data().toString());
    ui->lbl_tree->setText(tree);
    editstatus = e;
}

void DlgDivisionesAlmacen::bloquear_campos(bool state)
{
    ui->listSecciones->setEnabled(state);
    ui->listFamilias->setEnabled(state);
    ui->listSubfamilias->setEnabled(state);

    ui->btnAddSeccion->setEnabled(state);
    ui->btnAddFamily->setEnabled(false);
    ui->btnAddSub->setEnabled(false);

    ui->txtcodigo->setReadOnly(state);
    ui->txtnombre->setReadOnly(state);

    ui->btnGuardar->setEnabled(!state);
    ui->btnDeshacer->setEnabled(!state);
    ui->btnCambiarimagen->setEnabled(!state);

    ui->btnEdit->setEnabled(state);
}

void DlgDivisionesAlmacen::vaciar_campos()
{
    ui->txtcodigo->clear();
    ui->txtnombre->clear();
    ui->lbl_imagen->setPixmap(QPixmap());
    _current_img.clear();
}

void DlgDivisionesAlmacen::add_new(DlgDivisionesAlmacen::editState e)
{
    editando = false;
    editstatus = e;
    vaciar_campos();
    bloquear_campos(false);
}

void DlgDivisionesAlmacen::on_btnEdit_clicked()
{
    bloquear_campos(false);
    editando = true;
}

void DlgDivisionesAlmacen::on_btnDeshacer_clicked()
{
    bloquear_campos(true);
    switch (editstatus) {
    case DlgDivisionesAlmacen::Seccion:
        on_listSecciones_clicked(ui->listSecciones->currentIndex());
        break;
    case DlgDivisionesAlmacen::Familia:
        on_listFamilias_clicked(ui->listFamilias->currentIndex());
        break;
    case DlgDivisionesAlmacen::SubFamilia:
        on_listSubfamilias_clicked(ui->listSubfamilias->currentIndex());
        break;
    }
}

bool DlgDivisionesAlmacen::save_seccion(int row)
{
    QByteArray ba_64 = _current_img.toBase64();
    bool succes = model_secciones->setData(model_secciones->index(row,model_secciones->fieldIndex("seccion")),ui->txtnombre->text());
    succes &= model_secciones->setData(model_secciones->index(row,model_secciones->fieldIndex("codigo")),ui->txtcodigo->text());
    succes &= model_secciones->setData(model_secciones->index(row,model_secciones->fieldIndex("image")),ba_64);
    succes &= model_secciones->submitAll();
    if(!succes)
        model_secciones->revertAll();
    return succes;
}

bool DlgDivisionesAlmacen::save_family(int row)
{
    QByteArray ba_64 = _current_img.toBase64();
    bool succes = model_familias->setData(model_familias->index(row,model_familias->fieldIndex("familia")),ui->txtnombre->text());
    succes &= model_familias->setData(model_familias->index(row,model_familias->fieldIndex("codigo")),ui->txtcodigo->text());
    succes &= model_familias->setData(model_familias->index(row,model_familias->fieldIndex("image")),ba_64);
    succes &= model_familias->submitAll();
    if(!succes)
        model_familias->revertAll();
    return succes;
}

bool DlgDivisionesAlmacen::save_subfamily(int row)
{
    QByteArray ba_64 = _current_img.toBase64();
    bool succes = model_subfamilias->setData(model_subfamilias->index(row,model_subfamilias->fieldIndex("sub_familia")),ui->txtnombre->text());
    succes &= model_subfamilias->setData(model_subfamilias->index(row,model_subfamilias->fieldIndex("codigo")),ui->txtcodigo->text());
    succes &= model_subfamilias->setData(model_subfamilias->index(row,model_subfamilias->fieldIndex("image")),ba_64);
    succes &= model_subfamilias->submitAll();
    if(!succes)
        model_subfamilias->revertAll();
    return succes;
}

bool DlgDivisionesAlmacen::save_new_seccion()
{
    int row_count = model_secciones->rowCount();
    bool succes = model_secciones->insertRow(row_count);
    succes &= save_seccion(row_count);
    return succes;
}

bool DlgDivisionesAlmacen::save_new_family()
{
    int row_count = model_familias->rowCount();
    bool succes = model_familias->insertRow(row_count);
    int id_seccion = model_secciones->record(ui->listSecciones->currentIndex().row()).value("id").toInt();
    succes &= model_familias->setData(model_familias->index(row_count,model_familias->fieldIndex("id_seccion")),id_seccion);
    succes &= save_family(row_count);
    return succes;
}

bool DlgDivisionesAlmacen::save_new_subfamily()
{
    int row_count = model_subfamilias->rowCount();
    bool succes = model_subfamilias->insertRow(row_count);
    int id_familia = model_familias->record(ui->listFamilias->currentIndex().row()).value("id").toInt();
    succes &= model_subfamilias->setData(model_subfamilias->index(row_count,model_subfamilias->fieldIndex("id_familia")),id_familia);
    succes &= save_subfamily(row_count);
    return succes;
}

bool DlgDivisionesAlmacen::save_edit_seccion()
{
    return save_seccion(ui->listSecciones->currentIndex().row());
}

bool DlgDivisionesAlmacen::save_edit_family()
{
    return save_family(ui->listFamilias->currentIndex().row());
}

bool DlgDivisionesAlmacen::save_edit_subfamily()
{
    return save_subfamily(ui->listSubfamilias->currentIndex().row());
}

void DlgDivisionesAlmacen::on_btnGuardar_clicked()
{    
    bool succes;
    QString target;
    QString error;

    if(editando)
    {
        switch (editstatus) {
        case DlgDivisionesAlmacen::Seccion:
            succes = save_edit_seccion();
            break;
        case DlgDivisionesAlmacen::Familia:
            succes = save_edit_family();
            break;
        case DlgDivisionesAlmacen::SubFamilia:
            succes = save_edit_subfamily();
            break;
        }
    }
    else
    {
        switch (editstatus) {
        case DlgDivisionesAlmacen::Seccion:
            succes = save_new_seccion();
            break;
        case DlgDivisionesAlmacen::Familia:
            succes = save_new_family();
            break;
        case DlgDivisionesAlmacen::SubFamilia:
            succes = save_new_subfamily();
            break;
        }
    }

    switch (editstatus) {
    case DlgDivisionesAlmacen::Seccion:
        target = tr("Sección");
        error = model_secciones->lastError().text();
        break;
    case DlgDivisionesAlmacen::Familia:
        target = tr("Familia");
        error = model_familias->lastError().text();
        break;
    case DlgDivisionesAlmacen::SubFamilia:
        target = tr("SubFamilia");
        error = model_subfamilias->lastError().text();
        break;
    }

    if(succes)
        TimedMessageBox::Box(this,tr("%1 guardada con éxito").arg(target));
    else
        QMessageBox::critical(this,tr("Error al guardar %1").arg(target),error);
    bloquear_campos(succes);
}

void DlgDivisionesAlmacen::on_btnAddSeccion_clicked()
{
    add_new(Seccion);
}

void DlgDivisionesAlmacen::on_btnAddFamily_clicked()
{
    add_new(Familia);
}

void DlgDivisionesAlmacen::on_btnAddSub_clicked()
{
    add_new(SubFamilia);
}

void DlgDivisionesAlmacen::on_btnCambiarimagen_clicked()
{
    QString ficheroImagen = QFileDialog::getOpenFileName(this,tr("Abrir fichero de imagen MAX 15MB"),"","Imagenes (*.bmp *.png *.xpm *.jpg)");
    if (!ficheroImagen.isEmpty())
    {
        QByteArray ba;
        QFile f(ficheroImagen);
        if(f.size() > 15777215) //15 MB
        {
            QMessageBox::warning(this,tr("Error de tamaño"),tr("Archivo demasiado grande.\Tamaño máximo 15Mb."));
            return;
        }
        if(f.open(QIODevice::ReadOnly))
            ba = f.readAll();
        f.close();
        load_imagen(ba);
    }
}

void DlgDivisionesAlmacen::load_imagen(QSqlTableModel *model , int row)
{
    QByteArray ba1 = model->record(row).value("image").toByteArray();
    QByteArray ba_64 = QByteArray::fromBase64(ba1);
    load_imagen(ba_64);
}

void DlgDivisionesAlmacen::load_imagen(QByteArray b)
{
    QPixmap px;
    px.loadFromData(b);
    ui->lbl_imagen->setPixmap(px);
    _current_img = b;
}

bool DlgDivisionesAlmacen::borrar_subfamilias(int id_familia)
{
    QString error;
    return SqlCalls::SqlDelete("subfamilias",Configuracion_global->groupDB,QString("id_familia = %1").arg(id_familia),error);
}

bool DlgDivisionesAlmacen::borrar_familia(int id_seccion)
{
    QString error;
    bool success = true;
    QMap<int,int> familias = SqlCalls::SelectMap<int,int>("familias","id","id",QString("id_seccion = %1").arg(id_seccion),Configuracion_global->groupDB,error);
    QMapIterator<int,int> _it(familias);

    while (_it.hasNext())
        success&= borrar_subfamilias(_it.next().key());

    success &= SqlCalls::SqlDelete("familias",Configuracion_global->groupDB,QString("id_seccion = %1").arg(id_seccion),error);
    return success;
}

void DlgDivisionesAlmacen::refresh()
{
    model_secciones->select();
    model_familias->select();
    model_subfamilias->select();
}


void DlgDivisionesAlmacen::on_btnBorrarSec_clicked()
{
    if(ui->listSecciones->currentIndex().isValid())
    {
        Configuracion_global->transaction();

        int id_seccion = model_secciones->record(ui->listSecciones->currentIndex().row()).value("id").toInt();

        bool success = model_secciones->removeRow(ui->listSecciones->currentIndex().row());
        success &= model_secciones->submitAll();
        success &= borrar_familia(id_seccion);
        if(!success)
        {
            QMessageBox::critical(this,tr("Error al borrar sección"),model_secciones->lastError().text()+"\n"+SqlCalls::lastError);
            model_secciones->revertAll();
            Configuracion_global->rollback();
        }
        else
        {
            refresh();
            Configuracion_global->commit();
            TimedMessageBox::Box(this,tr("Sección borrada con éxito"));
        }
    }
}

void DlgDivisionesAlmacen::on_btnBorrarFam_clicked()
{
    if(ui->listFamilias->currentIndex().isValid())
    {
        Configuracion_global->transaction();
        int id_familia = model_familias->record(ui->listFamilias->currentIndex().row()).value("id").toInt();

        bool success = model_familias->removeRow(ui->listFamilias->currentIndex().row());
        success &= model_familias->submitAll();
        success &= borrar_subfamilias(id_familia);
        if(!success)
        {
            QMessageBox::critical(this,tr("Error al borrar familia"),model_familias->lastError().text()+"\n"+SqlCalls::lastError);
            model_familias->revertAll();
            Configuracion_global->rollback();
        }
        else
        {
            refresh();
            Configuracion_global->commit();
            TimedMessageBox::Box(this,tr("Familia borrada con éxito"));
        }
    }
}

void DlgDivisionesAlmacen::on_btnBorrarSub_clicked()
{
    if(ui->listSubfamilias->currentIndex().isValid())
    {
        bool success = model_subfamilias->removeRow(ui->listSubfamilias->currentIndex().row());
        success &= model_subfamilias->submitAll();
        if(!success)
        {
            QMessageBox::critical(this,tr("Error al borrar subfamilia"),model_subfamilias->lastError().text());
            model_subfamilias->revertAll();
        }
        else
        {
            refresh();
            TimedMessageBox::Box(this,tr("Subfamilia borrada con éxito"));
        }
    }
}
