#ifndef ANALITICA_H
#define ANALITICA_H

#include "../Auxiliares/Globlal_Include.h"

class Analitica : public QObject
{
    Q_OBJECT

public:
    int id;
    int id_paciente;
    int id_episodio;
    QString analisis;
    QString comentarios;
    QString valorreferencia;
    QString valor;
    QDate fecha;

    explicit Analitica(QObject *parent = 0);
    void AnadirAnalitica();
    void AnadirLineas(int id_analitica, QString descripcion, QString valor, QString referencia, QString comentarios, QString tipo);
    void recuperarDatos(int nid);
    void GuardarDatos(int nid);
    void GuardarLineas(int id, QString valor, QString referencia, QString comentarios);
    void EliminarAnalitica(int id);
    void EliminarLinea(int id);
    void setid(int id) {this->id =id;}
    void setid_episodio(int id_episodio) {this->id_episodio = id_episodio;}
    void setAnalisis(QString Analisis) {this->analisis = Analisis;}
    void secomentarios(QString comentarios) {this->comentarios = comentarios;}
    void setValor(QString valor) {this->valor = valor;}
    void setValorReferencia(QString valor) {this->valorreferencia = valor;}
    void setfecha_analisis(QDate fecha) {this->fecha=fecha;}

    int getid() {return this->id;}
    int getid_episodio() {return this->id_episodio;}
    QString getAnalisis() {return this->analisis;}
    QString gecomentarios() {return this->comentarios;}
    QString getValor() {return this->valor;}
    QString getValorReferencia() {return this->valorreferencia;}
    QDate getfecha_analisis() {return this->fecha; }

signals:
    
public slots:

    
};

#endif // ANALITICA_H
