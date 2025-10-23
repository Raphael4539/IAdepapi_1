// diversNeuroneSub.h   Bernard Ribordy
//    du 2024_06_03

#ifndef DIVERSNEURONESSUB_H
#define DIVERSNEURONESSUB_H

#include <QTableWidget>
#include  "reseauDeNeurones_variablesConstantes.h"
#include "neurones.h"
#include <QMessageBox>

void adapteFile(QWidget * monParent, QString fileToCor, QString fileCorr);

bool lectureFichierExemplesDyn(QWidget *monParent, QString fileName, quint32 &nex,
    quint16 &nx, quint16 &ns, QVector<QVector<double>> &xexDyn,
        QVector<QVector<double>> &targetDyn);


void FindNQNCdeQ(QTableWidget  *reseauDescr,quint16 &NQ, quint16 NC[],
                 quint16 NB[], quint16 &NCMax, quint16 &NCNBMax);


void networkConstructorDyn(QVector<QVector<neurone*>> &tabBiaisDyn,
                           QVector<QVector<neurone*>> &tabCellDyn,
                           quint16 NQ, quint16 NC[], quint16 NB[], bool &Maintest);

void cellLinkConstructorDyn(QTableWidget *reseauDescr,
                            QVector<QVector<neurone*>> &tabBiaisDyn, QVector<QVector<neurone*>> &tabCellDyn);

void networkDestructorDyn(QVector<QVector<neurone*>> &tabBiaisDyn,
                          QVector<QVector<neurone*>> &tabCellDyn,
                          quint16 NQ, quint16 NC[], quint16 NB[]);


#endif // DIVERSNEURONESSUB_H
