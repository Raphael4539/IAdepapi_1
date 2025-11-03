#include "param.h"

using namespace std;


param::param(QString path, QString fileName, bool &ok){   //,  struct m_convParametre convParam[]){

 //   convParametre m_convParam[3];
//    bool ok = true;
    m_fileName = fileName; m_fichier = m_fieName; // *** 3/11/25
    m_path = path;

    QFile fichierMNIST(fileName);
    if(fichierMNIST.isOpen())fichierMNIST.close();
    if (fichierMNIST.open(QIODevice::ReadOnly | QIODevice::Text) and fichierMNIST.size() > 10){
        QTextStream monFluxMNIST(&fichierMNIST);
        QString entree ="";
        int i=0;
        qDebug() << fichierMNIST.size() ;
        while (!monFluxMNIST.atEnd()){
            monFluxMNIST >> entree;
            if (entree == "nberOfImages=" && i==0){  // *** 20/5/25
                monFluxMNIST >> m_nberOfImages;
                ok = true;
            }
            if(!ok)break;
            monFluxMNIST >> entree;
            if(entree == "nberOfConvolutions=") monFluxMNIST >> m_nberOfConvolutions;

            monFluxMNIST >> entree;
            if(entree == "nberOfPixels=") monFluxMNIST >> m_nberOfPixels;

            for(int nconv = 0;nconv < m_nberOfConvolutions;nconv++){
                monFluxMNIST >> entree;
                if (entree == "featurePadding="){
                    monFluxMNIST >> entree;
                    if(entree == "true" ) {convParam[nconv].featurePadding = true;}
                    else {convParam[nconv].featurePadding = false;}
                }
                monFluxMNIST >> entree;
                if(entree == "stride=") monFluxMNIST >> convParam[nconv].stride;

                monFluxMNIST >> entree;
                if(entree == "nberOfFilters=")
                    monFluxMNIST >> convParam[nconv].nberOfFilters;

                monFluxMNIST >> entree;
                if(entree == "filterType=") monFluxMNIST >> convParam[nconv].filterType;

                monFluxMNIST >> entree;
                if(entree == "maxPool="){
                    monFluxMNIST >> entree;
                    if(entree == "true") {convParam[nconv].maxPool = true;}
                    else {convParam[nconv].maxPool = false;}
                }
                monFluxMNIST >> entree;
                if(entree == "RELU="){
                     monFluxMNIST >> entree;
                    if(entree == "true") {convParam[nconv].RELU = true;}
                    else {convParam[nconv].RELU = false;}
                }
                monFluxMNIST >> entree;
                if(entree == "stridePooling=") monFluxMNIST >>
                     convParam[nconv].stridePooling;

                monFluxMNIST >> entree;
                if(entree == "poolingPair="){
                    monFluxMNIST >> entree;
                    if(entree == "true" ) {convParam[nconv].poolingPair = true;}
                    else {convParam[nconv].poolingPair = false;}
                }

            } // end for
            i++;
        } // end while

        if(!ok)QMessageBox::warning(this, "Erreur fchier des paramètres",
                                 "des paramètres ne correspondent pas");
        fichierMNIST.close();
    }else{ QMessageBox::warning(this, "Erreur fchier des paramètres",
                             "des paramètres ne correspondent pas");
        m_nberOfConvolutions = 0;
        m_nberOfImages = 0;
        m_nberOfPixels = 28;
        ok = false;
    }

     // end open file


    for(int nconv = m_nberOfConvolutions; nconv < 3; nconv++){
        convParam[nconv].featurePadding = false;
        convParam[nconv].stride = 0;
        convParam[nconv].nberOfFilters = 0;
        convParam[nconv].filterType = 0;
        convParam[nconv].maxPool = false;
        convParam[nconv].RELU = false;
        convParam[nconv].stridePooling = 0;
        convParam[nconv].poolingPair = false;
    }


    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                     QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    grpConvParam = new QVBoxLayout;
        hboxNberOfConv   = new  QHBoxLayout;
            l_nberOfImages = new QLabel("Nombre d images: ");
            editNberOfImages = new QLineEdit();
            editNberOfImages->setFixedWidth(50);
            editNberOfImages->setText(QString::number(m_nberOfImages));
            editNberOfImages->setInputMask("00000");
            QValidator *validator = new QIntValidator(1, 60000, this);
            // the edit lineedit will only accept integers between 1 and 60'000
      //      editNberOfImages->setValidator(validator);
            if(editNberOfImages->text() > "60000") editNberOfImages->setText("60000");
            l_nberOfPixels = new QLabel(   "Nombre de Pixels: ");
            editNberOfPixels    = new QLineEdit();
            editNberOfPixels->setInputMask("0000");
            editNberOfPixels->setFixedWidth(40);
            editNberOfPixels->setText(QString::number(28));
            editNberOfPixels->setText(QString::number(m_nberOfPixels));
            editNberOfPixels->setReadOnly(true);
            l_nberOfConv = new QLabel(" nombre de convolutions: ");
            sp_nberOfConvolutions = new QSpinBox;
            sp_nberOfConvolutions->setFixedWidth(50);
            sp_nberOfConvolutions->setValue(m_nberOfConvolutions);
            sp_nberOfConvolutions->setMinimum(0);
            sp_nberOfConvolutions->setMaximum(3);
            hboxNberOfConv->addWidget(l_nberOfImages);
            hboxNberOfConv->addWidget(editNberOfImages);
            hboxNberOfConv->addWidget(l_nberOfPixels);
            hboxNberOfConv->addWidget(editNberOfPixels);
            hboxNberOfConv->addWidget(l_nberOfConv);
            hboxNberOfConv->addWidget(sp_nberOfConvolutions);
 //     editNberOfConv = new QLineEdit("Nombre de convolutions:");
 //       editNberOfConv->setText(QString::number(m_nberOfConvolutions));
        grpConvParam->addLayout(hboxNberOfConv);

    labels[0] = "    Padding(same):";
    labels[1] = "      Stride(pas):";
    labels[2] = "Nombre de filtres:";
    labels[3] = "   Type de filtre:";
    labels[4] = "         Max pool:";
    labels[5] = "             RELU:";
    labels[6] = "   Stride pooling:";
    labels[7] = "      pooling pair";

    int nconv = 0;

    grpParam_1 = new QGroupBox("Convolution 1");
        int l = 0;
        gridLayout[0] = new QGridLayout();
            l_padding_1 = new QLabel(labels[l]); l++;
            padding_1 = new QCheckBox();
            padding_1->setChecked(false);
            padding_1->setChecked(convParam[nconv].featurePadding);
            l_stride_1 = new QLabel(labels[l]); l++;
            stride_1 = new QSpinBox;
            stride_1->setValue(convParam[nconv].stride);
            stride_1->setRange(1,3);
            l_nberOfFilters_1 = new QLabel(labels[l]); l++;
            nberOfFilters_1 = new QSpinBox;
            nberOfFilters_1->setValue(convParam[nconv].nberOfFilters);
            l_filterType_1 = new QLabel(labels[l]); l++;
            filterType_1    = new QSpinBox;
            filterType_1->setRange(3,5);
            filterType_1->setSingleStep(2);
            filterType_1->setValue(3);
            filterType_1->setValue(convParam[nconv].filterType);
            l_maxPool_1 = new QLabel(labels[l]); l++;
            maxPool_1       = new QCheckBox;
            maxPool_1->setChecked(true);
            maxPool_1->setChecked(convParam[nconv].maxPool);
            l_RELU_1        = new QLabel(labels[l]); l++;
            RELU_1          = new QCheckBox;
            RELU_1->setChecked(true);
            RELU_1->setChecked(convParam[nconv].RELU);
            l_stridePooling_1   = new QLabel(labels[l]); l++;
            stridePooling_1     = new QSpinBox;
            stridePooling_1->setRange(2,4);
            stridePooling_1->setValue(2);
            stridePooling_1->setValue(convParam[nconv].stridePooling);
            l_pooling_1   = new QLabel(labels[l]);
            pooling_1           = new QCheckBox();
            pooling_1->setChecked(true);
            pooling_1->setChecked(convParam[nconv].poolingPair);
            gridLayout[0]->addWidget(l_padding_1,0,0);
            gridLayout[0]->addWidget(padding_1,0,1);
            gridLayout[0]->addWidget(l_stride_1,0,2);
            gridLayout[0]->addWidget(stride_1,0,3);
            gridLayout[0]->addWidget(l_nberOfFilters_1,0,4);
            gridLayout[0]->addWidget(nberOfFilters_1,0,5);
            gridLayout[0]->addWidget(l_filterType_1,1,0);
            gridLayout[0]->addWidget(filterType_1,1,1);
            gridLayout[0]->addWidget(l_maxPool_1,1,2);
            gridLayout[0]->addWidget(maxPool_1,1,3);
            gridLayout[0]->addWidget(l_RELU_1,1,4,Qt::AlignRight);
            gridLayout[0]->addWidget(RELU_1,1,5);
            gridLayout[0]->addWidget(l_stridePooling_1,2,0);
            gridLayout[0]->addWidget(stridePooling_1,2,1);
            gridLayout[0]->addWidget(l_pooling_1,2,2);
            gridLayout[0]->addWidget(pooling_1,2,3);



            gridLayout[0]->setAlignment(Qt::AlignCenter);
/*
            gridLayout[0]->setColumnStretch(1,10);
            gridLayout[0]->setColumnStretch(2,20);
            gridLayout[0]->setColumnStretch(3,10);
            gridLayout[0]->setColumnStretch(4,20);
            gridLayout[0]->setColumnStretch(5,10);
*/
 /*
            gridLayout[0]->setColumnMinimumWidth(0,40);
            gridLayout[0]->setColumnMinimumWidth(1,40);
            gridLayout[0]->setColumnMinimumWidth(2,40);
*/

        grpParam_1->setLayout(gridLayout[0]);
    grpConvParam->addWidget(grpParam_1);

        nconv++;

 //     if(nconv < nberOfConvolutions){
    grpParam_2 = new QGroupBox("Convolution 2");
            nconv = 1; l=0;
        gridLayout[1] = new QGridLayout();
            l_padding_2 = new QLabel(labels[l]); l++;
            padding_2 = new QCheckBox();
            padding_2->setChecked(false);
            padding_2->setChecked(convParam[nconv].featurePadding);
            l_stride_2 = new QLabel(labels[l]); l++;
            stride_2 = new QSpinBox;
            stride_2->setValue(convParam[nconv].stride);
            stride_2->setRange(1,3);
            l_nberOfFilters_2 = new QLabel(labels[l]); l++;
            nberOfFilters_2 = new QSpinBox;
            nberOfFilters_2->setValue(convParam[nconv].nberOfFilters);
            l_filterType_2 = new QLabel(labels[l]); l++;
            filterType_2    = new QSpinBox;
            filterType_2->setRange(3,5);
            filterType_2->setSingleStep(2);
            filterType_2->setValue(3);
            filterType_2->setValue(convParam[nconv].filterType);
            l_maxPool_2 = new QLabel(labels[l]); l++;
            maxPool_2       = new QCheckBox;
            maxPool_2->setChecked(true);
            maxPool_2->setChecked(convParam[nconv].maxPool);
            l_RELU_2        = new QLabel(labels[l]); l++;
            RELU_2          = new QCheckBox;
            RELU_2->setChecked(true);
            RELU_2->setChecked(convParam[nconv].RELU);
            l_stridePooling_2   = new QLabel(labels[l]); l++;
            stridePooling_2     = new QSpinBox;
            stridePooling_2->setRange(2,4);
            stridePooling_2->setValue(2);
            stridePooling_2->setValue(convParam[nconv].stridePooling);
            l_pooling_2   = new QLabel(labels[l]);
            pooling_2           = new QCheckBox();
            pooling_2->setChecked(true);
            pooling_2->setChecked(convParam[nconv].poolingPair);
            gridLayout[1]->addWidget(l_padding_2,0,0);
            gridLayout[1]->addWidget(padding_2,0,1);
            gridLayout[1]->addWidget(l_stride_2,0,2);
            gridLayout[1]->addWidget(stride_2,0,3);
            gridLayout[1]->addWidget(l_nberOfFilters_2,0,4);
            gridLayout[1]->addWidget(nberOfFilters_2,0,5);
            gridLayout[1]->addWidget(l_filterType_2,1,0);
            gridLayout[1]->addWidget(filterType_2,1,1);
            gridLayout[1]->addWidget(l_maxPool_2,1,2);
            gridLayout[1]->addWidget(maxPool_2,1,3);
            gridLayout[1]->addWidget(l_RELU_2,1,4,Qt::AlignRight);
            gridLayout[1]->addWidget(RELU_2,1,5);
            gridLayout[1]->addWidget(l_stridePooling_2,2,0);
            gridLayout[1]->addWidget(stridePooling_2,2,1);
            gridLayout[1]->addWidget(l_pooling_2,2,2);
            gridLayout[1]->addWidget(pooling_2,2,3);

            gridLayout[1]->setAlignment(Qt::AlignCenter);

 /*
            gridLayout[1]->setColumnStretch(0,10);
            gridLayout[1]->setColumnStretch(1,10);
            gridLayout[1]->setColumnStretch(2,10);
 */
/*
            gridLayout[1]->setColumnMinimumWidth(0,40);
            gridLayout[1]->setColumnMinimumWidth(1,40);
            gridLayout[1]->setColumnMinimumWidth(2,40);
*/
        grpParam_2->setLayout(gridLayout[1]);

    grpConvParam->addWidget(grpParam_2);
 //     }
        nconv++;

 //     if(nconv < nberOfConvolutions){
            grpParam_3 = new QGroupBox("Convolution 2");
            l = 0;
            gridLayout[2] = new QGridLayout();

            l_padding_3 = new QLabel(labels[l]); l++;
            padding_3 = new QCheckBox();
            padding_3->setChecked(false);
            padding_3->setChecked(convParam[nconv].featurePadding);
            l_stride_3 = new QLabel(labels[l]); l++;
            stride_3 = new QSpinBox;
            stride_3->setValue(convParam[nconv].stride);
            stride_3->setRange(1,3);
            l_nberOfFilters_3 = new QLabel(labels[l]); l++;
            nberOfFilters_3 = new QSpinBox;
            nberOfFilters_3->setValue(convParam[nconv].nberOfFilters);
            l_filterType_3 = new QLabel(labels[l]); l++;
            filterType_3    = new QSpinBox;
            filterType_3->setRange(3,5);
            filterType_3->setSingleStep(2);
            filterType_3->setValue(3);
            filterType_3->setValue(convParam[nconv].filterType);
            l_maxPool_3 = new QLabel(labels[l]); l++;
            maxPool_3       = new QCheckBox;
            maxPool_3->setChecked(true);
            maxPool_3->setChecked(convParam[nconv].maxPool);
            l_RELU_3        = new QLabel(labels[l]); l++;
            RELU_3          = new QCheckBox;
            RELU_3->setChecked(true);
            RELU_3->setChecked(convParam[nconv].RELU);
            l_stridePooling_3   = new QLabel(labels[l]); l++;
            stridePooling_3     = new QSpinBox;
            stridePooling_3->setRange(2,4);
            stridePooling_3->setValue(2);
            stridePooling_3->setValue(convParam[nconv].stridePooling);
            l_pooling_3   = new QLabel(labels[l]);
            pooling_3           = new QCheckBox();
            pooling_3->setChecked(true);
            pooling_3->setChecked(convParam[nconv].poolingPair);
            gridLayout[2]->addWidget(l_padding_3,0,0);
            gridLayout[2]->addWidget(padding_3,0,1);
            gridLayout[2]->addWidget(l_stride_3,0,2);
            gridLayout[2]->addWidget(stride_3,0,3);
            gridLayout[2]->addWidget(l_nberOfFilters_3,0,4);
            gridLayout[2]->addWidget(nberOfFilters_3,0,5);
            gridLayout[2]->addWidget(l_filterType_3,1,0);
            gridLayout[2]->addWidget(filterType_3,1,1);
            gridLayout[2]->addWidget(l_maxPool_3,1,2);
            gridLayout[2]->addWidget(maxPool_3,1,3);
            gridLayout[2]->addWidget(l_RELU_3,1,4,Qt::AlignRight);
            gridLayout[2]->addWidget(RELU_3,1,5);
            gridLayout[2]->addWidget(l_stridePooling_3,2,0);
            gridLayout[2]->addWidget(stridePooling_3,2,1);
            gridLayout[2]->addWidget(l_pooling_3,2,2);
            gridLayout[2]->addWidget(pooling_3,2,3);

            gridLayout[2]->setAlignment(Qt::AlignCenter);

 /*
            gridLayout[2]->setColumnStretch(0,10);
            gridLayout[2]->setColumnStretch(1,10);
            gridLayout[2]->setColumnStretch(2,10);
 */
 /*
            gridLayout[2]->setColumnMinimumWidth(0,40);
            gridLayout[2]->setColumnMinimumWidth(1,40);
            gridLayout[2]->setColumnMinimumWidth(2,40);
 */
 //           grpParam_3->setMaximumWidth(600);
        grpParam_3->setLayout(gridLayout[2]);
 //           if(sp_nberOfConvolutions->value() < 3)grpParam_3->setVisible(false);
    grpConvParam->addWidget(grpParam_3);


    grpConvParam->addWidget(buttonBox);
    grpConvParam->setGeometry(QRect(50,50,200,299));
    setLayout(grpConvParam);


    setWindowTitle("PARAMETRES DE CONVOLUTIONS");
//    exec();

}


