#ifndef CONVOLUTIONDYN_H
#define CONVOLUTIONDYN_H
#include<QWidget>
#include "lectureMNISTDyn.h"
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QTextStream>
#include <QVector>
#include <QDebug>
#include <QDate>

class convolutionDyn {

public:

    convolutionDyn(lectureMNISTDyn *MNIST,convolutionDyn *convPrec,
        quint16 m_nberOfPixels, int convNber, bool featurePadding, int stride,
        int stridePooling, int nberOfFilters, int filterType, bool RELU, bool maxPool,
        bool poolingPair, int nberOfConvolutions,  QString fileName);
    int getFeatureDim()const;
    void setFeature(int filterNber, int row, int column, double val );
    double getFeature(int filterNber, int row, int column) const;
    void convDyn(quint32 imageNber, quint16 nberOfBoucle);
    int getNberOfFilters() const;
 //   bool getFlatten() const;

    ~convolutionDyn();
private:
    QString m_fileName;
    QString  m_file;
 //    QTextStream m_convTest;
    QFile * m_testConv;
    int m_featureDim;
    int p_featureDim;
    int m_featurePrecDim;
    convolutionDyn * m_convPrec;
    int m_zeroPadding;
    int m_filterType;
    int m_nberOfFilters;
    quint16 m_nberOfPixels;
    bool m_RELU;
    bool m_maxPool;
    int m_stride;
    lectureMNISTDyn *m_MNIST;
    int m_stridePooling;
    int m_convNber;
 //   int m_pixelsOfImage;
    double m_cumulTransversal;
    int m_nberOfFiltersPrec;
 //   int m_rowPrec;
 //   int m_columnPrec;
    int m_nberOfconvolutions;
 //    bool m_flatten;
    bool m_padding;
    bool m_pooling;
    QVector< double >  m_cumul;
    QVector < QVector < QVector< double > > >  m_feature;
    QVector < QVector < QVector< double > > > p_feature;
    QVector < QVector < QVector< double > > > m_filtres;
    QVector < QVector< double > > m_cumulFeature;
};


#endif // CONVOLUTIONDYN_H
