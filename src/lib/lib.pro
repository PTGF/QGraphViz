# This file is part of the Parallel Tools GUI Framework (PTGF)
# Copyright (C) 2010-2011 Argo Navis Technologies, LLC
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

include(../QGraphViz.pri)

QT       += core gui

TEMPLATE = lib
DESTDIR = $$LIBRARY_PATH

TARGET = $${APPLICATION_TARGET}$${LIB_POSTFIX}

win32:target.path = /
else:target.path  = /lib
INSTALLS         += target

HEADERS  += QGraphVizNode.h \
            QGraphVizEdge.h \
            QGraphVizView.h \
            QGraphVizPIP.h \
            QGraphVizZoomWidget.h \
            QGraphVizScene.h \
            QGraphVizLibrary.h

SOURCES +=  QGraphVizNode.cpp \
            QGraphVizEdge.cpp \
            QGraphVizView.cpp \
            QGraphVizPIP.cpp \
            QGraphVizZoomWidget.cpp \
            QGraphVizScene.cpp

LIBS += -lgraph -lcdt -lpathplan -lxdot -lgvc

DEFINES          += QGRAPHVIZ_LIBRARY

#debug:DEFINES    += QGRAPHVIZVIEW_DEBUG
#debug:DEFINES    += QGRAPHVIZSCENE_DEBUG
#debug:DEFINES    += QGRAPHVIZNODE_DEBUG
#debug:DEFINES    += QGRAPHVIZEDGE_DEBUG

qGraphVizHeaders.path = /include
qGraphVizHeaders.files = QGraphVizLibrary.h QGraphVizView.h QGraphVizScene.h QGraphVizNode.h QGraphVizEdge.h
INSTALLS += qGraphVizHeaders
