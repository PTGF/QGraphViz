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
#include "QGraphVizScene.h"
#include "QGraphVizEdge.h"

#define STROKE_WIDTH 1.5

QGraphVizNode::QGraphVizNode(node_t *node, QGraphVizScene *graphViz, QGraphicsItem * parent) :
    QGraphicsItem(parent),
    m_GraphVizNode(node),
    m_GraphViz(graphViz),
    m_Collapsed(false),
    m_Transparent(false),
    m_HeadEdgesInitialized(false),
    m_TailEdgesInitialized(false)
{
    updateGeometry();
    setFlag(QGraphicsItem::ItemIsSelectable, true);
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
    if(transparent()) {
        return;
    }

    m_Collapsed = collapse;

    foreach(QGraphVizEdge *edge, m_GraphViz->getEdges()) {
        if(edge->tail() == this) {
            edge->head()->setTransparent(m_Collapsed);
            edge->update();
        }
    }

    prepareGeometryChange();
    update();
}

void QGraphVizNode::toggleCollapse()
{
    setCollapsed(!collapsed());
}

bool QGraphVizNode::transparent()
{
    return m_Transparent;
}


QList<QGraphVizEdge*> QGraphVizNode::headEdges()
{
    if(!m_HeadEdgesInitialized) {
        foreach(QGraphVizEdge *edge, m_GraphViz->getEdges()) {
            if(edge->head() == this) {
                m_HeadEdges.append(edge);
            }
        }

        m_HeadEdgesInitialized = true;
    }
    return m_HeadEdges;
}

QList<QGraphVizEdge*> QGraphVizNode::tailEdges()
{
    if(!m_TailEdgesInitialized) {
        foreach(QGraphVizEdge *edge, m_GraphViz->getEdges()) {
            if(edge->tail() == this) {
                m_TailEdges.append(edge);
            }
        }
        m_TailEdgesInitialized = true;
    }
    return m_TailEdges;
}


void QGraphVizNode::setTransparent(bool transparent)
{
    m_Transparent = transparent;

    setFlag(QGraphicsItem::ItemIsSelectable, !m_Transparent);

    foreach(QGraphVizEdge *edge, m_GraphViz->getEdges()) {
        if(edge->tail() == this) {
            if(!collapsed()) {
                edge->head()->setTransparent(transparent);
                edge->update(edge->boundingRect().adjusted(-5,-5,20,5));
            }
        }
    }

    prepareGeometryChange();
    update();
}


QRectF QGraphVizNode::boundingRect() const
{
    return m_BoundingRect;
}

void QGraphVizNode::updateGeometry()
{
    QPointF newPos = m_GraphViz->transformPoint(m_GraphVizNode->u.coord);
    if(newPos != pos()) {
        setPos(newPos);
    }

    m_BoundingRect = QRectF();

    // Get bounding box for path
    updatePath();
    QRectF adjusted = m_Path.boundingRect().adjusted(-STROKE_WIDTH, -STROKE_WIDTH, STROKE_WIDTH, STROKE_WIDTH);
    m_BoundingRect = m_BoundingRect.united(adjusted);

    updateLabel();

    prepareGeometryChange();
    update();
}

void QGraphVizNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    if(!isVisible()) {
        return;
    }

    QByteArray currHash = m_GraphViz->getHash(m_GraphVizNode);
    if(m_LastHash != currHash) {
        updateGeometry();
        m_LastHash = currHash;
    }

    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

    if(!graphicsEffect()) {
        setGraphicsEffect(new QGraphicsOpacityEffect(scene()));
    }

    QGraphicsOpacityEffect *effect = qobject_cast<QGraphicsOpacityEffect*>(graphicsEffect());
    if(effect) {
        if(transparent()) {
                effect->setOpacity(0.15);
        } else {
            effect->setOpacity(1.0);
        }
    }

    // Paint the path
    if(lod >= 0.01 && !m_Path.isEmpty()) {
        QPen pen = QPen(m_PathPen);
        QBrush brush = QBrush(m_PathBrush);

        if(collapsed()) {
            pen.setStyle(Qt::DotLine);
        }
        if(isSelected()) {
            pen.setColor(Qt::red);
            brush.setColor(brush.color().lighter());
        }

        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawPath(m_Path);
    }

    // Draw the labels
    if(lod >= 0.45 && !m_LabelText.isEmpty()) {
        painter->setPen(m_LabelColor);
        painter->setFont(m_LabelFont);
        painter->drawText(m_Path.boundingRect(), m_LabelText, m_LabelOptions);
    }
}

