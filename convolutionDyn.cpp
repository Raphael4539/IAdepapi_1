// convolutionDyn.cpp   Bernard Ribordy
//    du 2024_06_03
// si choix du fichier exemple MNIST.txt on effectue des convolutions - pooling


#include "convolutionDyn.h"
#include "pooling.h"

using namespace std;

#define notest
#define methode2

/*
Dimention des images apres un convolution
 O = ((I–K+2P)/S) +1

avec :

    O : taille de l’image de sortie (largeur)
    I : taille de l’image d’entrée (largeur)
    K : taille du kernel utilisé (largeur)
    N : nombre de filtres
    S : le stride appliqué
    P : le padding appliqué
*/

convolutionDyn::convolutionDyn(lectureMNISTDyn *MNIST, convolutionDyn *convPrec,
        quint16 nberOfPixels, int convNber, bool FeaturePadding, int stride,
        int stridePooling, int nberOfFilters, int filterType, bool RELU, bool maxPool,
        bool poolingPair, int nberOfConvolutions,  QString fileName ){
    m_fileName = fileName;
    m_MNIST = MNIST;
    m_convNber = convNber;
    m_nberOfPixels = nberOfPixels;
    m_filterType = filterType;
    m_nberOfFilters = nberOfFilters;
    m_RELU = RELU;
    m_convPrec = convPrec;
    m_stride = stride;
    m_maxPool = maxPool;
    m_stridePooling = stridePooling;
    m_nberOfconvolutions = nberOfConvolutions;

    //    m_convPrec = convPrec;
    m_padding = FeaturePadding;//if FeaturePadding = true, Feature Dim = "same as input",
    m_pooling = poolingPair;
    m_zeroPadding = 0;
    m_file = "";
    m_testConv = nullptr;
    p_featureDim = 0;
    m_cumulTransversal = 0.0;
    m_nberOfFiltersPrec = 0;


    if(FeaturePadding and m_stride == 1)
        m_zeroPadding = (m_filterType - stride) / 2; // if "same"
    qDebug() << " 60 ";
    if (FeaturePadding and filterType == 3)m_zeroPadding =1;
    else if (FeaturePadding and filterType == 5)m_zeroPadding = 2;
    else m_zeroPadding = 0;

    if (m_convPrec ==nullptr) m_featurePrecDim = 0;
    else {
        m_featurePrecDim = convPrec->getFeatureDim();
        m_nberOfFiltersPrec = m_convPrec->getNberOfFilters();
    }
    if (m_convNber == 0)m_featureDim = m_nberOfPixels;
	// on transmettra l'image dans cette convol. 0
    else  // dimention de la sortie (feature)
        m_featureDim = ((m_featurePrecDim - filterType + 2*m_zeroPadding)/stride) +1;

    p_featureDim = m_featureDim / m_stridePooling;
    bool pair = (p_featureDim) % 2 < 0.1;
    if ((m_pooling == true and !pair)) p_featureDim += 1;//

    p_feature.resize(m_nberOfFilters, QVector<QVector<double>>(p_featureDim +4,
        QVector<double>(p_featureDim + 4,0.0)));
    qDebug() << "m_convNber: " << m_convNber << ", m_featureDim: " <<
		m_featureDim << ", p_featureDim: " << p_featureDim;

    m_filtres.resize(m_nberOfFilters,QVector<QVector<double>>(m_filterType,
        QVector<double>(m_filterType,0.0)));
    qDebug() << " 60 ";

    if(m_convNber > 0)
        createFilters(m_convNber, m_nberOfFilters, m_filterType, m_filtres);
    qDebug() << " 78 ";

    m_cumulFeature.resize(m_featurePrecDim +4, QVector<double>(m_featurePrecDim + 4,0.0));
    m_feature.resize(m_nberOfFilters, QVector<QVector<double>>(m_featureDim +4,
        QVector<double>(m_featureDim + 4,0.0))); // on initialise une nouvelle feature

#ifdef test
    m_testConv = new QFile(m_fileName + "_testConvolution_"  +
	QDate::currentDate().toString()+".csv" );
        // voir  RESEAUDENEURONES_VARIABLESCONSTANTES_H
    if (m_testConv->exists() && m_testConv->isOpen()) m_testConv->close();
    if (m_convNber == 0 and m_testConv->open(QIODevice::WriteOnly))
        m_testConv->close();
#endif

    if (m_convNber == 0) { m_cumul.resize(1);}
    else { m_cumul.resize(m_convPrec->m_featureDim);}                                                                                                if (m_convNber == 0) m_cumul.resize(0);
#ifdef test
    qDebug() << "convNber= " <<m_convNber <<" featureDim avant pooling " <<m_featureDim;
#endif
}

