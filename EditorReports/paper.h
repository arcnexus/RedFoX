#ifndef PAPER_H
#define PAPER_H

#include <QFileDialog>
#include <QApplication>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QPrinter>
#include <QPdfWriter>

#include <QDomNode>

#include "detailsection.h"
#include "pageheadersection.h"

/*namespace Paper_size {

struct _size {
        double width;
        double heigth;
    };

    _size StandarSizes [];
}*/

class Paper :public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    explicit Paper(QGraphicsItem *parent = 0);

    Q_PROPERTY(_Orientacion Orientacion READ Orientacion WRITE setOrientacion NOTIFY orientacionChanged)
    Q_PROPERTY(double margenSuperior READ margenSuperior WRITE setmargenSuperior NOTIFY margenSuperiorChanged)
    Q_PROPERTY(double margenInferior READ margenInferior WRITE setmargenInferior NOTIFY margenInferiroChanged)
    Q_PROPERTY(double margenIzquierdo READ margenIzquierdo WRITE setmargenIzquierdo NOTIFY margenIzquierdoChanged)
    Q_PROPERTY(double margenDerecho READ margenDerecho WRITE setmargenDerecho NOTIFY margenDerechoChanged)
    Q_PROPERTY(docType tipoDoc READ tipoDoc WRITE settipoDoc NOTIFY tipoDocChanged)

    Q_PROPERTY(double lblDistH READ lblDistH WRITE setlblDistH NOTIFY lblDistHChanged)
    Q_PROPERTY(double lblDistV READ lblDistV WRITE setlblDistV NOTIFY lblDistVChanged)
    Q_PROPERTY(int lblColumnCount READ lblColumnCount WRITE setlblColumnCount NOTIFY lblColumnCountChanged)
    Q_PROPERTY(int lblRowCount READ lblRowCount WRITE setlblRowCount NOTIFY lblRowCountChanged)
    Q_PROPERTY(bool lblPrinter READ lblPrinter WRITE setlblPrinter NOTIFY lblPrinterChanged)

    Q_PROPERTY(double lblPaperW READ lblPaperW WRITE setlblPaperW NOTIFY lblPaperWChanged)
    Q_PROPERTY(double lblPaperH READ lblPaperH WRITE setlblPaperH NOTIFY lblPaperHChanged)
    Q_PROPERTY(double lblPaperMarginS READ lblPaperMarginS WRITE setlblPaperMarginS NOTIFY lblPaperMarginSChanged)
    Q_PROPERTY(double lblPaperMarginInf READ lblPaperMarginInf WRITE setlblPaperMarginInf NOTIFY lblPaperMarginInfChanged)
    Q_PROPERTY(double lblPaperMarginIzq READ lblPaperMarginIzq WRITE setlblPaperMarginIzq NOTIFY lblPaperMarginIzqChanged)
    Q_PROPERTY(double lblPaperMarginDer READ lblPaperMarginDer WRITE setlblPaperMarginDer NOTIFY lblPaperMarginDerChanged)


    Q_ENUMS (_Orientacion)
    enum _Orientacion { Retrato , Apaisado };

    Q_ENUMS (_Sizes)
    enum _Sizes { A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,
                  B0,B1,B2,B3,B4,B5,B6,B7,B8,B9,B10,
                  C5E,Comm10E,DLE,Executive,Folio,
                  Ledger,Legal,Letter,Tabloid,
                  Custom};

    Q_ENUMS (itemType)
    enum itemType { RoundRectIt , Label, Linea , CodeBarIt , Imagen, Campo , CampoRelacional , Parametro , Acumulador};

    Q_ENUMS (docType)
    enum docType { _Report , _etiqueta , _sobre, _mail };

    _Orientacion Orientacion() const;

    QRectF margin();
    QRectF paper();
    QSizeF paperSize();
    _Sizes StandartSize();

    double margenSuperior() const;
    double margenInferior() const;
    double margenIzquierdo() const;
    double margenDerecho() const;

    void setSize(_Sizes siz, double w=0, double h=0, _Orientacion o=Retrato);

    bool addSection(QString nombre ,Section::SectionType sectionType);
    void removeSection(QString nombre);

    void prepareItemInsert(itemType sectionType);
    void stopInsertingItems();

    bool parseXML(QString xml, QString &error);
    int save(QString file );
    QDomDocument preview();


    QList<Section *> getSeccionPool() const;

    void removeItems(QList<QGraphicsItem *> items);
    void insertSection(Section* sec);
    void subirSeccion(QString name);
    void bajarSeccion(QString name);
    void borrarSeccion(QString name);
    void updatePaper();

    static qreal cmToPx(double cm);
    static qreal pxTocm(int px);
    docType tipoDoc() const;

    double lblDistH() const;
    double lblDistV() const;
    int lblColumnCount() const;
    int lblRowCount() const;
    bool lblPrinter() const;

    double lblPaperH() const;
    double lblPaperW() const;
    double lblPaperMarginS() const;
    double lblPaperMarginInf() const;
    double lblPaperMarginIzq() const;
    double lblPaperMarginDer() const;

