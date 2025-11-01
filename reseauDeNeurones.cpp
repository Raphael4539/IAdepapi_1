 /*
 IAdePapiCpp - reseauDeNeurones.cpp Bernard Ribordy
 du 2025_05_20

*/

/*
  le perceptron multicouche est lui aussi capable d'apprentissage.
  En effet, il existe également un algorithme permettant de corriger les poids vis à vis
  d'un ensemble d'exemples donnés.
 Cet algorithme est appelé algorithme de rétro-propagation du gradient.

Répéter
    Prendre un exemple (vecteur_x,vecteur_y) et calculer g(vecteur_x)

    Pour toute cellule de sortie i   di <- si(1-si)(yi-si) finPour
    Pour chaque couche de q-1 à 1
            Pour chaque cellule i de la couche courante
                di = oi(1-oi) * Somme [pour k appartenant aux indices des neurones
                    prenant en entrée la sortie du neurone i] de dk*w_ki
            finPour
    finPour

    Pour tout poids w_ij <- w_ij + epsilon*di*x_ij finPour
finRépéter

 */

#include "reseauDeNeurones_variablesConstantes.h"
#include "reseauDeNeurones.h"
#include <QSize>
#include "diversNeuronesSub.h"
#include <QWidget>
//#include "reseauDeNeuronesTest1.h"

#define nomaintest // ******* DIRECTIVES DE COMPILATION
#define notest
#define notestNeurones
#define hp


reseauDeNeurones::reseauDeNeurones(QWidget *parent ): QWidget( parent)
{
    parent->resize(1500,600);
    parent->show();
    m_nberOfImages = 0; //22.4
    m_MNISTFlag = false;
    m_test1Image = false;
    resultats = "";
    NQ=0; NCMax=0; NQPrec = 0;    // nombre de couches ***
    NCNBMax =0;
    m_nberOfConvolutions = 0;

    m_Dir = "";
    nbLignesUtiles=0;
    nex=0;  // nombre d'exemples en cours
    nx=0;   // nombre d'entrées  en cours
    ns=0;   // nombre de sortie du réseau en cours
    reAppliqueConstruction = false;
    reAppliqueApprentissage = false;

    exemple = 0;
    nBoucle = 0;  // = époques
    nBouclesTotal = 0;
    okPourToutEx = false;
    tdApprentissage = 0.2;
    m_diffAcceptable = 0.0;
    m_difference = 0.0;
    premierPas = true;
    NC[nbQMax-1]={0}; // nombre de cellules(neurones) selon la couche
    NB[nbQMax-1]={0};  // nombre de biais selon la couche
    NCPrec[nbQMax-1]={0}; // nombre de cellules(neurones) selon la couche ***
    NBPrec[nbQMax-1]={0};  // nombre de biais selon la couche    ***
    m_WMaxMaxNegatif=0.0;
    m_WMaxMaxPositif=0.0;
    nouveauReseauButton=nullptr;
    aPartirDunFichier=nullptr;
    aConstruire=nullptr;
    nbEntrees=nullptr;
    nbSorties=nullptr;
    nbCoucheCachees=nullptr;
    nbNeuronesCoucheCachee1=nullptr;
    nbNeuronesCoucheCachee2=nullptr;
    nbNeuronesCoucheCachee3=nullptr;
    constructionReseauButton=nullptr;
    quitter=nullptr;
    apprentissage=nullptr;
    ReseauFile ="";
    reseauGraph = nullptr;
    fenResultats = nullptr;
    m_MNIST = nullptr;
    monTestReseau1 = nullptr;
    testText = "";
    m_pasApas = false;
    m_testNeurones = nullptr;
    m_fileExempleName = "";
    m_path = "";
    m_fichier = nullptr;
    m_convPrec = nullptr;
    for(auto i = 0; i<4; i++)convParam[i]={false,0,0,0,false,false,0,false} ;
    for(auto i = 0; i<4; i++)m_convolution[i]=nullptr ;
    for(auto i = 0; i<nbEpochMax; i++)tauxDeReussiteParBoucle[i]=0.0 ;

    layoutPrincipalGauche = nullptr;
    layoutPrincipal = nullptr;
    pasApas = nullptr;
    testReseau = nullptr;
    afficheDetails = nullptr;
    tauxdApprentissage = nullptr;
    differenceAcceptable = nullptr;
    poidsInitiaux = nullptr;
    nbBoucleMax = nullptr;
    reseauDescr = nullptr;
        //  *** 16/4/25
    grpMethode = nullptr;
    grpExecution = nullptr;
    grpGraph = nullptr;
    tabBiaisDyn.resize(0,QVector<neurone*>(0,nullptr));
    tabCellDyn.resize(0,QVector<neurone*>(0,nullptr));
    gridItemDyn.resize(0,QVector<QTableWidgetItem*>(0,nullptr));
    xexDyn.resize(0,QVector<double>(0,0.0));
    targetDyn.resize(0,QVector<double>(0,0.0));
    m_flattenedDyn.resize(0);
    targetMNIST.resize(0);
    m_forPrintOut.resize(0,QVector<forPrintStruct>(0));

 //   m_param = new param("C:/Qt/", "C:/Qt/MNIST_7x3x3.txt");
 //   m_param->exec();

#ifdef testNeurones
    // voir  RESEAUDENEURONES_VARIABLESCONSTANTES_H
    m_testNeurones = new QFile(m_path + "testNeurones"+ " " + //  m_path****15/4/25
        QDate::currentDate().toString() +".txt");
    m_testNeurones->open(QIODevice::WriteOnly);
    m_testNeurones->close();
#endif
    // groupe méthode de construction
    grpMethode = new QGroupBox( "Méthode de construction") ;

    QHBoxLayout *HBexemplesFile = new QHBoxLayout;
    l_exemplesFile = new QLabel("database choisi: ");
    exemplesFile    = new QLineEdit();

    HBexemplesFile->addWidget(l_exemplesFile);
    HBexemplesFile->addWidget(exemplesFile);
 //   grpMethode->setLayout(HBexemplesFile);

    aPartirDunFichier = new QRadioButton("A partir d'un fichier \n"
        "(fichier d'entrée_netok.txt ou fichier d'entrée_net.txt)" );// ******4/25
    nouveauReseauButton = new QRadioButton("Nouveau réseau\n");

    QVBoxLayout *methodeConstruction  = new  QVBoxLayout;
    methodeConstruction->addLayout(HBexemplesFile);
    methodeConstruction->addWidget(aPartirDunFichier);
    methodeConstruction->addWidget(nouveauReseauButton);

    grpMethode->setLayout(methodeConstruction);

    // Groupe réseau standard
    QGroupBox *grpArchitecture   = new QGroupBox("Architecture"); // *** 22/5/25

    nbEntrees = new  QLineEdit();
    nbEntrees->setReadOnly(true);
    //   nbEntrees->setRange(1,nberOfFilters_2 * 6 * 6);
    nbSorties = new  QLineEdit;
    nbSorties->setReadOnly(true);
    nbCoucheCachees = new  QSpinBox();
    nbCoucheCachees->setValue(0);  // pour test
    nbCoucheCachees->setMinimum(-3);
    nbCoucheCachees->setMaximum(3);
    nbNeuronesCoucheCachee1  = new  QSpinBox();
    nbNeuronesCoucheCachee1->setValue(0);
    nbNeuronesCoucheCachee1->hide();
    nbNeuronesCoucheCachee2  = new  QSpinBox();
    nbNeuronesCoucheCachee2->setValue(0);
    nbNeuronesCoucheCachee2->hide();

    nbNeuronesCoucheCachee3  = new  QSpinBox();
    nbNeuronesCoucheCachee3->setValue(0);
    nbNeuronesCoucheCachee3->hide();
    poidsInitiaux           = new QLineEdit;
    poidsInitiaux->setText("rd");           // *****14/4/25
    poidsInitiaux->setModified(false);
    constructionTableButton = new QPushButton("1: Construisons la table (full connected)\n"
        "(déterminer d'abord les couches cachées si nécessaire)");
    constructionReseauButton = new  QPushButton("2: Construisons le réseau");
    constructionReseauButton->setFocus();

    QFormLayout  *descriptionReseau  = new  QFormLayout;
    descriptionReseau->addRow("Nbre d'entrées",nbEntrees);
    descriptionReseau->addRow("Nbre de sorties",nbSorties);
    descriptionReseau->addRow("Nbre de couches cachées,\n"
                              "si négatif ponts suppl. Q0->Q2, Q1->Q3",nbCoucheCachees);
    descriptionReseau->addRow("Nbre de neurones de la couche cachée 1",
                              nbNeuronesCoucheCachee1);
    descriptionReseau->addRow("Nbre de neurones de la couche cachée 2"
                              ,nbNeuronesCoucheCachee2);
    descriptionReseau->addRow("Nbre de neurones de la couche cachée 3",
                              nbNeuronesCoucheCachee3);
    descriptionReseau->addRow("Poids initiaux (rd=random)",poidsInitiaux);
    descriptionReseau->addWidget(constructionTableButton);
    descriptionReseau->addWidget(constructionReseauButton);

    grpArchitecture->setLayout(descriptionReseau);       //*** 16/5/25

    // Groupe apprentissage
    QGroupBox *grpApprentissage = new  QGroupBox("grpApprentissage");
    tauxdApprentissage      = new QLineEdit;
    tauxdApprentissage->setText("0.5");
    differenceAcceptable    = new QLineEdit;
    differenceAcceptable->setText("0.20");
     nbBoucleMax             = new QLineEdit;
    nbBoucleMax->setText("1");
    afficheDetails          =new QCheckBox;

     QFormLayout  *layoutApprentissage = new  QFormLayout;
    layoutApprentissage->addRow("Taux d'apprentissage",tauxdApprentissage);
    layoutApprentissage->addRow("différence acceptable",differenceAcceptable);
    layoutApprentissage->addRow("Nombre de boucles(epoch)",nbBoucleMax);
    layoutApprentissage->addRow("Afficher les détails",afficheDetails);

    grpApprentissage->setLayout(layoutApprentissage);

    QGroupBox *grpExecution = new QGroupBox("grpExecution");
    pasApas                 = new QPushButton("Pas à pas");
    apprentissage = new  QPushButton("3: Apprentissage");
 //   apprentissage->hide();

    QHBoxLayout *boutonApprent = new  QHBoxLayout;
    boutonApprent->addWidget(apprentissage);
    boutonApprent->addWidget(pasApas);
    quitter   = new  QPushButton("Quitter");
    grpExecution->setLayout(boutonApprent);

    layoutPrincipalGauche = new QVBoxLayout;;

    layoutPrincipalGauche->addWidget(grpMethode);
    layoutPrincipalGauche->addWidget(grpArchitecture);
    layoutPrincipalGauche->addWidget(grpApprentissage);
    layoutPrincipalGauche->addWidget(grpExecution);
    layoutPrincipalGauche->addWidget(quitter);
    layoutPrincipalGauche->setAlignment(Qt::AlignLeft);

 //   layoutPrincipalGauche->addSpacing(1);
    layoutTable = new  QVBoxLayout;

    reseauDescr = new QTableWidget(1,nbColMax);

    QStringList entetes;
    entetes << "Neurone\nde Départ" << "  X  " << "  Poids   " <<
        "Neurone\nd'arrivée"  << "index";

    reseauDescr->setHorizontalHeaderLabels(entetes);
    QLabel *enteteLabel = new  QLabel;
    enteteLabel->setFont( QFont( "Comic Sans MS", 10)) ;

    reseauDescr->resizeColumnsToContents();
    reseauDescr->resizeRowsToContents();
    layoutTable->addWidget(reseauDescr);

    layoutPrincipal = new  QHBoxLayout;
    layoutPrincipal->addLayout(layoutPrincipalGauche,1);
    layoutPrincipal->addLayout(layoutTable,2);
    parent->setLayout(layoutPrincipal);

    nbCoucheCachees->setReadOnly(true);
    constructionTableButton->setDisabled(true);
    constructionReseauButton->setDisabled(true);
    apprentissage->setDisabled(true);
    pasApas->setDisabled(true);
    poidsInitiaux->setReadOnly(true);

    lectureExemples();      // lis le fichier des exemples

 //   QObject::connect( standard, SIGNAL(clicked(bool)) , this,
 //                    SLOT(Standard())) ;
    QObject::connect( constructionTableButton, SIGNAL(clicked()) , this,
                     SLOT(tableConstruction())) ;

    QObject::connect( aPartirDunFichier, SIGNAL(clicked(bool)) , this,
                     SLOT(APartirDunFichier())) ;

    QObject::connect( nouveauReseauButton, SIGNAL(clicked(bool)) , this,
                     SLOT(nouveauReseau())) ;

    // QObject::connect( aConstruire, SIGNAL(clicked(bool)) , this, //abandonné****14/4/25
    //                  SLOT(AConstruire())) ; ****** abandonn

    QObject::connect( nbCoucheCachees, SIGNAL(valueChanged(int)) , this,
                     SLOT(AdapteCouchesCachee(int))) ;

    QObject::connect( constructionReseauButton, SIGNAL(clicked()) , this,
                     SLOT(constructionReseau())) ;

    QObject::connect( apprentissage, SIGNAL(clicked()) , this,
                     SLOT(Apprentissage())) ;

    QObject::connect( poidsInitiaux, SIGNAL(editingFinished()), this,
                     SLOT(AdaptPoidsInitiaux())) ;

    QObject::connect( pasApas, SIGNAL(clicked()) , this,
                     SLOT(Apprentissage_unPas())) ;

    QObject::connect( quitter, SIGNAL(clicked()) , qApp,
                     SLOT(quit())) ;

    QObject::connect( nbNeuronesCoucheCachee1, SIGNAL(valueChanged(int)) , this,
                     SLOT(disableAll())) ;
    QObject::connect( nbNeuronesCoucheCachee2, SIGNAL(valueChanged(int)) , this,
                     SLOT(disableAll())) ;
    QObject::connect( nbNeuronesCoucheCachee3, SIGNAL(valueChanged(int)) , this,
                     SLOT(disableAll())) ;

}

