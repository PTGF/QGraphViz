#-------------------------------------------------
#
# Project created by QtCreator 2012-05-17T14:42:38
#
#-------------------------------------------------

QT       += core gui

TARGET = DirectedGraphView
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    GraphScene.cpp \
    QGraphViz.cpp \
    QGraphVizAttributes.cpp \
    QGraphVizNode.cpp \
    QGraphVizEdge.cpp \
    QGraphVizLabel.cpp

HEADERS  += MainWindow.h \
    GraphScene.h \
    QGraphViz.h \
    QGraphVizAttributes.h \
    QGraphVizNode.h \
    QGraphVizEdge.h \
    QGraphVizLabel.h

FORMS    += MainWindow.ui

LIBS += -L/usr/lib64 -lgraph -lcdt -lpathplan -lxdot -lgvc
