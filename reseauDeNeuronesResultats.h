// reseauDeNeuronesResultats.h  Bernard Ribordy
//    du 2024_06_03
// ************************************************

#ifndef RESEAUDENEURONESRESULTATS_H
#define RESEAUDENEURONESRESULTATS_H


#include <QtWidgets>


class reseauDeNeuronesResultats : public QDialog
{
public:
    reseauDeNeuronesResultats(QString &resultats, QWidget *parent);

private:
    QTextEdit       *mesResultats;
    QPushButton     *fermer;

};

#endif // RESEAUDENEURONESRESULTATS_H

