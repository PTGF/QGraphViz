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

#include "QGraphVizLabel.h"


QGraphVizLabel::QGraphVizLabel(textlabel_t *label, QGraphicsItem * parent) :
    QGraphicsSimpleTextItem(parent),
    m_Alignment(Qt::AlignCenter),
    m_UpdateDimensions(false)
{
    if(!label) {
        return;
    }

    // Don't calculate new dimensions based on our parent if we're given the information we need
    m_UpdateDimensions = (label->pos.x == 0.0 && label->pos.y == 0.0 && label->dimen.x == 0.0 && label->dimen.y == 0.0);

    setText(label->text);

    // Set the font parameters
    QFont font;
    font.setFamily(label->fontname);
    font.setPointSizeF(label->fontsize * .75);
    setFont(font);

    // Set the font color
    setPen(QColor(label->fontcolor));  // HEX 'name' (i.e. "#FFFFFF\0"), but works with QColor just fine

//    const QGraphVizNodeAttributes &nodeAttr = QGraphVizNodeAttributes(node);  //TODO: Make copy constructor
//    if(nodeAttr.contains("labeljust")) {
//        if(!nodeAttr.value("labeljust").compare("c", Qt::CaseInsensitive)) {
//            setAlignment(Qt::AlignHCenter);
//        }
//        //TODO: Other alignments
//    }

    if(!m_UpdateDimensions) {
        QPointF position = QPointF(label->pos.x, -label->pos.y);
        if(parent) {
            position -= parent->pos();                       // Transform to parent object coordinates
        }
        setPos(position);
    } else {
        updateDimensions();
    }
}

void QGraphVizLabel::setText(const QString &text)
{
    if(text == QGraphicsSimpleTextItem::text()) {
        return;
    }

    QGraphicsSimpleTextItem::setText(text);

    updateDimensions();
}

void QGraphVizLabel::setFont(const QFont &font)
{
    if(font == QGraphicsSimpleTextItem::font()) {
        return;
    }

    QGraphicsSimpleTextItem::setFont(font);

    updateDimensions();
}

Qt::AlignmentFlag QGraphVizLabel::alignment() const
{
    return m_Alignment;
}

void QGraphVizLabel::setAlignment(const Qt::AlignmentFlag &alignment)
{
    if(alignment == m_Alignment) {
        return;
    }

    m_Alignment = alignment;

    updateDimensions();
}

void QGraphVizLabel::updateDimensions()
{
    if(!m_UpdateDimensions && text().isEmpty()) {
        return;
    }

    const QRectF &parentRect = parentItem()->boundingRect();

    if(parentRect.width() <= 0 || parentRect.height() <= 0) {
        return;
    }

    // Our fonts may not match GraphViz's layout, so scale it to fit the box
    while(boundingRect().width() > (parentRect.width() - 5)) {
        QFont font = QGraphicsSimpleTextItem::font();

        if(font.pixelSize() <= 0) {
            font.setPixelSize(parentRect.height()/2);
        } else if(font.pixelSize() <= 2) {
            break;
        } else {
            font.setPixelSize(font.pixelSize() - 1);
        }

        QGraphicsSimpleTextItem::setFont(font);
    }

    // Text alignment
    QPointF position = pos();
    if(alignment() & Qt::AlignLeft) {
        position.setX(0);
    } else if(alignment() & Qt::AlignRight) {
        position.setX(parentRect.width() - boundingRect().width());
    } else {
        position.setX((parentRect.width() - boundingRect().width()) / 2);
    }

    if(alignment() & Qt::AlignTop) {
        position.setY(0);
    } else if(alignment() & Qt::AlignBottom) {
        position.setY(parentRect.height() - boundingRect().height());
    } else {
        position.setY((parentRect.height() - boundingRect().height()) / 2);
    }
    setPos(position);
}
