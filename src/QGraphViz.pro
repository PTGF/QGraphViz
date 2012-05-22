#-------------------------------------------------
#
# Project created by QtCreator 2012-05-17T14:42:38
#
#-------------------------------------------------

QT       += core gui

TARGET = QGraphViz
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    QGraphViz.cpp \
    QGraphVizAttributes.cpp \
    QGraphVizNode.cpp \
    QGraphVizEdge.cpp \
    QGraphVizLabel.cpp \
    QGraphVizScene.cpp

HEADERS  += MainWindow.h \
    QGraphViz.h \
    QGraphVizAttributes.h \
    QGraphVizNode.h \
    QGraphVizEdge.h \
    QGraphVizLabel.h \
    QGraphVizScene.h

FORMS    += MainWindow.ui

LIBS += -L/usr/lib64 -lgraph -lcdt -lpathplan -lxdot -lgvc
