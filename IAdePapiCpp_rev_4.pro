QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    convolutionDyn.cpp \
    diversNeuronesSub.cpp \
    lectureMNISTDyn.cpp \
    main.cpp \
    neurones.cpp \
    param.cpp \
    pooling.cpp \
    reseauDeNeurones.cpp \
    reseauDeNeuronesGraph.cpp \
    reseauDeNeuronesResultats.cpp \
    scene.cpp \
    vue.cpp \
    widget.cpp

HEADERS += \
    convolutionDyn.h \
    diversNeuronesSub.h \
    lectureMNISTDyn.h \
    neurones.h \
    param.h \
    pooling.h \
    reseauDeNeurones.h \
    reseauDeNeuronesGraph.h \
    reseauDeNeuronesResultats.h \
    reseauDeNeurones_variablesConstantes.h \
    scene.h \
    vue.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
