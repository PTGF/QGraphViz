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
#include "QGraphVizScene.h"

#ifdef QT_DEBUG
#include <QtDebug>
#endif

QGraphVizEdge::QGraphVizEdge(edge_t *edge, QGraphVizScene *scene, QGraphicsItem * parent) :
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
            QPointF startPoint = scene->transformPoint(bez.list[0].x, bez.list[0].y);
            QPointF point1 = scene->transformPoint(bez.list[1].x, bez.list[1].y) - startPoint;
            QPointF point2 = scene->transformPoint(bez.list[2].x, bez.list[2].y) - startPoint;
            QPointF point3 = scene->transformPoint(bez.list[3].x, bez.list[3].y) - startPoint;
            QPointF endPoint = scene->transformPoint(bez.ep.x, bez.ep.y) - startPoint;

            //TEST: This needs to be tested with a larger dot file with bigger beziers; I'm not certain this is what they are intending with five points
            //path.moveTo(0.0,0.0);
            path.lineTo(point1);            // Line between start and point 1
            path.quadTo(point2, point3);    // Bezier between points 1, 2, and 3
            path.lineTo(endPoint);          // Line between point 3 and end
            path.closeSubpath();

            // Add an arrowhead
            QLineF line(endPoint, point3);
            if(!qFuzzyCompare(line.length(), qreal(0.0))) {  // Taken from Qt4 edge example
                static const double Pi = 3.14159265358979323846264338327950288419717;
                static double TwoPi = 2.0 * Pi;
                static double ThirdPi = Pi / 3;
                static const qreal arrowSize = 10;

                qreal angle = acos(line.dx() / line.length());
                if(line.dy() >= 0) {
                    angle = TwoPi - angle;
                }

                QPointF arrowP1 = QPointF(sin(angle + ThirdPi) * arrowSize, cos(angle + ThirdPi) * arrowSize);
                path.moveTo(endPoint);
                path.lineTo(arrowP1 + endPoint);
                path.closeSubpath();

                QPointF arrowP2 = QPointF(sin(angle + Pi - ThirdPi) * arrowSize, cos(angle + Pi - ThirdPi) * arrowSize);
                path.moveTo(endPoint);
                path.lineTo(arrowP2 + endPoint);
                path.closeSubpath();
            }

            m_PathItem->setPos(startPoint);

#ifdef QT_DEBUG
        } else {
            qDebug() << "Unknown bezier size! Contains " << bez.size << " points";
#endif
        }
    }
    QPen pen(Qt::black);
    pen.setWidthF(edgeInfo.weight * 1.5);
    m_PathItem->setPen(pen);
    m_PathItem->setPath(path);

    if(edgeInfo.label) {
        m_Label = new QGraphVizLabel(edgeInfo.label, scene, this);
    }
    if(edgeInfo.head_label) {
        m_LabelHead = new QGraphVizLabel(edgeInfo.head_label, scene, this);
    }
    if(edgeInfo.tail_label) {
        m_LabelTail = new QGraphVizLabel(edgeInfo.tail_label, scene, this);
    }
    if(edgeInfo.xlabel) {
        setToolTip(edgeInfo.xlabel->text);
    }
}

void QGraphVizEdge::updateDimensions()
{
}
