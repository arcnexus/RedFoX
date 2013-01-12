#include "frmbuscarpoblacion.h"
#include "ui_frmbuscarpoblacion.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QtSql>

FrmBuscarPoblacion::FrmBuscarPoblacion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmBuscarPoblacion)
{
    ui->setupUi(this);
    ModelPoblaciones = 0;
}

FrmBuscarPoblacion::~FrmBuscarPoblacion()
{
    delete ui;
    //delete ModelPoblaciones;
}

int FrmBuscarPoblacion::DevolverID()
{
    if(ModelPoblaciones)
    {
    QModelIndex celda=ui->ListaPoblaciones->currentIndex();
    QModelIndex index=ModelPoblaciones->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=ModelPoblaciones->data(index,Qt::EditRole);
    return pKey.toInt();
    }
    return -1;
}

void FrmBuscarPoblacion::setcPoblacion(QString cPoblacion, int nPoblacion)
{
    // cPoblacion = Cadena a Buscar
    // nPoblacion = 1 - buscar por nombre de la población (se pasará el nombre de la población como cadena)
    // nPoblacion = 0 - Buscar por cp (se pasará el código postal como cadena)
    //NOTE - pointer parent
    ModelPoblaciones = new QSqlQueryModel(this);

    //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
    Cabecera = new QHeaderView(Qt::Horizontal,this);
    QString cSQL;
    this->cPoblacion = cPoblacion;
    if (nPoblacion ==1)
    {
       cSQL = "Select  col_1 as id,col_3 as Poblacion,col_4 as CP from poblaciones where col_3 like '"+cPoblacion.trimmed()+
                    "%'";
    }
    else
    {
       //NOTE - Busqueda mas amplia
       cSQL = "Select  col_1 as id,col_3 as Poblacion,col_4 as CP from poblaciones where col_4 like '"+cPoblacion.trimmed()+
                 "%'";
    }
    ui->ListaPoblaciones->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ListaPoblaciones->setAlternatingRowColors(true);
    ModelPoblaciones->setQuery(cSQL,QSqlDatabase::database("terra"));
    ui->ListaPoblaciones->setModel(ModelPoblaciones);
    // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
    ui->ListaPoblaciones->setHorizontalHeader(Cabecera);

    /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0". (en nuestro caso está oculta ya que muestra el id de la tabla y esto no nos interesa que lo vea el usuario */
    //  Cabecera->setResizeMode(0,QHeaderView::Fixed);
    Cabecera->resizeSection(0,0);
    //  Cabecera->setResizeMode(1,QHeaderView::Fixed);
    Cabecera->resizeSection(1,600);
    // Cabecera->setResizeMode(2,QHeaderView::Fixed);
     Cabecera->resizeSection(2,50);
     ui->ListaPoblaciones->setFocus();
}

