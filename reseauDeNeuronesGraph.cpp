// reseauDeNeuronesGraph.cpp   Bernard Ribordy
//    du 2024_06_03

#include "reseauDeNeurones_variablesConstantes.h"
#include "neurones.h"
#include <QWidget>
#include <cmath>
#include <QString>
#include "reseauDeNeuronesGraph.h"

using namespace std;

reseauDeNeuronesGraph::reseauDeNeuronesGraph(QWidget *parent) :QWidget(parent){
    resize(1300,1000);
}

void reseauDeNeuronesGraph::ConstructeurDyn(quint16 &NQ, quint16 NC[], quint16 NB[],
    quint16 &NCNBMax, QVector<QVector<neurone*>> &tabBiaisDyn,
    QVector<QVector<neurone*>> &tabCellDyn, QCheckBox* afficheDetails){

    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
 //   resize(1900, 1000);
 //   resize(2500,1000);
    QPen penBiais(Qt::red,2,Qt::SolidLine);
    QPen penCell(Qt::blue,2,Qt::SolidLine);
    QPen penLine(Qt::black ,2,Qt::SolidLine);

    QLineF maLigne;
    double  margeH = height()/10;
 //   double margeW  = width()/10;  // *** 15/5/25
    double distEntreQ = (width() - 2 * margeH) / (NQ + 2.0);
    double distEntreC;
    double diamEllipse = qMin(width(),height()) / (NCNBMax *5);
    double moveHBais = distEntreQ / 4.0;
    double deltaXQ0;
    double deltaYQ0;
    double deltaXnoQ0;
    double deltaYnoQ0;
    double rotation;
    double deltaYTextOut;
    double deltaYTextDi;
    double deltaXTextOut;
    double deltaXTextDi;
    double diamEllipsePourAffichage(40);

    if (diamEllipse > diamEllipsePourAffichage){       // peu de neurones
        deltaXQ0 = -14.0;      // le texte se trouve à gauche du cercle pour Q0
        deltaYQ0 = -16.0;
        deltaXnoQ0 = -20.0; // le texte se trouve au centre du cercle pour Q>Q0
        deltaYnoQ0 = -16.0;
        penLine.setWidth(2);  // new QPen(Qt::black ,2,Qt::SolidLine);
        deltaYTextOut = 12.0; // en dessous du nom
        deltaYTextDi =24.0;  // en dessous du Di
        deltaXTextDi = 0.0;    // par rapport à deltaXnoQ0
        deltaXTextOut = 0.0;   // idem
    }
    else {                       // beaucoup de neurones
        deltaXQ0 = -55.0;      // le texte se trouve à gauche du cercle pour Q0
        deltaYQ0 = 10.0;
        deltaXnoQ0 = 0.0;        // à droite pour les autres couches ///
        deltaYnoQ0 = +10;
        penLine.setWidth(1);
        deltaYTextOut = 12.0; // en dessous du nom
        deltaYTextDi =24.0;  // en dessous du Di
        deltaXTextDi = 10.0;    // par rapport à deltaXnoQ0
        deltaXTextOut = -5.0;   // idem
    }
    QRectF rectangle;
    QPointF centreNeuroneDep;
    QPointF centreNeuroneArr;
    QPointF  centreNeuroneNom;
    m_scene = new QGraphicsScene;//(this);

    for ( quint16 q = 0;q < NQ; q++){
        distEntreC = height()/(NC[q]+NB[q] + 1.0);
        for (quint16 b = 0; b < NB[q]; b++){//attribue un rectangle graphique au biais
            rectangle =  QRectF(margeH + q * distEntreQ + moveHBais,  b * distEntreC, //
                               diamEllipse, diamEllipse);
            tabBiaisDyn[q][b]->setRect(rectangle);
            m_ellips = m_scene->addEllipse(rectangle,penBiais);
            // le cercle sera à l'interieur du rectangle
            tabBiaisDyn[q][b]->setGraphicsEllips(m_ellips);
            //le pointeur est copié comme attribue dans la cellule (neurone)

            QString m_texteNom = tabBiaisDyn[q][b]->getNom();
            QGraphicsSimpleTextItem  *textNom =
                m_scene->addSimpleText(m_texteNom,QFont("Times", 6));
            centreNeuroneNom = tabBiaisDyn[q][b]->getRect().center();
            if (q == 0){
                textNom->setX(centreNeuroneNom.x() + deltaXQ0);
                textNom->setY(centreNeuroneNom.y() + deltaYQ0);
            }
            else {
                textNom->setX(centreNeuroneNom.x() + deltaXnoQ0);
                textNom->setY(centreNeuroneNom.y() + deltaYnoQ0);
            }
            m_texteOutput = m_scene->addSimpleText("Out=1",QFont("Times",6));
            m_texteOutput->setX(textNom->x() + deltaXTextOut);
            m_texteOutput->setY(textNom->y()+ deltaYTextOut);   // en dessous du nom
            tabBiaisDyn[q][b]->setGraphicsTextOutput(m_texteOutput);
            // on ajoute le pointeur de ce texte dans le neurone

            m_texteDi = m_scene->addSimpleText("Di",QFont("Times",6));
            m_texteDi->setX(textNom->x()+ deltaXTextDi);
            m_texteDi->setY(textNom->y()+ deltaYTextDi);
            tabBiaisDyn[q][b]->setGraphicsTextDi(m_texteDi);
            // on ajoute le pointeur de ce texte dans le neurone
            if (!afficheDetails->isChecked()){
                m_texteDi->setText("");
                m_texteOutput->setText("");
            }
        }
        for (quint16 c = 0;c  < NC[q]; c++){
            rectangle = QRectF( margeH + q * distEntreQ , NB[q]*distEntreC +
                c * distEntreC, diamEllipse,diamEllipse);
            tabCellDyn[q][c]->setRect(rectangle);
            // attribue un rectangle graphique au neurones
            m_ellips = m_scene->addEllipse(rectangle,penCell);
            // le cercle sera à l'interieur du rectangle
            tabCellDyn[q][c]->setGraphicsEllips(m_ellips);
            //le pointeur est copié dans le neurone

            QString m_texteNom = "    " + tabCellDyn[q][c]->getNom() + "    ";
            QGraphicsSimpleTextItem  *textNom =
                m_scene->addSimpleText(m_texteNom,QFont("Times", 7));
            centreNeuroneNom= tabCellDyn[q][c]->getRect().center();
            if (q == 0){
                textNom->setX(centreNeuroneNom.x() + deltaXQ0);
                textNom->setY(centreNeuroneNom.y() + deltaYQ0);
            } else {
                textNom->setX(centreNeuroneNom.x() + deltaXnoQ0);
                textNom->setY(centreNeuroneNom.y() + deltaYnoQ0);
            }
            m_texteOutput = m_scene->addSimpleText("Out",QFont("Times",7));
            m_texteOutput->setX(textNom->x() + deltaXTextOut);
            m_texteOutput->setY(textNom->y()+ deltaYTextOut);//en dessous du nom
            tabCellDyn[q][c]->setGraphicsTextOutput(m_texteOutput);
                // on ajoute le pointeur de ce texte dans le neurone
            m_texteDi = m_scene->addSimpleText("Di",QFont("Times",6));
            m_texteDi->setX(textNom->x()+ deltaXTextDi);
            m_texteDi->setY(textNom->y()+ deltaYTextDi);
            tabCellDyn[q][c]->setGraphicsTextDi(m_texteDi);
                // on ajoute le pointeur de ce texte dans le neurone
            if (!afficheDetails->isChecked()){
                m_texteDi->setText("");
                m_texteOutput->setText("");
            }
        }
    }
    for ( quint16 q = 1;q < NQ; q++){
        for (quint16 c = 0; c < NC[q]; c++){
            if (c<20){
                centreNeuroneArr = tabCellDyn[q][c]->getRect().center();
                quint16 tabArrsize = tabCellDyn[q][c]->gettabpArrSize();
                // cherchons les pointeurs vers les couches inferieures
                // et reprenons les noms des neurones
                for (quint16 i = 0; i < tabArrsize; i++){ // dessin des lignes
                    centreNeuroneDep = tabCellDyn[q][c]->getpArr(i)->getRect().center();
                    double Arc = atan((centreNeuroneArr.y()-centreNeuroneDep.y()) /
                                      (centreNeuroneArr.x()-centreNeuroneDep.x()));
                    double deltaX = diamEllipse/2 * cos(Arc);
                    double deltaY = diamEllipse/2 * sin(Arc);
                    maLigne = QLineF(centreNeuroneDep.x()+deltaX,
						centreNeuroneDep.y()+deltaY, centreNeuroneArr.x()-deltaX,
						centreNeuroneArr.y()-deltaY);
                    m_liaison = m_scene->addLine(maLigne,penLine);

                    tabCellDyn[q][c]->setGraphicsLine(m_liaison, maLigne.angle(), i);
                    // copie pour modif par neurone
                    // range le pt de cette ligne dans la table des liaison
                    // vers le neurone de depart
                    QGraphicsSimpleTextItem *textPoids =
                        m_scene->addSimpleText("poids",QFont("Times",7));
                    textPoids->setX(maLigne.center().x());
                    textPoids->setY(maLigne.center().y());
                    rotation = m_liaison->line().angle();
                    textPoids->setRotation(360.0-rotation);
                    tabCellDyn[q][c]->setGraphicsTextPoids(textPoids,i);
                    if (!afficheDetails->isChecked()){
                        textPoids->setText("");
                    }
                }
            }
        }
    }
    m_view = new QGraphicsView(m_scene);
 //   m_view->setContentsMargins(20,20,20,20);
    layoutPrincipal->addWidget(m_view,3);
    setLayout(layoutPrincipal);
}
/*
QGraphicsView* reseauDeNeuronesGraph::getGraphiqueView(){
    return m_view;
}
*/
reseauDeNeuronesGraph::~reseauDeNeuronesGraph()
{};