void reseauDeNeurones::disableAll(){
    constructionReseauButton->setDisabled(true);
    apprentissage->setDisabled(true);
    pasApas->setDisabled(true);
}

void reseauDeNeurones::lectureExemples(){
    // ******************Lecture du fichier des exemples ********************

 //   bool createFile = false;
    QMessageBox msgBox;         // *** 20/5/25
    msgBox.setText("Choisissez le fichier des exemples (dataset)"
            " par ex.: BcdToOutput.txt, XORxx.txt ou MNISTxx.txt, etc., "
            "(il ne doit pas contenir _net.txt ou _netok.txt)"
            " Si le dataset contient MNISTxx.txt,"
            "il s'agit du fichier des paramètres de convolution");
    msgBox.exec();

    m_fileExempleName = exFileEx.getOpenFileName(this,"Ouvrir le fichier",
        "/home/","Texte Files (*.txt)");
    if(m_fileExempleName == ""){    // on a appuie sur Cancel
        qDebug() << m_fileExempleName;
        ok = false;
        exit(0);
    }
    int i = m_fileExempleName.size();
    while (m_fileExempleName.mid(i,1) != "/" or i <= 2) {
        i--;
    }
    m_path = m_fileExempleName.left(i + 1);
    qDebug() <<  "m_path = " << m_path;
    exemplesFile->setText(m_fileExempleName); // 30/6/25

    //    QMessageBox::information( this, "Fichier",
    //              "Vous avez sélectionné: \n" + m_fileExempleName) ;
    //  m_MNIST = new lectureMNISTDyn(this,m_nberOfImages); // lecture de 2images+labels

    if (m_fileExempleName.toUpper().contains("MNIST")){
        m_MNISTFlag = true;
        m_param = new param(m_path,m_fileExempleName, ok);
        m_param->exec();
        if(!ok) return;  // 22/6/25
		m_fileExempleName = m_param->getNewFileName(); // ****1/11/25
        exemplesFile->setText(m_fileExempleName); // ****1/11/25

        if( !lectureFichierConvParam(this, m_fileExempleName)){
            QMessageBox::warning(this, "Erreur fchier des paramètres",
                "des paramètres ne correspondent pas");
            return;
        }
        else {
            m_MNIST = new lectureMNISTDyn(this, m_nberOfImages, m_path);
            m_MNIST->divisePixelsPar255(m_nberOfImages, m_nberOfPixels);
            // pseudo convolution qui recevra l'image
            convolutionDyn * convPrec;

            for(auto convNber= 0; convNber < m_nberOfConvolutions ; convNber++){
                if(convNber == 0){
                convPrec = nullptr;
                }
                else convPrec = m_convolution[convNber -1];
                m_convolution[convNber]= new convolutionDyn(m_MNIST, convPrec,
                    m_nberOfPixels, convNber,
                    convParam[convNber].featurePadding, convParam[convNber].stride,
                    convParam[convNber].stridePooling, convParam[convNber].nberOfFilters,
                    convParam[convNber].filterType, convParam[convNber].RELU,
                    convParam[convNber].maxPool, convParam[convNber].poolingPair,
                    m_nberOfConvolutions, m_fileExempleName);
            }
            nx = m_convolution[m_nberOfConvolutions-1]->getFeatureDim();
            nx = nx * nx * m_convolution[m_nberOfConvolutions-1]->getNberOfFilters();
            m_nberOfFiltersPrec2 = m_convolution[m_nberOfConvolutions-1]->getNberOfFilters();
            m_featureDim2 = m_convolution[m_nberOfConvolutions-1]->getFeatureDim();
            ns = nombreDeSorties;
            targetMNIST.resize(ns);
            nex = m_nberOfImages;
            m_flattenedDyn.resize(nx,0.0);
            for (quint32 ex=0; ex <nex; ex++){
                xexDyn.push_back(QVector <double>(nx));
            }
        }
    } else {
                // 21/6/25
        bool createFile = false;
        QMessageBox msgBox2;         // *** 20/5/25

        msgBox2.setInformativeText("Voulez-vous recréer le fihier?");
        msgBox2.setStandardButtons( QMessageBox::Yes | QMessageBox::No);
        msgBox2.setDefaultButton(QMessageBox::No);
         int ret = msgBox2.exec();
        QString nbExemples = "";
        switch (ret) {  // *** 20/5/25
        case QMessageBox::Yes:{
            createFile = true;
            nbExemples = QInputDialog::getText(this, "", "Combien d'exemples ?",
                QLineEdit::Normal, nbExemples, &ok);
            break;
        }
        case QMessageBox::No:{ break;}
        }

        if(nbExemples != "") m_nberOfImages = nbExemples.toUInt();
        m_MNIST = new lectureMNISTDyn(this,m_nberOfImages, m_path); // lecture des images+labels
        if(createFile == true){  // ici m_nberOfImages = nbExemples
            if(m_fileExempleName == m_path + "BcdToOutput.txt")
                m_MNIST->createExemplesFileForBcdToOutput(m_nberOfImages);
            else if(m_fileExempleName == m_path + "XORNew.txt")
                m_MNIST->createExemplesFileForXORFunction(m_nberOfImages);
            else if(m_fileExempleName == m_path + "ORNew.txt")
                m_MNIST->createExemplesFileForORFunction(m_nberOfImages);
            else if(m_fileExempleName == m_path + "decimalToBCD.txt")
                m_MNIST->createExemplesFileForDecimalToBCDFunction(m_nberOfImages);
            else { QMessageBox dataBase;
                dataBase.setText( "database inexistante !!");
                exit(0);
            }
        }
    }
    if ( ! m_MNISTFlag ){
        QFile fichierExple(m_fileExempleName);
        if (fichierExple.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream monFlux(&fichierExple);
            quint16 i=0;
            if (!monFlux.atEnd()){
                QString manyCar;
                do {
                    i++;
                    manyCar = monFlux.read(1);
                } while (i<50 && manyCar != ",");
                fichierExple.close();
                if (i<48){
                    // le fichier est issu de Visual Basic avec virgule séparateur
                    QString const ExfileCorr(m_fileExempleName.left(
                         m_fileExempleName.size()-4) + "_corr.txt");
                    adapteFile(this, m_fileExempleName,  ExfileCorr);
                    //on supprime le virgules
                    m_fileExempleName = ExfileCorr;
                }
                if(!lectureFichierExemplesDyn(this, m_fileExempleName,nex,nx, ns, xexDyn,
				   targetDyn)){
                    QMessageBox::critical( this, "Fichier",
					"Le fichier selectionné ne correspond pas /n"
                         "au fichier des exemples: " + m_fileExempleName);
                    return;
                }
            }
        }
    }
    nbEntrees->setText(QString::number(nx)); //value(nx);
    nbSorties->setText(QString::number(ns)) ;
    m_nberOfImages = nex;
    if (m_nberOfImages <= nberOfResultOutput) {m_forPrintSize = m_nberOfImages;}
    else {m_forPrintSize = nberOfResultOutput;}
    // nberOfResultOutput: voir reseauDeNeurones_variablesConstantes.h
    m_forPrintOut.resize(m_forPrintSize,QVector<forPrintStruct>(ns));
}

