// neurones.cpp   Bernard Ribordy
//    du 2025_03_02
// ************************************************

#include "neurones.h"
#include<math.h>
#include <QTextStream>
#include <QString>

//#define hp // ******* sur pc hp  DIRECTIVES DE COMPILATION
#define notest // ******* DIRECTIVES DE COMPILATION

using namespace std;

neurone::neurone(QString nom, char typeDeCell){
    //   const int nbLiMax_1(800);   // nombre de liaisons max pour 1 neurone

    //    test = false;
    //**************** test = **********************
    m_nom=nom;
    m_Rectangle=QRectF(0,0,0,0);
    m_out = 0;
    m_type=typeDeCell;
    if (m_type=='B')
    { m_out = 1;};
    m_E = 0;
    m_Di =0;
    m_WMaxNegatif = 0;
    m_WMaxPositif = 0;
    m_graphicsEllips = nullptr;
    m_graphicsTextDi = nullptr;
    m_graphicsTextOutput = nullptr;

    m_tabArrDyn.resize(0);
    m_tabpDepDyn.resize(0);
    fichierTest = "";
    m_Qnum = m_nom.mid(1,1).toInt(&ok);
    m_Cnum = m_nom.mid(3,4).toInt(&ok);
}

neurone::~neurone(){ //destructeur
    for (int a=0; a < m_tabArrDyn.size();a++){
        m_tabArrDyn[a].m_pArr= nullptr; //pointeur vers neurone donnant le signal
        m_tabArrDyn[a].m_WLink=0; // poids de la liaison (calcul des outputs)
    }
    for (int d=0;d<m_tabpDepDyn.size();d++){
        m_tabpDepDyn[d]= nullptr; // pointeur vers neurones recevant le signal (calcul Di)
    }
}

void neurone::calculE(QString &testTexte){ // E = entree d'un neurone
    double E =0;
    for (auto i=0; i < m_tabArrDyn.size(); i++){
        E +=  m_tabArrDyn[i].m_WLink * m_tabArrDyn[i].m_pArr->getOutput();
        m_E = E;
    }
}

void neurone::addPointerAndSetW(neurone *pt, double poids, bool typedeCALCUL,
                                QString &testTexte ){
    const bool calculOut = true;

    if (typedeCALCUL == calculOut){
        m_linkArr.m_pArr = pt;
        m_linkArr.m_WLink = poids;
        m_linkArr.m_graphicsPoids = nullptr;
        m_linkArr.m_graphicsLine = nullptr;
        m_linkArr.m_rotation = 0.0;

        m_tabArrDyn.push_back(m_linkArr);
    }
    else{
        m_tabpDepDyn.push_back(pt);
    }
}

// recherche la branche qui me regarde et returne son poids * Di
double neurone::searchAndGetDixWk(neurone *cible){
    double DixWk =0;
    for (int k=0;k < m_tabArrDyn.size(); k++){
        if(m_tabArrDyn[k].m_pArr == cible){ // la branche regarde la cible
         //   qDebug() << "k " << k << "m_tabArrDyn[k].m_pArr " << m_tabArrDyn[k].m_pArr->m_nom <<
         //       " cible " << cible;
            DixWk =  getW(k) * getDi();           //m_tabArrDyn[k].m_WLink * getDi();
            break;
        }
    }
    return DixWk;
}

