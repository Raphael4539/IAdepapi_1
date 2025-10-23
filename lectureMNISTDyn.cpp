// lecturemnist.cpp   Bernard Ribordy
//    du 2024_06_03

// lis le fichier MNIST, cree les filtres
#include <QtGui>
#include "lectureMNISTDyn.h"
#include <QMessageBox>
#include <QDataStream>
#include <QImage>
#include <QRgb>
#include <QLineF>
#define hp
#define notest

lectureMNISTDyn::lectureMNISTDyn(QWidget *w,quint32 nberOfImages, QString path){

    m_pixelDyn.resize(nberOfImages, QVector<QVector<int>>(32,
        QVector < int > (32,0)));

    m_pixelDiv255Dyn.resize(nberOfImages, QVector<QVector<double>>(32,
        QVector <double> (32,0.0)));
    labelDyn.resize(nberOfImages,0);
    quint32 image;
    quint8 row;
    quint8 column;
    m_canaux = 1;
    m_path = path;
    m_fichier = nullptr;
    // ? pas nécessaire si initialisation avec resize
    for (image = 0; image < nberOfImages; image++) {
        for  (row = 0;row < 32; row++) {
            for (column = 0; column < 32; column++) {
                m_pixelDyn[image][row][column] = 0;
                m_pixelDiv255Dyn[image][row][column] = 0;
            }
        }
    }
    QDir pathQt(m_path);
    pathQt.cdUp();                  // remonter /Qt/
    QString QtPath = pathQt.path();
    qDebug() << "QtPath " << QtPath;
    QFile fLabel(QtPath + "/train-labels-idx1-ubyte");

    if (fLabel.isOpen())
    {fLabel.close();};

    if (fLabel.open(QIODevice::ReadOnly)){
        QDataStream in(&fLabel);
        in >> magicNumber;
        in >> nberOfItems;
        for  (quint32 l=0;l<nberOfImages;l++){
            in >> labelDyn[l];
        };
    }
    else {
        QMessageBox::information(w,"ERREUR lecture fichier label",
            m_path + "train-labels-idx1-ubyte");
    }
    fLabel.close();


    QFile fImages( QtPath + "/train-images-idx3-ubyte");
    // voir RESEAUDENEURONES_VARIABLESCONSTANTES_H

    if (fImages.isOpen())  fImages.close();
    if (fImages.open(QIODevice::ReadOnly)){
        QDataStream inImage(&fImages);
        inImage >> magicNberImage;
        inImage >> nberOfRows;
        inImage >> nberOfColumns;

        // prevu pour 2 x 2 zeropadding à utiliser si nécessaire dans les convolutions
        for ( image = 0; image < nberOfImages; image++){
            for  (row = 2;row < 28 +2; row++) {
                for (column = 2; column < 28 + 2; column++){
                    quint8 im;
                    inImage >> im;
                    m_pixelDyn[image][row][column]= im;
                }
            }
        }
    }
    else {
        QMessageBox::information(w,"ERREUR lecture images","train-images-idx3-ubyte");
        //       qDebug( "erreur file images");
    }
    fImages.close();
}
// voir: https://machinelearningmastery.com/how-to-develop-a-convolutional-
// neural-network-from-scratch-for-mnist-handwritten-digit-classification/
// voir https://setosa.io/ev/image-kernels/

quint8 lectureMNISTDyn::getmonLabel(quint32 i) const{
    return labelDyn[i];			 // affichage du label 5
}

int lectureMNISTDyn::getPixel(quint32 image,quint8 row, quint8 column) const{
    return m_pixelDyn[image][row][column];
};

double lectureMNISTDyn::getPixelDivise(quint32 image,quint8 row, quint8 column) const{
    return m_pixelDiv255Dyn[image][row][column];
};


void lectureMNISTDyn::setPixel(quint32 image,quint8 row, quint8 column,int pixel){
    m_pixelDyn[image][row][column] = pixel;
};

void lectureMNISTDyn::setPixelDivise(quint32 image,quint8 row,
    quint8 column, double pixel){m_pixelDiv255Dyn[image][row][column] = pixel;
};

void lectureMNISTDyn::divisePixelsPar255(quint32 nberOfImages, int nberOfPixels){
    for (quint32 i = 0;i < nberOfImages;i++)
        for (int row = 2;row < nberOfPixels+2;row++)
            for (int col = 2;col < nberOfPixels+2;col++){
                m_pixelDiv255Dyn[i][row][col] = m_pixelDyn[i][row][col]/255.0;
#ifdef test
                m_pixelDiv255Dyn[i][row][col] = 0.2;
#endif
            }
}

lectureMNISTDyn::~lectureMNISTDyn()  {};

