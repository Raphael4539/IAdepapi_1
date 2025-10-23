// reseauDeNeuronesGraph.h   Bernard Ribordy
//    du 2024_06_03

#ifndef RESEAUDENEURONESGRAPH_H
#define RESEAUDENEURONESGRAPH_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QRectF>
#include <QLineF>
#include <QColor>
#include <QPen>
#include <QPoint>
#include<QGraphicsEllipseItem>
#include <QPushButton>
#include <QTableWidget>
#include <QGraphicsGridLayout>
#include <QTextEdit>
#include <QGraphicsWidget>
#include  "neurones.h"
#include <QCheckBox>

//const quint16 nbCMax_graph=30;

class reseauDeNeuronesGraph : public QWidget {

    Q_OBJECT

public:

    reseauDeNeuronesGraph(QWidget *parent=nullptr);

    void ConstructeurDyn(quint16 &NQ, quint16 NC[], quint16 NB[],
        quint16 &NCNBMax, QVector<QVector<neurone*>> &tabBiaisDyn,
        QVector<QVector<neurone*>> &tabCellDyn, QCheckBox* afficheDetails);
 //   QGraphicsView* getGraphiqueView();
    ~reseauDeNeuronesGraph();
    quint16 getRotation() const;

private:
    QString * m_texteArr;
    QString * m_texteDep;
    QString * m_texteNom;
    QString * m_textePoids;
    QGraphicsSimpleTextItem * m_texteOutput;
    QGraphicsSimpleTextItem * m_texteDi;
    QGraphicsEllipseItem *m_ellips;
    QGraphicsLineItem *m_liaison;
    QGraphicsScene  *m_scene;
    QGraphicsView   *m_view;
    QPushButton     *fermer;
};

#endif // RESEAUDENEURONESGRAPH_H
