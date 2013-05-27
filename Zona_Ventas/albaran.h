#ifndef ALBARAN_H
#define ALBARAN_H

#include "../Auxiliares/Globlal_Include.h"

class Albaran : public QObject
{
    Q_OBJECT
public:
    explicit Albaran(QObject *parent = 0);
    ~Albaran();

    int id;
    int nAlbaran;
    QDate dFecha;
    QString cPedidoCli;
    int iId_Cliente;
    QString cCodigoCliente;
    QString cCliente;
    QString cDireccion;
    QString cDireccion2;
    QString cPoblacion;
    QString cProvincia;
    QString cCp;
    int idPais;
    QString cPais;
    QString cCif;
    int lRecargoEquivalencia;
    double rSubtotal;
    double nDto;
    double rDto;
    double rBase1;
    double rBase2;
    double rBase3;
    double rBase4;
    double nPorcentajeIVA1;
    double nPorcentajeIVA2;
    double nPorcentajeIVA3;
    double nPorcentajeIVA4;
    double rImporteIva1;
    double rImporteIva2;
    double rImporteIva3;
    double rImporteIva4;
    double nPorcentajeRecargoEq1;
    double nPorcentajeRecargoEq2;
    double nPorcentajeRecargoEq3;
    double nPorcentajeRecargoEq4;
    double rImporteRecargoEq1;
    double rImporteRecargoEq2;
    double rImporteRecargoEq3;
    double rImporteRecargoEq4;
    double rTotal1;
    double rTotal2;
    double rTotal3;
    double rTotal4;
    double rBaseTotal;
    double rIvaTotal;
    double rRecargoEqTotal;
    double rTotalAlbaran;
    int lImpreso;
    int lFacturado;
    QString cFactura;
    QDate dFechaFactura;
    QString tComentario;
    double rACuenta;
signals:

public slots:
    // Metodos de trabajo Clase Albaran
    bool AnadirAlbaran();
    bool RecuperarAlbaran(QString cSQL);
    bool borrarLineas(int nId_Albaran);
    bool GuardarAlbaran(int nId_Albaran);
    int NuevoNumeroAlbaran();

    void FacturarAlbaran();
};

#endif // ALBARAN_H