void QGraphVizNode::updatePath()
{
    if(!m_GraphVizNode->u.shape) {
        m_Path = QPainterPath();
        return;
    }

#if 0
    // Get the attributes together into a dictionary structure
    QHash<QString,QString> attr = m_GraphViz->getAttributes(m_GraphVizNode);

    // If there's a specified fill color, use it
    if(!attr["fillcolor"]) {
        brush = QBrush(QColor(attr["fillcolor"]));
    }
#else
    /* Seriously, WTF?!  The friggin' color values are in the attr array, but don't come out of the agattr function
       set.  We get to pull them manually.  GraphViz is a thing of wonder. */

    QStringList attr;
    for(uint i = 0; i < sizeof(m_GraphVizNode->attr); ++i) {
        attr.append(m_GraphVizNode->attr[i]);
    }

    if(attr.count() >= 6) {
        m_PathBrush = QBrush(QColor(attr[6]));
    }
#endif


    // Set the stroke pen and width
    m_PathPen = QPen(Qt::black);
    m_PathPen.setWidthF(STROKE_WIDTH);
    m_PathPen.setJoinStyle(Qt::RoundJoin);


    m_Path = QPainterPath();
#if 0
    //TODO: Need to figure out translation of points from GraphViz to our coordinates
    //shape_desc *shapeDescription = m_GraphVizNode->u.shape;

    QPolygonF polygon;
    polygon_t *poly = (polygon_t*)m_GraphVizNode->u.shape_info;
    for(int i=0; i < poly->sides; ++i) {
        polygon << m_GraphViz->transformPoint(poly->vertices[i]);
    }
    polygon << m_GraphViz->transformPoint(poly->vertices[0]);    // Close the polygon

    path.moveTo(m_GraphViz->transformPoint(poly->vertices[0]));
    path.addPolygon(polygon);

#else

    // Just draw the bounding rectangle (minus the stroke width) for now
    QPointF size(m_GraphVizNode->u.width * 72, m_GraphVizNode->u.height * 72);
    QRectF rectDraw = QRectF(-size/2, size/2);  // Center point of overall block

    m_Path.addRect(rectDraw);
#endif

    prepareGeometryChange();
    update();
}

void QGraphVizNode::updateLabel()
{
    textlabel_t *label = m_GraphVizNode->u.label;
    if(!label) {
        return;
    }

    m_LabelFont.setStyleHint(QFont::Serif);
    m_LabelFont.setStyleStrategy((QFont::StyleStrategy)(QFont::PreferAntialias | QFont::PreferQuality));
#if 0
    font.setFamily(label->fontname);    // Usually "Times New Roman"
#else
    //! \note This was set manually in the STAT GUI, so I'm doing the same here
    m_LabelFont.setFamily("sans-serif");
#endif
    m_LabelFont.setPointSizeF(label->fontsize * .75);

    //HACK: Pre-render to get optimal font size to fit in bounding box without overflow/wrap
    QPainterPath labelPath;
    labelPath.addText(0, 0, m_LabelFont, label->text);

    QPointF size(m_GraphVizNode->u.width * 72, m_GraphVizNode->u.height * 72);
    QRectF rectDraw = QRectF(-size/2, size/2);  // Center point of overall block

    if((rectDraw.width()-20) < labelPath.boundingRect().width()) {
        m_LabelFont.setPointSizeF(m_LabelFont.pointSizeF() * ((rectDraw.width()-20) / labelPath.boundingRect().width()));
    }

    m_LabelColor = QColor(label->fontcolor);

    // If we're using the default text pen color, see if a human can actually read it against
    // the background color
    if(m_LabelColor == Qt::black) {
        // For getting human-percieved color brightness, checkout my blog post on the subject
        // http://blog.danegardner.com/2010/10/getting-perceived-brightness-from-rgb.html

        // This is not the same thing as the HSL lightness value.  For example, dark blues appear darker
        // than dark reds!

        int red, green, blue;
        m_PathBrush.color().getRgb(&red, &green, &blue);
        int perceivedBackgroundBrightness = qRound(qSqrt(
                                                       (qPow(red, 2) * 0.241) +
                                                       (qPow(red, 2) * 0.691) +
                                                       (qPow(red, 2) * 0.068) ));

        if(perceivedBackgroundBrightness < 64) {  // A good threshold after some initial testing
            m_LabelColor = Qt::white;
        }
    }


    // Center the text and wrap if we screwed up at the pre-render
    m_LabelOptions.setAlignment(Qt::AlignCenter);
    m_LabelOptions.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);


    m_LabelText = label->text;

    prepareGeometryChange();
    update();
}
