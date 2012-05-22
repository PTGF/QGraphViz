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
    m_PathItem = new QGraphicsPathItem(this);

    QPainterPath path;
    for(int i=0; i < edge->u.spl->size; ++i) {
        bezier bez = edge->u.spl->list[i];

        if(bez.size == 2) {                                            // Draw a line
            QPointF point0 = QPoint(bez.list[0].x, -bez.list[0].y);
            QPointF point1 = QPoint(bez.list[1].x, -bez.list[1].y) - point0;

            path.lineTo(point1);

            point0.setY(point0.y() + path.boundingRect().height()/2);  //TODO: Figure out the proper shift
            m_PathItem->setPos(point0);

        } else if(bez.size == 4) {                                     // Draw a bezier curve
            QPointF point0 = QPoint(bez.list[0].x, -bez.list[0].y);
            QPointF point1 = QPoint(bez.list[1].x, -bez.list[1].y) - point0;
            QPointF point2 = QPoint(bez.list[2].x, -bez.list[2].y) - point0;
            QPointF point3 = QPoint(bez.list[3].x, -bez.list[3].y) - point0;

            path.cubicTo(point1, point2, point3);

            point0.setY(point0.y() + path.boundingRect().height()/2);  //TODO: Figure out the proper shift
            m_PathItem->setPos(point0);

#ifdef QT_DEBUG
        } else {
            qDebug() << "Unknown line type! Contains " << bez.size << " points";
#endif
        }
    }
    m_PathItem->setPath(path);

    if(edge->u.label) {
        m_Label = new QGraphVizLabel(edge->u.label, this);
    }
    if(edge->u.head_label) {
        m_LabelHead = new QGraphVizLabel(edge->u.head_label, this);
    }
    if(edge->u.tail_label) {
        m_LabelTail = new QGraphVizLabel(edge->u.tail_label, this);
    }
}

void QGraphVizEdge::updateDimensions()
{
}
