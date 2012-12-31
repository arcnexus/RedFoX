#ifndef ANALITICA_H
#define ANALITICA_H

#include <QObject>

class Analitica : public QObject
{
    Q_OBJECT

private:
    int id;
    int idepisodio;
    QString analisis;
    QString tipoanalisis;
    QString comentarios;
    QString valorreferencia;
    QString valor;

public:
    explicit Analitica(QObject *parent = 0);
    void AnadirAnalitica();
    void AnadirLineas(int idanalitica, QString descripcion, QString valor, QString referencia, QString comentarios, QString tipo);
    void setId(int id) {this->id =id;}
    void setIdEpisodio(int idepisodio) {this->idepisodio = idepisodio;}
    void setAnalisis(QString Analisis) {this->analisis = Analisis;}
    void setTipoAnalisis(QString tipo) {this->tipoanalisis = tipo;}
    void setComentarios(QString comentarios) {this->comentarios = comentarios;}
    void setValor(QString valor) {this->valor = valor;}
    void setValorReferencia(QString valor) {this->valorreferencia = valor;}


    int getId() {return this->id;}
    int getIdEpisodio() {return this->idepisodio;}
    QString getAnalisis() {return this->analisis;}
    QString getTipoAnalisis() {return this->tipoanalisis;}
    QString getComentarios() {return this->comentarios;}
    QString getValor() {return this->valor;}
    QString getValorReferencia() {return this->valorreferencia;}

signals:
    
public slots:

    
};

#endif // ANALITICA_H