void reseauDeNeurones::setnbLignesUtiles(quint16 nbLignes)
{nbLignesUtiles = nbLignes;}

quint16  reseauDeNeurones::getnbLignesUtiles() const {return nbLignesUtiles ;}

QCheckBox* reseauDeNeurones::getPtAfficheDetails() const
{return afficheDetails;}

quint16 reseauDeNeurones::getNberOfPixels() const {return m_nberOfPixels;}

QString reseauDeNeurones::getm_Dir() const {return m_Dir;}


// ************* nouveauReseau() ************************ 23/5/25

void reseauDeNeurones::nouveauReseau(){         // *** 23/5/25
    nbCoucheCachees->setReadOnly(false);
    constructionTableButton->setDisabled(false);
 //   constructionReseauButton->setCheckable();
    apprentissage->setDisabled(true);
    pasApas->setDisabled(true);
    poidsInitiaux->setReadOnly(false);
}

//   ************* tableConstruction *******************

void reseauDeNeurones::tableConstruction(){
  //  if(reAppliqueConstruction){ // ***
        apprentissage->setDisabled(true);
        pasApas->setDisabled(true);
        NQPrec = NQ;
        reseauDescr->setVisible(true);
        networkDestructorDyn(tabBiaisDyn, tabCellDyn, NQ, NC, NB);
        for (auto q = 0; q < nbQMax; q++) {NB[q] = 0; NC[q] = 0;}
        if (reseauGraph != nullptr){
            layoutPrincipal->removeWidget(reseauGraph); // ***
            reseauGraph->setVisible(false);  // ***
            delete reseauGraph;
            reseauGraph = nullptr;
        }
 //   }
    bool ok;

    // Construction de la grille décrivant le  des liaisons entre entrées,
    // neurones, sorties. Des liaisons pourront etre ajoutées (ré-appliquez !)

    quint16 q, c, i;
    quint16 li;
    NQ= abs(nbCoucheCachees->value())+2; // +couche d'entrée + couche de sortie
    NC[0]=0;NC[1]=0;NC[2]=0;NC[3]=0;NC[4]=0;
    QString strDep, strArr;
    NC[0]= nx;  //nbEntrees->value();
    switch (abs(nbCoucheCachees->value()))
    {
    case 0:
        NC[1]=ns;
        break;
    case 1:
        NC[1]=nbNeuronesCoucheCachee1->value();
        NC[2]=ns;
        break;
    case 2:
        NC[1]=nbNeuronesCoucheCachee1->value();
        NC[2]=nbNeuronesCoucheCachee2->value();
        NC[3]=ns;
        break;
    case 3:
        NC[1]=nbNeuronesCoucheCachee1->value();
        NC[2]=nbNeuronesCoucheCachee2->value();
        NC[3]=nbNeuronesCoucheCachee3->value();
        NC[4]=ns;
        break;
    default:
        break;
    }
    reseauDescr->clearContents(); // ***
    reseauDescr->setRowCount(1);
        // ***
    qDebug() << "rowCount " << reseauDescr->rowCount();
    li=0;
    QString strNumber = "";

    for (q=0;q<NQ;q++){
        for  (c=0;c < NC[q+1];c++){
            for (i=0;i<NC[q];i++){
                // construit une cellule normale
                strNumber = QString::number(i);
                if(strNumber.size() == 1)strNumber="000" + strNumber;
                else if(strNumber.size() == 2)strNumber="00" + strNumber; // ***07/05
                else if(strNumber.size() == 3)strNumber="0" + strNumber; // ***07/05
                strDep= "Q"+QString::number(q)+ "C" + strNumber;
                strNumber = QString::number(c);
                if(strNumber.size() == 1)strNumber="000" + strNumber;
                else if(strNumber.size() == 2)strNumber="00" + strNumber; // ***07/05
                else if(strNumber.size() == 3)strNumber="0" + strNumber; // ***07/05

                strArr= "Q"+QString::number(q+1)+ "C" + strNumber;

                gridItemDyn.push_back(QVector<QTableWidgetItem*>(5,nullptr));
                gridItemDyn[li][0]= new QTableWidgetItem;
                gridItemDyn[li][0]->setText(strDep);
                reseauDescr->setItem(li,0,gridItemDyn[li][0]);

                gridItemDyn[li][3]= new QTableWidgetItem;
                gridItemDyn[li][3]->setText(strArr);
                reseauDescr->setItem(li,3,gridItemDyn[li][3]);

                gridItemDyn[li][2]= new QTableWidgetItem;
                gridItemDyn[li][2]->setData(0,poidsInitiaux->text().toDouble(&ok));
                reseauDescr->setItem(li,2,gridItemDyn[li][2]);

                li++;
                reseauDescr->insertRow(li);
            }
            // et maintenant construit un biais
            strNumber = QString::number(c);
            if(strNumber.size() == 1)strNumber="000" + strNumber;
            else if(strNumber.size() == 2)strNumber="00" + strNumber;
            else if(strNumber.size() == 1)strNumber="0" + strNumber;
            strDep= "Q"+QString::number(q)+ "B" + strNumber;
            strArr= "Q"+QString::number(q+1)+ "C" + strNumber;

            gridItemDyn.push_back(QVector<QTableWidgetItem*>(5,nullptr));

            gridItemDyn[li][0]= new QTableWidgetItem;
            gridItemDyn[li][0]->setText(strDep);
            reseauDescr->setItem(li,0,gridItemDyn[li][0]);

            gridItemDyn[li][3]= new QTableWidgetItem;
            gridItemDyn[li][3]->setText(strArr);
            reseauDescr->setItem(li,3,gridItemDyn[li][3]);

            gridItemDyn[li][2]= new QTableWidgetItem;
            gridItemDyn[li][2]->setData(0,poidsInitiaux->text().toDouble(&ok));
            reseauDescr->setItem(li,2,gridItemDyn[li][2]);

            gridItemDyn[li][1]= new QTableWidgetItem;
            gridItemDyn[li][1]->setData(0,1.0);
            reseauDescr->setItem(li,1,gridItemDyn[li][1]);

            li++;
            reseauDescr->insertRow(li);

        }
    };

    //      Ajoute les liaisons fictives des sorties

    for (i=0;i<ns;i++){
        strNumber = QString::number(i);
        if(strNumber.size() == 1)strNumber="000" + strNumber;
        else if(strNumber.size() == 2)strNumber="00" + strNumber;
        else if(strNumber.size() == 1)strNumber="0" + strNumber;

        strDep= "Q"+QString::number(NQ-1)+ "C" + strNumber ;//QString::number(i);
        strArr= "Y"  +  strNumber; //QString::number(i);
        gridItemDyn.push_back(QVector<QTableWidgetItem*>(5,nullptr));

        gridItemDyn[li][0]= new QTableWidgetItem;
        gridItemDyn[li][0]->setText(strDep);
        reseauDescr->setItem(li,0,gridItemDyn[li][0]);

        gridItemDyn[li][3]= new QTableWidgetItem;
        gridItemDyn[li][3]->setText(strArr);
        reseauDescr->setItem(li,3,gridItemDyn[li][3]);
        ++li;
        reseauDescr->insertRow(li);
    }

    //   qDebug() << "nbLignesUtiles " << nbLignesUtiles << "\n";

    if(nbCoucheCachees->value() < 0){ // si négatif,
        for (q=2;q<NQ;q++){ // ajoute des liaisons par dessus: couche Q0 vers Q2
            for  (c=0;c<NC[q];c++){ // eventuelement Q1 vers Q3
                for (i=0;i<NC[q-2];i++){
                    strNumber = QString::number(i);
                    if(strNumber.size() == 1)strNumber="000" + strNumber;
                    else if(strNumber.size() == 2)strNumber="00" + strNumber;
                    else if(strNumber.size() == 3)strNumber="0" + strNumber;
                    strDep= "Q"+QString::number(q-2)+ "C" + strNumber;

                    strNumber = QString::number(c);
                    if(strNumber.size() == 1)strNumber="000" + strNumber;
                    else if(strNumber.size() == 2)strNumber="00" + strNumber;
                    else if(strNumber.size() == 3)strNumber="0" + strNumber;
                    strArr= "Q"+QString::number(q)+ "C" + strNumber;

                    gridItemDyn.push_back(QVector<QTableWidgetItem*>(5,nullptr));
                    gridItemDyn[li][0]= new QTableWidgetItem;
                    gridItemDyn[li][0]->setText(strDep);
                    reseauDescr->setItem(li,0,gridItemDyn[li][0]);

                    gridItemDyn[li][3]= new QTableWidgetItem;
                    gridItemDyn[li][3]->setText(strArr);
                    reseauDescr->setItem(li,3,gridItemDyn[li][3]);

                    gridItemDyn[li][2]= new QTableWidgetItem;
                    gridItemDyn[li][2]->setData(0,poidsInitiaux->text().toDouble(&ok));
                    reseauDescr->setItem(li,2,gridItemDyn[li][2]);

                    li++;
                    reseauDescr->insertRow(li);
                }
            }
        }
    }
    reseauDescr->removeRow(li); //** 16/03/25
    nbLignesUtiles = li;
    AdaptPoidsInitiaux();
    constructionReseauButton->setEnabled(true);

}   // fin *************** tableConstruction ***********



