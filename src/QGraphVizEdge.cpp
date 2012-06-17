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
#include "QGraphVizScene.h"
#include "QGraphVizNode.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
static double ThirdPi = Pi / 3;
static const qreal ArrowSize = 10;

QGraphVizEdge::QGraphVizEdge(edge_t *edge, QGraphVizScene *graphViz, QGraphicsItem * parent) :
    QGraphicsItem(parent),
    m_GraphVizEdge(edge),
    m_GraphViz(graphViz),
    m_Head(NULL),
    m_Tail(NULL)
{
    updateGeometry();
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
    return m_BoundingRect;
}

void QGraphVizEdge::updateGeometry()
{
    QPointF position = m_GraphViz->transformPoint(m_GraphVizEdge->u.spl->list[0].list[0]);
    if(position != pos()) {
        setPos(position);
    }

    m_BoundingRect = QRectF();

    updatePath();
    m_BoundingRect = m_BoundingRect.united(m_Path.boundingRect().adjusted(-5.0, -5.0, 5.0, 5.0));
    m_BoundingRect = m_BoundingRect.united(m_PathArrow.boundingRect().adjusted(-5.0, -5.0, 5.0, 5.0));

    // Pre-render the label to get the bounding box
    updateLabel();
    QPainterPath label;
    label.addText(m_LabelPosition, m_LabelFont, m_LabelText);
    m_BoundingRect = m_BoundingRect.united(label.boundingRect().translated(m_LabelPosition).adjusted(-5.0, -5.0, 5.0, 5.0));

    prepareGeometryChange();
    update();
}


void QGraphVizEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    if(tail()->collapsed() || !tail()->isVisible() || !head()->isVisible()) {
        return;
    }

    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

    QByteArray currHash = m_GraphViz->getHash(m_GraphVizEdge);
    if(m_LastHash != currHash) {
        updateGeometry();
        m_LastHash = currHash;
    }

    // Draw path
    if(lod >= 0.075 && !m_Path.isEmpty()) {
        painter->setPen(m_PathPen);
        painter->setBrush(m_PathBrush);
        painter->drawPath(m_Path);

        if(lod >= 0.35 && !m_PathArrow.isEmpty()) {
            painter->drawPath(m_PathArrow);
        }
    }

    // Draw label
    if(lod >= 0.45 && !m_LabelText.isEmpty()) {
        painter->setFont(m_LabelFont);
        painter->setPen(m_LabelColor);
        painter->drawText(m_LabelPosition, m_LabelText);
    }
}


void QGraphVizEdge::updatePath()
{
    if(!m_GraphVizEdge->u.spl) {
        m_Path = QPainterPath();
        m_PathArrow = QPainterPath();
        return;
    }


    m_PathBrush = QBrush(Qt::transparent);


    m_PathPen = QPen(Qt::black);
    m_PathPen.setWidthF(m_GraphVizEdge->u.weight * 1.5);


    m_Path = QPainterPath();
    bezier bez = m_GraphVizEdge->u.spl->list[0];  // Only ever one spl

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
        m_Path.cubicTo(point1, point2, point3);
    }

    m_Path.lineTo(endPoint);


    // Add an arrowhead
    m_PathArrow =  QPainterPath();
    QLineF line(endPoint, point3);
    if(!qFuzzyCompare(line.length(), qreal(0.0))) {  // Taken from Qt4 edge example
        qreal angle = acos(line.dx() / line.length());
        if(line.dy() >= 0) {
            angle = TwoPi - angle;
        }

        QPointF arrowP1 = QPointF(sin(angle + ThirdPi) * ArrowSize, cos(angle + ThirdPi) * ArrowSize);
        m_PathArrow.moveTo(endPoint);
        m_PathArrow.lineTo(arrowP1 + endPoint);

        QPointF arrowP2 = QPointF(sin(angle + Pi - ThirdPi) * ArrowSize, cos(angle + Pi - ThirdPi) * ArrowSize);
        m_PathArrow.moveTo(endPoint);
        m_PathArrow.lineTo(arrowP2 + endPoint);
    }

    prepareGeometryChange();
    update();
}

void QGraphVizEdge::updateLabel()
{
    textlabel_t *label = m_GraphVizEdge->u.label;
    if(!label) {
        m_LabelText = QString();
        return;
    }

    m_LabelPosition = m_GraphViz->transformPoint(label->pos) - pos();

    m_LabelFont.setStyleHint(QFont::Serif);
    m_LabelFont.setStyleStrategy((QFont::StyleStrategy)(QFont::PreferAntialias | QFont::PreferQuality));
#if 0
    font.setFamily(label->fontname);    // Usually "Times New Roman"
#else
    //! \note This was set manually in the STAT GUI, so I'm doing the same here
    m_LabelFont.setFamily("sans-serif");
#endif
    m_LabelFont.setPointSizeF(label->fontsize * .75);

    //HACK: Prerender to get bounding box for centering on position
    QPainterPath path;
    path.addText(0, 0, m_LabelFont, label->text);
    m_LabelPosition -= path.boundingRect().bottomRight() / 2;

    m_LabelColor = QColor(label->fontcolor);

    m_LabelText = label->text;

    prepareGeometryChange();
    update();
}


QGraphVizNode *QGraphVizEdge::head()
{
    if(!m_Head) {
        m_Head = m_GraphViz->getNode(m_GraphVizEdge->head->id);
    }

    return m_Head;
}

QGraphVizNode *QGraphVizEdge::tail()
{
    if(!m_Tail) {
        m_Tail = m_GraphViz->getNode(m_GraphVizEdge->tail->id);
    }

    return m_Tail;
}
