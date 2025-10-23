// reseauDeNeurones.h   Bernard Ribordy
//du 2025_05_01
// *********************************************************

#ifndef RESEAUDENEURONES_H
#define RESEAUDENEURONES_H

#include <QWidget>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QColor>
#include <QTableView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDataStream>
#include <QFile>
#include<QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QBrush>
#include <QtGui>
#include <QVariant>
#include "lectureMNISTDyn.h"
#include "convolutionDyn.h"
#include "neurones.h"
#include "reseauDeNeuronesGraph.h"
#include "reseauDeNeuronesResultats.h"
#include "reseauDeNeurones_variablesConstantes.h"
#include <QGraphicsSimpleTextItem>
#include <QSize>
#include <QVector>
#include "reseauDeNeuronesTest1.h"
#include <QScrollArea>
#include <QScrollBar>
#include "param.h"

class reseauDeNeurones : public QWidget{
    Q_OBJECT

public:
    bool Maintest;

    reseauDeNeurones(QWidget *parent = nullptr );
    ~reseauDeNeurones();

    QString testTexte;
    QCheckBox* getPtAfficheDetails() const;
    void setnbLignesUtiles(quint16 nbLignes);
    quint16  getnbLignesUtiles() const;
    bool lectureFichierConvParam(QWidget *monParent, QString fileName);
    void EffectueUnExemple();
    quint16 getNberOfPixels() const;
    QString getm_Dir() const;
    //   void setMonTestReseau1(bool test1);

public slots:
    void tableConstruction();
    void APartirDunFichier();
    void AConstruire();
    void AdapteCouchesCachee(int nbCC);
    void constructionReseau();
    void Apprentissage();
    void AdaptPoidsInitiaux();
    void Apprentissage_unPas();
    //    void EffectueUnExemple();
    void conclusion();
    void lectureExemples();
 //   void testLeReseau();
    void nouveauReseau();
    void disableAll();

private:
    QString fichierTest_2; // pour test uniquement sur ordi hp
    QHBoxLayout     *HBexemplesFile;
    QLabel          *l_exemplesFile;
    QLineEdit       *exemplesFile;
    QRadioButton    *nouveauReseauButton;
    QRadioButton    *aPartirDunFichier;
    QRadioButton    *aConstruire;
    QLineEdit       *nbEntrees;
    QLineEdit        *nbSorties;
    QSpinBox        *nbCoucheCachees;
    QSpinBox        *nbNeuronesCoucheCachee1;
    QSpinBox        *nbNeuronesCoucheCachee2;
    QSpinBox        *nbNeuronesCoucheCachee3;
    QPushButton     *constructionTableButton;
    QPushButton     *constructionReseauButton;
    QPushButton     *quitter;
    QPushButton     *pasApas;
    QPushButton     *testReseau;
    QCheckBox       *afficheDetails;
    QPushButton     *apprentissage;
    QLineEdit       *tauxdApprentissage;
    QLineEdit       *differenceAcceptable;
    QLineEdit       *poidsInitiaux;
    QLineEdit       *nbBoucleMax;
    //   QGridLayout     *gridReseau;
    QTableWidget    *reseauDescr;
    //   QTableWidget    reseauDescrDyn;
    int m_nberOfFiltersPrec2;
    int m_featureDim2;
    QGroupBox *grpMethode;
    QGroupBox *grpArchitecture;
    QGroupBox *grpApprentissage;
    QGroupBox *grpExecution;
    QGroupBox *grpGraph;
    QVBoxLayout *layoutPrincipalDroite;
    quint16 NQ, NCMax, NCNBMax, NQPrec;  // nombre de couches
        // nombre de biais selon la couche
    QVector<QVector<neurone*>> tabBiaisDyn;// tableau de pointeurs vers les biais [Q][C]
    QVector<QVector<neurone*>> tabCellDyn;// tableau de pointeurs vers les neurones
    QVector<QVector<QTableWidgetItem*>> gridItemDyn;// cée un item pour chaque case
    QVector <QVector<double>> xexDyn;
    QVector <QVector<double>> targetDyn;
    QVector <double> m_flattenedDyn;
    QVector <double> targetMNIST;

    quint32 nex;        //nombre d'exemples
    quint16 nx;         //nombre d'entrées
    quint16 ns;         //nombre de sortie du réseau
    quint32 exemple;
    quint16 nBoucle;
    quint16 nBouclesTotal;
    quint16 gr100Ex;
    bool okPourTouteBoucles;
    bool okPourToutEx;
    bool okPourToutesSortie;
    quint16 nberOfOkParBoucle;
    double tauxDeReussiteParBoucle[nbEpochMax];
    double m_tauxDeReussiteParBoucleMax;
    bool premierPas;
    double tdApprentissage;
    double m_diffAcceptable;
    double m_difference;
    double m_poidsInitiaux;
    //   QCursor m_curseur;
    QString fichierTest;
    QString ReseauFile;
    QString resultats;
    QString m_fileExempleName;
    QString testText; // pas utilise
    bool ok;
    bool m_memoirApplique;
    bool reAppliqueApprentissage;
    bool reAppliqueConstruction;
    bool reShow;
    QVBoxLayout *layoutTable;
    quint16 nbLignesUtiles;
    QVBoxLayout  *layoutPrincipalGauche;
    QHBoxLayout *layoutPrincipal;
    reseauDeNeuronesGraph *reseauGraph;
    reseauDeNeuronesResultats *fenResultats;
    // reseauDeNeuronesTest *monTestReseau;
    reseauDeNeuronesTest1 *monTestReseau1;
    double m_WMaxMaxPositif;
    double m_WMaxMaxNegatif;
    QFile *m_fichier;
    //   QTextStream m_reseauTest;
    QFile * m_testNeurones;
    QString m_fichierEssais;
    QTextStream m_fluxTest;
    QTextStream fluxTest;
    QTextStream m_neuronesTest;

    lectureMNISTDyn * m_MNIST;
    convolutionDyn * m_convPrec;
    convolutionDyn * m_convolution[4];

    quint16 m_nberOfPixels;
    quint32 m_nberOfImages;
    bool m_MNISTFlag;
    bool m_test1Image;
    bool m_pasApas;
    //   quint16 * m_NCGrapf;
    quint8 m_BCD;
    int forPr;
    bool forPrFlag;
    double m_cible;
    double m_output;
    struct forPrintStruct {
        quint32 exemple;
        int s;
        double outVal;
        double cible;
    };
    forPrintStruct m_forPrint;
    QVector<QVector<forPrintStruct>> m_forPrintOut;
    QVector<QVector<double>> forPrintOut;
    quint16 m_forPrintSize;
    int m_nberOfConvolutions;

    quint16 NC[nbQMax], NB[nbQMax];// nombre de cellules(neurones) selon la couche
    quint16 NCPrec[nbQMax], NBPrec[nbQMax];// si reprise
    struct convParametre {
        bool featurePadding;  int stride; int nberOfFilters;
        int filterType;  bool maxPool; bool RELU;
        int stridePooling; bool poolingPair;
    };
    convParametre convParam[4];
    convParametre convParamForLaout[3];
    QString m_DirHP;
    QString m_DirUBUNTU;
    QString m_Dir;
    QString m_path;
    QScrollArea * graphArea;
//    Dialog *menu;
    param *m_param;
    QFileDialog exFileEx;
};
#endif // RESEAUDENEURONES_H