//   ******************************** APartirDunFichier() *******************

void reseauDeNeurones::APartirDunFichier(){
    constructionTableButton->setCheckable(false);
    reseauDescr->clearContents();                                         // ***
    QString saveFile = m_fileExempleName + "_net ou " + m_fileExempleName + "_netok";
    QMessageBox::information( this, "A partir du fichier",
                             "Choisissez le réseau de neurones sauvé \n" +
                                 saveFile) ;
 //   QString choix1 = m_fileExempleName.left(m_fileExempleName.size()-4) + "_net";
 //   QString choix2 = m_fileExempleName.left(m_fileExempleName.size()-4) + "_netok";

    QString ReseauFile = QFileDialog::getOpenFileName(this, // *****16/4/25
        "Ouvrir le fichier " + m_fileExempleName + "_net ou _netok",
        m_fileExempleName.left(m_fileExempleName.size()-4),"text File(*_net.txt *_netok.txt)");

    QMessageBox::information( this, "Fichier", "Vous avez sélectionné: \n" + ReseauFile);

    if(m_fileExempleName.left(m_fileExempleName.size()-4) != ReseauFile.left(m_fileExempleName.size()-4)){
        QMessageBox::warning( this, "Fichier", "Le fichier selectionné ne correspond pas /n"
                                              "au fichier des exemples: " + m_fileExempleName);
    }else {

        QFile ResFile(ReseauFile);

        if (ResFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream monFlux(&ResFile);

            // Importe la grille
            // si l'on veux prendre les poids de datagrid1 la constante overWriteW = DBL_MAX
            // (voir début de main() sinon les poids seront mis à la valeur de overWriteW)

            quint16 li(0),nbreDeColonne(5);
            QString   tempStr;
            QString ligne,TypedeParam;
            quint16 tempInt, dummyquint16;
            double  tempDouble;

            reseauDescr->removeRow(1);

            monFlux >> TypedeParam;
            if (TypedeParam == "NombreDeLignes="){
                monFlux >> nbLignesUtiles;
            } else {
                QMessageBox::information(this, "ERREUR lecture fichier ", "NombreDeLignes");
            }
            monFlux >> TypedeParam;
            if (TypedeParam == "indexDep=") {            // plus utilisé
                for (li=0;li<nbLignesUtiles;li++){
                    monFlux >> dummyquint16; //indexDep[li];
                }
            } else QMessageBox::information( this, "ERREUR lecture fichier ", "indexDep");

            monFlux >> TypedeParam;
            if (TypedeParam == "indexArr=") {        // plus utilisé
                for (li=0;li<nbLignesUtiles;li++){
                    monFlux >> dummyquint16;  //indexArr[li];
                }
            } else QMessageBox::information( this, "ERREUR lecture fichier ", "indexArr");

            monFlux >> TypedeParam;

            if (TypedeParam == "NQ-NC(Q)-NB(Q)=") {      // plus utilisé
                monFlux >> NQ;
                for (li=0;li<NQ;li++) {
                    monFlux >> NC[li];
                }
                for (li=0;li<NQ;li++){
                    monFlux >> NB[li];
                }
            } else
                QMessageBox::information( this, "ERREUR lecture fichier ", "NQ,NC(Q),NB(Q)=");

            for (quint16 col=0;col<nbreDeColonne;col++){
                monFlux >> ligne;
            }
            for (li=0;li<nbLignesUtiles;li++) {
                gridItemDyn.push_back(QVector<QTableWidgetItem*>(5,nullptr));

                monFlux >> tempStr;      // neurone de départ
                gridItemDyn[li][0]= new QTableWidgetItem;
                gridItemDyn[li][0]->setText(tempStr);
                reseauDescr->setItem(li,0,gridItemDyn[li][0]);

                monFlux >> tempDouble;
                gridItemDyn[li][1]= new QTableWidgetItem;   // X
                gridItemDyn[li][1]->setData(0,tempDouble);
                reseauDescr->setItem(li,1,gridItemDyn[li][1]);

                monFlux >> tempDouble;
                gridItemDyn[li][2]= new QTableWidgetItem;   // W
                gridItemDyn[li][2]->setData(0,tempDouble);
                reseauDescr->setItem(li,2,gridItemDyn[li][2]);

                monFlux >> tempStr;                 // neurone d'arrivée
                gridItemDyn[li][3]= new QTableWidgetItem;
                gridItemDyn[li][3]->setText(tempStr);        //Dep
                reseauDescr->setItem(li,3,gridItemDyn[li][3]);

                monFlux >> tempInt;                // index
                gridItemDyn[li][4]= new QTableWidgetItem;
                gridItemDyn[li][4]->setData(0,tempInt);
                reseauDescr->setItem(li,4,gridItemDyn[li][4]);

                reseauDescr->insertRow(li+1);
            }
            reseauDescr->removeRow(li+1); //** 16/03/25
            reseauDescr->removeRow(li); //** 16/03/25
            ResFile.close();                                                    //***
        } else {
        QMessageBox::information( this, "ERREUR  ", "ERREUR lecture fichier réseau " );
        }
    }
    constructionReseauButton->setEnabled(true);
}



//   ***************************** AConstruire() ********* Abandonné *****************

void reseauDeNeurones::AConstruire(){
    //    if (nbLignesUtiles < nbLiMax){
    //       reseauDescr->setRowCount(nbLignesUtiles +nbLiAdd);
    quint16 li = 0;
    reseauDescr->removeRow(1);

    // complète la table en ajoutant des items
    QString strDep= "zzzzz", strArr= "zzzzz"; double X = 0.0; //126              "
    double poids = poidsInitiaux->text().toDouble(&ok);
    for (li = nbLignesUtiles  ; li < nbLignesUtiles +nbLiAdd; ++li ){

        gridItemDyn.push_back(QVector<QTableWidgetItem*>(5,nullptr));

        gridItemDyn[li][0]= new QTableWidgetItem;
        gridItemDyn[li][0]->setText(strDep);
        reseauDescr->setItem(li,0,gridItemDyn[li][0]);

        gridItemDyn[li][3]= new QTableWidgetItem;
        gridItemDyn[li][3]->setText(strArr);
        reseauDescr->setItem(li,3,gridItemDyn[li][3]);

        gridItemDyn[li][2]= new QTableWidgetItem;
        gridItemDyn[li][2]->setData(0,poids);
        reseauDescr->setItem(li,2,gridItemDyn[li][2]);

        gridItemDyn[li][1]= new QTableWidgetItem;
        gridItemDyn[li][1]->setData(0,X);
        reseauDescr->setItem(li,1,gridItemDyn[li][1]);

        gridItemDyn[li][4]= new QTableWidgetItem;
        gridItemDyn[li][4]->setData(0,X);
        reseauDescr->setItem(li,4,gridItemDyn[li][4]);

        reseauDescr->insertRow(li+1);
    }
    nbLignesUtiles = li;
    reseauDescr->removeRow(li+1); //** 16/03/25
    reseauDescr->removeRow(li); //** 16/03/25
}


