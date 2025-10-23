// reseauDeNeurones_variablesConstantes Bernard Ribordy
//    du 2024_06_03
// ****************************************************************************

// convertion de BCD vers décimal (10 sorties 1..10), ex: 0101 -> sortie 5
// 100% de réussite avec 3 époques de 800 exemples tirés du fichier des labels de MNIST
// zero couche intermediaire
// ****************************************************************************

#ifndef RESEAUDENEURONES_VARIABLESCONSTANTES_H
#define RESEAUDENEURONES_VARIABLESCONSTANTES_H
#include "qglobalstatic.h"
#include <QString>
#define hp  //pour ordinateur hp

// Variables globales
// ******************

const int nbQMax(5);          // nombre max de couches
const quint32 nberOfExemplesConst = 900; // min 900,  max 60'000 ?
const int nbSMax(11);         // nombre de sorties max
const  int nbLiAdd(10);
const int nbColMax(5);        // nombre de colonnes max du tableau des liaisons
const int nbEpochMax(20);
const int nberOfImagesForTest(1);
const int nberOfFiltersForTest(2);
const int nombreDeSorties(10);
const int nberOfResultOutput(500);
/*                                  //  ****15/4/25
#ifdef hp
const QString Dir =  "C:/Qt/"; // Dir H/P
#else
const QString Dir = "/home/bernard/QtBernard/"; // Dir ACER UBUNTU
#endif
*/
const bool testReseau(false);

const QString testFile =  "testFile_2.txt";
const QString testConvFile =  "testConvolution2";
const QString BcdToOutputFile =  "BcdToOutput.txt";
const QString testNeurones =  "testNeurones.txt";
#endif // RESEAUDENEURONES_VARIABLESCONSTANTES_H
