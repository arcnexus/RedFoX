#include "frmformas_pago.h"
#include "ui_frmformas_pago.h"
#include "../Busquedas/db_consulta_view.h"


FrmFormas_pago::FrmFormas_pago(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmFormas_pago),
  toolButton(tr("Formas de\n Pago"),":/Icons/PNG/fpagos.png",this),
  menuButton(QIcon(":/Icons/PNG/fpagos.png"),tr("Formas de Pago"),this),
  push(new QPushButton(QIcon(":/Icons/PNG/transport.png"),"",this))

{
    ui->setupUi(this);
    //-------------------
    // iniciar variables
    //-------------------
    anadiendo = false;
    ui->txtcod_cuenta_contable->installEventFilter(this);

    //-------------------
    // Tabla buscar
    //-------------------
    m=  new QSqlQueryModel(this);
    m->setQuery("select id, codigo, forma_pago, dia_pago1, dia_pago2, dia_pago3, dia_pago4, dias_entre_plazos, "
                "numero_plazos, cuenta_cont_pago  from formpago order by codigo",
                Configuracion_global->groupDB);
    ui->tabla_buscar->setModel(m);
    QStringList headers;
    QVariantList sizes;
    ui->tabla_buscar->setColumnHidden(0,true);
    headers << "id" << tr("código") <<tr("Forma de pago") << tr("Día 1") << tr("Día 2") << tr("Día 3") << tr("Día 4");
    headers << tr("dias entre plazos") << tr("Plazos") << tr("PGC");
    sizes << 0 << 70 << 350 << 50 << 50 << 50 << 50 << 130 << 60 << 100;
    for(int i = 0; i < headers.size();i++)
    {
        ui->tabla_buscar->setColumnWidth(i,sizes.at(i).toInt());
        m->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
    QStringList orden,modo;

    orden <<  tr("Código") << tr("Descripción");
    modo << tr("A-Z") <<tr("Z-A");
    ui->cboOrden->addItems(orden);
    ui->cboModo->addItems(modo);
    ui->stackedWidget->setCurrentIndex(0);
    ui->txtBuscar->setFocus();
    oVtos = new vencimientos(this);
    bloquear_campos(true);
}

FrmFormas_pago::~FrmFormas_pago()
{
    delete ui;
}

void FrmFormas_pago::on_tabla_buscar_doubleClicked(const QModelIndex &index)
{
    int id = ui->tabla_buscar->model()->data(ui->tabla_buscar->model()->index(index.row(),0),Qt::EditRole).toInt();
    QStringList opciones,extras;
    opciones << QString("id= %1").arg(id);
    extras << "";
    oVtos->recuperar(opciones,extras);
    llenar_campos();
    ui->stackedWidget->setCurrentIndex(1);
}

void FrmFormas_pago::on_tabla_buscar_clicked(const QModelIndex &index)
{
    int id = ui->tabla_buscar->model()->data(ui->tabla_buscar->model()->index(index.row(),0),Qt::EditRole).toInt();
    QStringList opciones,extras;
    opciones << QString("id= %1").arg(id);
    extras << "";
    oVtos->recuperar(opciones,extras);
    llenar_campos();
}

void FrmFormas_pago::llenar_campos()
{
    ui->txtcod_forma_pago->setText(oVtos->codigo);
    ui->txtForma_pago->setText(oVtos->forma_pago);
    ui->txtcod_cuenta_contable->setText(oVtos->codigo_cuenta_contable);
    ui->txtCuenta_contable->setText(oVtos->desc_cuenta_cont);
    ui->spinDia_pago1->setValue(oVtos->dia_pago1);
    ui->spinDia_pago2->setValue(oVtos->dia_pago2);
    ui->spinDia_pago3->setValue(oVtos->dia_pago3);
    ui->spinDia_pago4->setValue(oVtos->dia_pago4);
    ui->spinDias_entre_plazos->setValue(oVtos->dias_entre_plazos);
    ui->spinNumero_plazos->setValue(oVtos->numero_plazos);
}

void FrmFormas_pago::llenar_objeto()
{
    oVtos->codigo = ui->txtcod_forma_pago->text();
    oVtos->forma_pago = ui->txtForma_pago->text();
    oVtos->codigo_cuenta_contable = ui->txtcod_cuenta_contable->text();
    oVtos->desc_cuenta_cont = ui->txtCuenta_contable->text();
    oVtos->dia_pago1 = ui->spinDia_pago1->value();
    oVtos->dia_pago2 = ui->spinDia_pago2->value();
    oVtos->dia_pago3 = ui->spinDia_pago3->value();
    oVtos->dia_pago4 = ui->spinDia_pago4->value();
    oVtos->dias_entre_plazos = ui->spinDia_pago1->value();
    oVtos->numero_plazos = ui->spinNumero_plazos->value();

}

void FrmFormas_pago::refrescar_tabla()
{
    // <<  tr("Código") << tr("Descripción")
    QHash <QString,QString> h;
    h[tr("Código")] = "codigo";
    h[tr("Descripción")] = "forma_pago";
    QString order = h.value(ui->cboOrden->currentText());
    QString arg1 = ui->txtBuscar->text();
    QString modo;
    if(ui->cboModo->currentText() == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    m->setQuery("select id, codigo, forma_pago, dia_pago1, dia_pago2, dia_pago3, dia_pago4, dias_entre_plazos,"
                "numero_plazos, cuenta_cont_pago  from formpago where "+order+" like '%"+arg1.trimmed()+
                "%' order by "+order +" "+modo,Configuracion_global->groupDB);
}

bool FrmFormas_pago::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj == ui->txtcod_cuenta_contable)
        {
            if(keyEvent->key() == Qt::Key_F1)
            {
                buscando = true;
                consultar_cuenta();
            }
        }
        return false;
    }
}

