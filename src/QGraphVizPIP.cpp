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

#include "QGraphVizView.h"
#include "QGraphVizPIP.h"

QGraphVizPIP::QGraphVizPIP(QGraphicsScene * scene, QGraphVizView * parent) :
    QGraphicsView(scene, parent),
    m_GraphVizView(parent),
    m_MoveViewPort(false)
{
    setCacheMode(QGraphicsView::CacheBackground);

    setRenderHint(QPainter::Antialiasing, true);

    setOptimizationFlag(QGraphicsView::DontSavePainterState, true);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

    setMouseTracking(true);
    setFrameStyle(Plain);

    updateViewPortRect();
}

void QGraphVizPIP::setViewPortRect(qreal x, qreal y, qreal width, qreal height)
{
    setViewPortRect(QRectF(x, y, width, height));
}

void QGraphVizPIP::setViewPortRect(QRectF rect)
{
    m_ViewPortRect = rect;
    updateViewPortRect();
}

void QGraphVizPIP::drawBackground(QPainter *painter, const QRectF &rect)
{

    if(!rect.intersects(scene()->sceneRect())) {
        return;
    }

    QPen borderPen;
    borderPen.setColor(Qt::black);
    painter->setPen(borderPen);

    painter->drawRect(QRectF(mapToScene(0,0), mapToScene(width(),height())));
}


void QGraphVizPIP::drawForeground(QPainter *painter, const QRectF &rect)
{
    if(!rect.intersects(m_ViewPortRect)) {
        return;
    }

    if(m_ViewPortRect.contains(scene()->sceneRect())) {
        return;
    }

    QColor color(Qt::red);

    QPen pen;
    pen.setColor(color);
    painter->setPen(pen);

    QBrush brush;
    color.setAlphaF(.1);
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);

    QRectF sceneRect = this->sceneRect().adjusted(-10,-10,20,20);

    QRectF viewPortRect;
    viewPortRect.setLeft(qMax(sceneRect.left(), m_ViewPortRect.left()));
    viewPortRect.setTop(qMax(sceneRect.top(), m_ViewPortRect.top()));
    viewPortRect.setRight(qMin(sceneRect.right(), m_ViewPortRect.right()));
    viewPortRect.setBottom(qMin(sceneRect.bottom(), m_ViewPortRect.bottom()));

    painter->drawRect(viewPortRect);
}

void QGraphVizPIP::updateViewPortRect()
{
    QSize size;
    qreal aspectRatio = sceneRect().width() / sceneRect().height();
    if(aspectRatio > 1.0) {
        size.setWidth(maximumWidth());
        size.setHeight(maximumWidth() / aspectRatio);
        if(size.height() > maximumHeight()) {
            size.setWidth(maximumHeight() * aspectRatio);
            size.setHeight(maximumHeight());
        }
    } else {
        size.setWidth(maximumHeight() * aspectRatio);
        size.setHeight(maximumHeight());
        if(size.width() > maximumWidth()) {
            size.setWidth(maximumWidth());
            size.setHeight(maximumWidth() / aspectRatio);
        }
    }
    if(this->size() != size) {
        resize(size);
    }

    centerOn(scene()->sceneRect().center());
    fitInView(sceneRect().adjusted(-20,-20,20,20), Qt::KeepAspectRatio);
    resetCachedContent();
}

void QGraphVizPIP::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton || event->buttons() == Qt::MidButton) {
        if(m_ViewPortRect.contains(mapToScene(event->pos()))) {
            m_MoveViewPort = true;
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void QGraphVizPIP::mouseReleaseEvent(QMouseEvent *event)
{
    m_MoveViewPort = false;

    QGraphicsView::mouseReleaseEvent(event);
}

void QGraphVizPIP::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::NoButton) {
        if(m_ViewPortRect.contains(mapToScene(event->pos()))) {
            viewport()->setCursor(Qt::PointingHandCursor);
        } else {
            viewport()->setCursor(Qt::ArrowCursor);
        }

    } else if(event->buttons() == Qt::LeftButton || event->buttons() == Qt::MidButton) {
        if(m_MoveViewPort) {
            viewport()->setCursor(Qt::ClosedHandCursor);
            m_GraphVizView->centerOn(mapToScene(event->pos()));
        }
    }

    QGraphicsView::mouseMoveEvent(event);
}

