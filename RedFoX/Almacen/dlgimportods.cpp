#include "dlgimportods.h"
#include "ui_dlgimportods.h"

#include "../Auxiliares/Globlal_Include.h"
#include <ods.h>

DlgImportODS::DlgImportODS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgImportODS)
{
    ui->setupUi(this);

    ods = new QStandardItemModel(this);

    ui->tableView->setModel(ods);
    QString error;
    tarifas = SqlCalls::SelectRecord("codigotarifa",QString(),Configuracion_global->groupDB,error);
    int row = 0;
    for(QMap<int,QSqlRecord>::const_iterator it = tarifas.begin(); it!= tarifas.end(); ++it)
    {
        ui->tableTarifas->setRowCount(row+1);

        QTableWidgetItem * desc_item = new QTableWidgetItem(it.value().value("descripcion").toString());
        ui->tableTarifas->setItem(row,0,desc_item);

        ui->tableTarifas->setCellWidget(row,1,new QSpinBox(this));

        QTableWidgetItem * id_item = new QTableWidgetItem(it.value().value("id").toString());
        ui->tableTarifas->setItem(row,2,id_item);

        row++;
    }
    ui->tableTarifas->setColumnWidth(0,280);
    ui->tableTarifas->setColumnWidth(1,280);
    ui->tableTarifas->setColumnHidden(2,true);
}

DlgImportODS::~DlgImportODS()
{
    _cods.clear();
    _cods_b.clear();
    _cods_f.clear();
    empresas.clear();
    tarifas.clear();
    secciones.clear();
    familias.clear();
    subfamilias.clear();
    proveedores.clear();
    delete ui;
}

void DlgImportODS::on_btnFile_clicked()
{
    ods->clear();
    QString fichero = QFileDialog::getOpenFileName(this,tr("Abrir Hoja de cálculo"),"","Hoja de cálculo (*.ods)");
    if(!fichero.isEmpty() && ODS::ODStoTable(fichero,ods))
        ui->txtFile->setText(fichero);
    else if(!fichero.isEmpty())
        QMessageBox::critical(this,tr("Error al abrir archivo"),ODS::lastError);
}

