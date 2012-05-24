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

#include "QGraphVizNode.h"
#include "QGraphViz.h"
#include "QGraphVizEdge.h"

#define STROKE_WIDTH 2

QGraphVizNode::QGraphVizNode(node_t *node, QGraphViz *graphViz, QGraphicsItem * parent) :
    QGraphicsItem(parent),
    m_GraphVizNode(node),
    m_GraphViz(graphViz),
    m_Collapsed(false)
{
    setPos(m_GraphViz->transformPoint(m_GraphVizNode->u.coord));
}

int QGraphVizNode::type() const
{
    return UserType + 1;
}

int QGraphVizNode::getGVID()
{
    return m_GraphVizNode->id;
}

QString QGraphVizNode::getGVName()
{
    return m_GraphVizNode->name;
}

bool QGraphVizNode::collapsed()
{
    return m_Collapsed;
}

void QGraphVizNode::setCollapsed(bool collapse)
{
    m_Collapsed = collapse;
}

void QGraphVizNode::toggleCollapse()
{
    setCollapsed(!collapsed());
}


QRectF QGraphVizNode::boundingRect() const
{
    QRectF rect;

#if 0
    QPointF position = m_GraphViz->transformPoint(m_GraphVizNode->u.coord);
    QPointF lowerLeft = QPointF(m_GraphViz->transformPoint(m_GraphVizNode->u.bb.LL)) - position;
    QPointF upperRight = QPointF(m_GraphViz->transformPoint(m_GraphVizNode->u.bb.UR)) - position;
    rect = QRectF(lowerLeft.x(), upperRight.y(), upperRight.x(), lowerLeft.y());
#else
    //! \note For some reason, the bounding box is not defined when we need it; use height and width instead
    QPointF size(m_GraphVizNode->u.width * 72, m_GraphVizNode->u.height * 72);
    rect = QRectF(-size/2, size/2);  // Center point of overall block
#endif

    return rect;
}

void QGraphVizNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    QList<QGraphVizEdge*> edges = m_GraphViz->getEdges();
    foreach(QGraphVizEdge *edge, edges) {
        if(edge->tail() == this) {
            if(!edge->head()->isVisible() || edge->head()->collapsed()) {
                setVisible(false);
            } else {
                setVisible(true);
            }
        }
    }

    if(!isVisible()) {
        return;
    }

    if(collapsed()) {
        //TODO: Draw differently to indicate collapsed node
        painter->setPen(Qt::red);
        painter->drawRect(boundingRect().adjusted(-10,-10,10,10));
    }

    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

    QPointF newPos = m_GraphViz->transformPoint(m_GraphVizNode->u.coord);
    if(newPos != pos()) {
        setPos(newPos);
        prepareGeometryChange();
    }

    if(lod >= 0.01) {

        QPainterPath path;

//        TODO: Need to figure out translation of points from GraphViz to our coordinates
//        if(m_GraphVizNode->u.shape) {
////            shape_desc *shapeDescription = m_GraphVizNode->u.shape;

//            QPolygonF polygon;
//            polygon_t *poly = (polygon_t*)m_GraphVizNode->u.shape_info;
//            for(int i=0; i < poly->sides; ++i) {
//                polygon << m_GraphViz->transformPoint(poly->vertices[i]);
//            }
//            polygon << m_GraphViz->transformPoint(poly->vertices[0]);    // Close the polygon

//            path.moveTo(m_GraphViz->transformPoint(poly->vertices[0]));
//            path.addPolygon(polygon);

//            qDebug() << boundingRect() << m_GraphViz->transformPoint(poly->vertices[0]) << position;

//        } else {
//            qWarning() << "No shape data";
//        }

        // Just draw the bounding rectangle (minus the stroke width) for now
        path.addRect(boundingRect().adjusted(STROKE_WIDTH/2, STROKE_WIDTH/2, -STROKE_WIDTH/2, -STROKE_WIDTH/2));

#if 0
        // Get the attributes together into a dictionary structure
        QHash<QString,QString> attr = m_GraphViz->getAttributes(m_GraphVizNode);

        // If there's a specified fill color, use it
        if(!attr["fillcolor"]) {
            painter->setBrush(QColor(attr["fillcolor"]));
        }
#else
        /* Seriously, WTF?!  The friggin' color values are in the attr array, but don't come out of the agattr function
           set.  We get to pull them manually.  GraphViz is a thing of wonder. */

        QStringList attr;
        for(uint i = 0; i < sizeof(m_GraphVizNode->attr); ++i) {
            attr.append(m_GraphVizNode->attr[i]);
        }

        if(attr.count() >= 6) {
            painter->setBrush(QColor(attr[6]));
        }
#endif

        // Set the stroke pen and width
        QPen pen = QPen(Qt::black);
        pen.setWidthF(STROKE_WIDTH);
        pen.setJoinStyle(Qt::RoundJoin);
        painter->setPen(pen);

        // Paint the path
        painter->drawPath(path);
    }

    // Draw the labels
    if(lod >= 0.35) {
        QList<textlabel_t*> labels;
        if(m_GraphVizNode->u.label) labels.append(m_GraphVizNode->u.label);
        if(m_GraphVizNode->u.xlabel) labels.append(m_GraphVizNode->u.xlabel);

        foreach(textlabel_t *label, labels) {
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

            //HACK: Pre-render to get optimal font size to fit in bounding box without overflow/wrap
            QPainterPath labelPath;
            labelPath.addText(0, 0, font, label->text);
            if((boundingRect().width()-20) < labelPath.boundingRect().width()) {
                font.setPointSizeF(font.pointSizeF() * ((boundingRect().width()-20) / labelPath.boundingRect().width()));
            }

            // Get the text color
            QColor color(label->fontcolor);

            // Center the text and wrap if we screwed up at the pre-render
            QTextOption options;
            options.setAlignment(Qt::AlignCenter);
            options.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

            // Draw it!
            painter->setFont(font);
            painter->setPen(color);
            painter->drawText(boundingRect(), label->text, options);
        }
    }
}
