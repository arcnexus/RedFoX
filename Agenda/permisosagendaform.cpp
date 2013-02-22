#include "permisosagendaform.h"
#include "ui_permisosagendaform.h"

PermisosAgendaForm::PermisosAgendaForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PermisosAgendaForm),
    model(this,QSqlDatabase::database("terra"))
{
    model.setTable("permisos_agenda");
    model.setRelation(model.fieldIndex("id_Usuario_agenda"),QSqlRelation("usuarios","id","nombre"));

    ui->setupUi(this);
    ui->combo_from->setModel(Configuracion_global->usuarios_model);
    ui->combo_from->setModelColumn(Configuracion_global->usuarios_model->fieldIndex("nombre"));

    ui->combo_to->setModel(Configuracion_global->usuarios_model);
    ui->combo_to->setModelColumn(Configuracion_global->usuarios_model->fieldIndex("nombre"));

    model.setEditStrategy(QSqlTableModel::OnFieldChange);

    ui->listView->setModel(&model);
    ui->listView->setModelColumn(2);
}

PermisosAgendaForm::~PermisosAgendaForm()
{
    delete ui;
}

void PermisosAgendaForm::on_combo_from_currentIndexChanged(int index)
{
    int editor_id = Configuracion_global->usuarios_model->record(index).value(0).toInt();
    QString filter = QString("id_Usuario_editor = %1").arg(editor_id);
    model.setFilter(filter);
    model.select();
}

void PermisosAgendaForm::on_btn_add_clicked()
{
    if(ui->combo_from->currentIndex() != ui->combo_to->currentIndex())
    {
        int id_editor = Configuracion_global->usuarios_model->record(ui->combo_from->currentIndex()).value(0).toInt();
        int id_agenda = Configuracion_global->usuarios_model->record(ui->combo_to->currentIndex()).value(0).toInt();

        QSqlQuery q(QSqlDatabase::database("terra"));
        q.prepare("INSERT INTO permisos_agenda (id_Usuario_editor ,id_Usuario_agenda)"
                  "VALUES ( :editor, :agenda)");
        q.bindValue(":editor",id_editor);
        q.bindValue(":agenda",id_agenda);

        if(!q.exec())
            qDebug() << q.lastError();

        model.select();
    }
}

void PermisosAgendaForm::on_btn_remove_clicked()
{
    model.removeRow(ui->listView->currentIndex().row());
    model.submitAll();
}