void DlgImportODS::on_btnImpor_clicked()
{
    Configuracion_global->transaction();

    QString error;
    secciones = SqlCalls::SelectMap<QString,int>("secciones","codigo","id",QString("id > 0"),Configuracion_global->groupDB,error);
    familias = SqlCalls::SelectMap<QString,int>("familias","codigo","id",QString("id > 0"),Configuracion_global->groupDB,error);
    subfamilias = SqlCalls::SelectMap<QString,int>("subfamilias","codigo","id",QString("id > 0"),Configuracion_global->groupDB,error);
    proveedores = SqlCalls::SelectMap<QString,int>("proveedores","codigo","id",QString("id > 0"),Configuracion_global->groupDB,error);
    ivas = SqlCalls::SelectMap<int,double>("tiposiva","id","iva",QString("id > 0"),Configuracion_global->groupDB,error);

    _cods = SqlCalls::SelectMap<QString,bool>("articulos","codigo","kit",QString("id > 0"),Configuracion_global->groupDB,error);
    _cods_b = SqlCalls::SelectMap<QString,bool>("articulos","codigo_barras","kit",QString("id > 0"),Configuracion_global->groupDB,error);
    _cods_f = SqlCalls::SelectMap<QString,bool>("articulos","codigo_fabricante","kit",QString("id > 0"),Configuracion_global->groupDB,error);

    _use_codigo  = ui->spinCodigo->value() > 0;
    _use_codigo_b= ui->spinCBarras->value() > 0;
    _use_codigo_f= ui->spinCFabricante->value() > 0;
    _use_desc= ui->spinDesc->value() > 0;
    _use_desc_red = ui->spinDesRed->value() > 0;
    _use_iva = ui->spinIVA->value() > 0;
    _use_coste = ui->spinCoste->value() > 0;
    _use_stock = ui->spinStock->value() > 0;
    _use_web = ui->spinWeb->value() > 0;
    _use_secc = ui->spinSeccion->value() > 0;
    _use_fam = ui->spinFamilia->value() > 0;
    _use_subfam = ui->spinSubfamilia->value() > 0;
    _update_tar = ui->groupTarifas->isChecked();

    if(_update_tar)
    {
        for (auto i= 0; i< ui->tableTarifas->rowCount(); ++i)
        {
            int column = qobject_cast<QSpinBox*>(ui->tableTarifas->cellWidget(i,1))->value();
            if(column > 0)
            {
                int id = ui->tableTarifas->item(i,2)->data(Qt::EditRole).toInt();
                _tarifa_index.insert(id,column-1);
            }
        }
    }

    bool use_anything = _use_codigo || _use_codigo_b || _use_codigo_f || _use_desc || _use_desc_red;
    if(!use_anything)
    {
        QMessageBox::warning(this,tr("Seleccion vacía"),tr("Debe seleccionar alguna columna a importar..."));
        return;
    }

    _check_on = ui->checkBox->isChecked();

    int c_index  = ui->spinCodigo->value() - 1;
    int c_b_index = ui->spinCBarras->value() - 1;
    int c_f_index = ui->spinCFabricante->value() - 1;

    int des_index = ui->spinDesc->value() - 1;
    int des_r_index = ui->spinDesRed->value() -1;

    int iva_index = ui->spinIVA->value() - 1;
    int coste_index = ui->spinCoste->value() - 1;
    int stock_index = ui->spinStock->value() - 1;
    int web_index = ui->spinWeb->value() - 1;
    int secc_index = ui->spinSeccion->value() - 1;
    int fam_index = ui->spinFamilia->value() - 1;
    int subfam_index = ui->spinSubfamilia->value() - 1;

    QSqlTableModel articulos(this,Configuracion_global->groupDB);
    articulos.setTable("articulos");
    articulos.setEditStrategy(QSqlTableModel::OnManualSubmit);
    articulos.select();

    QString codigo;
    QString codigo_b;
    QString codigo_f;
    QString desc;
    QString desc_r;
    int iva;
    double t_iva;
    double coste;
    bool stock;
    bool web;
    QString cod_sec;
    QString cod_fam;
    QString cod_sub;
    int secc;
    int fam;
    int subfam;


    QProgressDialog dlg(this);
    dlg.setMaximum( ods->rowCount());

    for(auto i= 0; i<ods->rowCount(); ++i)
    {
        if(dlg.wasCanceled())
        {
            Configuracion_global->rollback();
            return;
        }

        dlg.setValue(i);
        if(i % 50 == 0)
            QApplication::processEvents();

        int row = articulos.rowCount();

        if(_use_codigo)
            codigo = ods->data(ods->index(i,c_index)).toString();               
        if(_use_codigo_b)
            codigo_b = ods->data(ods->index(i,c_b_index)).toString();
        if(_use_codigo_f)
            codigo_f = ods->data(ods->index(i,c_f_index)).toString();
        if(_use_desc)
            desc = ods->data(ods->index(i,des_index)).toString();
        if(_use_desc_red)
            desc_r = ods->data(ods->index(i,des_r_index)).toString();
        if(_use_iva)
        {
            iva = ods->data(ods->index(i,iva_index)).toInt();
            t_iva = ivas.value(iva);
            if(iva < 1 || iva > 4)
            {
                QMessageBox::critical(this,tr("Error en datos"),tr("El tipo iva debe ser un valor entre 1 y 4: linea %1").arg(i));
                Configuracion_global->rollback();
                return;
            }
        }
        else
        {
            iva = 1;
            t_iva = ivas.value(iva);
        }
        if(_use_coste)
            coste = ods->data(ods->index(i,coste_index)).toDouble();
        if(_use_stock)
            stock = ods->data(ods->index(i,stock_index)).toBool();
        if(_use_web)
            web = ods->data(ods->index(i,web_index)).toBool();
        if(_use_secc)
        {
            cod_sec = ods->data(ods->index(i,secc_index)).toString();
            if(secciones.contains(cod_sec))
                secc = secciones.value(cod_sec);
            else
            {
                QMessageBox::critical(this,tr("Error en datos"),tr("El codigo %1 no pertenece a ninguna sección: linea %2").arg(cod_sec).arg(i));
                Configuracion_global->rollback();
                return;
            }
        }
        if(_use_fam)
        {
            cod_fam = ods->data(ods->index(i,fam_index)).toString();
            if(familias.contains(cod_fam))
                fam = familias.value(cod_fam);
            else
            {
                QMessageBox::critical(this,tr("Error en datos"),tr("El codigo %1 no pertenece a ninguna familia: linea %2").arg(cod_fam).arg(i));
                Configuracion_global->rollback();
                return;
            }
        }
        if(_use_subfam)
        {
            cod_sub = ods->data(ods->index(i,subfam_index)).toString();
            if(subfamilias.contains(cod_sub))
                subfam = familias.value(cod_sub);
            else
            {
                QMessageBox::critical(this,tr("Error en datos"),tr("El codigo %1 no pertenece a ninguna subfamilia: linea %2").arg(cod_sub).arg(i));
                Configuracion_global->rollback();
                return;
            }
        }

        bool row_ok;
        bool already_on = alreadyOn(&articulos,codigo,codigo_b,codigo_f,row);

        if(!already_on)
            row_ok = articulos.insertRow(row);

        if(_use_codigo)
            row_ok = articulos.setData(articulos.index(row,articulos.fieldIndex("codigo")),codigo);
        if(_use_codigo_b)
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("codigo_barras")),codigo_b);
        if(_use_codigo_f)
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("codigo_fabricante")),codigo_f);
        if(_use_desc)
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("descripcion")),desc);
        if(_use_desc_red)
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("descripcion_reducida")),desc_r);
        //IVA sí o sí
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("id_tipos_iva")),iva);
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("tipo_iva")),t_iva);

        if(_use_coste)
        {
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("coste")),coste);
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("coste_real")),coste);
        }
        if(_use_stock)
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("controlar_stock")),stock);
        if(_use_web)
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("mostrar_web")),web);
        if(_use_secc)
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("id_seccion")),secc);
        if(_use_fam)
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("id_familia")),fam);
        if(_use_subfam)
            row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("id_subfamilia")),subfam);

        double pvp = 0.0;
        if(!already_on)
        {
            row_ok &= articulos.submitAll();
            int new_id = articulos.data(articulos.index(row,articulos.fieldIndex("id"))).toInt();
            row_ok &= createAcumsAndTarifa(new_id, ods, i, coste,pvp);
        }
        else if(_update_tar)
        {
            row_ok &= updateTarifa(articulos.data(articulos.index(row,articulos.fieldIndex("id"))).toInt(),ods, i,coste,pvp);
        }

        row_ok &= articulos.setData(articulos.index(row,articulos.fieldIndex("pvp")),pvp);

        if(!row_ok)
        {            
            QMessageBox::critical(this,tr("Error al insertar linea"),articulos.lastError().text());
            Configuracion_global->rollback();
            return;
        }
    }
    if(articulos.submitAll())
    {
        Configuracion_global->commit();
        QMessageBox::information(this,tr("Proceso terminado"),tr("Importado con éxito"));
    }
    else
    {
        QMessageBox::critical(this,tr("Error al importar"),articulos.lastError().text());
        Configuracion_global->rollback();
    }
}