void neurone::calculDi(const double &valeurCible){ // nouveau
    // DI est une valeur de correction des poids amont, issue de la différence entre la
    //cible et la valeur de sortie d'un neurone en supposant que la fonction de transfert du
    // neurone est une sigmoid.
    // Pour chaque neurone aval relie au neurone en cours, le Di aval est multiplié par
    // le poids (W) qui le relie au neurone en cours puis additionne (voir "Di" ci-dessous).
    // Cette somme sera ensuite multipliée par m_out *(1 - m_out) pour donner le DI du
    // neurone en cours. m_tabpDepDyn est ne table de pointeurs vers les neurones avals,
    // m_tabArrDyn est une table de pointeurs vers l'amont.

    // m_tabpDepDyn[i]-> m_tabArrDyn trouve cette table du neurone aval.

    // si(m_tabpDepDyn[i] == m_tabpDepDyn[i]-> m_tabArrDyn[k].m_pArr), les deux neurones
    // se regardent

    double Di=0.0, m_Di = 0.0;
    if (m_type == 'Y'){ // couche de sortie
        this->m_Di = (valeurCible - m_out) * m_out *(1 - m_out); // signal d'erreur dk
    }
    else if (m_type == 'C'){ // couche cach�e
        for (int i=0; i< m_tabpDepDyn.size(); i++){
            // pour toute cellules aval qui me regarde
            //retourne Wi-k * Dk  du neurone aval
 //           qDebug() <<  " i " << i <<" this " << this ;

            Di += m_tabpDepDyn[i]->searchAndGetDixWk(this);
        }
        m_Di = Di * m_out *(1 - m_out); // signal d'erreur dj
    }
}


void neurone::setW(double pasdApprentissage, QString &testTexte){
    int i;
    for (i=0; i < m_tabArrDyn.size(); i++){
        m_tabArrDyn[i].m_WLink += pasdApprentissage *
		m_tabArrDyn[i].m_pArr->getOutput() * m_Di;
        if (m_tabArrDyn[i].m_WLink > m_WMaxPositif && m_tabArrDyn[i].m_WLink >= 0)
            m_WMaxPositif = m_tabArrDyn[i].m_WLink; // pour la couleur des liaisons
        if (m_tabArrDyn[i].m_WLink < m_WMaxNegatif && m_tabArrDyn[i].m_WLink < 0)
            m_WMaxNegatif = m_tabArrDyn[i].m_WLink;
    }
}

void neurone::afficher(QString &testTexte){
    // provisoir
    testTexte += " neurone: " + m_nom + ", type= " + m_type  + ", m_out: " +
                 QString::number(m_out) +", m_E: " + QString::number(m_E) + " ";
    if (m_type == 'C' || m_type == 'Y' )
        testTexte += ", m_Di= " + QString::number( m_Di);
    //   else testTexte += "\n";
    for(auto i=0;i < m_tabpDepDyn.size();i++){
        if(i % 3 == 0 and i != 0) testTexte += "\n";
        testTexte += " pt aval: " +QString::number(i) +" "+m_tabpDepDyn[i]->getNom() + ", ";
    }
    testTexte += "\n";
    for(auto i=0;i<m_tabArrDyn.size();i++){
        if(i % 3 == 0 and i != 0) testTexte += "\n";
        testTexte += " pt amont: " +QString::number(i) +" " + 
		m_tabArrDyn[i].m_pArr->getNom()
			 +", poids: " + QString::number(m_tabArrDyn[i].m_WLink) + ", m-outPrec: " +
                     QString::number(m_tabArrDyn[i].m_pArr->getOutput())
            ;
    }
    testTexte += "\n\n";
}

double neurone::getWMaxPositif() const {return m_WMaxPositif;}

double neurone::getWMaxNegatif() const {return m_WMaxNegatif;}

void neurone::setX(double entreeX) {m_X=entreeX;}

double neurone::getDi() const {return m_Di;}

QString neurone::getNom() const {return m_nom;}

double neurone::getW(quint16 index){ // retourne le poids de la liaison pour le calcul
    // de la somme W(k)*di(k) et le graphique
    return  m_tabArrDyn[index].m_WLink;
}

void neurone::setE(double E){ m_E = E;}

double neurone::getE() const
{return m_E;}

double neurone::getX() const {return m_X;}

quint16 neurone::getQnum() const {return m_Qnum;}

quint16 neurone::getCnum() const {return m_Cnum;}

