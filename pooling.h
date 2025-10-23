// sous programmes pour le pooling et création des filtres

#ifndef POOLING_H
#define POOLING_H

#include<QWidget>
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QTextStream>
#include <QVector>
#include <QDebug>
#include <QDate>
#include <random>


using namespace std;
//  public:

void  pooling(QVector < QVector < QVector< double > > > &m_feature, bool pooling,
    int stride, int m_featureDim, int p_featureDim,
    QVector < QVector < QVector< double > > > &p_feature);

void createFilters(int convNber, quint8 nberOfFilters, quint8 filterType,
    QVector<QVector<QVector<double>>> &m_filtres);

#endif //POOLING_H