bool DlgImportODS::alreadyOn(QSqlTableModel *model, QString cod, QString cod_b, QString cod_f, int& row)
{
    if(!_check_on)
        return false;

    bool on = false;
    if(_use_codigo)
    {
        on = _cods.contains(cod);
        _cods.insert(cod,false);
    }
    if(_use_codigo_b)
    {
        on |= _use_codigo_b && _cods_b.contains(cod_b);
        _cods_b.insert(cod_b,false);
    }
    if(_use_codigo_f)
    {
        on |= _use_codigo_f && _cods_f.contains(cod_f);
        _cods_f.insert(cod_f,false);
    }

    if(on)
    {
        for(auto i=0; i< model->rowCount(); ++i)
        {
            const QSqlRecord r = model->record(i);
            bool on_model = _use_codigo && r.value("codigo").toString() == cod;
            on_model |= _use_codigo_b && r.value("codigo_barras").toString() == cod_b;
            on_model |= _use_codigo_f && r.value("codigo_fabricante").toString() == cod_f;
            if(on_model)
            {
                row = i;
                return true;
            }
        }
    }
    return on;
}

bool DlgImportODS::createAcumsAndTarifa(int id, QStandardItemModel *model, int row, double coste, double &pvp)
{
    QString error;
    //CACHÉ de empresas
    if(empresas.isEmpty())
        empresas = SqlCalls::SelectMap<int,int>("empresas","id","id",QStringList(),Configuracion_global->groupDB,error);

    bool succes = true;
    for(QMap<int,int>::const_iterator it = empresas.begin(); it!= empresas.end(); ++it)
    {
        QHash <QString, QVariant> h;
        h["id_articulo"] = id;
        h["id_empresa"] = it.key();
        int new_id = SqlCalls::SqlInsert(h,"acum_articulos",Configuracion_global->groupDB,error);
        if(new_id<0)
        {
            QMessageBox::warning(this,tr("Añadir Artículo"),
                                 tr("Falló la inserción de un nuevo acumulado de artículo :\n %1").arg(error),
                                 QObject::tr("Ok"));
            succes = false;
        }

    }
    if(succes)
    {
        // Añado tarifas a artículo
        for(QMap<int,QSqlRecord>::const_iterator it = tarifas.begin(); it!= tarifas.end(); ++it)
        {
            const QSqlRecord r = it.value();
            int id_tarifa = r.value("id").toInt();

            QHash<QString,QVariant> _tar;
            _tar["id_articulo"]= id;
            _tar["id_pais"]=  r.value("id_pais");
            _tar["id_monedas"]= r.value("id_monedas");
            _tar["margen"]= r.value("margen");
            _tar["margen_minimo"]= r.value("margen_min");
            _tar["id_codigo_tarifa"]= r.value("id");

            if(_update_tar)
            {
                if(_tarifa_index.contains(id_tarifa))
                {
                    _tar["pvp"]= model->data(model->index(row,_tarifa_index.value(id_tarifa)));
                    if(_use_coste)
                    {
                         double margen = 100 - (coste * 100 / _tar.value("pvp").toDouble());
                        _tar["margen"]= margen;
                    }
                }
            }

            if(id_tarifa == 1)
                pvp = _tar.value("pvp").toDouble();

            if(SqlCalls::SqlInsert(_tar,"tarifas",Configuracion_global->groupDB,error) < 0)
            {
                QMessageBox::warning(qApp->activeWindow(),tr("Añadir tarifa"),
                                     tr("Falló la inserción de la tarifa: %1").arg(error),
                                     tr("Aceptar"));
                succes = false;
            }
        }
    }
    return succes;
}

bool DlgImportODS::updateTarifa(int id_art, QStandardItemModel *model, int row, double coste, double &pvp)
{
    for(QMap<int,int>::const_iterator it = _tarifa_index.begin(); it!= _tarifa_index.end(); ++it)
    {
        int id_tarifa = it.key();
        SqlData _tar;
        _tar["pvp"]= model->data(model->index(row,_tarifa_index.value(id_tarifa)));
        if(id_tarifa == 1)
            pvp = _tar.value("pvp").toDouble();
        if(_use_coste)
        {
             double margen = 100 - (coste * 100 / _tar.value("pvp").toDouble());
            _tar["margen"]= margen;
        }
        QString filter = QString("id_articulo = %1 and id_codigo_tarifa = %2").arg(id_art).arg(id_tarifa);
        QString error;
        if(!SqlCalls::SqlUpdate(_tar,"tarifas",Configuracion_global->groupDB,filter,error))
        {
            QMessageBox::critical(this,tr("Error actualizando tarifa"),error);
            return false;
        }
    }
    return true;
}
