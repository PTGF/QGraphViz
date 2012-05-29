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
    QGraphVizNode.cpp \
    QGraphVizEdge.cpp

HEADERS  += MainWindow.h \
    QGraphViz.h \
    QGraphVizNode.h \
    QGraphVizEdge.h

FORMS    += MainWindow.ui

LIBS += -L/usr/lib64 -lgraph -lcdt -lpathplan -lxdot -lgvc
