//diversNeuroneSub.cpp   Bernard Ribordy
//    du 2024_06_03
// Plus utilisé actuellement (pour fichiers des exemples repris de visual basic)
// Adapte un fichier "exemples" contenant des virgules et des lignes de
//commentaires
// une ligne de commentaire ne doit pas commencer par un chiffre, un - ou
//un point.
// supprime les espaces inutiles et les "
// le fichier de sortie compatible cpp n'aura plus de commentaires et aura
//l'extention "_corr.txt",

#include "reseauDeNeurones_variablesConstantes.h"
#include "diversNeuronesSub.h"
#include "neurones.h"
#define notest

    void adapteFile(QWidget * monParent,QString fileToCor, QString fileCorr){
    QFile inputfile(fileToCor);
    QFile outputFile(fileCorr);
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream outFlux(&outputFile);
        if (inputfile.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream monFlux(&inputfile);
            if (!monFlux.atEnd()){
                QString ch;
                while (!monFlux.atEnd()) {
                    ch = monFlux.read(1);
                    while (ch == ' ' || ch == '\"' ) {
                        ch = monFlux.read(1);  // on laisse tomber les espaces
                        //supplémentaires  ainsi que les "
                    }
                    if ( (ch >= '0' && ch <= '9') || ch == '.' || ch == '-' || ch == '\n'){
                        outFlux << ch;
                        continue;
                    }
                    else if (ch == ','){
                        ch=' ';   //on remplace les virgules par un espace
                        outFlux << ch;
                    }
                    else {monFlux.readLine();}
                    // car c'est une ligne de commentaires
                }
            }
        } else
        {
            QMessageBox::information(monParent,  "ERREUR",
                                     "LECTURE fichier a corriger: \n" + fileToCor ) ;
        }
    } else
    {
        QMessageBox::information( monParent, "ERREUR",
                                 "ECRITURE fichier exemple corr: \n" + fileCorr ) ;
    }
}

// ******************Lecture du fichier des exemples *************************

bool lectureFichierExemplesDyn(QWidget *monParent, QString fileName, quint32 &nex,
   quint16 &nx, quint16 &ns, QVector<QVector<double>> &xexDyn,
                               QVector<QVector<double>> &targetDyn){
    QFile fichierExple(fileName);
    quint16 exemple(0);
    quint32 elementsLus = 0;
    QString entree;
 //   bool ok = true;
    if (fichierExple.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream monFluxEx(&fichierExple);
        if (!monFluxEx.atEnd()){
            monFluxEx >> entree;
            if(entree != "nbrExemples=")
                return false;

            monFluxEx >> nex;
            monFluxEx >> nx;
            monFluxEx >> ns;
        };
        for (quint32 ex=0; ex <nex; ex++){
            xexDyn.push_back(QVector <double>(nx));
            targetDyn.push_back(QVector<double>(ns));
        }
        while (exemple < nex && !monFluxEx.atEnd()){
            elementsLus=3;
            int k = nx-1; //quint16 k=0;
            while (k>=0 && !monFluxEx.atEnd()){   //(k=0;k< nx; k++)
                monFluxEx >> xexDyn[exemple][k];
                k--; elementsLus++;
            };
            k=ns-1;
            while (k>=0 && !monFluxEx.atEnd()){   //( k = 0; k < ns; k++)
                monFluxEx >> targetDyn[exemple][k];
                k--; elementsLus++;
            };
            exemple++;
        };
        fichierExple.close();
 #ifdef test

        //       Pour test >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        QMessageBox::information( monParent, " fichier : " +
			fileName,"nbExemples= " +
			 QString::number(nex) + ", nbEntées= " +QString::number(nx) +
			 ", nbSorties " +QString::number(ns)) ;
        QMessageBox::information( monParent, " fichier : " + fileName,
		 "Nbre d'éléments à lire(nbExemples*(nbEntéesx+nbSorties)+3): " +
			 QString::number(nex*(nx+ns)+3) + " \n Eléments lus: " +
			 QString::number(elementsLus)) ;

 #endif
        return true;

    } else {
        QMessageBox::warning( monParent, "ERREUR",
            "lecture fichier d'exemples: \n" + fileName) ;
            return false;
    }
}


// *********************** FindNQNCdeQ ************************