void reseauDeNeurones::AdapteCouchesCachee(int nbCC){
    disableAll();
    nbNeuronesCoucheCachee1->setVisible(false);
    nbNeuronesCoucheCachee1->setValue(0);
    nbNeuronesCoucheCachee2->setVisible(false);
    nbNeuronesCoucheCachee2->setValue(0);
    nbNeuronesCoucheCachee3->setVisible(false);
    nbNeuronesCoucheCachee3->setValue(0);

    if (abs(nbCC) == 1){ // si nbCC negatifs on crée des branches suppl.
        nbNeuronesCoucheCachee1->setVisible(true);
    }else if (abs(nbCC) == 2){
        nbNeuronesCoucheCachee1->setVisible(true);
        nbNeuronesCoucheCachee2->setVisible(true);
    }else if (abs(nbCC) == 3){
        nbNeuronesCoucheCachee1->setVisible(true);
        nbNeuronesCoucheCachee2->setVisible(true);
        nbNeuronesCoucheCachee3->setVisible(true);
    }
}


void reseauDeNeurones::AdaptPoidsInitiaux(){
    if (nouveauReseauButton->isChecked() || aPartirDunFichier->isChecked()
 //       || aConstruire->isChecked()
        ){
        poidsInitiaux->setModified(true);
        if(poidsInitiaux->text().toUpper() != "RD"){
            m_poidsInitiaux = poidsInitiaux->text().toDouble(&ok);
            for (quint16 li = 0;li < reseauDescr->rowCount();li++){
                if (reseauDescr->item(li,3)->text().left(1) != "Y")
                    reseauDescr->item(li,2)->setData(0,m_poidsInitiaux);
            }
        }else{ std::default_random_engine  re;// distribution aléatoire
            std::uniform_int_distribution<int> distrib{-30,30};
            for (quint16 li = 0;li < reseauDescr->rowCount();li++){
                if (reseauDescr->item(li,3)->text().left(1) != "Y"){
                    m_poidsInitiaux = ((double)distrib(re)/10.0);
                    reseauDescr->item(li,2)->setData(0,m_poidsInitiaux);
                }
            }
        }
    } else QMessageBox::warning(this, "ERREUR", "choisissez d'abord: nouveau réseau ! ");
}


//   ***************************** constructionReseau() **************************

void reseauDeNeurones::constructionReseau(){
 //   layoutTable->removeWidget(reseauDescr);    // *** 10/5/25
    reseauDescr->setVisible(false);                // *** 10/5/25
    if (nouveauReseauButton->isChecked() || aPartirDunFichier->isChecked()){
        //  ||  aConstruire->isChecked())           // *** 10/5/25
        reseauDescr->sortItems(0,Qt::AscendingOrder);
        /*
        if (aConstruire->isChecked()){ // abandonné ******14/4/25
            nbLignesUtiles = 0;
            for (quint16 li =0; li < reseauDescr->rowCount();++li){
                if (reseauDescr->item(li,0)->text().size() != 0){
                    if (reseauDescr->item(li,0)->text().left(1) =="Q" ||
                        reseauDescr->item(li,0)->text().left(1) =="q" ){
                        nbLignesUtiles++;
                    }
                    else {  // p.ex. si des lignes ne commancent pas par 'C' ou 'c'
                        reseauDescr->item(li,0)->setText("uuuuu");
                        reseauDescr->item(li,3)->setText("zzzzz");
                    }
                } else { // size = 0
                    reseauDescr->item(li,0)->setText("uuuuu");
                    reseauDescr->item(li,3)->setText("zzzzz");
                }
            }
            reseauDescr->sortItems(0,Qt::AscendingOrder);  //  6/6/19
        }   // fin de aConstruire->isChecked()
     */
        reseauDescr->setRowCount(nbLignesUtiles);

        for (quint16 li=0; li < nbLignesUtiles; li++){
            // c ou q deviennent C ou Q
            reseauDescr->item(li,0)->setText(reseauDescr->item(li,0)->text().toUpper());
            if (reseauDescr->item(li,0)->text().length()==4)    // p. ex.C1 devient C01
                reseauDescr->item(li,0)->
                    setText(reseauDescr->item(li,0)->text().insert(3,"0"));

            reseauDescr->item(li,3)->setText(reseauDescr->item(li,3)->text().toUpper());
            if (reseauDescr->item(li,3)->text().length()==4)      // C1 devient C01
                reseauDescr->item(li,3)->
                    setText(reseauDescr->item(li,3)->text().insert(3,"0"));
        };
        for (quint16 li=0;li<reseauDescr->rowCount();++li){
            gridItemDyn[li][4]= new QTableWidgetItem;
            gridItemDyn[li][4]->setData(0,li);
            reseauDescr->setCurrentItem(gridItemDyn[li][4]);
        };
        reseauDescr->sortItems(0,Qt::AscendingOrder);            // ******4/25

        if (reAppliqueConstruction){ // ***
            /*
             si l'on a déjà construit une fois
             Suppression des neurones et m.a.z des pointeurs
             ceci permet une modification du réseau et de recommancer la construction
             sans sortir du programme
            //   layoutPrincipal->removeWidget(reseau); à vérifier

 //           networkDestructorDyn(tabBiaisDyn, tabCellDyn, NQPrec, NCPrec, NBPrec);
            */
        }

        tabBiaisDyn.resize(0,QVector<neurone*>(0,nullptr));
        tabCellDyn.resize(0,QVector<neurone*>(0,nullptr));


        // calul le nombre de couche (Q), nombre de neurones par couche NC(Q)
        FindNQNCdeQ(reseauDescr, NQ, NC, NB, NCMax, NCNBMax);

        for (quint16 q = 0;q < NQ;q++)
            tabBiaisDyn.push_back(QVector<neurone*>(NB[q],nullptr));

        for (quint16 q = 0;q < NQ;q++)
            tabCellDyn.push_back(QVector<neurone*>(NC[q],nullptr));

        //construction des neurones et remplissage du tabl. de point. vers les neurones
        networkConstructorDyn(tabBiaisDyn, tabCellDyn, NQ, NC, NB, Maintest);



        // ***************** cellLinkConstructor ****************

        // remplissage des tables de pointeurs des neurones aboutissant de
        //la droite (Q-1) et de la gauche (Q+1), ceci pour chaque neurone.
        // voir les fonctions dans diversNeuronesSub
        cellLinkConstructorDyn(reseauDescr, tabCellDyn, tabBiaisDyn); // indexArr, indexDep,


        // *********  dessine le réseau ****************************

            if (grpGraph != nullptr){
                layoutPrincipal->removeWidget(grpGraph); // ***
                delete grpGraph;
                grpGraph = nullptr;
                apprentissage->setEnabled(false); // ***
                pasApas->setEnabled(false);
            }
        tdApprentissage = tauxdApprentissage->text().toDouble(&ok);
        m_diffAcceptable = differenceAcceptable->text().toDouble(&ok);
        m_WMaxMaxNegatif = 0; // pour la couleur des liaisons
        m_WMaxMaxPositif = 0;
        grpGraph = new QGroupBox("groupe graph");
        QVBoxLayout *layoutGraph = new QVBoxLayout;
        reseauGraph = new reseauDeNeuronesGraph(this);
        reseauGraph->ConstructeurDyn( NQ,NC,NB, NCNBMax, tabBiaisDyn,tabCellDyn,
                                     afficheDetails);
        layoutGraph->addWidget(reseauGraph,5,Qt::AlignRight);
        grpGraph->setLayout(layoutGraph);
        layoutPrincipalDroite = new QVBoxLayout;

        layoutPrincipalDroite->addWidget(grpGraph);
        layoutPrincipalDroite->setContentsMargins(8,8,8,8);
        layoutPrincipal->addWidget(grpGraph);   // montre le Graphique
        reseauGraph->show();
        reAppliqueConstruction = true;   // on a déjà construit une fois
        apprentissage->setVisible(true);
        apprentissage->setEnabled(true);
        apprentissage->setFocus();
 //       pasApas->setVisible(true);
        pasApas->setEnabled(true);
    }
}       // fin appliquer

void reseauDeNeurones::Apprentissage(){
    using namespace std;
    m_forPrintOut.resize(m_forPrintSize,QVector<forPrintStruct>(ns));

    m_pasApas = false; afficheDetails->setChecked(false);
    //22.4
/*
    if (!reAppliqueApprentissage){
//        QMessageBox::information(this, "ERREUR", "Appuyer sur Appliquer d'abord");
    } else {

        // ** recherche les neurones de la couche superieure (aval)
        // qui pointe sur les neurone de la couche amont,
        // pour calcul des di (Somme Wki*di(k)). Table m_indexWDyn associée aux neurones
        // le reseau n'etant pas nécessairement "full connected".
        for (quint16 q=NQ-2;q>0;q--){ // pour couches intermediaires seulement
            // la couche d'entré Q0 est assimilée aux entrées du réseau  ***** modif NQ-2
            // donc n'a pas de poids
            for (quint16 c=0;c<NC[q];c++) // ??
                tabCellDyn[q][c]->FindAllIndex();
        }

 */

        //  ****************** Démarage de l'apprentissage *************

    afficheDetails->setChecked(false);
    okPourTouteBoucles = true;
    nBoucle = 0; // = epoques
    do {      // on répète jusqu'à ce que nBoucle == nBoucleMax choisi
        // à chaque boucle on présente tous les exemples
        nBoucle++;
        //               if(!okPourToutEx) okPourTouteBoucles = false;
        okPourTouteBoucles = !okPourToutEx;
        okPourToutEx = true;
        nberOfOkParBoucle = 0;
        forPr = 0;
        for (exemple =0; exemple < nex;exemple++)EffectueUnExemple();
        tauxDeReussiteParBoucle[nBoucle] = (double)nberOfOkParBoucle/nex;
     // if(nBoucle >1 and (tauxDeReussiteParBoucle[nBoucle] <
     //     tauxDeReussiteParBoucle[nBoucle-1]))
     //        conclusion(); // on a atteint un maximum

            nberOfOkParBoucle =0;
    } while (nBoucle < nbBoucleMax->text().toInt(&ok) && !okPourToutEx );
    m_tauxDeReussiteParBoucleMax = 0;
    nBouclesTotal += nBoucle;
    for (quint16 b = 1; b <= nBoucle; b++){
        if(tauxDeReussiteParBoucle[b]*100 > m_tauxDeReussiteParBoucleMax)
            m_tauxDeReussiteParBoucleMax = tauxDeReussiteParBoucle[b]*100;
    }

        conclusion();                   // termine
        reAppliqueApprentissage = true; //      *** 10/5/25
}