void param::accept(){
    bool ok = true;

 //   m_fileName = m_fileName.mid(m_path.size(), m_fileName.size() - m_path.size()-4)+".txt" +
 //                QDate::currentDate().toString("yy-MMM-dd") +"_" + QTime::currentTime().toString("hh mm ss") +".txt"; // + QTime::currentTime().toString()
    QString m_fichier = QFileDialog::getSaveFileName(this, "Enregistrer le fichier",
        m_fileName, ("*.txt"));  // ****1/11/25
 qDebug() <<  editNberOfImages->text().toULong(&ok,10);
    QFile f(m_fichier);              // ****1/11/25
    if(f.open(QIODevice::WriteOnly)){
        QTextStream fluxMNIST(&f);           //(&m_fileName);

        fluxMNIST << "\n" <<  "  nberOfImages= " << editNberOfImages->text().toULong(&ok,10);
        fluxMNIST << "     nberOfConvolutions= " << sp_nberOfConvolutions->value() <<  "\n" ;
        fluxMNIST  << "  nberOfPixels= " <<  editNberOfPixels->text().toUInt(&ok,10) <<  "\n\n\n";

        fluxMNIST << "   featurePadding= " ;
        if(padding_1->isChecked()){ fluxMNIST << " true";} else {fluxMNIST << " false";}
        fluxMNIST << "   stride=  " << stride_2->value();
        fluxMNIST << "       nberOfFilters=  " << nberOfFilters_1->value() << "\n";
        fluxMNIST << "   filterType=  " << filterType_1->value();
        fluxMNIST << "           maxPool= ";
        if(maxPool_1->isChecked()){ fluxMNIST << " true";} else {fluxMNIST << " false";}
        fluxMNIST << "   RELU= ";
        if(RELU_1->isChecked()) {fluxMNIST << " true";} else {fluxMNIST << " false" ;}
        fluxMNIST << "\n";
        fluxMNIST << "   stridePooling= " << stridePooling_1->value();
        fluxMNIST << "         poolingPair= ";
        if(pooling_1->isChecked()) {fluxMNIST << " true";} else {fluxMNIST << " false";}
        fluxMNIST << "\n\n\n";

        if(sp_nberOfConvolutions->value() > 1){
            fluxMNIST << "   featurePadding= " ;
            if(padding_2->isChecked()) {fluxMNIST << " true";} else {fluxMNIST << " false";}
            fluxMNIST << "   stride=  " << stride_2->value();
            fluxMNIST << "       nberOfFilters=  " << nberOfFilters_2->value() << "\n";
            fluxMNIST << "   filterType=  " << filterType_2->value();
            fluxMNIST << "           maxPool= ";
            if(maxPool_2->isChecked()) {fluxMNIST << " true";} else {fluxMNIST << " false";}
            fluxMNIST << "   RELU= ";
            if(RELU_2->isChecked()) {fluxMNIST << " true";} else {fluxMNIST << " false" ;}
            fluxMNIST << "\n";
            fluxMNIST << "   stridePooling= " << stridePooling_2->value();
            fluxMNIST << "         poolingPair= ";
            if(pooling_2->isChecked()) {fluxMNIST << " true";} else {fluxMNIST << " false";}
            fluxMNIST << "\n\n\n";
            }
        if(sp_nberOfConvolutions->value() > 2){
                fluxMNIST << "   featurePadding= " ;
            if(padding_3->isChecked()) {fluxMNIST << " true";} else {fluxMNIST << " false";}
                fluxMNIST << "   stride=  " << stride_3->value();
                fluxMNIST << "       nberOfFilters=  " << nberOfFilters_3->value() << "\n";
                fluxMNIST << "   filterType=  " << filterType_3->value();
                fluxMNIST << "           maxPool= ";
                if(maxPool_3->isChecked()) {fluxMNIST << " true";} else {fluxMNIST << " false";}
                fluxMNIST << "   RELU= ";
                if(RELU_3->isChecked()) {fluxMNIST << " true";} else {fluxMNIST << " false" ;}
                fluxMNIST << "\n";
                fluxMNIST << "   stridePooling= " << stridePooling_3->value();
                fluxMNIST << "         poolingPair= ";
                if(pooling_3->isChecked()) {fluxMNIST << " true";} else {fluxMNIST << " false";}
                fluxMNIST << "\n\n\n";
        }
        f.close();
    }
    reject();
}

/*
void param::reject(){
    reject();
}

*/
param::~param(){};

QString param::getNewFileName() const {return  m_fichier; } // ****1/11/25


