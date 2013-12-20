#ifndef REPORTRENDERER_H
#define REPORTRENDERER_H

#include <QObject>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QApplication>
#include <QPainter>

#include <QDomDocument>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
class ReportRenderer : public QObject
{
    Q_OBJECT
public:
    explicit ReportRenderer(QObject *parent = 0);
    QDomDocument render(QPrinter* printer , QDomDocument in , QMap<QString,QString> queryClausules, QMap<QString, QString> params, bool& error);
    QPrinter *getPrinter() const;
    void setPrinter(QPrinter *value);

    QDomDocument getDocIn() const;
    void setDocIn(const QDomDocument &value);

    QMap<QString, QString> getQueryClausules() const;
    void setQueryClausules(const QMap<QString, QString> &value);

    QMap<QString, QString> params() const;
    void setParams(const QMap<QString, QString> &params);

    void drawElement(qreal dpix, QPainter *painter, QDomElement element, int printResolution, qreal dpiy);
signals:
    void end();
public slots:
    void Print(QPrinter *printer);
    void PreRender();
private:
    QPrinter* printer;
    QPrintPreviewDialog* dlg;
    QPair<QString, QString> getSql(QString,QMap<QString,QString> queryClausules);
    QString getRelationField(QString s , QSqlRecord r);
    QDomNode startPage(double pageUsable, int RFooterSiz, int RHSiz, int RFootSiz, QDomDocument doc , bool pageHeader , QDomNode pHeaderNode, QMap<QString,QSqlRecord> selects, bool reporHeader = false, QDomNode rHeaderNode = QDomNode());
    void parseFooters(QDomNode RFooter , bool haveRfooter , QDomNode PFooter , bool havePFooter ,QMap<QString,QSqlRecord> selects );
    QDomDocument preRender(QPainter *painter, QDomDocument in , QMap<QString,QString> queryClausules, QMap<QString, QString> params, bool& error);
    QDomDocument m_doc;
    QDomDocument DocIn;
    QMap<QString,QString> queryClausules;
    QMap<QString,QString> _params;

    void drawRect(QDomElement e , QPainter * painter , double dpiX , double dpiY, int printResolution);
    void drawLabel(QDomElement e , QPainter * painter , double dpiX , double dpiY);
    void drawLine(QDomElement e , QPainter * painter , double dpiX , double dpiY, int printResolution);
    void drawImage(QDomElement e , QPainter * painter , double dpiX , double dpiY);
    void drawCodeBar(QDomElement e , QPainter * painter , double dpiX , double dpiY);

    QString ColorString(QColor c);
    QColor  ColorFromString(QString s );
    QString applyFormato(QString in , int formato);

    enum _Orientacion { Vertical , Horizontal };
    enum _Aling { Left , Center , Rigth };
    QDomNode endPage(QDomNode pageNode, QDomNode footerNode, QMap<QString, float> _Acums);
    float getNumber(QString in, int formato);
};

#endif // REPORTRENDERER_H
