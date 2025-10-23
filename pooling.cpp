// pooling.cpp
// sous programmes pour le pooling

#include "pooling.h"
#define notest
using namespace std;

void pooling( QVector < QVector < QVector< double > > > &m_feature,
             bool pooling, int stridePooling, int m_featureDim, int p_featureDim,
             QVector < QVector < QVector< double > > > &p_feature ){

    int p_nberOfFiltersPrec = p_feature.size();
    // on a ajouter 2* 2 zeroPadding au feature
    bool p_pooling = pooling;
    int p_stride = stridePooling;
    bool pair = (p_featureDim) % 2 < 0.1;
    if (p_pooling  and !pair) p_featureDim += 1;

    double maxValue = -100.0;
    int xo = 2, yo = 2; // si besoin de zero padding pour la convolution suivante
    for (int yi=2 ;yi<m_featureDim+2;yi+=p_stride){
        for (int xi=2; xi<m_featureDim+2;xi+=p_stride){
            for (int fNum=0; fNum<p_nberOfFiltersPrec;fNum++){
                for(int yf=0;yf<p_stride; yf++){
                    for(int xf=0;xf<p_stride; xf++){
                        if (m_feature[fNum][yi+yf][xi+xf] > maxValue){
                            maxValue = m_feature[fNum][yi+yf][xi+xf];
                        }
                    }
                }
                p_feature[fNum][yo][xo] = maxValue;
                maxValue = -100.0;
            }
            if(xo<p_featureDim +1)  xo++; else xo=2;
        }
        if(yo<p_featureDim +1) yo++; else yo=2;
    }
}