void FrmFormas_pago::consultar_cuenta()
{
    db_consulta_view consulta;
    QStringList campos;
    campos << "codigo_cta" <<"descripcion";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Plan de cuentas (PGC)");
    consulta.set_db("conta");
    consulta.set_SQL("select id,codigo_cta,descripcion from plan_general");
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Código") <<tr("Descripción cuenta");
    tamanos <<"0" << "100"  <<"300" ;
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);

    consulta.set_titulo("Busqueda de Subcuentas");
    if(consulta.exec())
    {
        int id_cuenta= consulta.get_id();

        QString filtro = QString("id=%1").arg(id_cuenta);
        QString error;
        QMap <int,QSqlRecord> m;
        m = SqlCalls::SelectRecord("plan_general",filtro,Configuracion_global->contaDB,error);
        if(error.isEmpty())
        {
            QMapIterator <int,QSqlRecord> r(m);
            while(r.hasNext())
            {
                r.next();
                ui->txtcod_cuenta_contable->setText(r.value().value("codigo_cta").toString());
                ui->txtCuenta_contable->setText(r.value().value("descripcion").toString());
            }

        }
    }
    buscando = false;
}

void FrmFormas_pago::on_btnSiguiente_2_clicked()
{
    QStringList opciones, extras;
    opciones << QString("codigo > '%1'").arg(oVtos->codigo);
    extras << "order by codigo " << "limit 0,1";
    oVtos->recuperar(opciones,extras);
    llenar_campos();
}

void FrmFormas_pago::on_btnAnterior_2_clicked()
{
    QStringList opciones, extras;
    opciones << QString("codigo < '%1'").arg(oVtos->codigo);
    extras << "order by id desc" << "limit 0,1";
    oVtos->recuperar(opciones,extras);
    llenar_campos();
}

void FrmFormas_pago::on_btnBuscar_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->txtBuscar->setFocus();
}

void FrmFormas_pago::on_btnAnadir_3_clicked()
{
    anadiendo = true;
    oVtos->clear();
    llenar_campos();
    ui->spinNumero_plazos->setValue(1);
    ui->txtCuenta_contable->clear();
    bloquear_campos(false);
    ui->txtcod_forma_pago->setFocus();


}

void FrmFormas_pago::bloquear_campos(bool state)
{
    //--------------------------
    // Botones
    //--------------------------
    ui->btnAnadir_3->setEnabled(state);
    ui->btnAnterior_2->setEnabled(state);
    ui->btnBuscar_2->setEnabled(state);
    ui->btn_borrar->setEnabled(state);
    ui->btndeshacer_2->setEnabled(!state);
    ui->btnEditar_3->setEnabled(state);
    ui->btnGuardar_2->setEnabled(!state);
    ui->btnSiguiente_2->setEnabled(state);


    //---------------------------
    // Campos
    //---------------------------
    ui->txtcod_forma_pago->setReadOnly(state);
    ui->txtForma_pago->setReadOnly(state);
    ui->txtcod_cuenta_contable->setReadOnly(state);
    ui->txtCuenta_contable->setReadOnly(state);
    ui->spinDia_pago1->setReadOnly(state);
    ui->spinDia_pago2->setReadOnly(state);
    ui->spinDia_pago3->setReadOnly(state);
    ui->spinDia_pago4->setReadOnly(state);
    ui->spinDias_entre_plazos->setReadOnly(state);
    ui->spinNumero_plazos->setReadOnly(state);

}