double neurone::getOutput() const {return m_out;}

quint16 neurone::gettabpArrSize() const
{return m_tabArrDyn.size();}

neurone*  neurone::getpArr( quint16 i) const
{return  m_tabArrDyn[i].m_pArr;}

QRectF neurone::getRect() const {return m_Rectangle;}

void neurone::setRect(QRectF rectangle)
{m_Rectangle=rectangle;}

void neurone::setGraphicsLine(QGraphicsLineItem *graphicsLine, float Rotation, quint16 i)
{m_tabArrDyn[i].m_graphicsLine = graphicsLine;
    m_tabArrDyn[i].m_rotation = Rotation;}

QGraphicsLineItem* neurone::getGraphicsLine(quint16 i) const
{return m_tabArrDyn[i].m_graphicsLine;}

void neurone::setGraphicsEllips(QGraphicsEllipseItem *graphicsEllips)
{m_graphicsEllips = graphicsEllips;}

QGraphicsEllipseItem* neurone::getGraphicsEllips() const
{return m_graphicsEllips;}

void neurone::setGraphicsTextPoids(QGraphicsSimpleTextItem *textPoids, quint16 i)
{m_tabArrDyn[i].m_graphicsPoids = textPoids;} // copie ici le pointeur

void neurone::setGraphicsTextDi(QGraphicsSimpleTextItem *textDi)
{m_graphicsTextDi = textDi;}

QGraphicsSimpleTextItem * neurone::getGraphicsTextDi() const  // ???
{return m_graphicsTextDi;}   // pointeur du poids (W)

void neurone::setGraphicsTextOutput(QGraphicsSimpleTextItem *textOutput)
{m_graphicsTextOutput = textOutput;}

QGraphicsSimpleTextItem * neurone::getGraphicsTextOutput() const
{return m_graphicsTextOutput;}   // pointeur de sortie


void neurone::setSoftmax(double out){m_softmax = out;}

double neurone::getSoftmax() const
{return m_softmax;}

void neurone::setOutput(double out){m_out = out;}

void neurone::modifieCouleurs(double &WMaxMaxNegatif,  double &WMaxMaxPositif){
    // couleur et poids des liaisons

#define PI 3.14159265
    int i(0);
    int portion;
    for (i=0; i < m_tabArrDyn.size(); i++){
        if (m_tabArrDyn[i].m_WLink >= 0){ // si poids > 0 -> rouge si proche de 0 -> vert
            portion = (int) fabs(255 * m_tabArrDyn[i].m_WLink / WMaxMaxPositif);
            if (fabs(portion) > 255) portion = 255;
            m_tabArrDyn[i].m_graphicsLine->setPen(QPen(QColor(portion, 100, 0)));
        } else {   // sinon si < 0  -> bleu
            portion = (int) fabs( 255 * m_tabArrDyn[i].m_WLink / WMaxMaxNegatif);
            if (fabs(portion) > 255) {portion = 255;}
            m_tabArrDyn[i].m_graphicsLine->setPen(QPen(QColor(0, 100, portion)));
        }
        if (m_tabArrDyn[i].m_graphicsPoids != nullptr) // si détail checked
            m_tabArrDyn[i].m_graphicsPoids->setText("W: " +
                QString::number(m_tabArrDyn[i].m_WLink));
    }
}

void neurone::modifieCouleurNeurone(QPen pen){
    if(m_graphicsEllips != nullptr)
        m_graphicsEllips->setPen(QPen(pen));
}

void neurone::modifieGraphDi(){
    if (m_graphicsTextDi != nullptr) // selon diamètre de l'ellipse
        m_graphicsTextDi->setText("Di: " + QString::number(m_Di));//(m_Di * 100) + "%");
}

void neurone::modifieGraphOutput(){
    if (m_graphicsTextOutput != nullptr) // si détail checked
        m_graphicsTextOutput->setText( "out: " + QString::number(m_out));
}
