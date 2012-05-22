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
#include "QGraphVizAttributes.h"

#include <QtDebug>

QGraphVizNode::QGraphVizNode(node_t *node, QGraphicsItem * parent) :
    QGraphicsItemGroup(parent),
    m_RectItem(NULL),
    m_LabelItem(NULL),
    m_Width(0.0),
    m_Height(0.0)
{
    Agnodeinfo_t &nodeInfo = node->u;

    m_RectItem = new QGraphicsRectItem(this);

    // Size and location
    setWidth(nodeInfo.width * 72);             // 72 pixels per inch
    setHeight(nodeInfo.height * 72);
    setX(nodeInfo.coord.x - width()/2);        // Coordinates are at centerpoint
    setY(-(nodeInfo.coord.y - height()/2));

    //TODO: Node color
    m_RectItem->setBrush((Qt::GlobalColor)(node->id % 16 + 2));

    if(node->u.label) {
        m_LabelItem = new QGraphVizLabel(node->u.label, this);
    }
}

qreal QGraphVizNode::width() const
{
    return m_Width;
}

void QGraphVizNode::setWidth(qreal width)
{
    if(width == m_Width) {
        return;
    }

    m_Width = width;

    updateDimensions();
}

qreal QGraphVizNode::height() const
{
    return m_Height;
}

void QGraphVizNode::setHeight(qreal height)
{
    if(height == m_Height) {
        return;
    }

    m_Height = height;

    updateDimensions();
}

void QGraphVizNode::updateDimensions()
{
    // Update the rectangle object
    QRectF rect = m_RectItem->rect();
    rect.setWidth(width());
    rect.setHeight(height());
    m_RectItem->setRect(rect);

    if(m_LabelItem) {
        m_LabelItem->updateDimensions();
    }
}

QRectF QGraphVizNode::boundingRect() const
{
    return QRectF(-width()/2.0, -height()/2.0, width(), height());
}