// Calcule le nombre de couches (Q) et le nombre de cellules (C) par couche
void FindNQNCdeQ(QTableWidget  *reseauDescr,quint16 &NQ, quint16 NC[],
                 quint16 NB[], quint16 &NCMax, quint16 &NCNBMax){
    quint16 q,c;
    bool ok;
    NQ=0;
    for (quint16 i = 0; i < reseauDescr->rowCount();i++){
        if (reseauDescr->item(i,0)->text().sliced(1,1).toInt(&ok) == NQ){
            NQ++;
        };
    };
    for (quint16 i = 0; i < NQ; i++){
        NC[i]=0;
        NB[i]=0;
    }
    NCMax=0;
    for (quint16 i=0; i < reseauDescr->rowCount(); i++){
        q= reseauDescr->item(i,0)->text().mid(1,1).toInt(&ok);
        if (reseauDescr->item(i,0)->text().mid(2,1)=="B"){
            NB[q]++;
        };
        if (reseauDescr->item(i,0)->text().mid(2,1)=="C"){
            c = reseauDescr->item(i,0)->text().mid(3,4).toInt(&ok);
            if (c>NC[q]){
                NC[q]= c;}; // C0000 = 1
        }
      //  if (NC[q]>NCMax) {NCMax=NC[q];};// nombre max de neurones
      //  if (NC[q] + NB[q] > NCNBMax) // nombre max de neurones + biais
      //  {NCNBMax=NC[q] + NB[q];};
    }
    for(q=0; q<NQ; q++){
        NC[q]+=1;
        if (NC[q]>NCMax)
            NCMax=NC[q];// nombre max de neurones
        if (NC[q] + NB[q] > NCNBMax) // nombre max de neurones + biais
            NCNBMax=NC[q] + NB[q];
    };

}

// *************************** networkConstructDyn **************************

// construction des cellules
void networkConstructorDyn(QVector<QVector<neurone*>> &tabBiaisDyn,
    QVector<QVector<neurone*>> &tabCellDyn, quint16 NQ, quint16 NC[], quint16 NB[],
        bool &Maintest){
    QString testTexte = "";
    char typeDeCell;
    QString strNumber = "";
    for (quint16 q = 0;q < NQ; q++){
 //       tabBiaisDyn[q][0] = nullptr;
        for (quint16 b = 0;b < NB[q];b++){ // B1...B[NB[q]]
            // nouveau  biais pointée par cell
            // copie le pointeur dans le tableau
            strNumber = QString::number(b);
            if(strNumber.size() == 1)strNumber="000" + strNumber;
            else if(strNumber.size() == 2)strNumber="00" + strNumber;
            else if(strNumber.size() == 3)strNumber="0" + strNumber;
            QString nom = "Q"+QString::number(q)+"B"+strNumber;
            tabBiaisDyn[q][b]= new neurone(nom,'B');
        }
 //      tabCellDyn[q][0] = nullptr;
        for (quint16 c = 0;c < NC[q];c++){ //  C1... C[NC[q]
            if (q==0)  {typeDeCell ='X';}  //  'X' = neurone d'entrée
            else if (q == (NQ -1)) {typeDeCell='Y';} //'Y'=neurones de sortie,
            else  {typeDeCell='C';}              //   'C' = autres celules

            // nouveau neurone pointé par un élément du tableau
            strNumber = QString::number(c);
            if(strNumber.size() == 1)strNumber="000" + strNumber;
            else if(strNumber.size() == 2)strNumber="00" + strNumber; // ***07/05
            else if(strNumber.size() == 3)strNumber="0" + strNumber; // ***07/05
            tabCellDyn[q][c]= new neurone("Q"+ QString::number(q)+"C"+strNumber,
                typeDeCell);
        }
    }
}

/*
  ******************** cellLinkConstructor *******************************

 construction des liaisons normales entre neurones pour calcul des outputs
 on se sert des index Arr (issue de datagrid1 trié sur la
 colonne des neurones d'arrivées).
 On remplit le tableau de pointeurs vers les neurones (*m_tabpArrDyn)

 Complète la table interne *m_tabpArrDyn des neurone en copiant les pointeurs
 des neurones de couche inférieure qui aboutissent au neurone en cours
 en vue du calcul de la somme des X * W -> Output

*/

