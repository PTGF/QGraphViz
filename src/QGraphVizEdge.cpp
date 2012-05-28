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
#include "QGraphViz.h"

#ifdef QT_DEBUG
#include <QtDebug>
#endif

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
static double ThirdPi = Pi / 3;
static const qreal ArrowSize = 10;

QGraphVizEdge::QGraphVizEdge(edge_t *edge, QGraphViz *graphViz, QGraphicsItem * parent) :
    QGraphicsItem(parent, graphViz),
    m_GraphVizEdge(edge),
    m_GraphViz(graphViz),
    m_PathItem(new QGraphicsPathItem(this)),
    m_Label(new QGraphVizLabel(m_GraphViz, this)),
    m_LabelHead(new QGraphVizLabel(m_GraphViz, this)),
    m_LabelTail(new QGraphVizLabel(m_GraphViz, this))
{
    preRender();
}

int QGraphVizEdge::type() const
{
    return UserType + 2;
}

int QGraphVizEdge::getGVID()
{
    return m_GraphVizEdge->id;
}


void QGraphVizEdge::updateDimensions()
{
    Agedgeinfo_t &edgeInfo = m_GraphVizEdge->u;

    if(edgeInfo.spl) {
        QPainterPath path;
        for(int i=0; i < edgeInfo.spl->size; ++i) {
            bezier bez = edgeInfo.spl->list[i];

            /*! \note Hell if I know why they don't use 'sp' as the start point, and instead stick the damn thing in the
                      first point of the bezier array (each bezier is 3 points) */

            QPointF startPoint = m_GraphViz->transformPoint(bez.list[0]);
            QPointF endPoint = m_GraphViz->transformPoint(bez.ep) - startPoint;

            QPointF point1, point2, point3;
            for(int j=1; j < bez.size; ++j) {
                point1 = m_GraphViz->transformPoint(bez.list[j]) - startPoint; ++j;
                point2 = m_GraphViz->transformPoint(bez.list[j]) - startPoint; ++j;
                point3 = m_GraphViz->transformPoint(bez.list[j]) - startPoint;
                path.cubicTo(point1, point2, point3);
            }

            path.lineTo(endPoint);
    //        path.closeSubpath();

            // Add an arrowhead
            QLineF line(endPoint, point3);
            if(!qFuzzyCompare(line.length(), qreal(0.0))) {  // Taken from Qt4 edge example
                qreal angle = acos(line.dx() / line.length());
                if(line.dy() >= 0) {
                    angle = TwoPi - angle;
                }

                QPointF arrowP1 = QPointF(sin(angle + ThirdPi) * ArrowSize, cos(angle + ThirdPi) * ArrowSize);
                path.moveTo(endPoint);
                path.lineTo(arrowP1 + endPoint);

                QPointF arrowP2 = QPointF(sin(angle + Pi - ThirdPi) * ArrowSize, cos(angle + Pi - ThirdPi) * ArrowSize);
                path.moveTo(endPoint);
                path.lineTo(arrowP2 + endPoint);
            }

            m_PathItem->setPos(startPoint);
        }
        QPen pen(Qt::black);
        pen.setWidthF(edgeInfo.weight * 1.5);
        m_PathItem->setPen(pen);
        m_PathItem->setPath(path);
    }

    // Update label objects
    m_Label->setGraphVizLabel(edgeInfo.label);
    m_LabelHead->setGraphVizLabel(edgeInfo.head_label);
    m_LabelTail->setGraphVizLabel(edgeInfo.tail_label);

    if(edgeInfo.xlabel) {
        setToolTip(edgeInfo.xlabel->text);
    } else {
        setToolTip(QString());
    }
}

void QGraphVizEdge::preRender()
{
    // See if the object has changed from the last pre-render
    static QByteArray lastMD5 = QGraphViz::getHash(m_GraphVizEdge);
    QByteArray currentMD5 = QGraphViz::getHash(m_GraphVizEdge);
    if(lastMD5 == currentMD5) {
        return;
    }
    lastMD5 = currentMD5;



}

QRectF QGraphVizEdge::boundingRect() const
{
    return QRectF();
}


void QGraphVizEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}
