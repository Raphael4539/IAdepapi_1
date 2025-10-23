#include "reseauDeNeuronesTest1.h" // pas utilisé pour l'instant



reseauDeNeuronesTest1::reseauDeNeuronesTest1(QWidget *parent, quint16 nberOfPixels,
    QString path) : QDialog(parent){

    m_nberOfPixels =  nberOfPixels;
    posX = 20.0*m_nberOfPixels; // 560
    posY = 4*m_nberOfPixels;  // 112
    width = 18.0*m_nberOfPixels; // 504
    heigh = 18.0*m_nberOfPixels; // 504
    m_path = path;
    m_pen = QPen(Qt::black,3,Qt::SolidLine);
    buttonTest = new QPushButton("TEST",this);
    buttonTest->setGeometry(10,450,100,30);
    buttonFermer = new QPushButton("Fermer",this);
    buttonFermer->setGeometry(150,450,150,30);
    buttonClear = new QPushButton("ClearImage",this);
    buttonClear->setGeometry(300,450,100,30);
    for(auto row = 0;row < m_nberOfPixels +4; row++) // 0..31
        for(auto column = 0; column < m_nberOfPixels +4; column++)
            m_pixelDiv155ForTest[row][column] = 0.0;
    cetMNIST = new lectureMNISTDyn(this,5, m_path); // *** provisoir pour test 5 images
    cetMNIST->divisePixelsPar255(5,m_nberOfPixels);
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0.0,0.0,width/2.0,width/2.0); // 252
    m_view = new QGraphicsView(this);
    deltaSceneX = 227; deltaSceneY = 185;// décalage entre curseur et scene
    xPrec = 0; yPrec = 0;
    m_test1Image = false;
    /*
    vue = new Vue();
    vue->setMouseTracking(true);
    vue->show();
*/


    // x    y   w  h
    dummyButtonT = new QPushButton("-----------------------",this); // en haut
    dummyButtonT->setGeometry(231,200,220,5);
    dummyButtonL = new QPushButton("-----------------------",this); // a gauche
    dummyButtonL->setGeometry(231,200,5,225 );
    dummyButtonB = new QPushButton("-----------------------",this); // en bas
    dummyButtonB->setGeometry(231,420,232,5);
    dummyButtonR = new QPushButton("-----------------------",this); // a droite
    dummyButtonR->setGeometry(456,200,5,225);


    //   m_painter = new QPainter(this);


    connect(buttonFermer, SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonTest, SIGNAL(clicked()), this, SLOT(test1Image()));
    connect(buttonClear, SIGNAL(clicked()), this, SLOT(clearImage()));
    setMouseTracking(false);

}

//void reseauDeNeuronesTest1::mousePressEvent(QMouseEvent *event3){


//}

void reseauDeNeuronesTest1::mouseMoveEvent(QMouseEvent *event){
    x= event->position().x();
    y= event->position().y();
    if (xPrec != 0){
        qDebug() << "x= "<< x << "y= " << y;
        dessine(x,y,deltaSceneX, deltaSceneY);
    }
    xPrec = x; yPrec = y;

    buttonFermer->setText("FRERMER(pos " + QString::number(x)+ "), " +
        QString::number(y));
}
/*
void reseauDeNeuronesTest1::mouseReleaseEvent(QMouseEvent *event2)
{
    xPrec = 0; yPrec = 0;
};
*/
void reseauDeNeuronesTest1::dessine(double x, double y, const double deltaSceneX,
                                    const double deltaSceneY) {
    double sceneXcorr = x-deltaSceneX;
    double sceneYcorr = y-deltaSceneY;
    m_scene->addLine(xPrec-deltaSceneX,yPrec-deltaSceneY, sceneXcorr,sceneYcorr,m_pen);
    m_view->setScene(m_scene);

    qDebug() << "xPrec-deltaSceneX " <<xPrec-deltaSceneX << "yPrec-deltaSceneY "
             << yPrec-deltaSceneY << "sceneXcorr "<< sceneXcorr << "sceneYcorr "<< sceneYcorr;
    m_column = (quint8)  - sceneXcorr/8 -1;// +2; intervertion row column
    m_row = (quint8) - sceneYcorr/8 -2;//+2;
    if(m_row > m_nberOfPixels) m_row = m_nberOfPixels;
    else if(m_row <2) m_row = 2;
    if(m_column > m_nberOfPixels) m_column = m_nberOfPixels;
    else if(m_column < 2) m_column = 2;
    qDebug() << "28 -m_row= " << QString::number(28-m_row) << "28- m_column= "
             //   << QString::number(28 - m_column);
        << QString::number(28 - m_column);  // *** prov
    m_pixelDiv155ForTest[28 - m_row][28 - m_column]  = 0.992; // 2 zeros padding  prov
};

void reseauDeNeuronesTest1::test1Image(){   //elargissement du trait
    for(auto row = 0;row < m_nberOfPixels +4; row++){ // 0..31
        for(auto column = 0; column < m_nberOfPixels +4; column++){
            if(row < 2 or row >= m_nberOfPixels +2 or column < 2 or
                column >= m_nberOfPixels + 2)
                m_pixelDiv155ForTest[row][column] = 0.0; // m.a.z pour zero padding
            else
                if(m_pixelDiv155ForTest[row][column] > 0.9){
                    for(auto r = row-2; r < row + 3; r++){
                        if(r > 1 and r < m_nberOfPixels +2 and r != row){
                            // elargissement du trait Hor.
                            if(m_pixelDiv155ForTest[r][column] < 0.99)
                                m_pixelDiv155ForTest[r][column] = 0.9;
                        }
                    }
                    for(auto c = column -2; c < column +3; c++){
                        if(c > 1 and c < m_nberOfPixels +2 and c != column){
                            //elargissement du trait Vert.
                            if(m_pixelDiv155ForTest[row][c] < 0.99)
                                m_pixelDiv155ForTest[row][c] = 0.9;
                        }
                    }
                }
                else if(m_pixelDiv155ForTest[row][column] < 0.8)
                    m_pixelDiv155ForTest[row][column] = 0.0;
        }
    }
    /*
   // provisoir, pour test
    for(auto row = 0;row < m_nberOfPixels +4; row++) // 0..31
        for(auto column = 0; column < m_nberOfPixels +4; column++)
            m_pixelDiv155ForTest[row][column] =  cetMNIST->getPixelDivise(2,row,column);
 */
    m_test1Image = true;
}


bool reseauDeNeuronesTest1::getTest1Image(){ return m_test1Image;}

void reseauDeNeuronesTest1::setTest1Image(bool test1Image){
    m_test1Image = test1Image;
}

double reseauDeNeuronesTest1::get1ImageForTest(quint8 row,quint8 column){
    return m_pixelDiv155ForTest[row][column];
};

void reseauDeNeuronesTest1::setTexteResult(QString resultat){
    m_resultat = resultat;
    buttonTest->setText(m_resultat);
}

void reseauDeNeuronesTest1::clearImage(){
    for(auto row=0; row < m_nberOfPixels+4; row++)
        for(auto column = 0; column < m_nberOfPixels +4; column++)
            m_pixelDiv155ForTest[row][column] = 0.0;
    m_test1Image = false;
    m_scene->clear();
    m_view->setScene(m_scene);
}


reseauDeNeuronesTest1::~reseauDeNeuronesTest1(){
    m_test1Image = false;
}