void cellLinkConstructorDyn(QTableWidget *reseauDescr,
    QVector<QVector<neurone*>> &tabCellDyn, QVector<QVector<neurone*>> &tabBiaisDyn){

    // la table est trillée selon la colonne d'arrivée (ascending)
    reseauDescr->sortItems(3,Qt::AscendingOrder);
    QString Arr,  Dep,  cleArr,  cleDep;
    quint16 cleQArr, cleCArr, cleQDep, cleCDep, QDep, CDep, QArr, CArr;
    bool calculOut = true;
    bool calculDi = false;
    bool ok;
    quint16 li = 0;
    QString testTexte = "";

    while (li < reseauDescr->rowCount()){
        // lis les clés
        if (reseauDescr->item(li,3)->text().at(0)=='Y'){li++;continue;};
        // on laisse tomber les lignes fictives

        cleQArr = reseauDescr->item(li,3)->text().mid(1,1).toInt(&ok);
        cleCArr = reseauDescr->item(li,3)->text().mid(3,4).toInt(&ok);
        cleArr = reseauDescr->item(li,3)->text();
        do {
            QDep = reseauDescr->item(li,0)->text().mid(1,1).toInt(&ok);
            CDep = reseauDescr->item(li,0)->text().mid(3,4).toInt(&ok);
            Dep = reseauDescr->item(li,0)->text();
 //           if(cleQArr == 1 and cleCArr == 10)
 //               qDebug() << "cleQArr " << cleQArr << "  cleCArr "<< cleCArr;
            // copie le pointeur de QArrCArr dans la table du neurone QDepCDep
            // et mets le poids issue de la table
            if (reseauDescr->item(li,0)->text().mid(2,1)=="C"){
                // transfert du poids W dans les liaisons arrivants dans le neurone
                tabCellDyn[cleQArr][cleCArr]->addPointerAndSetW(tabCellDyn[QDep][CDep],
                    reseauDescr->item(li,2)->text().toDouble(), calculOut, testTexte);
            }
            else if (reseauDescr->item(li,0)->text().mid(2,1)=="B"){
                // transfert du poids W dans les liaisons arrivants dans le biais
                tabCellDyn[cleQArr][cleCArr]->addPointerAndSetW(tabBiaisDyn[QDep][CDep],
                    reseauDescr->item(li,2)->text().toDouble(), calculOut, testTexte);
            }
            li++;
            if (li == reseauDescr->rowCount())
                break;
        } while (reseauDescr->item(li,3)->text() == cleArr); //.toInt(&ok) == cleQArr);
            // rupture de clé
    }       // tant qu'il y a des lignes dans le tableau
    /*
    Complète la table interne *m_tabpDepDyn des neurone en copiant les pointeurs
    des neurones de couche supérieure qui aboutissent au neurone en cours
    en vue du calcul des gradiants Di permettant la modification des poids W
    */

    // la table est trillée selon la colonne de départ par ordre décroissant
    reseauDescr->sortItems(0,Qt::DescendingOrder);

    li = 0;
    const double dummy = 0.0;
    while (li< reseauDescr->rowCount()){
        // lis les cles
        if ( reseauDescr->item(li,0)->text().mid(2,1)=="B" ||
            reseauDescr->item(li,3)->text().at(0)=='Y')
        {li++;continue;};   // on laisse tombé les biais et les ligne fictives
        //car il n'y a pas de calcul de gradient Di
        // sur les biais

        cleQDep = reseauDescr->item(li,0)->text().mid(1,1).toInt(&ok);
        cleCDep = reseauDescr->item(li,0)->text().mid(3,4).toInt(&ok);
        cleDep = reseauDescr->item(li,0)->text();
        do{
            QArr = reseauDescr->item(li,3)->text().mid(1,1).toInt(&ok);
            CArr = reseauDescr->item(li,3)->text().mid(3,4).toInt(&ok);
            Arr = reseauDescr->item(li,3)->text();

            // copie le pointeur de QArrCArr dans la table du neurone QDepCDep
            if (reseauDescr->item(li,0)->text().mid(2,1)=="C"){
                tabCellDyn[cleQDep][cleCDep]->addPointerAndSetW(tabCellDyn[QArr][CArr],
                    dummy,calculDi, testTexte);
            }
            li++;
            if (li == reseauDescr->rowCount()) break;
        } while (reseauDescr->item(li,0)->text() == cleDep);
    }

}

void networkDestructorDyn(QVector<QVector<neurone*>> &tabBiaisDyn,
    QVector<QVector<neurone*>> &tabCellDyn, quint16 NQ, quint16 NC[], quint16 NB[]){
    for (quint16 q = 0; q < NQ; q++){
        for (quint16 b = 0;b < NB[q]; b++){  // supprime les B1...B[NB[q]]
            if (tabBiaisDyn[q][b] != nullptr){ // si le pointeur a déjà été utilisé
                delete tabBiaisDyn[q][b]; tabBiaisDyn[q][b] = nullptr;
            }
        }
        for (quint16 c = 0;c < NC[q]; c++){   // supprime les C1... C[NC[q]
            if (tabCellDyn[q][c] != nullptr){
                delete tabCellDyn[q][c];
                tabCellDyn[q][c] = nullptr;
            }
        }
    }
    tabBiaisDyn.resize(0,QVector<neurone*>(0,nullptr));
    tabCellDyn.resize(0,QVector<neurone*>(0,nullptr));
}

