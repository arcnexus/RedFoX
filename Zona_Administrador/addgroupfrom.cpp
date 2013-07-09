#include "addgroupfrom.h"
#include "ui_addgroupfrom.h"

addGroupFrom::addGroupFrom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addGroupFrom)
{
    ui->setupUi(this);
    connect(ui->txtHost,&QLineEdit::textChanged,this, &addGroupFrom::setHost);
    connect(ui->txtName,&QLineEdit::textChanged,this, &addGroupFrom::setNombre);
    connect(ui->txtUser,&QLineEdit::textChanged,this, &addGroupFrom::setUser);
    connect(ui->txtPass,&QLineEdit::textChanged,this, &addGroupFrom::setPass);
    connect(ui->txtPort,&QLineEdit::textChanged,this, &addGroupFrom::setSPort);

    ui->frame->hide();
}

addGroupFrom::~addGroupFrom()
{
    delete ui;
}

QString addGroupFrom::getNombre() const
{
    return nombre;
}

void addGroupFrom::setNombre(const QString &value)
{
    nombre = value;
}

QString addGroupFrom::getHost() const
{
    return host;
}

void addGroupFrom::setHost(const QString &value)
{
    host = value;
}

QString addGroupFrom::getUser() const
{
    return user;
}

void addGroupFrom::setUser(const QString &value)
{
    user = value;
}

QString addGroupFrom::getPass() const
{
    return pass;
}

void addGroupFrom::setPass(const QString &value)
{
    pass = value;
}

int addGroupFrom::getPort() const
{
    return port;
}

void addGroupFrom::setPort(int value)
{
    port = value;
}

void addGroupFrom::setSPort(const QString &value)
{
    setPort(value.toInt());
}

void addGroupFrom::_insertMonedas(QSqlQuery q, bool error)
{
    QFile f(":/Icons/divisas.txt");
    if(f.open((QIODevice::ReadOnly | QIODevice::Text)))
    {

        QTextStream in(&f);
        in.setCodec("UTF-8");
        QString all = in.readAll();
        all.remove("\r");
        all.remove("\n");
        QStringList blocks = all.split("},",QString::SkipEmptyParts);
        foreach(const QString &moneda , blocks)
        {
            QStringList subBlocks = moneda.split("\",",QString::SkipEmptyParts);
            QString nombre_corto = subBlocks.at(0).mid(subBlocks.at(0).lastIndexOf("\"")+1);
            QString nombre = subBlocks.at(1).mid(subBlocks.at(1).lastIndexOf("\"")+1);
            QString simbol = subBlocks.at(4).mid(subBlocks.at(4).lastIndexOf("\"")+1);
            simbol.remove("}");
            simbol = simbol.trimmed();
            q.prepare("INSERT INTO monedas ("
                      "moneda ,"
                      "nombre_corto ,"
                      "simbolo "
                      ")"
                      "VALUES ("
                      ":moneda, :nombre_corto, :simbolo );");
            q.bindValue(":moneda",nombre);
            q.bindValue(":nombre_corto",nombre_corto);
            q.bindValue(":simbolo",simbol);
            if(!q.exec())
                error = true;
        }
    }
}

void addGroupFrom::_insertIVA(QSqlQuery q, bool error)
{
    error &= q.exec("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, regargo_equivalencia) VALUES ('base1', 'NORMAL'       ,    '21%', 21.00, 5.20);");
    error &= q.exec("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, regargo_equivalencia) VALUES ('base2', 'Reducido'     ,    '10%', 10.00, 1.40);");
    error &= q.exec("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, regargo_equivalencia) VALUES ('base3', 'Superreducido',     '4%',  4.00, 0.50);");
    error &= q.exec("INSERT INTO tiposiva (nombre_interno, tipo, descripcion_tipo_iva, iva, regargo_equivalencia) VALUES ('base4', 'Exento'       , 'Exento',  0.00, 0.00);");
}

void addGroupFrom::_insertNivelAcesso(bool error, QSqlQuery q)
{
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (1,'Sin Acceso');");
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (2,'Lectura parcial');");
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (3,'Lectura total');");
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (4,'Escritura parcial (editar)');");
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (5,'Escritura parcial (añadir)');");
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (6,'Escritural total');");
    error &= q.exec("INSERT INTO `nivelacceso` (`id`,`nombre`) VALUES (7,'Administrador');");
}

