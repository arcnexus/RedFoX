#include "entregascuenta.h"

EntregasCuenta::EntregasCuenta(QObject *parent) :
    QObject(parent)
{
}

bool EntregasCuenta::Anadir(int tipo, int id_pro_cli, double importe, QDate fecha,QString concepto)
{
    QSqlQuery queryEntregas(QSqlDatabase::database("Maya"));
    QSqlDatabase::database("Maya").transaction();
    switch (tipo) {
    case 1: // Clientes

        queryEntregas.prepare("INSERT INTO clientes_entregas ("
                              "id_cliente,fecha_entrega,importe,concepto,disponible)"
                              " VALUES (:id_cliente,:fecha_entrega,:importe,:concepto,:disponible);");


        queryEntregas.bindValue(":id_cliente",id_pro_cli);
        queryEntregas.bindValue(":fecha_entrega",fecha);
        queryEntregas.bindValue(":importe",importe);
        queryEntregas.bindValue(":concepto",concepto);
        queryEntregas.bindValue(":disponible",importe);
        if(queryEntregas.exec())
        {
            queryEntregas.prepare("update clientes set rImporteACuenta = rImporteACuenta + :importe"
                                  " where id =:id_cli");
            if(queryEntregas.exec()){
                QSqlDatabase::database("Maya").commit();
                return true;
            } else
            {
                QSqlDatabase::database("Maya").rollback();
                return false;
            }
        } else
        {
            QSqlDatabase::database("Maya").rollback();
            return false;
        }
        break;
    case 2: // Proveedores
        queryEntregas.prepare("INSERT INTO proveedor_a_cuenta ("
                              "id_proveedor,fecha_entrega,importe,concepto,disponible)"
                              " VALUES (:id_cliente,:fecha_entrega,:importe,:concepto,:disponible);");


        queryEntregas.bindValue(":id_cliente",id_pro_cli);
        queryEntregas.bindValue(":fecha_entrega",fecha);
        queryEntregas.bindValue(":importe",importe);
        queryEntregas.bindValue(":concepto",concepto);
        queryEntregas.bindValue(":disponible",importe);
        if(queryEntregas.exec())
        {
            queryEntregas.prepare("update proveedores set rEntregadoaCuenta = rEntregadoaCuenta + :importe"
                                  " where id =:id_pro");
            queryEntregas.bindValue(":importe",importe);
            queryEntregas.bindValue(":id_pro",id_pro_cli);
            if(queryEntregas.exec()){
                QSqlDatabase::database("Maya").commit();
                return true;
            } else
            {
                QSqlDatabase::database("Maya").rollback();
                return false;
            }
        } else
        {
            QSqlDatabase::database("Maya").rollback();
            return false;
        }
        break;

    default:
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir Entrega a cuenta"),
                             tr("Error en definición de tipo"), tr("Aceptar"));

        return false;
        break;
    }
}
