// reseauDeNeuronesResultats
// du 2024_06_03  Bernard Ribordy
// ************************************************

#include "reseauDeNeuronesResultats.h"

reseauDeNeuronesResultats::reseauDeNeuronesResultats(QString &resultats,
    QWidget *parent = nullptr) : QDialog(parent){
    mesResultats = new QTextEdit("ceci est un texte");
    mesResultats->setPlainText(resultats);
    mesResultats->setReadOnly(true);
    mesResultats->setFont(QFont("Courier"));
    mesResultats->setLineWrapMode(QTextEdit::NoWrap);

    fermer = new QPushButton("Fermer");

    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addWidget(mesResultats);
    layoutPrincipal->addWidget(fermer);
    resize(800, 600);
    setLayout(layoutPrincipal);

    connect(fermer, SIGNAL(clicked()), this, SLOT(accept()));
}