void FrmFormas_pago::on_btnEditar_3_clicked()
{
    bloquear_campos(false);
    Configuracion_global->groupDB.transaction();
    ui->txtcod_forma_pago->setFocus();


}

void FrmFormas_pago::on_btnGuardar_2_clicked()
{
    llenar_objeto();
    if(anadiendo)
        oVtos->anadir();
    else
        oVtos->guardar();
    Configuracion_global->groupDB.commit();
    bloquear_campos(true);
}

void FrmFormas_pago::on_btndeshacer_2_clicked()
{
    anadiendo = false;
    Configuracion_global->groupDB.rollback();
    QStringList condiciones, extras;
    condiciones << QString("id=%1").arg(oVtos->id);
    extras << "limit 0,1";
    oVtos->recuperar(condiciones,extras);
    llenar_campos();
    bloquear_campos(true);
}


void FrmFormas_pago::on_stackedWidget_currentChanged(int arg1)
{
    if(arg1 == 0)
        refrescar_tabla();
}

void FrmFormas_pago::on_btnAnadir_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    on_btnAnadir_3_clicked();
}

void FrmFormas_pago::on_btnEditar_2_clicked()
{
    QModelIndex  index = ui->tabla_buscar->currentIndex();
    int id = ui->tabla_buscar->model()->data(ui->tabla_buscar->model()->index(index.row(),0),Qt::EditRole).toInt();
    QStringList opciones,extras;
    opciones << QString("id= %1").arg(id);
    extras << "";
    oVtos->recuperar(opciones,extras);
    llenar_campos();
    ui->stackedWidget->setCurrentIndex(1);
    on_btnEditar_3_clicked();
}

void FrmFormas_pago::on_btnLimpiar_clicked()
{
    ui->cboModo->setCurrentIndex(1);
    ui->cboOrden->setCurrentIndex(1);
    ui->txtBuscar->clear();
    refrescar_tabla();
    ui->txtBuscar->setFocus();
}

void FrmFormas_pago::on_cboOrden_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    refrescar_tabla();
}

void FrmFormas_pago::on_cboModo_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    refrescar_tabla();
}

void FrmFormas_pago::on_btn_borrar_clicked()
{
    if(QMessageBox::warning(this,tr("Formas de pago"),
                            tr("Si borra esta forma de pago, clientes,"
                               " proveedores y distos documentos que la estén usando perderán esta información\n"
                               "¿Seguro que quiere borrarla?\n"
                               "Esta opción no tiene marcha atrás"),tr("No"),tr("Sí"))==QMessageBox::Accepted)
    {
        oVtos->borrar();
        oVtos->clear();
        llenar_campos();
    }
}

void FrmFormas_pago::on_btnborrar_2_clicked()
{
    QModelIndex  index = ui->tabla_buscar->currentIndex();
    int id = ui->tabla_buscar->model()->data(ui->tabla_buscar->model()->index(index.row(),0),Qt::EditRole).toInt();
    QStringList opciones,extras;
    opciones << QString("id= %1").arg(id);
    extras << "";
    oVtos->recuperar(opciones,extras);
    llenar_campos();
    on_btn_borrar_clicked();

}

void FrmFormas_pago::on_txtcod_cuenta_contable_editingFinished()
{
    QLineEdit* edit = qobject_cast<QLineEdit*>(sender());
    if(edit->isReadOnly() || buscando)
        return;
    QString error;
    ui->txtCuenta_contable->setText(SqlCalls::SelectOneField("plan_general","descripcion",
                                                      QString("codigo_cta ='%1'").arg(ui->txtcod_cuenta_contable->text()),
                                                      Configuracion_global->contaDB,error).toString());
    if(!error.isEmpty())
        QMessageBox::warning(this,tr("Formas de pago"),tr("Error al asociar cuenta contable: %1").arg(error),tr("Aceptar"));
}
