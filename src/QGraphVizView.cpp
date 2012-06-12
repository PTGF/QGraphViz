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

#include "QGraphViz.h"
#include "QGraphVizPIP.h"
#include "QGraphVizNode.h"
#include "QGraphVizZoomWidget.h"

QGraphVizView::QGraphVizView(QWidget *parent) :
    QGraphicsView(parent),
    m_Scale(1.0),
    m_PictureInPicture(NULL),
    m_ZoomWidget(NULL)
{
    init();
}

QGraphVizView::QGraphVizView(QGraphicsScene * scene, QWidget * parent) :
    QGraphicsView(scene, parent),
    m_Scale(1.0),
    m_PictureInPicture(NULL),
    m_ZoomWidget(NULL)
{
    init();
}

void QGraphVizView::init()
{
    setCacheMode(QGraphicsView::CacheBackground);
    setRenderHint(QPainter::Antialiasing);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setMouseTracking(true);

    m_PictureInPicture = new QGraphVizPIP(scene(), this);
    m_PictureInPicture->setMaximumSize(width()/4, height()/4);
    m_PictureInPicture->move(2, 2);

    QMatrix matrix;
    matrix.scale(m_Scale, m_Scale);
    setMatrix(matrix);

    setSceneRect(scene()->sceneRect().adjusted(-10, -10, 10, 10));

    m_ZoomWidget = new QGraphVizZoomWidget(this);
    m_ZoomWidget->move(2, m_PictureInPicture->rect().bottom() + 2);
}

void QGraphVizView::drawForeground(QPainter *painter, const QRectF &rect)
{
    //TODO: Draw zoom scroller
}

void QGraphVizView::setZoom(qreal zoom)
{
    if(qFuzzyCompare(m_Scale, zoom)) {
        return;
    }

    if(zoom < SCALE_MIN) {
        zoom = SCALE_MIN;
    }

    if(zoom > SCALE_MAX) {
        zoom = SCALE_MAX;
    }

    m_Scale = zoom;

    QMatrix matrix;
    matrix.scale(m_Scale, m_Scale);
    setMatrix(matrix);

    QRectF viewPortRect = QRectF(mapToScene(0,0), mapToScene(viewport()->width(), viewport()->height()));
    viewPortRect.moveTo(mapToScene(viewport()->x(), viewport()->y()));
    m_PictureInPicture->setViewPortRect(viewPortRect);
}

void QGraphVizView::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::MidButton) {
        m_MidPress = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}

void QGraphVizView::mouseReleaseEvent(QMouseEvent *event)
{
    // The event::buttons() is always 0; so the mouseMoveEvent simply processes as a no button situation
    mouseMoveEvent(event);

    QGraphicsView::mouseReleaseEvent(event);
}

void QGraphVizView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::NoButton) {
        QGraphicsItem *item = itemAt(event->pos());
        if(item && (item->type() == (QGraphicsItem::UserType + 1))) {
            viewport()->setCursor(Qt::PointingHandCursor);
        } else {
            viewport()->setCursor(Qt::ArrowCursor);
        }

    } else if(event->buttons() == Qt::MidButton) {
        viewport()->setCursor(Qt::ClosedHandCursor);

        QPointF delta = m_MidPress - event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() + delta.y());
        m_MidPress =event->pos();
    }

    QGraphicsView::mouseMoveEvent(event);
}

void QGraphVizView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsItem *item = itemAt(event->pos());
    if(item && (item->type() == (QGraphicsItem::UserType + 1))) {
        QGraphVizNode *node = dynamic_cast<QGraphVizNode *>(item);
        if(node) {
            node->toggleCollapse();
        }
    }
}

void QGraphVizView::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx, dy);

    QRectF viewPortRect = QRectF(mapToScene(0,0), mapToScene(viewport()->width(), viewport()->height()));
    viewPortRect.moveTo(mapToScene(viewport()->x(), viewport()->y()));
    m_PictureInPicture->setViewPortRect(viewPortRect);
}

void QGraphVizView::wheelEvent(QWheelEvent *event)
{
    qreal delta = event->delta() / 120.0;
    zoom(delta);
}

void QGraphVizView::zoom(qreal delta)
{
    delta *= qLn(1.0 + (qPow(m_Scale, 2.0) / 7.5)) + SCALE_MIN;
    setZoom(m_Scale + delta);
}

void QGraphVizView::zoomIn()
{
    zoom(1);
}

void QGraphVizView::zoomOut()
{
    zoom(-1);
}