void reseauDeNeurones::Apprentissage_unPas() {
    m_pasApas = true;
    forPr = 0;                  // *** 12/5/25
 //   if(monTestReseau1 != nullptr)    // abandonné pour l'instant
 //       m_test1Image = monTestReseau1->getTest1Image();
 /*                 *** 10/5/25
    if (!reAppliqueApprentissage and ! m_test1Image){
        QMessageBox::information(this, "ERREUR", "Appuyer sur Appliquer d'abord" );
    }
    else {
  */
    //  afficheDetails->setChecked(true);
    if (exemple == 0) { okPourToutEx = true;}
    EffectueUnExemple();
    exemple++;
    if (exemple >= nex){
        exemple = 0;
        nBoucle++;
        // if (okPourToutEx) {conclusion();}  // *******peut être introduit
    }
//    }
}

void reseauDeNeurones::EffectueUnExemple(){
    /*
    pour chaque exemple, on
    attribue des valeurs d'entrées aux neurones de la couche Q0 \
    (p.ex. Q0C01 identique à input 1, reçoit xex[exemple][1]) \
    Q0C02 identique à input 2, reçoit xex[exemple][2]
    16/4/24
 */
    if (m_MNISTFlag){
        //        LoadImage( m_convolution0, m_MNIST, exemple);
        for (int row=0; row < 32; row++){
            for (int column=0;column < 32; column++){
                if(!m_test1Image){                      // image de MNIST 6/9/24
                    m_convolution[0]->setFeature(0,row,column, m_MNIST->
                        getPixelDivise(exemple,row,column));
                } else if(monTestReseau1 != nullptr){      //abandonné pour l'instant
  //                  m_convolution[0]->setFeature(0,row,column,
  //                      monTestReseau1->get1ImageForTest(row,column));
                }
            }
        }
        //    effectue les convolutions
        for(auto conv = 1; conv < m_nberOfConvolutions;conv++)
            m_convolution[conv]->convDyn(exemple, nBoucle);
    }

    if (m_MNISTFlag){   // mis à plat pour attribution aux entrées
        int fl=0;
        for  (int fnum = 0;fnum <m_nberOfFiltersPrec2;fnum++){
            for (int y=2; y<m_featureDim2+2; y++){
                // on avait ajoute 4* 2 zeroPadding au feature
                for (int x = 2; x<m_featureDim2+2; x++){
                    m_flattenedDyn[fl] = m_convolution[m_nberOfConvolutions-1]->
                                         getFeature(fnum,y,x);
                    fl++;
                }
            }
        }
    }
    double flatt =0;
    for (int input = 0; input < nx; input++){ // attribue les exemples aux entrées
        if (m_MNISTFlag ){
            flatt = m_flattenedDyn[input];
            tabCellDyn[0][input]->setX(flatt);
        } else tabCellDyn[0][input]->setX(xexDyn[exemple][input]);//Q0C1, Q0C2,.Q0C(nx)
    }

    //       fluxTest << "******************  CALCUL DES SORTIES ***********" << '\n';

    // pour chaque exemple,
    // attribution des valeurs de sorties aux cibles (target[exemple][s])
    // celles-ci seront comparées aux valeurs obtenues par calcul et
    // apparaissant à la sortie des neurones de la couche de sortie Q[N-1]
    // p.ex. la valeur de Q[N-1]C01->output sera comparée à la cible[exemple][0]
    //      Q0C02 identique à output2 sera comparée à la cible[exemple][1].etc

    // Si la différence est acceptable pour l'exemple 1, on aura ok[1]=true
    // Si la différence est acceptable pour tous les exemples, on aura okPourToutEx =true
    // sinon, on re-calcule les gradients Di issus des différences que l'on propage
    // aux neurones des couches inférieures, en modifiant au passage les poids associés.
    // voir articles sur les réseaux de neurones artificiels Feed Forward

    for (auto c=0; c < NC[0]; c++) // POUR la couche d'entrée, outPut = X (entrée)
        tabCellDyn[0][c]->setOutput(tabCellDyn[0][c]->getX());

    for (auto q=1; q<NQ; q++){ // pour les couches suivantes
        for(auto c = 0; c < NC[q]; c++){//calcule l'entrée E des neurones de la couche q
            tabCellDyn[q][c]->calculE(testTexte);
        }
        if (NC[q-1] > 3 or NC[q] > 5){ // pour les réseaux importants
            // la sigmoide pourrait saturer donc:
            //fonction d'activation  avec softmax (sorties normmalisées entre 0 et 1)
            double y[nbSMax] = {0.0};
            double somme = 0.0;
            double softmax = 0.0;
            double valMax = -100000.0;
            for (quint16 c=0; c<NC[q]; c++){
                y[c] = tabCellDyn[q][c]->getE();
                if(y[c]>valMax) valMax = y[c]; // valmax des outputs
            }
            for (quint16 c=0;c<NC[q];c++){
                y[c] = exp(tabCellDyn[q][c]->getE()-valMax);
                somme += y[c];
            }
            for (quint16 c=0;c<NC[q];c++){
                softmax = y[c] / somme;
                tabCellDyn[q][c]->setSoftmax(softmax); //normalisation
            }
            // on remplace les outputs calculés précédemment par softmax
            for (quint16 c=0;c<NC[q];c++){
                tabCellDyn[q][c]->setOutput(tabCellDyn[q][c]->getSoftmax());
            }        //normalisation 31/12
        }else{
            for(auto c = 0; c < NC[q]; c++) // petits réseaux
                tabCellDyn[q][c]->
                    setOutput(1/(1+exp(- tabCellDyn[q][c]->getE()))); // sigmoide
        }
    }

    if(m_pasApas and m_MNISTFlag){  //*** 21/4/25
        bool testpaApaOk = true; int sOk = 0;
        for( int s= 0; s<ns; s++){
            if(tabCellDyn[NQ-1][s]->getOutput() > 0.8){
                testpaApaOk = true;
                sOk = s;
            }
        }
        for(int ss = 0;ss < ns; ss++){
            if((tabCellDyn[NQ-1][ss]->getOutput() > 0.2) and (ss != sOk)){
                testpaApaOk = false;
                //                  break;
            }
        }

        QMessageBox msgBox;

        if(testpaApaOk){
            if(m_test1Image) msgBox.setText("Résultat = " + QString::number(sOk));
            else msgBox.setText("Résultat = " + QString::number(sOk)+
                ", Cible = " + QString::number(m_MNIST->getmonLabel(exemple)));
            msgBox.exec();
        }else{
            msgBox.setText(" Cible = " + QString::number(m_MNIST->getmonLabel(exemple)) +
                           " \n . Aucune sortie >0.8 ou < 0.2");
            msgBox.exec();
            //  monTestReseau1->setTexteResult(" not ok ");
            //  pasApas->setText(" NOT OK ");
        }
      /*
        if(monTestReseau1 != nullptr and m_test1Image){
            monTestReseau1->setTest1Image(false);
            monTestReseau1->raise();
            monTestReseau1->activateWindow(); // redonne l'acces à monTestReseau1
        }
     */
    } else {  // si pas de test1Image

        //  ******* comparaison avec les valeurs cibles (target) *************

        quint16 s=0;
        okPourToutesSortie = true;
        if(m_MNISTFlag){ // conversion du label de binaire en decimal
            for(int ss=0;ss<ns;ss++)targetMNIST[ss] = 0.0;
            m_BCD = m_MNIST->getmonLabel(exemple);
            targetMNIST[m_BCD]= 1;
        } // si not MNIST
        for (s=0; s<ns; s++){
            m_output = tabCellDyn[NQ-1][s]->getOutput();
            if (m_MNISTFlag){
                m_cible = targetMNIST[s];
            }else {
                m_cible = targetDyn[exemple][s];
            }
            m_difference = m_output - m_cible;

            if (fabs(m_difference) > m_diffAcceptable){
                okPourToutEx=false;
                okPourToutesSortie = false;
            }
            //  qDebug() << "BCD= " <<m_BCD<< " s= "<< s <<" getOutput[s]= " <<
            //  tabCellDyn[NQ-1][s+1]->getOutput()<< " targetMNIST[s]= " <<
            //  targetMNIST[s];// << "\n";

            // dans la conclusion, on imprime maximum 1000 derniers exemples
            if(nex <= m_forPrintSize){
                m_forPrintOut[forPr][s].exemple = exemple;
                m_forPrintOut[forPr][s].s = s;
                m_forPrintOut[forPr][s].cible = m_cible;
                m_forPrintOut[forPr][s].outVal = m_output;
                forPrFlag= true;
            } else if((exemple >(nex - nberOfResultOutput)) and (exemple < nex)){
                m_forPrintOut[forPr][s].exemple=exemple;
                m_forPrintOut[forPr][s].s = s;
                m_forPrintOut[forPr][s].cible = m_cible;
                m_forPrintOut[forPr][s].outVal = m_output;
                forPrFlag = true;
            }
        }
        if(forPrFlag) forPr++;
        forPrFlag = false;
        if (okPourToutesSortie){
            nberOfOkParBoucle++;
        }
        if (!okPourToutEx){

            //        *******  CALCUL DES rétropropagations *******

            // alternative 1: le calcul des Di en amont tiennent comptent des
            for (quint16 q=NQ-1;q>0;q--){ // des nouveaux poids en aval
                for (quint16 c=0;c<NC[q];c++){
                    if (m_MNISTFlag)
                        if(q == NQ-1)tabCellDyn[q][c]->calculDi(targetMNIST[c]);
                        else tabCellDyn[q][c]->calculDi(0.0);
                    else{
                        if(q == NQ-1)tabCellDyn[q][c]->calculDi(targetDyn[exemple][c]);
                            else tabCellDyn[q][c]->calculDi(0.0);
                    }
                    //                  /* 8.3.2025
                    tabCellDyn[q][c]->setW(tdApprentissage, testTexte);
                    // recherchons les poids extremes pour calculer la couleur
                    if (tabCellDyn[q][c]->getWMaxNegatif() < m_WMaxMaxNegatif){
                        m_WMaxMaxNegatif = tabCellDyn[q][c]->getWMaxNegatif();}
                    if (tabCellDyn[q][c]->getWMaxPositif() > m_WMaxMaxPositif)
                        m_WMaxMaxPositif = tabCellDyn[q][c]->getWMaxPositif();                    
                }
            }

            /*
             // alternative 2: le calcul des Di en amont ne tiennent comptent des
             // des nouveaux poids en aval
            for (quint16 q=NQ-1;q>0;q--){ // alternative
                for (quint16 c=0;c<NC[q];c++){
                    tabCellDyn[q][c]->setW(tdApprentissage, testTexte);
                    // recherchons les poids extremes pour calculer la couleur
                    if (tabCellDyn[q][c]->getWMaxNegatif() < m_WMaxMaxNegatif)
                    {m_WMaxMaxNegatif = tabCellDyn[q][c]->getWMaxNegatif();}
                    if (tabCellDyn[q][c]->getWMaxPositif() > m_WMaxMaxPositif)
                        m_WMaxMaxPositif = tabCellDyn[q][c]->getWMaxPositif();
                    //                       fluxTest << testTexte;
                }
            }
            */
        }
    }

#ifdef testNeurones // on sort des informations sur les neurones
    // and nBoucle == 5
    if ((exemple == 0 or exemple == 1) and nBoucle == 1 and not m_MNISTFlag and
        m_testNeurones->open(QIODevice::Append)){
        QTextStream m_neuronesTest(m_testNeurones) ;
        testTexte = "\n\n *******boucle: " + QString::number(nBoucle) + ", exemple: " +
                    QString::number(exemple) + " Taux d'apprentissage: " +
                    QString::number(tdApprentissage) + ", Poids initiaux: " +
                    QString::number(m_poidsInitiaux) + "\n";
        for (auto q=NQ-1;q>0;q--){
            for (auto c=0;c<NC[q];c++){
                tabCellDyn[q][c]->afficher(testTexte);
            }
        }
        m_neuronesTest << testTexte;
        m_testNeurones->close();
    }
#endif

    // ******* Modification de la couleur des liaison *******

    if (afficheDetails->isChecked()){
        for (quint16 q=1;q<NQ;q++){
            for (quint16 c=0;c<NC[q];c++){
                tabCellDyn[q][c]->modifieCouleurs(m_WMaxMaxNegatif, m_WMaxMaxPositif);
            }
        }
    }

    //******** Ecriture des outPut, gradients Di  et output dans le graphique
    if (afficheDetails->isChecked()){
        for (quint16 q=0; q<NQ; q++){
            for (quint16 b=0; b<NB[q]; b++){
                tabBiaisDyn[q][b]->modifieGraphOutput();
            }
            for (quint16 c=0; c<NC[q]; c++){
                if (q>0) tabCellDyn[q][c]->modifieGraphDi();
                tabCellDyn[q][c]->modifieGraphOutput();
            }
        }
    }

    //******** mettre en rouge le neurone cible (= 1)

    if (afficheDetails->isChecked()){
        QPen pen1(Qt::red,2,Qt::SolidLine);
        QPen pen0(Qt::blue,2,Qt::SolidLine);

        for(auto ss=0;ss<ns;ss++){
            if(m_MNISTFlag){
                if(targetMNIST[ss]==1)
                    tabCellDyn[NQ-1][ss]->modifieCouleurNeurone(pen1);
                else tabCellDyn[NQ-1][ss]->modifieCouleurNeurone(pen0);
            } else {
                if( targetDyn[exemple][ss]==1)
                    tabCellDyn[NQ-1][ss]->modifieCouleurNeurone(pen1);
                else tabCellDyn[NQ-1][ss]->modifieCouleurNeurone(pen0);
            }
        }
    }
    reAppliqueApprentissage = true;
}

