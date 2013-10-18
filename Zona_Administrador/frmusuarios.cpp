#include "frmusuarios.h"
#include "ui_frmusuarios.h"

void FrmUsuarios::getModulesFromDB()
{
    QSqlQuery q(Configuracion_global->groupDB);
    q.prepare("SELECT * FROM modulos;");
    q.exec();
    while(q.next())
    {
        AuxModule* m = new AuxModule(this,q.record().value(2).toString());
        m->id = q.record().value(0).toInt();
        MayaModule::module_zone z = static_cast<MayaModule::module_zone>(q.record().value(1).toInt());
        m->zone = z;
        switch (z) {
        case MayaModule::Mantenimiento:
            addMantenModule(m);
            break;
        case MayaModule::NoZone:
            break;
        case MayaModule::Contabilidad:
            addContaModule(m);
            break;
        case MayaModule::SecretariaMedica:
            addSecMedicModule(m);
            break;
        case MayaModule::InformacionMedica:
            addInfoMedicModule(m);
            break;
        case MayaModule::Compras:
            addComprasModule(m);
            break;
        case MayaModule::Ventas:
            addVentasModule(m);
            break;
        case MayaModule::Archivos:
            addArchivosModule(m);
            break;
        case MayaModule::Almacen:
            addAlmacenModule(m);
            break;
        case MayaModule::Utilidades:
            addUtilidadesModule(m);
            break;
        case MayaModule::AdminZone :
            addAdminModule(m);
            break;

        default:
            addNoZoneModule(m);
            break;
        }
        _modulos.append(m);
    }

    addMantenModule(0);
    addComprasModule(0);
    addVentasModule(0);
    addArchivosModule(0);
    addAlmacenModule(0);
    addUtilidadesModule(0);
    addNoZoneModule(0);
    addAdminModule(0);
    addContaModule(0);
    addSecMedicModule(0);
    addInfoMedicModule(0);
}

FrmUsuarios::FrmUsuarios(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmUsuarios),

    menuButton(QIcon(":/Icons/PNG/users.png"),tr("Usuarios"),this)
{
    ui->setupUi(this);
    connect(ui->botCerrar_2,SIGNAL(clicked()),this,SLOT(close()));

    ui->txt_pass_user->setReadOnly(true);

    getModulesFromDB();
    on_botSiguiente_user_clicked();
}

FrmUsuarios::~FrmUsuarios()
{
    delete ui;
}