double lectureMNISTDyn::BcdToOutput0to9(quint8 BCD, int sortieNber){
    double output[10] = {  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    output[BCD] = 1;
    return output[sortieNber];
}

int lectureMNISTDyn::getFeatureDim() const {return 28; }

int lectureMNISTDyn::getCanaux()const {return m_canaux;}

void lectureMNISTDyn::createExemplesFileForBcdToOutput(quint32 nberOfImages){
    /*
     On crée un fichier d'exemples pour l'entra^inement d'une fonction
        "binary coded decimal (0..9) vers decimal (10 sorties)
     à partir des labels" de MNIST
    */

    QFile m_fichierEx(m_path + "BcdToOutput.txt");
    if ( m_fichierEx.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream monFlux(&m_fichierEx);
        monFlux << "nbrExemples= " << nberOfImages << " 4 10 \n \n";
		// 4=nbre d'entrees, 10=nbre de sorties
        for (quint32 i = 0; i<nberOfImages;i++){
            switch (labelDyn[i]){
            case 0:
                monFlux << "0 0 0 0 \n 0 0 0 0 0 0 0 0 0 1 \n \n";
                break;
            case 1: monFlux << " 0 0 0 1 \n 0 0 0 0 0 0 0 0 1 0 \n \n";
                break;
            case 2: monFlux << " 0 0 1 0 \n 0 0 0 0 0 0 0 1 0 0 \n \n";
                break;
            case 3: monFlux << " 0 0 1 1 \n 0 0 0 0 0 0 1 0 0 0 \n \n";
                break;
            case 4: monFlux << " 0 1 0 0 \n 0 0 0 0 0 1 0 0 0 0 \n \n";
                break;
            case 5: monFlux << " 0 1 0 1 \n 0 0 0 0 1 0 0 0 0 0 \n \n";
                break;
            case 6: monFlux << " 0 1 1 0 \n 0 0 0 1 0 0 0 0 0 0 \n \n";
                break;
            case 7: monFlux << " 0 1 1 1 \n 0 0 1 0 0 0 0 0 0 0 \n \n";
                break;
            case 8: monFlux << " 1 0 0 0 \n 0 1 0 0 0 0 0 0 0 0 \n \n";
                break;
            case 9: monFlux << " 1 0 0 1 \n 1 0 0 0 0 0 0 0 0 0 \n \n";
            }
        }
    }
    m_fichierEx.close();
}

void lectureMNISTDyn::createExemplesFileForXORFunction(quint32 nberOfImages){
    /*
     On crée un fichier d'exemples pour l'entra^inement d'une fonction XOR
     à partir des labels" de MNIST
    */
    QFile m_fichierEx(m_path + "XORNew.txt");
    if ( m_fichierEx.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream monFlux(&m_fichierEx);
        monFlux << "nbrExemples= " << nberOfImages << " 2  1 \n \n";
		// 2= nbre d'entrées, 1= nbre de sorties
        quint32 i =0;
        while (i < nberOfImages){
            switch (labelDyn[i]){
            case 0:
                monFlux << "0 0  \n 0  \n \n";
                i++;
                break;
            case 1:
                monFlux << "0 1 \n 1 \n \n";
                i++;
                break;
            case 2:
                monFlux << "1 0 \n 1 \n \n";
                i++;
                break;
            case 3:
                monFlux << "1 1 \n 0 \n \n";
                i++;
                break;
            default: i++;
            }
        }
    }
    m_fichierEx.close();
}

void lectureMNISTDyn::createExemplesFileForORFunction(quint32 nberOfImages){
    /*
     On crée un fichier d'exemples pour l'entra^inement d'une fonction OR
     à partir des labels" de MNIST
    */

    QFile m_fichierEx(m_path + "ORNew.txt");
    if ( m_fichierEx.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream monFlux(&m_fichierEx);
        monFlux << "nbrExemples= " << nberOfImages << " 2  1 \n \n";
		// 2=nbre d'entrées, 1=nbre de sortie
        quint32 i =0;
        while (i < nberOfImages){
            switch (labelDyn[i]){
            case 0:
                monFlux << "0 0  \n 0  \n \n"; i++;
                break;
            case 1: monFlux << "0 1 \n 1 \n \n"; i++;
                break;
            case 2: monFlux << "1 0 \n 1 \n \n"; i++;
                break;
            case 3: monFlux << "1 1 \n 1 \n \n"; i++;
                break;
            default: i++;
            }
        }
    }
    m_fichierEx.close();
}

void lectureMNISTDyn::createExemplesFileForDecimalToBCDFunction(quint32 nberOfImages){
    /*
     On crée un fichier d'exemples pour l'entra^inement d'une fonction
        "binary coded decimal (0..9) vers decimal (10 sorties)
     à partir des labels" de MNIST
    */

    QFile m_fichierEx(m_path + "decimalToBCD.txt");
    if ( m_fichierEx.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream monFlux(&m_fichierEx);
        monFlux << "nbrExemples= " << nberOfImages << " 10 4 \n \n";
		// 10 entrees, 4 sorties
        for (quint32 i = 0; i<nberOfImages;i++){
            switch (labelDyn[i]){
            case 0:
                monFlux << " 0 0 0 0 0 0 0 0 0 1 \n 0 0 0 0 \n \n";
                break;
            case 1: monFlux << " 0 0 0 0 0 0 0 0 1 0 \n 0 0 0 1 \n \n";
                break;
            case 2: monFlux << " 0 0 0 0 0 0 0 1 0 0 \n 0 0 1 0 \n \n";
                break;
            case 3: monFlux << " 0 0 0 0 0 0 1 0 0 0 \n 0 0 1 1 \n \n";
                break;
            case 4: monFlux << " 0 0 0 0 0 1 0 0 0 0 \n 0 1 0 0 \n \n";
                break;
            case 5: monFlux << " 0 0 0 0 1 0 0 0 0 0 \n 0 1 0 1 \n \n";
                break;
            case 6: monFlux << " 0 0 0 1 0 0 0 0 0 0 \n 0 1 1 0 \n \n";
                break;
            case 7: monFlux << " 0 0 1 0 0 0 0 0 0 0 \n 0 1 1 1 \n \n";
                break;
            case 8: monFlux << " 0 1 0 0 0 0 0 0 0 0 \n 1 0 0 0 \n \n";
                break;
            case 9: monFlux << " 1 0 0 0 0 0 0 0 0 0 \n 1 0 0 1 \n \n";
            }
        }
    }
    m_fichierEx.close();
}
