#ifndef ANALITICA_H
#define ANALITICA_H

#include "../Auxiliares/Globlal_Include.h"

class Analitica : public QObject
{
    Q_OBJECT

public:
    int id;
    int idpaciente;
    int idepisodio;
    QString analisis;
    QString comentarios;
    QString valorreferencia;
    QString valor;
    QDate fecha;

    explicit Analitica(QObject *parent = 0);
    void AnadirAnalitica();
    void AnadirLineas(int idanalitica, QString descripcion, QString valor, QString referencia, QString comentarios, QString tipo);
    void recuperarDatos(int nId);
    void GuardarDatos(int nId);
    void GuardarLineas(int id, QString valor, QString referencia, QString comentarios);
    void EliminarAnalitica(int id);
    void EliminarLinea(int id);
    void setId(int id) {this->id =id;}
    void setIdEpisodio(int idepisodio) {this->idepisodio = idepisodio;}
    void setAnalisis(QString Analisis) {this->analisis = Analisis;}
    void setComentarios(QString comentarios) {this->comentarios = comentarios;}
    void setValor(QString valor) {this->valor = valor;}
    void setValorReferencia(QString valor) {this->valorreferencia = valor;}
    void setFechaAnalisis(QDate fecha) {this->fecha=fecha;}

    int getId() {return this->id;}
    int getIdEpisodio() {return this->idepisodio;}
    QString getAnalisis() {return this->analisis;}
    QString getComentarios() {return this->comentarios;}
    QString getValor() {return this->valor;}
    QString getValorReferencia() {return this->valorreferencia;}
    QDate getFechaAnalisis() {return this->fecha; }

signals:
    
public slots:

    
};

#endif // ANALITICA_H