signals:
    
    void orientacionChanged(_Orientacion arg);

    void margenSuperiorChanged(double arg);
    void margenInferiroChanged(double arg);
    void margenIzquierdoChanged(double arg);
    void margenDerechoChanged(double arg);
    void itemInserted();

    void tipoDocChanged(docType arg);
    void lblDistHChanged(double arg);
    void lblDistVChanged(double arg);
    void lblColumnCountChanged(int arg);
    void lblRowCountChanged(int arg);
    void lblPrinterChanged(bool arg);
    void lblPaperHChanged(double arg);
    void lblPaperWChanged(double arg);
    void lblPaperMarginSChanged(double arg);
    void lblPaperMarginInfChanged(double arg);
    void lblPaperMarginIzqChanged(double arg);
    void lblPaperMarginDerChanged(double arg);

public slots:

    void setOrientacion(_Orientacion arg);

    void setMargen(double arg);
    void setmargenSuperior(double arg);
    void setmargenInferior(double arg);
    void setmargenIzquierdo(double arg);
    void setmargenDerecho(double arg);

    void reCalculateSeccion(Section * = 0);
    void settipoDoc(docType arg);

    void setlblDistH(double arg);
    void setlblDistV(double arg);
    void setlblColumnCount(int arg);
    void setlblRowCount(int arg);
    void setlblPrinter(bool arg);

    void setlblPaperH(double arg);
    void setlblPaperW(double arg);
    void setlblPaperMarginS(double arg);
    void setlblPaperMarginInf(double arg);
    void setlblPaperMarginIzq(double arg);
    void setlblPaperMarginDer(double arg);

    void newDoc();
private slots:
    void itemMoved(Container *);
private:
    _Orientacion m_orientacion;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void setSize(double w, double h);
    _Sizes mySize;

    double m_margenSuperior;

    itemType _insertingType;
    bool _inserting;
    QPointF _insertingPoint;

    QList<Section *> seccionPool;
    void _insertSection(Section* sec);

    QList<Container*> itemPool;
    void insertRoundRect(Section *);
    void insertLabel(Section *sec);
    void insertLinea(Section *);
    void insertCodeBar(Section *);
    void insertImagen(Section *);
    void insertCampo(Section *sec);
    void insertCampoRelacional(Section *sec);
    void insertParametro(Section *sec);
    void insertAcumulador(Section *sec);

    double m_margenInferiro;
    double m_margenIzquierdo;
    double m_margenDerecho;
    docType m_tipoDoc;
    double m_lblDistH;
    double m_lblDistV;
    int m_lblColumnCount;
    int m_lblRowCount;
    bool m_lblPrinter;
    double m_lblPaperH;
    double m_lblPaperW;
    double m_lblPaperMarginS;
    double m_lblPaperMarginInf;
    double m_lblPaperMarginIzq;
    double m_lblPaperMarginDer;

    double m_cmW;
    double m_cmH;
};

#endif // PAPER_H
