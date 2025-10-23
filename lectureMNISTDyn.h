#ifndef LECTUREMNISTDYN_H
#define LECTUREMNISTDYN_H

#include <QWidget>
#include <QMessageBox>
#include <QDataStream>
#include <QFile>
#include "reseauDeNeurones_variablesConstantes.h"
#include <QDir>

using namespace std;

class lectureMNISTDyn{
public:
    lectureMNISTDyn(QWidget *w,quint32 nberOfImages, QString path);
    QImage getmonImage() const;
    quint8 getmonLabel(quint32 i) const;
    void createExemplesFileForBcdToOutput(quint32 nberOfImages);
    void createExemplesFileForXORFunction(quint32 nberOfImages);
    void createExemplesFileForORFunction(quint32 nberOfImages);
    void createExemplesFileForDecimalToBCDFunction(quint32 nberOfImages);

    int getPixel(quint32 image,quint8 row, quint8 column) const;
    double getPixelDivise(quint32 image,quint8 row, quint8 column) const;
    void setPixelDivise(quint32 image,quint8 row, quint8 column, double pixel);
    void setPixel(quint32 image,quint8 row, quint8 column,int pixel);
    void divisePixelsPar255(quint32 nberOfImages, int nberOfPixels);
    double getFilter(quint8 filterType, quint32 filterNber,quint8 row,
        quint8 column) const;
    double BcdToOutput0to9(quint8 BCD, int sortieNber);
    int getCanaux()const;
    int getFeatureDim() const;
    void init();

    ~lectureMNISTDyn();

private:
    qint32 magicNumber;
    qint32 nberOfItems;
    quint32 magicNberImage;
    quint32 nberOfImages;       // 60000
    quint32 nberOfRows;         //28
    quint32 nberOfColumns;      //28
    QVector<QVector<QVector<int>>> m_pixelDyn;
    QVector< quint8> labelDyn;
    QVector<QVector<QVector<double>>> m_pixelDiv255Dyn;
    QString  m_fichierExemples;
    QFile * m_fichier;
    QFile * m_fichierEx;
    QString * output;
    int m_canaux;
    QString m_path;
};

#endif // LECTUREMNISTDYN_H