void createFilters(int convNber, quint8 nberOfFilters, quint8 filterType,
                   QVector<QVector<QVector<double>>> &m_filtres){

    if(convNber == 1){
        if(filterType == 3){
            double tab[9][3][3]={
        #ifdef test
           {{0,0,0},{0,1,0},{0,0,0}},
           {{0,0,0},{0,1,0},{0,0,0}},
        #else
           {{1,0,-1},{2,0,-2},{1,1,-1}},// left sobel
        #endif
           {{-1,-2,-1},{0,0,0},{1,2,1}},// bottom sobel
           {{-1,0,1},{-2,0,-2},{-1,0,1}},// right sobel
           {{1,2,1},{0,0,0},{-1,-2,-1}},// top sobel
           {{-2,-1,0},{-1,1,1},{0,1,2}},// emboss (gaufrer)
           {{0,-1,0},{-1,5,-1},{0,-1,0}},// sharpen (aiguiser)
           {{0,1,0},{1,-4,1},{0,1,0}}, // contours
           {{0,0,0},{-1,1,0},{0,0,0}}}; // renforcement des bords

            int t = 0;
            while(t < 8 and t < nberOfFilters) {
                for (int row=0; row < filterType; row++){
                    for (int col=0; col  < filterType; col++){
                        m_filtres[t][row][col] = tab[t][row][col] ;
                    }
                }
                t++;
            }
            if(t < nberOfFilters){
                std::default_random_engine  re;
                std::uniform_int_distribution<int> distrib{-40,40};
                while ( t < nberOfFilters ) {
                    for (int row=0; row  < filterType; row++){
                        for (int col=0; col  < filterType; col++){
                            m_filtres[t][row][col] = ((double)distrib(re)/10.0);
                        }
                    }
                    t++;
                }
            }

        } else
        if(filterType == 5){
            double tab[5][5][5]={
            {{0,0,-1,0,0},{0,-1,-2,-1,0},{-1,-2,17,-2,-1},{0,-1,-2,-1,0},
             {0,0,-1,0,0}},//Laplacien 5x5
            {{-1,-3,-4,-3,-1},{-3,0,6,0,-3},{-4,6,21,6,-4},{-3,0,6,0,-3},
             {-1,-3,-4,-3,-1}},//Affinage 5x5
            {{1,1,1,1,1},{1,4,4,4,1},{1,4,12,4,1},{1,4,4,4,1},
             {1,1,1,1,1}},//Lissage 5x5
            {{0,0,0,0,0},{0,0,-1,0,0},{0,-1,5,-1,0},{0,0,-1,0,0},
             {0,0,0,0,0}},// + de contraste
            {{0,0,0,0,0},{0,1,1,1,0},{0,1,1,1,0},{0,1,1,1,0},{0,0,0,0,0}}}; //Floutage
            int t=0;
            while(t < 5 and t< nberOfFilters){
                for (int row=0; row < filterType; row++){
                    for (int col=0; col  < filterType; col++){
                        m_filtres[t][row][col] = tab[t][row][col] ;
                    }
                }
                t++;
            }
            if(t < nberOfFilters){
                std::default_random_engine  re;
                std::uniform_int_distribution<int> distrib{-60,60};
                while ( t < nberOfFilters ) {
                    for (int row=0; row  < filterType; row++){
                        for (int col=0; col  < filterType; col++){
                            m_filtres[t][row][col] = ((double)distrib(re)/10.0);
                        }
                    }
                    t++;
                }
            }
        }
    }else
    if(convNber == 2){
        if(filterType == 3){
            double tab[9][3][3]={
        #ifdef test
            {{0,0,0},{0,2,0},{0,0,0}},
            {{0,0,0},{0,3,0},{0,0,0}}};
        #else
            {{-2,-1,0},{-1,1,1},{0,1,2}},// emboss (gaufrer)
            {{0,-1,0},{-1,5,-1},{0,-1,0}},// sharpen (aiguiser)
            {{-1,-2,-1},{0,0,0},{1,2,1}},// bottom sobel
            {{-1,0,1},{-2,0,-2},{-1,0,1}},// right sobel
            {{1,2,1},{0,0,0},{-1,-2,-1}},// top sobel
            {{-2,-1,0},{-1,1,1},{0,1,2}},// emboss (gaufrer)
            {{0,-1,0},{-1,5,-1},{0,-1,0}},// sharpen (aiguiser)
            {{0,1,0},{1,-4,1},{0,1,0}}, // contours
            {{0,0,0},{-1,1,0},{0,0,0}}}; // renforcement des bords

        #endif
            int t=0;
            while(t < 9 and t < nberOfFilters){
                for (int row=0; row < filterType; row++){
                    for (int col=0; col  < filterType; col++){
                        m_filtres[t][row][col] = tab[t][row][col] ;
                    }
                }
                t++;
            }
            if(t < nberOfFilters){
                std::default_random_engine  re;
                std::uniform_int_distribution<int> distrib{-40,40};
                while ( t < nberOfFilters ) {
                    for (int row=0; row  < filterType; row++){
                        for (int col=0; col  < filterType; col++){
                            m_filtres[t][row][col] = ((double)distrib(re)/10.0);
                        }
                    }
                    t++;
                }
            }
        } else if (filterType == 5){
            double tab[5][5][5]={
            {{0,0,-1,0,0},{0,-1,-2,-1,0},{-1,-2,17,-2,-1},{0,-1,-2,-1,0},
             {0,0,-1,0,0}},//Laplacien 5x5
            {{-1,-3,-4,-3,-1},{-3,0,6,0,-3},{-4,6,21,6,-4},{-3,0,6,0,-3},
             {-1,-3,-4,-3,-1}},//Affinage 5x5
            {{1,1,1,1,1},{1,4,4,4,1},{1,4,12,4,1},{1,4,4,4,1},
             {1,1,1,1,1}},//Lissage 5x5
            {{0,0,0,0,0},{0,0,-1,0,0},{0,-1,5,-1,0},{0,0,-1,0,0},
             {0,0,0,0,0}},// + de contraste
            {{0,0,0,0,0},{0,1,1,1,0},{0,1,1,1,0},{0,1,1,1,0},{0,0,0,0,0}}}; //Floutage
            int t=0;
            while(t < 5 and t< nberOfFilters){
                for (int row=0; row < filterType; row++){
                    for (int col=0; col  < filterType; col++){
                        m_filtres[t][row][col] = tab[t][row][col] ;
                    }
                }
                t++;
            }
            if(t < nberOfFilters){
                //        }
                std::default_random_engine  re;
                std::uniform_int_distribution<int> distrib{-60,60};
                while ( t < nberOfFilters ) {
                    for (int row=0; row  < filterType; row++){
                        for (int col=0; col  < filterType; col++){
                            m_filtres[t][row][col] = ((double)distrib(re)/10.0);
                        }
                    }
                    t++;
                }
            }
        }
    }
    if(convNber == 3){
        if(filterType == 3){
            double tab[9][3][3]={
                #ifdef test
                {{0,0,0},{0,2,0},{0,0,0}},
                {{0,0,0},{0,3,0},{0,0,0}}};
                #else
                {{-2,-1,0},{-1,1,1},{0,1,2}},// emboss (gaufrer)
                {{0,-1,0},{-1,5,-1},{0,-1,0}},// sharpen (aiguiser)
                {{-1,-2,-1},{0,0,0},{1,2,1}},// bottom sobel
                {{-1,0,1},{-2,0,-2},{-1,0,1}},// right sobel
                {{1,2,1},{0,0,0},{-1,-2,-1}},// top sobel
                {{-2,-1,0},{-1,1,1},{0,1,2}},// emboss (gaufrer)
                {{0,-1,0},{-1,5,-1},{0,-1,0}},// sharpen (aiguiser)
                {{0,1,0},{1,-4,1},{0,1,0}}, // contours
                {{0,0,0},{-1,1,0},{0,0,0}}}; // renforcement des bords
                #endif
            int t=0;
            while(t < 9 and t < nberOfFilters){
                for (int row=0; row < filterType; row++){
                    for (int col=0; col  < filterType; col++){
                        m_filtres[t][row][col] = tab[t][row][col] ;
                    }
                }
                t++;
            }
            if(t < nberOfFilters){
                std::default_random_engine  re;
                std::uniform_int_distribution<int> distrib{-40,40};
                while ( t < nberOfFilters ) {
                    for (int row=0; row  < filterType; row++){
                        for (int col=0; col  < filterType; col++){
                            m_filtres[t][row][col] = ((double)distrib(re)/10.0);
                        }
                    }
                    t++;
                }
            }
        } else if (filterType == 5){
            double tab[5][5][5]={
                {{0,0,-1,0,0},{0,-1,-2,-1,0},{-1,-2,17,-2,-1},{0,-1,-2,-1,0},
                 {0,0,-1,0,0}},//Laplacien 5x5
                {{-1,-3,-4,-3,-1},{-3,0,6,0,-3},{-4,6,21,6,-4},{-3,0,6,0,-3},
                 {-1,-3,-4,-3,-1}},//Affinage 5x5
                {{1,1,1,1,1},{1,4,4,4,1},{1,4,12,4,1},{1,4,4,4,1},
                 {1,1,1,1,1}},//Lissage 5x5
                {{0,0,0,0,0},{0,0,-1,0,0},{0,-1,5,-1,0},{0,0,-1,0,0},
                 {0,0,0,0,0}},// + de contraste
                {{0,0,0,0,0},{0,1,1,1,0},{0,1,1,1,0},{0,1,1,1,0},{0,0,0,0,0}}}; //Floutage
            int t=0;
            //     while (t < nberOfFilters){
            while(t < 5 and t< nberOfFilters){
                for (int row=0; row < filterType; row++){
                    for (int col=0; col  < filterType; col++){
                        m_filtres[t][row][col] = tab[t][row][col] ;
                    }
                }
                t++;
            }
            if(t < nberOfFilters){
                //        }
                std::default_random_engine  re;
                std::uniform_int_distribution<int> distrib{-60,60};
                while ( t < nberOfFilters ) {
                    for (int row=0; row  < filterType; row++){
                        for (int col=0; col  < filterType; col++){
                            m_filtres[t][row][col] = ((double)distrib(re)/10.0);
                        }
                    }
                    t++;
                }
            }
        }
    }
}
