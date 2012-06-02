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
#include "QGraphViz.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
static double ThirdPi = Pi / 3;
static const qreal ArrowSize = 10;

QGraphVizEdge::QGraphVizEdge(edge_t *edge, QGraphViz *graphViz, QGraphicsItem * parent) :
    QGraphicsItem(parent),
    m_GraphVizEdge(edge),
    m_GraphViz(graphViz)
{
}

int QGraphVizEdge::type() const
{
    return UserType + 2;
}

int QGraphVizEdge::getGVID()
{
    return m_GraphVizEdge->id;
}

QRectF QGraphVizEdge::boundingRect() const
{
    QRectF rect;

    QPointF position = m_GraphViz->transformPoint(m_GraphVizEdge->u.spl->list[0].list[0]);

#if 0
    QPointF lowerLeft = QPointF(m_GraphViz->transformPoint(m_GraphVizEdge->u.spl->bb.LL)) - position;
    QPointF upperRight = QPointF(m_GraphViz->transformPoint(m_GraphVizEdge->u.spl->bb.UR)) - position;
    rect = QRectF(lowerLeft.x(), upperRight.y(), upperRight.x(), lowerLeft.y());
#else
    //! \note For some reason, the bounding box is not defined when we need it; find it manually instead

    QList<QPointF> points;
    for(int i = 0; i < m_GraphVizEdge->u.spl->size; ++i) {
        points.append(m_GraphViz->transformPoint(m_GraphVizEdge->u.spl->list[i].ep) - position);
        for(int j = 0; j < m_GraphVizEdge->u.spl->list[i].size; ++j) {
            points.append(m_GraphViz->transformPoint(m_GraphVizEdge->u.spl->list[i].list[j]) - position);
        }
    }

    foreach(QPointF point, points) {
        qreal left = qMin(rect.left(), point.x());
        rect.setLeft(left);
        qreal right = qMax(rect.right(), point.x());
        rect.setRight(right);
        qreal top = qMin(rect.top(), point.y());
        rect.setTop(top);
        qreal bottom = qMax(rect.bottom(), point.y());
        rect.setBottom(bottom);
    }
#endif

    return rect;
}

void QGraphVizEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

    Agedgeinfo_t &edgeInfo = m_GraphVizEdge->u;

    if(lod >= 0.01) {
        if(edgeInfo.spl) {
            QPen pen(Qt::black);
            pen.setWidthF(edgeInfo.weight * 1.5);
            painter->setPen(pen);

            QPainterPath path;
            for(int i=0; i < edgeInfo.spl->size; ++i) {
                bezier bez = edgeInfo.spl->list[i];

                /*! \note Hell if I know why they don't use 'sp' as the start point, (you know, 'cause they named it that)
                          and instead stick the damn thing in the first point of the bezier array (each bezier is 3 points,
                          so bez.size always equals (1 + (3 * size)) for your mental checksum) */

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

                // Add an arrowhead
                if(lod >= 0.25) {
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
                }

                if(startPoint != pos()) {
                    setPos(startPoint);
                    prepareGeometryChange();
                }
            }

            painter->drawPath(path);
        }
    }

    // Draw labels
    if(lod >= 0.35) {
        QList<textlabel_t*> labels;
        if(edgeInfo.label) labels.append(edgeInfo.label);
        if(edgeInfo.head_label) labels.append(edgeInfo.head_label);
        if(edgeInfo.tail_label) labels.append(edgeInfo.tail_label);
        if(edgeInfo.xlabel) labels.append(edgeInfo.xlabel);

        foreach(textlabel_t *label, labels) {
            QPointF labelPosition = m_GraphViz->transformPoint(label->pos) - pos();

            // Set the font parameters
            QFont font = painter->font();
            font.setStyleHint(QFont::Serif);
            font.setStyleStrategy((QFont::StyleStrategy)(QFont::PreferAntialias | QFont::PreferQuality));
#if 0
            font.setFamily(label->fontname);    // Usually "Times New Roman"
#else
            //! \note This was set manually in the STAT GUI, so I'm doing the same here
            font.setFamily("sans-serif");
#endif
            font.setPointSizeF(label->fontsize * .75);

            //HACK: Prerender to get bounding box for centering on position
            QPainterPath path;
            path.addText(0, 0, font, label->text);
            labelPosition -= path.boundingRect().bottomRight() / 2;

            QColor color(label->fontcolor);

            painter->setFont(font);
            painter->setPen(color);
            painter->drawText(labelPosition, label->text);
        }
    }
}
