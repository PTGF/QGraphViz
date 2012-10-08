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

#include "QGraphVizNodeEffect.h"


QGraphVizNodeEffect::QGraphVizNodeEffect(QObject *parent) :
    QGraphicsEffect(parent),
    m_Opacity(1.0),
    m_BlurFilter(NULL),
    m_BlurCached(false)
{
    m_BlurFilter = new QPixmapBlurFilter(this);
    m_BlurFilter->setBlurHints(QGraphicsBlurEffect::AnimationHint);
}

qreal QGraphVizNodeEffect::opacity()
{
    return m_Opacity;
}

void QGraphVizNodeEffect::setOpacity(qreal opacity)
{
    m_Opacity = qBound(0.0, opacity, 1.0);
}

qreal QGraphVizNodeEffect::blurRadius()
{
    return m_BlurFilter->radius();
}

void QGraphVizNodeEffect::setBlurRadius(qreal blurRadius)
{
    m_BlurFilter->setRadius(blurRadius);
}

void QGraphVizNodeEffect::draw(QPainter *painter)
{
    qreal opacity = this->opacity();
    qreal blurRadius = this->blurRadius();

    // Don't do anything if it's completely transparent
    if(qFuzzyCompare(opacity, 0.0)) {
        return;
    }

    // Just draw it if there's no effect applied
    if(blurRadius < 1.0 && qFuzzyCompare(opacity, 1.0)) {
        drawSource(painter);
        return;
    }

    PixmapPadMode mode = PadToEffectiveBoundingRect;
    if(painter->paintEngine()->type() == QPaintEngine::OpenGL2)
        mode = NoPad;

    QPoint offset;
    QPixmap pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset, mode);
    if(pixmap.isNull()) {
        return;
    }

    painter->save();

    // Draw Opacity
    painter->setOpacity(opacity);


    // Draw Blur
    if(blurRadius >= 1.0) {
        m_BlurFilter->draw(painter, offset, pixmap);
    } else {
        painter->drawPixmap(offset, pixmap);
    }

    painter->restore();
}

QRectF QGraphVizNodeEffect::boundingRectFor(const QRectF &sourceRect) const
{
    return m_BlurFilter->boundingRectFor(sourceRect);
}