void addGroupFrom::_createTables(bool error, QSqlDatabase db)
{
    QFile f(":/Icons/DB/DBGrupo.sql");
    f.open(QFile::ReadOnly);
    QString s = f.readAll();
    f.close();
    QString grupo = "Grupo";
    grupo.append(getNombre());
    s.replace("@grupo@",grupo);
    s.replace("\r\n"," ");
    QStringList querys = s.split(";",QString::SkipEmptyParts);
    ui->frame->show();
    ui->lblProgres->setText(tr("Conectando"));
    ui->progressBar->setMaximum(querys.size());
    int i;
    QSqlQuery q(db);
    for(i = 0; i< querys.size();i++)
    {
        if(!q.exec(querys.at(i)))
        {
            qDebug() << q.lastError().text();
            qDebug() << querys.at(i);
            error = true;
            break;
        }
        ui->progressBar->setValue(i);
        ui->lblProgres->setText(tr("Creando tablas"));
        QApplication::processEvents();
    }
}

void addGroupFrom::_insertNewGroup(QString grupo)
{
    QSqlQuery q2(QSqlDatabase::database("Maya"));
    port = getPort() == 0 ? 3306 : getPort();


    q2.prepare("INSERT INTO `mayaglobal`.`grupos` "
               "(`nombre`, `bd_name`, `bd_driver`, "
               "`bd_host`, `bd_user`, `bd_pass`, `bd_port`) "
               "VALUES "
               "(:nombre, :bd_name, :bd_driver, "
               ":bd_host, :bd_user, :bd_pass, :bd_port) ");

    q2.bindValue(":nombre",getNombre());
    q2.bindValue(":bd_name",grupo);
    q2.bindValue(":bd_driver","QMYSQL");
    q2.bindValue(":bd_host",getHost());
    q2.bindValue(":bd_user",getUser());
    q2.bindValue(":bd_pass",getPass());
    q2.bindValue(":bd_port",port);

    if(!q2.exec())
    {
        QMessageBox::critical(this,tr("Error"),
                              q2.lastError().text());
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
    }
    else
    {
        id = q2.lastInsertId().toInt();
        TimedMessageBox * t = new TimedMessageBox(this, tr("Grupo creado con exito"));
        this->accept();
    }
}

void addGroupFrom::_insertAdminUser(bool error, QSqlQuery q)
{
    error &= q.exec("INSERT INTO `usuarios` (`nombre`, `contrasena`, `nivel_acceso`, `categoria`) VALUES ('admin', 'jGl25bVBBBW96Qi9Te4V37Fnqchz/Eu4qB9vKrRIqRg=', '99', 'ADMINISTRADOR');");
}

void addGroupFrom::_insertPoblaciones(bool error, QSqlQuery q)
{
    QFile f(":/Icons/DB/poblaciones.sql");
    if(f.open((QIODevice::ReadOnly | QIODevice::Text)))
    {
        QTextStream in(&f);
        in.setCodec("UTF-8");
        QString all = in.readAll();
        all.remove("\r");
        all.remove("\n");
        q.prepare(all);
        if(!q.exec())
            error = true;
    }
}

void addGroupFrom::on_pushButton_2_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(getHost());
    db.setUserName(getUser());
    db.setPassword(getPass());
    if(getPort()!=0)
        db.setPort(getPort());
    bool error = !db.open();

    if(!error)
        _createTables(error, db);

    if(!error)
    {
        QString grupo = "Grupo";
        grupo.append(getNombre());

        db.close();
        db.setHostName(getHost());
        db.setUserName(getUser());
        db.setPassword(getPass());
        db.setDatabaseName(grupo);
        if(getPort()!=0)
            db.setPort(getPort());
        db.open();

        QSqlQuery q(db);

        _insertMonedas(q, error);

        _insertIVA(q, error);

        _insertNivelAcesso(error, q);

        _insertPoblaciones(error,q);

        _insertAdminUser(error, q);

        //TODO Reports por defecto

        if(!error)
        {
            _insertNewGroup(grupo);
        }
    }
}
