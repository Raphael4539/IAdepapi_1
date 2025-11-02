// main.cpp @ MNISTTabDyn  Bernard Ribordy
//    du 2025/03/20

// voir:
//https://www.kaggle.com/cdeotte/how-to-choose-cnn-architecture-mnist
// https://alp.developpez.com/tutoriels/intelligence-artificielle/reseaux-de-neurones/
// http://penseeartificielle.fr/focus-reseau-neurones-convolutifs/



#include <QWidget>
#include "reseauDeNeurones.h"
#include <QApplication>
#include <qapplication.h>
#include <QtMessageHandler>
#include <QPushButton>
#include <QtDebug>
#include <QDate>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    qDebug() << "Date " << QDate::currentDate();
    QWidget *w = nullptr;
    w = new QWidget;
    w->resize(2500,900);
    reseauDeNeurones fenetre(w);
    fenetre.show();
    return app.exec();
}