void convolutionDyn::convDyn(quint32 imageNber, quint16 BoucleNber){
#ifdef methode1
    // tous les filtres en même temps + RELU
    //   for (quint32 iNum = 0; iNum < nberOfImages;iNum++){
    //   if (m_convNber > 0){
    for (int fnberPrec =0;fnberPrec<m_featurePrecDim;fnberPrec++)m_cumul[fnberPrec]=0.0;
    int xo = 2, yo = 2; // si besoin de zero padding pour la convolution suivante
    int fprec = 0;
    m_cumulTransversal = 0.0;
    //   if(convNber==1){ // convolution source MNIST
    for (int yi=2 - m_zeroPadding;yi<m_featureDim + 2 - m_zeroPadding;yi+=m_stride){
        for (int xi=2 - m_zeroPadding;xi<m_featureDim + 2 - m_zeroPadding;xi+=m_stride){
            for (int fNum=0; fNum<m_nberOfFilters;fNum++){
                for(int yf=0;yf<m_filterType;yf++){
                    for(int xf=0;xf<m_filterType;xf++){
                        // applique transversalement la convolution à chaque feature prec.
                        m_cumul[fprec]=0.0; // en cumulant les resultats apres RELU
                        for( fprec = 0; fprec < m_convPrec->m_nberOfFilters;fprec++){
                            m_cumul[fprec] += (double)m_convPrec->
                              getFeature(fprec, yi+yf, xi+xf) * m_filtres[fNum][yf][xf];
                        }
                    }
                }
                // RELU s'applique individuellement pour chaque feature precedant
                for( fprec = 0; fprec < m_convPrec->m_nberOfFilters;fprec++){
                    if (abs(m_cumul[fprec])<0.01) m_cumul[fprec]=0.00;
                    if (m_RELU && m_cumul[fprec]<0)  m_cumul[fprec] = 0.00;
                    // RELU s'applique individuellement pour chaque feature precedant
                    m_cumulTransversal += m_cumul[fprec];
                    m_cumul[fprec]=0.0;
                }
                m_feature[fNum][yo][xo]=m_cumulTransversal;
                m_cumulTransversal = 0.0;
                    // RELU  on elimine les valeurs negatives
            }
            if(xo<m_featureDim +1)  xo++; else xo=2;
        }
        //la feature est contruite avec 2 zeros padding
        if(yo<m_featureDim + 1) yo++; else yo=2;
    }
#endif  // methode1

#ifdef methode2

    int xo = 2, yo = 2; // si besoin de zero padding pour la convolution suivante
    // on commance par cumuler transversalement les features précédentes
    //   if(convNber==1){ // convolution source MNIST
    for (int yi=0; yi < m_featurePrecDim + 4 ;yi++){
        for (int xi=0 ;xi < m_featurePrecDim + 4;xi++){
            m_cumulFeature[yi][xi]=0.0;
        }
    }
    for (int yi=0; yi < m_featurePrecDim + 4 ;yi++){
        for (int xi=0 ;xi < m_featurePrecDim + 4;xi++){
            for (int fNum=0; fNum < m_nberOfFiltersPrec;fNum++){
              m_cumulFeature[yi][xi] += m_convPrec->getFeature(fNum,yi,xi) / 10;//29/6/25
            }
        }
    }
    // on applique les filtres
    for (int yi =2-m_zeroPadding; yi < m_featureDim + 2-m_zeroPadding;yi+=m_stride){
        for (int xi =2-m_zeroPadding; xi < m_featureDim + 2-m_zeroPadding;xi+=m_stride){
            for (int fNum=0; fNum<m_nberOfFilters;fNum++){
                m_feature[fNum][yo][xo]=0.0;
                for(int yf=0;yf<m_filterType;yf++){
                    for(int xf=0;xf<m_filterType;xf++){
                        m_feature[fNum][yo][xo] += m_cumulFeature[yi+yf][xi+xf] *
                                                   m_filtres[fNum][yf][xf];
                    }
                }
                if (abs(m_feature[fNum][yo][xo]) < 0.01) m_feature[fNum][yo][xo]=0.0;
                // on applique RELU pour chaque feature
                if (m_RELU && m_feature[fNum][yo][xo] < 0.0) m_feature[fNum][yo][xo]=0.0;
            }
            if (xo < m_featureDim +1)  xo++; else xo=2;
        }
        //la feature est contruite avec 2 zeros padding
        if (yo < m_featureDim + 1) yo++; else yo=2;
    }

#endif // methode2
    if(m_maxPool){
        pooling(m_feature, m_pooling, m_stridePooling, m_featureDim, p_featureDim,
                p_feature);
    }


#ifdef test 
    // test de la convolution sur la première image qui représente un 5 \
    // voir  RESEAUDENEURONES_VARIABLESCONSTANTES_H
    if ((imageNber == 0 or imageNber == 220 or imageNber == 221) and BoucleNber == 1){
		if (m_testConv->open(QIODevice::Append)){
            QTextStream convTest(m_testConv) ;
            convTest.setRealNumberPrecision(5);
            convTest << QDate::currentDate().toString() << "\n";
            convTest << "\n\n Convolution Nber: " << m_convNber << "\n RELU: " <<
                m_RELU << "\n";
            convTest  << "ImageNber: " << imageNber << " Label: " <<
                m_MNIST->getmonLabel(imageNber) << " faturePrec[0] "  << "\n\n , ,";

            if(m_convNber == 1){ // seulement pour l'image (convNber=0).
                for(int i=0;i<32;i++) convTest << i << ", ";
                convTest << "\n\n";
                for ( int i = 0; i < m_convPrec->getFeatureDim()+4; i++){
                    convTest << ", ,";
                    for (int j = 0; j <m_convPrec->getFeatureDim()+4; ++j){
                        convTest << m_convPrec->getFeature(0,i,j) << " ,";
                    }
                    convTest << "\n";
                }
            }else if(m_convNber  >1){
                convTest << "\n" << "cumul feature prec" << "\n";
                for(int i=0;i<32;i++) convTest << i << ", ";
                convTest << "\n\n";
                for (int i=0; i < m_featurePrecDim + 4 ;i++){
                    convTest << ", ,";
                    for (int j=0 ;j < m_featurePrecDim + 4; j++){
                        convTest << m_cumulFeature[i][j] << " ,";
                    }
                    convTest << "\n";
                }
            }
            convTest << "\n";

            int fMax = 0;
            if(m_nberOfFilters < nberOfFiltersForTest) fMax = m_nberOfFilters;
            else fMax = nberOfFiltersForTest;
            for (int fNum = 0; fNum < fMax;fNum++){
                convTest << "\n" << "filtre " << fNum << "\n";
                for ( int i = 0; i < m_filterType; ++i){
                    convTest << ", ,";
                    for (int j = 0; j < m_filterType; ++j){
                        convTest << m_filtres[fNum][i][j]  << ",";
                    }
                    convTest << "\n";
                }
                convTest << "\n" << "feature avec filtre " << fNum << "\n"
					" m_featureDim = "  << m_featureDim << "\n zeroPadding(left) = "
                         << m_zeroPadding << "\n";
                convTest << " , ,     ";

                for(int i=0;i< m_featureDim + 4;i++) convTest << i << ", ";
                convTest << "\n\n";
                for ( int i = 0; i < m_featureDim + 4; ++i){
                    convTest <<"i=" << i << ", , ";
                    for (int j = 0; j < m_featureDim + 4; ++j){
                        convTest << "" << m_feature[fNum][i][j] << ", ";
                    }
                    convTest << "\n";
                }
                convTest << "\n";
            }
            if(m_maxPool){

                for (int fNum=0; fNum<fMax;fNum++){
                    convTest << "\n" << "filtre " << fNum << "\n";
                    convTest << "\n" << " maxPool fNum, " << fNum <<
                        "\n p_featureDim = "  << p_featureDim << "\n";
                    //                   convTest << "      ";

                    convTest << "\n , ,";
                    for(int i=0;i< p_featureDim + 4;i++) convTest << i << ", ";
                    convTest << "\n";
                    for ( int i = 0; i < p_featureDim + 4; ++i){
                        convTest <<"i= " << i << ", ,";
                        for (int j = 0; j < p_featureDim + 4; ++j){
                            convTest << "" << p_feature[fNum][i][j] << ", ";
                        }
                        convTest << "\n";
                    }
                    convTest << "\n";
                }
            }
        }
		  m_testConv->close();
	}
  
#endif

}

int convolutionDyn::getFeatureDim()const{
    if(m_maxPool)  return p_featureDim;
    else return m_featureDim;
}
//double convolutionPoolingDyn::getFlattened(int nb){return flattenedDyn[nb];}
void convolutionDyn::setFeature(int filterNber, int row, int column, double val ){
    m_feature[filterNber][row][column] = val;
}

double convolutionDyn::getFeature(int filterNber, int row, int column) const {
    if(m_maxPool)  return p_feature[filterNber][row][column];
    else return m_feature[filterNber][row][column];}

int convolutionDyn::getNberOfFilters() const {return m_nberOfFilters;}



convolutionDyn::~convolutionDyn(){};