void FrmUsuarios::addAdminModule(AuxModule * m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_admin->layout())
        QGridLayout *l= new QGridLayout(ui->tab_admin);

    if(m)
    {
        ((QGridLayout *)ui->tab_admin->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_admin->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void FrmUsuarios::addMantenModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_manten->layout())
        QGridLayout *l= new QGridLayout(ui->tab_manten);

    if(m)
    {
        ((QGridLayout *)ui->tab_manten->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_manten->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void FrmUsuarios::addComprasModule(AuxModule * m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_compras->layout())
        QGridLayout *l= new QGridLayout(ui->tab_compras);

    if(m)
    {
        ((QGridLayout *)ui->tab_compras->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_compras->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void FrmUsuarios::addVentasModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_ventas->layout())
        QGridLayout *l= new QGridLayout(ui->tab_ventas);

    if(m)
    {
        ((QGridLayout *)ui->tab_ventas->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_ventas->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void FrmUsuarios::addArchivosModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_archivos->layout())
        QGridLayout *l= new QGridLayout(ui->tab_archivos);

    if(m)
    {
        ((QGridLayout *)ui->tab_archivos->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_archivos->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void FrmUsuarios::addAlmacenModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_almacen->layout())
        QGridLayout *l= new QGridLayout(ui->tab_almacen);

    if(m)
    {
        ((QGridLayout *)ui->tab_almacen->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_almacen->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void FrmUsuarios::addUtilidadesModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_utils->layout())
        QGridLayout *l= new QGridLayout(ui->tab_utils);

    if(m)
    {
        ((QGridLayout *)ui->tab_utils->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_utils->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void FrmUsuarios::addNoZoneModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_noZone->layout())
        QGridLayout *l= new QGridLayout(ui->tab_noZone);

    if(m)
    {
        ((QGridLayout *)ui->tab_noZone->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_noZone->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void FrmUsuarios::addContaModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_conta->layout())
        QGridLayout *l= new QGridLayout(ui->tab_conta);

    if(m)
    {
        ((QGridLayout *)ui->tab_conta->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_conta->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void FrmUsuarios::addSecMedicModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_secMed->layout())
        QGridLayout *l= new QGridLayout(ui->tab_secMed);

    if(m)
    {
        ((QGridLayout *)ui->tab_secMed->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_secMed->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void FrmUsuarios::addInfoMedicModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_med->layout())
        QGridLayout *l= new QGridLayout(ui->tab_med);

    if(m)
    {
        ((QGridLayout *)ui->tab_med->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_med->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}
void FrmUsuarios::crear_User()
{
    QSqlQuery add_user(Configuracion_global->groupDB);

    add_user.prepare("INSERT INTO usuarios (nombre,contrasena,categoria,cuenta_smtp,usuario_mail,password_mail) "
                     "VALUES (:nombre,:contrasena,:categoria,:cuenta_smtp,:usuario_mail,:password_mail)");

    add_user.bindValue(":nombre","");
    add_user.bindValue(":contrasena","");
    add_user.bindValue(":categoria","");
    add_user.bindValue(":cuenta_smtp",ui->txtcuenta_smtp->text());
    add_user.bindValue(":usuario_mail",ui->txtusuario_mail->text());
    add_user.bindValue(":password_mail",ui->txtpassword_mail->text());

    int last_id=-1;
    if(add_user.exec())
        last_id=add_user.lastInsertId().toInt();

    ui->txt_id_user->setText(QString::number(last_id));

    QVector<AuxModule*>::Iterator i;
    QSqlQuery q(Configuracion_global->groupDB);
    for(i=_modulos.begin(); i!=_modulos.end(); ++i)
    {
        q.prepare("INSERT INTO accesousuarios (id_user, id_modulo, id_nivel_acceso) VALUES (:id, :mod, 1);");
        q.bindValue(":id",last_id);
        q.bindValue(":mod",(*i)->id);
        q.exec();
    }
    llenarModulos(last_id);
}

void FrmUsuarios::llenar_user(QSqlRecord record)
{
    ui->txt_id_user->setText(record.value("id").toString());
    ui->txt_nombre_user->setText(record.value("nombre").toString());
    ui->txt_pass_user->setText(record.value("contrasena").toString());
    ui->txt_categoria_user->setText(record.value("categoria").toString());
    ui->txtcuenta_smtp->setText(record.value("cuenta_smtp").toString());
    ui->txtusuario_mail->setText(record.value("usuario_mail").toString());
    ui->txtpassword_mail->setText(record.value("password_mail").toString());
    llenarModulos(record.value(0).toInt());
}

void FrmUsuarios::llenarModulos(int id_user)
{
    QSqlQuery q(Configuracion_global->groupDB);
    q.prepare("SELECT * FROM accesousuarios WHERE id_user=:id;");
    q.bindValue(":id",id_user);
    q.exec();
    while(q.next())
    {
        int id_modulo = q.record().value(2).toInt();
        int idAcceso = q.record().value(3).toInt();
        QVector<AuxModule*>::Iterator i;
        for(i=_modulos.begin();i!=_modulos.end();++i)
        {
            if((*i)->id == id_modulo)
            {
                (*i)->setNivel(idAcceso);
                break;
            }
        }
    }
}
void FrmUsuarios::on_botGuardar_user_clicked()
{
    int id=ui->txt_id_user->text().toInt();
    QSqlQuery q(Configuracion_global->groupDB);
    q.prepare("UPDATE usuarios SET nombre=:name, categoria=:cat, cuenta_smtp=:cuenta_smtp,"
              "usuario_mail =:usuario_mail,password_mail =:password_mail WHERE id=:id;");
    q.bindValue(":name",ui->txt_nombre_user->text());
    //q.bindValue(":lvl",ui->spin_nacceso_user->value());
    q.bindValue(":cat",ui->txt_categoria_user->text());
    q.bindValue(":cuenta_smtp",ui->txtcuenta_smtp->text());
    q.bindValue(":usuario_mail",ui->txtusuario_mail->text());
    q.bindValue(":password_mail",ui->txtpassword_mail->text());
    q.bindValue(":id",id);

    if(q.exec())
    {
        QVector<AuxModule*>::Iterator i;
        QSqlQuery q2(Configuracion_global->groupDB);
        for(i=_modulos.begin(); i!=_modulos.end(); ++i)
        {
            q2.prepare("UPDATE accesousuarios SET id_nivel_acceso=:lvl WHERE id_modulo=:mod AND id_user=:id;");
            q2.bindValue(":id",id);
            q2.bindValue(":mod",(*i)->id);
            q2.bindValue(":lvl",(*i)->nivel());
            q2.exec();
        }
        TimedMessageBox * t = new TimedMessageBox(this,tr("Se ha guardado con exito"));
    }
    else
        QMessageBox::critical(this,tr("Error"),tr("No se ha podido guardar el usuario.\n%1").arg(q.lastError().text()),tr("Aceptar"));
}

void FrmUsuarios::on_botBorrar_user_clicked()
{
    //TODO borrar user
}

void FrmUsuarios::on_btn_modPass_clicked()
{
    if(ui->btn_modPass->text() == tr("Modificar"))
    {
        ui->btn_modPass->setText(tr("Guardar"));
        ui->txt_pass_user->setReadOnly(false);
    }
    else
    {
        ui->btn_modPass->setText(tr("Modificar"));
        ui->txt_pass_user->setReadOnly(true);
        QSqlQuery q(Configuracion_global->groupDB);
        q.prepare("UPDATE usuarios SET contrasena=:pass WHERE id=:id;");
        q.bindValue(":id",ui->txt_id_user->text());
        q.bindValue(":pass",Configuracion::SHA256HashString(ui->txt_pass_user->text()));
        if(q.exec())
            TimedMessageBox* t = new TimedMessageBox(this,"Contraseña actualizada");
    }
}
void FrmUsuarios::on_botSiguiente_user_clicked()
{
    QSqlQuery next_user(Configuracion_global->groupDB);
    QString sql = QString("SELECT * FROM usuarios WHERE id > %1 LIMIT 1 ")
            .arg(ui->txt_id_user->text());
    next_user.prepare(sql);
    if(next_user.exec())
    {
        if(next_user.next())
            llenar_user(next_user.record());
        else
            TimedMessageBox * t = new TimedMessageBox(this,
                                     tr("No exsten mas usuarios"));

    }
}

void FrmUsuarios::on_botAnterior_user_clicked()
{
    QSqlQuery next_user(Configuracion_global->groupDB);
    QString sql = QString("SELECT * FROM usuarios WHERE id < %1 ORDER BY id DESC LIMIT 1")
            .arg(ui->txt_id_user->text());
    next_user.prepare(sql);
    if(next_user.exec())
    {
        if(next_user.next())
            llenar_user(next_user.record());
        else
            TimedMessageBox * t = new TimedMessageBox(this,
                                     tr("Este es el primer usuario"));

    }
}

void FrmUsuarios::on_botBuscar_user_clicked()
{
    //TODO buscar user
}

void FrmUsuarios::on_botAnadir_user_clicked()
{
    crear_User();
}
