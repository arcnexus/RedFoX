#include "frmlistadosarticulo.h"
#include "ui_frmlistadosarticulo.h"
#include <parameter.h>
#include "openreports.h"

frmListadosArticulo::frmListadosArticulo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmListadosArticulo)
{
    ui->setupUi(this);
    connect(ui->btnPrevison,SIGNAL(clicked()),this,SLOT(btn_Preview_clicked()));
}

frmListadosArticulo::~frmListadosArticulo()
{
    delete ui;
}

void frmListadosArticulo::btn_Preview_clicked()
{
    ParameterList params;
    params.append( "item_id", 1 );

    orReport report("articulos", params);
    ;
    if (report.isValid())
      report.print(0,true,true);
    else
      report.reportError(this);
}
