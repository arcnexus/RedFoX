/*
 * ColumnaMoneda.cpp
 *
 *  Created on: September 2012
 *      Author: ArcNexus
 */

#include "columnamoneda.h"
#include <QStyleOptionViewItem>

ColumnaMoneda::ColumnaMoneda()
    {
    // TODO Auto-generated constructor stub

    }

ColumnaMoneda::~ColumnaMoneda()
    {
    // TODO Auto-generated destructor stub
    }

void ColumnaMoneda::paint(QPainter *painter, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
 {
    QString texto = index.model()->data(index, Qt::DisplayRole).toString();

    /* Verificamos el Index */
    QString ctexto = texto;
    // Cambio . por , o por signo elegido por los usuarios
    int tamano = ctexto.length();
    int posDec;
    posDec = tamano -3;
    if ((ctexto.midRef(posDec,1) !=".") && (ctexto.midRef(posDec,1) != ",") &&  (ctexto.midRef((posDec+1),1) !=".")
       &&  (ctexto.midRef((posDec+1),1) !=".")) {
           ctexto.append(",00") ;
    } else {
           if ((ctexto.midRef((posDec+1),1) =="."))
               ctexto.append("0");
    }

    ctexto.replace(".",",");
    // Iniciamos conversión en importe de 14 digitos
    if (ctexto.length()==14) {
        ctexto.insert(2,".");
        ctexto.insert(6,".");
        ctexto.insert(10,".");
    }
    if (ctexto.length()==13) {
        ctexto.insert(1,".");
        ctexto.insert(5,".");
        ctexto.insert(9,".");
    }
    if (ctexto.length()==12) {
        ctexto.insert(3,".");
        ctexto.insert(7,".");
    }
    if (ctexto.length()==11) {
        ctexto.insert(2,".");
        ctexto.insert(6,".");
    }
    if (ctexto.length()== 10) {
        ctexto.insert(1,".");
        ctexto.insert(5,".");
    }
    if (ctexto.length() == 9 ) {

        ctexto.insert(3, ".");
    }
    if (ctexto.length() == 8 ) {

        ctexto.insert(2, ".");
    }
    if (ctexto.length() == 7 ) {

        ctexto.insert(1, ".");
    }
    texto = ctexto;

    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
    drawDisplay(painter, myOption, myOption.rect,texto);
    drawFocus(painter, myOption, myOption.rect);

 }
