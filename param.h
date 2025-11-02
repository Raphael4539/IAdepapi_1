#ifndef PARAM_H
#define PARAM_H

#include <QWidget>
#include <QString>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include<QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QDateTime>
#include <QTime>


class param  : public QDialog{
    Q_OBJECT

public:
    param(QString path, QString fileName, bool &ok);  //  ,  struct m_convParametre convParam[]);
    QString getNewFileName() const; // ****1/11/25

    ~param();


private slots:
 //    void enregistrer();
    void accept();
 //   void reject();

private:
    QSpinBox   *sp_nberOfConvolutions;
    quint8 nberOfConvolutions;

    struct m_convParametre {
        bool featurePadding;  int stride; int nberOfFilters;
        int filterType;  bool maxPool; bool RELU;
        int stridePooling; bool poolingPair;
    } convParam[3];

    QHBoxLayout *hboxNberOfConv;
        QLabel      *l_nberOfImages;
        QLineEdit   *editNberOfImages;
        QLabel      *l_nberOfPixels;
        QLineEdit   *editNberOfPixels;
        QLabel      *l_nberOfConv;
        QSpinBox    *nberOfConv;
        QLabel      *image;
        QMenuBar    *menuBar;

    QString labels[8];

 // DESCRIPTION
    QLabel   *l_padding_1, *l_padding_2, *l_padding_3;
    QLabel   *l_stride_1,  *l_stride_2,   *l_stride_3;
    QLabel   *l_nberOfFilters_1, *l_nberOfFilters_2, *l_nberOfFilters_3;
    QLabel   *l_filterType_1,  *l_filterType_2, *l_filterType_3;
    QLabel   *l_maxPool_1,  *l_maxPool_2, *l_maxPool_3;
    QLabel   *l_RELU_1, *l_RELU_2, *l_RELU_3;
    QLabel   *l_stridePooling_1, *l_stridePooling_2, *l_stridePooling_3;
    QLabel   *l_pooling_1, *l_pooling_2, *l_pooling_3;

    // conv 1
    QCheckBox   *padding_1;
    QSpinBox    *stride_1;
    QSpinBox    *nberOfFilters_1;
    QSpinBox    *filterType_1;
    QCheckBox   *maxPool_1;
    QCheckBox   *RELU_1;
    QSpinBox    *stridePooling_1;
    QCheckBox   *pooling_1;

    // conv 2
    QCheckBox   *padding_2;
    QSpinBox    *stride_2;
    QSpinBox    *nberOfFilters_2;
    QSpinBox    *filterType_2;
    QCheckBox   *maxPool_2;
    QCheckBox   *RELU_2;
    QSpinBox    *stridePooling_2;
    QCheckBox   *pooling_2;

    // conv 3
    QCheckBox   *padding_3;
    QSpinBox    *stride_3;
    QSpinBox    *nberOfFilters_3;
    QSpinBox    *filterType_3;
    QCheckBox   *maxPool_3;
    QCheckBox   *RELU_3;
    QSpinBox    *stridePooling_3;
    QCheckBox   *pooling_3;

    QVBoxLayout *grpConvParam;

    // Labels

    QGroupBox   *grpParam_1;
    QGroupBox   *grpParam_2;
    QGroupBox   *grpParam_3;

    struct strLayout{
        QLabel l_padding; QCheckBox featurePadding;
        QLabel l_stride; QSpinBox stride;
        QLabel l_nberOfFilters; QSpinBox nberOfFilters;
        QLabel l_filterType; QSpinBox filterType;
        QLabel l_maxPool; QCheckBox maxPool;
        QLabel l_RELU; QCheckBox RELU;
        QLabel l_stridePooling; QSpinBox stridePooling;
        QLabel l_pooling; QCheckBox pooling;
    } layout[3];
    QGridLayout *gridLayout[3];;


    QMenu *fileMenu;
    QDialogButtonBox *buttonBox;
    QAction *exitAction;
    quint32 m_nberOfImages;
    quint16 m_nberOfPixels;
    int m_nberOfConvolutions;
    QString m_fileName;
    QString m_path;
    QString m_fichier;  // ****1/11/25
};
#endif // PARAM_H
