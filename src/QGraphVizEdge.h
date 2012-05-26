/*!
   \file
   \author Dane Gardner <dane.gardner@gmail.com>
   \version

   \section LICENSE
   This file is part of the Parallel Tools GUI Framework (PTGF)
   Copyright (C) 2010-2011 Argo Navis Technologies, LLC

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by the
   Free Software Foundation; either version 2.1 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
   for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

   \section DESCRIPTION

 */

#ifndef QGRAPHVIZEDGE_H
#define QGRAPHVIZEDGE_H

#include <QtCore>
#include <QtGui>

#include <graphviz/types.h>

class QGraphVizLabel;
class QGraphViz;

class QGraphVizEdge : public QGraphicsItemGroup
{
public:
    explicit QGraphVizEdge(edge_t *edge, QGraphViz *graphViz, QGraphicsItem * parent = 0);

protected:
    void updateDimensions();

private:
    edge_t *m_GraphVizEdge;
    QGraphViz *m_GraphViz;

    QGraphicsPathItem *m_PathItem;

    QGraphVizLabel *m_Label;
    QGraphVizLabel *m_LabelHead;
    QGraphVizLabel *m_LabelTail;

};

#endif // QGRAPHVIZEDGE_H
