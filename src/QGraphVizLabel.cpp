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
#include "QGraphViz.h"

QGraphVizLabel::QGraphVizLabel(QGraphViz *graphViz, QGraphicsItem * parent) :
    QGraphicsSimpleTextItem(parent),
    m_GraphVizLabel(NULL),
    m_GraphViz(graphViz),
    m_Alignment(Qt::AlignCenter)
{
    updateDimensions();
}

void QGraphVizLabel::setText(const QString &text)
{
    if(text == QGraphicsSimpleTextItem::text()) {
        return;
    }

    QGraphicsSimpleTextItem::setText(text);
}

void QGraphVizLabel::setFont(const QFont &font)
{
    if(font == QGraphicsSimpleTextItem::font()) {
        return;
    }

    QGraphicsSimpleTextItem::setFont(font);
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
}

void QGraphVizLabel::setGraphVizLabel(textlabel_t *label)
{
    if(m_GraphVizLabel == label) {
        return;
    }

    m_GraphVizLabel = label;

    if(!m_GraphVizLabel) {
        setText(QString());
        setFont(QFont());
        return;
    }

    updateDimensions();
}


void QGraphVizLabel::updateDimensions()
{
//    if(!m_GraphVizLabel) {
//        return;
//    }

//    // Set the text parameters
//    setText(m_GraphVizLabel->text);

//    // Set the font parameters
//    QFont font = this->font();
//    font.setStyleHint(QFont::Serif);
//    font.setStyleStrategy((QFont::StyleStrategy)(QFont::PreferAntialias | QFont::PreferQuality));
//    font.setFamily(m_GraphVizLabel->fontname);
//    font.setPointSizeF(m_GraphVizLabel->fontsize * .75);
//    setFont(font);

//    // Set the font color
//    QPen pen = this->pen();
//    pen.setWidthF(1.0);
//    pen.setColor(m_GraphVizLabel->fontcolor);
//    setPen(pen);                       // HEX 'name' (i.e. "#FFFFFF\0"), but works with QColor just fine

////    const QGraphVizNodeAttributes &nodeAttr = QGraphVizNodeAttributes(node);  //TODO: Make copy constructor
////    if(nodeAttr.contains("labeljust")) {
////        if(!nodeAttr.value("labeljust").compare("c", Qt::CaseInsensitive)) {
////            setAlignment(Qt::AlignHCenter);
////        }
////        //TODO: Other alignments
////    }


//    // Don't calculate new dimensions based on our parent if we're given the information we need
//    if(m_GraphVizLabel->pos.x == 0.0 &&
//        m_GraphVizLabel->pos.y == 0.0 &&
//        m_GraphVizLabel->dimen.x == 0.0 &&
//        m_GraphVizLabel->dimen.y == 0.0) {

//        QPointF position = m_GraphViz->transformPoint(m_GraphVizLabel->pos.x, m_GraphVizLabel->pos.y);
//        if(parentItem()) {
//            position -= parentItem()->pos();                       // Transform to parent object coordinates
//        }
//        setPos(position);

//        return;
//    }



//    //-- Calculate new dimensions and locations based on our parent object --//

//    const QRectF &parentRect = parentItem()->boundingRect();

//    if(parentRect.width() <= 0 || parentRect.height() <= 0) {
//        return;
//    }

//    // Our fonts may not match GraphViz's layout, so scale it to fit the box
//    while(boundingRect().width() > (parentRect.width() - 5)) {
//        QFont font = QGraphicsSimpleTextItem::font();

//        if(font.pixelSize() <= 0) {
//            font.setPixelSize(parentRect.height()/2);
//        } else if(font.pixelSize() <= 2) {
//            break;
//        } else {
//            font.setPixelSize(font.pixelSize() - 1);
//        }

//        QGraphicsSimpleTextItem::setFont(font);
//    }

//    // Text alignment
//    QPointF position = pos();
//    if(alignment() & Qt::AlignLeft) {
//        position.setX(0);
//    } else if(alignment() & Qt::AlignRight) {
//        position.setX(parentRect.width() - boundingRect().width());
//    } else {
//        position.setX((parentRect.width() - boundingRect().width()) / 2);
//    }

//    if(alignment() & Qt::AlignTop) {
//        position.setY(0);
//    } else if(alignment() & Qt::AlignBottom) {
//        position.setY(parentRect.height() - boundingRect().height());
//    } else {
//        position.setY((parentRect.height() - boundingRect().height()) / 2);
//    }
//    setPos(position);
}
