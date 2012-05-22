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

#include "QGraphVizEdge.h"

#include "QGraphVizLabel.h"

#ifdef QT_DEBUG
#include <QtDebug>
#endif

QGraphVizEdge::QGraphVizEdge(edge_t *edge, QGraphicsItem * parent) :
    QGraphicsItemGroup(parent),
    m_PathItem(NULL),
    m_Label(NULL),
    m_LabelHead(NULL),
    m_LabelTail(NULL)
{
    Agedgeinfo_t &edgeInfo = edge->u;

    m_PathItem = new QGraphicsPathItem(this);

    QPainterPath path;
    for(int i=0; i < edgeInfo.spl->size; ++i) {
        bezier bez = edgeInfo.spl->list[i];
        if(bez.size == 4) {
            // The group will be placed at the starting point (point0)
            QPointF point0 = QPoint(bez.list[0].x, -bez.list[0].y);

            // Everything has to be translated from within the group
            QPointF point1 = QPoint(bez.list[1].x, -bez.list[1].y) - point0;
            QPointF point2 = QPoint(bez.list[2].x, -bez.list[2].y) - point0;
            QPointF point3 = QPoint(bez.list[3].x, -bez.list[3].y) - point0;
            path.cubicTo(point1, point2, point3);

            //TODO: Add an arrowhead
            path.addEllipse(point3, 5, 5);

            point0.setY(point0.y() + path.boundingRect().height()/2);  //TODO: Figure out the proper shift
            m_PathItem->setPos(point0);

#ifdef QT_DEBUG
        } else {
            qDebug() << "Unknown bezier size! Contains " << bez.size << " points";
#endif
        }
    }
    m_PathItem->setPath(path);

    if(edgeInfo.label) {
        m_Label = new QGraphVizLabel(edgeInfo.label, this);
    }
    if(edgeInfo.head_label) {
        m_LabelHead = new QGraphVizLabel(edgeInfo.head_label, this);
    }
    if(edgeInfo.tail_label) {
        m_LabelTail = new QGraphVizLabel(edgeInfo.tail_label, this);
    }
    if(edgeInfo.xlabel) {
        setToolTip(edgeInfo.xlabel->text);
    }
}

void QGraphVizEdge::updateDimensions()
{
}
