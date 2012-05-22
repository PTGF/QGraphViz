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
#include "QGraphVizLabel.h"
#include "QGraphVizScene.h"

#include <QtDebug>

QGraphVizNode::QGraphVizNode(node_t *node, QGraphVizScene *scene, QGraphicsItem * parent) :
    QGraphicsItemGroup(parent),
    m_RectItem(NULL),
    m_LabelItem(NULL)
{
    Agnodeinfo_t &nodeInfo = node->u;

    m_RectItem = new QGraphicsRectItem(this);

    // Size and location
    setWidth(nodeInfo.width * 72);             // 72 pixels per inch
    setHeight(nodeInfo.height * 72);

    QPointF position = scene->transformPoint(nodeInfo.coord.x, nodeInfo.coord.y);
    position -= QPointF(width()/2, height()/2);
    setPos(position);

    //TODO: Node color
    m_RectItem->setBrush((Qt::GlobalColor)(node->id % 16 + 2));

    if(node->u.label) {
        m_LabelItem = new QGraphVizLabel(node->u.label, scene, this);
    }

    if(nodeInfo.xlabel) {
        setToolTip(nodeInfo.xlabel->text);
    }
}

qreal QGraphVizNode::width() const
{
    if(!m_RectItem) {
        return 0.0;
    }

    return m_RectItem->rect().width();
}

void QGraphVizNode::setWidth(qreal width)
{
    if(!m_RectItem) {
        return;
    }

    QRectF rect = m_RectItem->rect();

    if(width == rect.width()) {
        return;
    }

    rect.setWidth(width);
    m_RectItem->setRect(rect);

    updateDimensions();
}

qreal QGraphVizNode::height() const
{
    if(!m_RectItem) {
        return 0.0;
    }

    return m_RectItem->rect().height();
}

void QGraphVizNode::setHeight(qreal height)
{
    if(!m_RectItem) {
        return;
    }

    QRectF rect = m_RectItem->rect();

    if(height == rect.height()) {
        return;
    }

    rect.setHeight(height);
    m_RectItem->setRect(rect);

    updateDimensions();
}

void QGraphVizNode::updateDimensions()
{
    if(m_LabelItem) {
        m_LabelItem->updateDimensions();
    }
}

QRectF QGraphVizNode::boundingRect() const
{
    return QRectF(-width()/2.0, -height()/2.0, width(), height());
}

