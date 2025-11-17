// main.cpp @ MNISTTabDyn  Bernard Ribordy
//    du 2025/03/20

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
    w->resize(1500,500);
    reseauDeNeurones fenetre(w);
    fenetre.show();
    return app.exec();
}



