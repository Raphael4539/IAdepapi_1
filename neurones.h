// neurone.h   Bernard Ribordy
//    du 2024_06_03
// ************************************************

#ifndef NEURONE_H
#define NEURONE_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRectF>
#include <QLineF>
#include <QGraphicsLineItem>
#include <QPen>
#include <QColor>
#include <QVector>
#include "reseauDeNeurones_variablesConstantes.h"

using namespace std;

class neurone{

public:
    bool test;
    ~neurone();
    neurone(QString nom, char typeDeCell);
    void calculE(QString &testTexte);
    void calculDi(neurone &cible);
    void afficher( QString &testTexte);
    void setValues(QString nom, quint16 num, neurone *c );
    void addPointerAndSetW(neurone *pt, double poids, bool typedeCALCUL,
                           QString &testTexte);
    double getX() const;
    void setW(double pasdApprentissage, QString &testTexte);
    void setX(double entreeX);
    double getOutput() const;
    double getDi() const;
    quint16 getQnum() const;
    quint16 getCnum() const;
    void calculDi(const double &valeurCible);
    quint16  FindIndexW(neurone * ptArr);
    //   void FindAllIndex();
    double getW(quint16 i); //retourne le poids de la liaison pour le calcul
    void setE(double E);
    double searchAndGetDixWk(neurone *cible);
    //réajuste les poids des liaisons amonts (ptArr)
    quint16 gettabpArrSize() const ;
    neurone * getpArr( quint16 i) const ;
    QString getNom() const;
    void setRect(QRectF m_Rectangle);
    QRectF getRect() const;
    void setGraphicsLine(QGraphicsLineItem *graphicsLine,
                         float rotation, quint16 i);
    QGraphicsLineItem * getGraphicsLine(quint16 i) const;
    void setGraphicsEllips(QGraphicsEllipseItem *graphicsEllips);
    QGraphicsEllipseItem* getGraphicsEllips() const;
    void setGraphicsTextPoids(QGraphicsSimpleTextItem *textPoids, quint16 i);
    double getWMaxPositif() const;
    double getWMaxNegatif() const;
    void modifieCouleurs(double &WMaxMaxNegatif, double &WMaxMaxPositif);
    void modifieCouleurNeurone(QPen pen);
    void setGraphicsTextDi(QGraphicsSimpleTextItem *textDi);
    QGraphicsSimpleTextItem * getGraphicsTextDi() const;
    void setGraphicsTextOutput(QGraphicsSimpleTextItem *textOutput);
    QGraphicsSimpleTextItem * getGraphicsTextOutput() const;
    void modifieGraphDi();
    void modifieGraphOutput();
    void setSoftmax(double out);
    double getSoftmax() const;
    void setOutput(double out);
    double getE() const;

private:
    //        bool test;
    bool ok;
    QString m_nom;
    quint16  m_Qnum;
    quint16  m_Cnum;
    QString m_type;     //  'X' = neurone d'entrée, 'Y' = neurones de sortie,
        //  'B' = biais, 'C' = autres neurones
    QRectF m_Rectangle;    // position d'un rectangle pour affichage graphique
    QGraphicsEllipseItem *m_graphicsEllips;// pointeur vers le cercle graphique
    QGraphicsSimpleTextItem * m_graphicsTextDi;
        // pointeur vers le texte du Di (gradient)
    QGraphicsSimpleTextItem * m_graphicsTextOutput;
        // pointeur vers le texte de la valeur de sortie (graph)
    double m_rotation;
    double m_X;
    double m_E;
    double m_out;
    double m_softmax; // normalisation de la derniere couche
    double m_Di;        // signal d'erreur de la neurone en cours
    //   quint16 m_tabpArrSize, m_tabpDepSize;

    QVector<neurone*> m_tabpDepDyn;
        //    QVector<quint16> m_indexWDyn;
        // indice de m_tabArr ou se trouve m_Wlink
        //  de Q+1 dépendant du neurone ou biais en cours
    struct LinkArr {
        neurone *m_pArr; // pt vers le neurones de couche inférieure pour somme des W * X
        double m_WLink; // poids de la liaison
        QGraphicsLineItem  *m_graphicsLine; // pointeur de la ligne pour QGraphicScene
        QGraphicsSimpleTextItem * m_graphicsPoids; // pointeur vers le texte
            //des poids (graph)
        float m_rotation;
    } ;
    LinkArr m_linkArr;
    QVector<LinkArr> m_tabArrDyn;

    double m_WMaxPositif;
    double m_WMaxNegatif;
    QString fichierTest;
};
#endif // NEURONE_H
