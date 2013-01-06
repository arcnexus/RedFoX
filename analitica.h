#ifndef ANALITICA_H
#define ANALITICA_H

#include <QObject>
#include <QDate>

class Analitica : public QObject
{
    Q_OBJECT

private:
    int id;
    int idepisodio;
    QString analisis;
    QString comentarios;
    QString valorreferencia;
    QString valor;
    QDateTime fecha;

public:
    explicit Analitica(QObject *parent = 0);
    void AnadirAnalitica();
    void AnadirLineas(int idanalitica, QString descripcion, QString valor, QString referencia, QString comentarios, QString tipo);
    void recuperarDatos(int nId);
    void setId(int id) {this->id =id;}
    void setIdEpisodio(int idepisodio) {this->idepisodio = idepisodio;}
    void setAnalisis(QString Analisis) {this->analisis = Analisis;}
    void setComentarios(QString comentarios) {this->comentarios = comentarios;}
    void setValor(QString valor) {this->valor = valor;}
    void setValorReferencia(QString valor) {this->valorreferencia = valor;}
    void setFechaAnalisis(QDate fecha) {this->fecha.setDate(fecha);}

    int getId() {return this->id;}
    int getIdEpisodio() {return this->idepisodio;}
    QString getAnalisis() {return this->analisis;}
    QString getComentarios() {return this->comentarios;}
    QString getValor() {return this->valor;}
    QString getValorReferencia() {return this->valorreferencia;}
    QDateTime getFechaAnalisis() {return this->fecha; }

signals:
    
public slots:

    
};

#endif // ANALITICA_H