// ************* CONCLUSION ******************************

void reseauDeNeurones::conclusion() {
    QString reseauFileOut;
    //   QString resultatsF = resultatsFile  ;
    QString resultatsF = m_fileExempleName.left(m_fileExempleName.size()-4)+"_tauxMax_de_réussite_" +
        QString::number( m_tauxDeReussiteParBoucleMax, 'f',0) + "%.txt";

    //voir "reseauDeNeurones_variablesConstantes.h"
    QFile fichier(resultatsF);
    if (fichier.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream fluxResult(&fichier);
        exemple = 0;  // permet de poursuivre avec le pas à pas
        premierPas = true;
        resultats = "";
        //           if (okPourToutEx){
        if(okPourToutEx){
            fluxResult << '\n' << " ******* APPRENTISSAGE REUSSI ******** \n \n";
            resultats= "\n   ******* APPRENTISSAGE REUSSI ******** \n \n";

            // pour sauvetage du réseau ( suffixe netok)
            reseauFileOut = m_fileExempleName.left(m_fileExempleName.size()-4)+
                            "_netok"+ ".txt";
        } else { // !okPourToutEx
            fluxResult << '\n' << " ******* APPRENTISSAGE NON TERMINE ******** \n\n";
            resultats = "\n  ******* APPRENTISSAGE NON TERMINE ******** \n\n ";
            reseauFileOut = m_fileExempleName.left(m_fileExempleName.size()-4)+
                            "_net" + ".txt";
            // pour sauvetage du réseau ( suffixe net)
        };
        fluxResult <<  " % de réussites par boucle (époque):  \n";
        resultats += " % de réussites par boucle (époque):  \n";
        for (quint16 b = 1;b <= nBoucle; b++){
            fluxResult << " boucle "  << b <<": " <<
                tauxDeReussiteParBoucle[b]*100 << "\n";
            resultats +=  " boucle " + QString::number(b) +
                ": " + QString::number(tauxDeReussiteParBoucle[b] * 100) + "\n";
        }
        fluxResult << " nombre de boucles total: " << nBouclesTotal << '\n';
        resultats += " nombre de boucles total: " +  QString::number(nBouclesTotal) + "\n";

        fluxResult << " Taux d'apprentissage: " << tauxdApprentissage->text() << '\n';
        resultats += " Taux d'apprentissage: " + tauxdApprentissage->text() + "\n";

        fluxResult << " nombre d'exemples (images): " << m_nberOfImages << '\n';
        resultats += " nombre d'exemples (images): " +
                     QString::number(m_nberOfImages) + "\n";

        fluxResult << " Fichier exemples = " << m_fileExempleName << '\n';
        resultats += " Fichier exemples = " + m_fileExempleName + "\n\n";

        fluxResult << " nombre de couches cachées = " << nbCoucheCachees->value() << '\n';
        resultats += "  nombre de couches cachées = " +
            QString::number(nbCoucheCachees->value()) + "\n\n";

        for(auto q=0; q < NQ; q++){
            fluxResult << " nombre de neurones de la couche " << q << " = " << NC[q] << '\n';
            resultats += " nombre de neurones de la couche "  + QString::number(q) + " = " +
                QString::number(NC[q]) + "\n\n";
        }
  /*
        fluxResult << " nbre de couches cachees = " << nbCoucheCachees->value() << '\n';
        resultats += "  nbre de couches cachees = " +
            QString::number(nbCoucheCachees->value()) + "\n\n";
        if(nbNeuronesCoucheCachee1->value() > 0){
            fluxResult << " nbre de neurones couches cachees 1 = "
                << QString::number(nbNeuronesCoucheCachee1->value()) << '\n';
            resultats += " nbre de neurones couches cachees 1 = " +
                QString::number(nbNeuronesCoucheCachee1->value()) + "\n";
        }

        if(nbNeuronesCoucheCachee2->value() > 0){
            fluxResult << " nbre de neurones couches cachees 2 = "
                       << QString::number(nbNeuronesCoucheCachee2->value()) << '\n';
            resultats += " nbre de neurones couches cachees 2 = " +
                         QString::number(nbNeuronesCoucheCachee2->value()) + "\n";
        }

        if(nbNeuronesCoucheCachee3->value() > 0){
            fluxResult << " nbre de neurones couches cachees 3 = "
                       << QString::number(nbNeuronesCoucheCachee3->value()) << '\n';
            resultats += " nbre de neurones couches cachees 3 = " +
                         QString::number(nbNeuronesCoucheCachee3->value()) + "\n";
        }
 */
        if(m_MNISTFlag){
            QFile fichierMNIST(m_fileExempleName);
            if(fichierMNIST.isOpen())fichierMNIST.close();
            if (fichierMNIST.open(QIODevice::ReadOnly | QIODevice::Text)){
                QTextStream monFluxMNIST(&fichierMNIST);
                QString entree ="";
                while (!monFluxMNIST.atEnd()){
                    entree = monFluxMNIST.readLine();                //>> entree;
                    fluxResult << entree << "\n";
                    resultats += entree + "\n";
                }
            }
        }
        fluxResult << "\n" << m_forPrintSize <<  " derniers résultats \n ";
        resultats += "\n" + QString::number(m_forPrintSize) + " derniers résultats \n";

        forPr = 0;
        quint16 exemplePrec = 10000; //imprime et sauve sur fichier
            //les résultats pour les 1000 derniers exemples que l'on a sauvés
        quint16 thisExemple;
        quint16 thisOutput;
        double thisOutVal;
        double thisCible;
        for(quint16 forPr=0;forPr<m_forPrintSize;forPr++){
            for(int s = 0; s < ns; s++){
                thisExemple = m_forPrintOut[forPr][s].exemple;
                thisOutput = m_forPrintOut[forPr][s].s;
                thisOutVal = m_forPrintOut[forPr][s].outVal;
                thisCible = m_forPrintOut[forPr][s].cible;
                if(thisExemple != exemplePrec){
                    fluxResult << '\n' << "exemple: "<< thisExemple <<  "\n";
                    resultats += "\n Exemple: " + QString::number(thisExemple) + "\n";
                }
                exemplePrec = thisExemple;
                fluxResult << "sortie: " << thisOutput << " output= " << thisOutVal <<
                    ", cible= " << thisCible << "  ";
                resultats += " sortie: " + QString::number(thisOutput) +
                             ", output= " + QString::number(thisOutVal) +
                             ", cible: " + QString::number(thisCible) + "  ";
                m_difference = thisCible - thisOutVal;
                fluxResult << "       cible - output: " << m_difference ;
                if (fabs(m_difference) > m_diffAcceptable) fluxResult << "  !!!!!!! \n";
                else fluxResult << " \n";
                resultats += "      cible - output: " + QString::number(m_difference) ;
                if (fabs(m_difference) > m_diffAcceptable) { resultats += "  !!!!!!! \n";}
                else {resultats += " \n";}
            }
        }
    }
    fichier.close();
        // sauve la table des liaisons reseauDescr pour l'option de reprise
        //    "a partir d'un fichier"
    QFile reseauFileToWrite(reseauFileOut);
    reseauFileToWrite.close();
    if (reseauFileToWrite.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream monFlux(&reseauFileToWrite);

        monFlux << "NombreDeLignes= ";
        monFlux << reseauDescr->rowCount() << '\n';
        monFlux << "indexDep= ";
        for (quint16 li=0;li<reseauDescr->rowCount();li++){monFlux << 0 << " ";}
        monFlux << '\n';         // compatibilité avec VB
        monFlux << "indexArr= ";
        for (quint16 li=0;li<reseauDescr->rowCount();li++){monFlux << 0 << " ";}
        monFlux << '\n';         // compatibilité avec VB
        monFlux << "NQ-NC(Q)-NB(Q)= " << NQ << " \n";
        for (quint16 q=0; q< NQ; q++) {monFlux << NC[q] << " ";}
        monFlux << '\n';
        for (quint16 q=0; q< NQ; q++) {monFlux << NB[q] << " ";}
        monFlux << '\n';
        monFlux << "DEP     X      W       ARR     Index \n";
        for (quint16 q = 1; q < NQ; q++){
            for (quint16 c = 0;c < NC[q]; c++){
                for (quint16 i =0; i < tabCellDyn[q][c]->gettabpArrSize();i++){
                    monFlux << tabCellDyn[q][c]->getpArr(i)->getNom() << " ";
                        // neurone de départ
                    monFlux << 0 << "  ";   //X
                    monFlux << tabCellDyn[q][c]->getW(i) << " ";//poids de la liaison;
                    monFlux << tabCellDyn[q][c]->getNom() << "    "; // neurone d'arrivée
                    monFlux << 0 << " \n" ; //index << 0 << " \n";
                }
            }
        }
        for (quint16 li = 0; li < reseauDescr->rowCount();li++){
            if (reseauDescr->item(li,3)->text().left(1)=="Y" ){
                // On rajoute les liaison fictives de sortie
                // telles qu'elles se trouvaient sur la grille
                monFlux << reseauDescr->item(li,0)->text() << " ";//neurone de départ
                monFlux << 0 << "  " << 0 << "         "; // entrée X, poids W
                monFlux << reseauDescr->item(li,3)->text() << "    ";//neurone d'arrivée
                monFlux << 0 << " \n" ;
            }
        }
    } else QMessageBox::information(this,  "ERREUR", "ECRITURE FICHIER RESEAU: \n"
                                                     + reseauFileOut) ;

    reseauFileToWrite.close();

    if (fenResultats != nullptr){
        delete fenResultats;
        fenResultats = nullptr;
    }
    fenResultats = new reseauDeNeuronesResultats(resultats , this);
    fenResultats->exec();
}
// lecture des paramètres de convolution qui sont associées au fichier MNIST...

