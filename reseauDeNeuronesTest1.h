#ifndef RESEAUDENEURONESTEST1_H
#define RESEAUDENEURONESTEST1_H
#include <QWidget>
#include <QPushButton>
#include <QGraphicsScene>
#include <QDialog>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QRectF>
#include <QLabel>
#include "lectureMNISTDyn.h"

//#include "vue.h"

class reseauDeNeuronesTest1 : public QDialog{
    Q_OBJECT
public:
    reseauDeNeuronesTest1(QWidget *parent, quint16 nberOfPixels, QString path);  //, quint16 nberOfPixels);
    ~reseauDeNeuronesTest1();
    double getPixelDiv155ForTest(quint8 row, quint8 column) const;
    void setPixelDiv155ForTest(quint8 row, quint8 column, double pixel);
    double get1ImageForTest(quint8 row,quint8 column);
    void setTest1Image(bool test1Image);
    bool getTest1Image();
    void setTexteResult(QString resultat);

public slots:
    void test1Image();
    void clearImage();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    //   void mouseMoveEvent(QMouseEvent *event) ;
    //  void mouseReleaseEvent(QMouseEvent *event2) ;
    void dessine(double x, double y, const double deltaSceneX , const double deltaSceneY);
    //   void mousePressEvent(QMouseEvent *event3) ;
    //    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);

private:
    qreal posX ; // 560
    qreal posY;  // 112
    qreal width; // 504
    qreal heigh; // 504
    QPushButton     *buttonFermer;
    QPushButton     *buttonTest;
    QPushButton     *buttonClear;
    QRectF          *m_rectangle;
    QGraphicsScene  *m_scene;
    QGraphicsView   *m_view;
    QLabel          *m_labelImage;
    double          xPrec, yPrec;
    double          x, y;
    QPen            m_pen;
    quint8          m_row, m_column;
    quint16             m_nberOfPixels;
    double deltaSceneX;
    double deltaSceneY;
    bool m_test1Image;
    QString  m_resultat;
    QPushButton    *dummyButtonT;
    QPushButton    *dummyButtonB;
    QPushButton    *dummyButtonL;
    QPushButton    *dummyButtonR;
    lectureMNISTDyn *cetMNIST;
    static const quint8 rowMax = 32; static const quint8 columnMax = 32;
    double m_pixelDiv155ForTest[rowMax][columnMax];
    QString m_path;
    //   Vue *vue;
    //  QString m_path;
};

#endif // RESEAUDENEURONESTEST1_H