bool reseauDeNeurones::lectureFichierConvParam(QWidget *parent, QString fileName){
    QFile fichierMNIST(fileName);
    bool ok = true;
    if (fichierMNIST.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream monFluxMNIST(&fichierMNIST);
        QString entree ="";
         int i=0;
        while (!monFluxMNIST.atEnd()){
            monFluxMNIST >> entree;
            if (entree == "nberOfImages=" && i==0){  // *** 20/5/25
                monFluxMNIST >> m_nberOfImages;
                ok = true;
            }
            if(!ok)break;
            monFluxMNIST >> entree;
            if(entree == "nberOfConvolutions="){
                monFluxMNIST >> m_nberOfConvolutions;
                m_nberOfConvolutions ++;
            }
            monFluxMNIST >> entree;
            if(entree == "nberOfPixels=") monFluxMNIST >> m_nberOfPixels;

            // pseudo convolution [0] sert à mettre l'image (l'exemple) dans la première "feature"
            //  pour la prochaine convolution ".
            //  Permet de faire une boucle pour l'appel des convolutions
            convParam[0].featurePadding = false;
            convParam[0].stride = 0;
            convParam[0].nberOfFilters = 1; // seul parametre utilise
            convParam[0].filterType = 0;
//            convParam[0].flatten = false; //pas utilise
            convParam[0].maxPool = false;
            convParam[0].RELU = false;
            convParam[0].stridePooling= 2;
            convParam[0].poolingPair = false;

            for(int nconv = 1;nconv < m_nberOfConvolutions;nconv++){ //24/06/25
                monFluxMNIST >> entree;
                if (entree == "featurePadding="){
                    monFluxMNIST >> entree;
                    if(entree == "true" ) convParam[nconv].featurePadding = true;
                    else convParam[nconv].featurePadding = false;
                }
                monFluxMNIST >> entree;
                if(entree == "stride=") monFluxMNIST >> convParam[nconv].stride;

                monFluxMNIST >> entree;
                if(entree == "nberOfFilters=")
                    monFluxMNIST >> convParam[nconv].nberOfFilters;

                monFluxMNIST >> entree;
                if(entree == "filterType=") monFluxMNIST >> convParam[nconv].filterType;

                monFluxMNIST >> entree;
                if(entree == "maxPool="){
                    monFluxMNIST >> entree;
                    if(entree == "true") convParam[nconv].maxPool = true;
                    else convParam[nconv].maxPool = false;
                }
                monFluxMNIST >> entree;
                if(entree == "RELU="){
                    monFluxMNIST >> entree;
                    if(entree == "true") convParam[nconv].RELU = true;
                    else convParam[nconv].RELU = false;
                }
                monFluxMNIST >> entree;
                if(entree == "stridePooling=") monFluxMNIST >>
                    convParam[nconv].stridePooling;

                monFluxMNIST >> entree;
                if(entree == "poolingPair="){
                    monFluxMNIST >> entree;
                    if(entree == "true" ) convParam[nconv].poolingPair = true;
                    else convParam[nconv].poolingPair = false;
                }
            }
            //     *****15/4/25 le path est tiré du path du fichier exemple ou param.
            i++;
        };
        if(!ok){fichierMNIST.close();return false;}
    }
    if(!ok){fichierMNIST.close();return false;} else {fichierMNIST.close();return true;}
}

/*
void reseauDeNeurones::testLeReseau(){ // **** abandonné pour l'instant
    if (monTestReseau1== nullptr) {
        monTestReseau1 = new reseauDeNeuronesTest1(this, m_nberOfPixels, m_path);
    }
    int posX = 20*m_nberOfPixels; int posY = m_nberOfPixels;
    int width = 18*m_nberOfPixels; int heigh = 18*m_nberOfPixels ;
    monTestReseau1->setGeometry(posX,posY,width,heigh);
    monTestReseau1->setModal(false);
    monTestReseau1->raise();
    monTestReseau1->activateWindow();
    monTestReseau1->show();
}
*/

reseauDeNeurones::~reseauDeNeurones(){}

