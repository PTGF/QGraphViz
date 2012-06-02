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

QGraphVizView::QGraphVizView(QWidget *parent) :
    QGraphicsView(parent),
    m_Scale(1.0),
    m_PictureInPicture(NULL)
{
    init();
}

QGraphVizView::QGraphVizView(QGraphicsScene * scene, QWidget * parent) :
    QGraphicsView(scene, parent),
    m_Scale(1.0),
    m_PictureInPicture(NULL)
{
    init();
}

void QGraphVizView::init()
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setCacheMode(QGraphicsView::CacheBackground);
    setRenderHint(QPainter::Antialiasing);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    m_PictureInPicture = new QGraphVizPIP(scene(), this);
    m_PictureInPicture->resize(150, 150);
    m_PictureInPicture->move(2, 2);

    centerOn(scene()->sceneRect().center());
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
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

bool QGraphVizView::viewportEvent(QEvent *event)
{
    if(event->type() == QEvent::MouseMove || event->type() == QEvent::Wheel) {
        QRectF viewPortRect = QRectF(mapToScene(0,0), mapToScene(viewport()->width(), viewport()->height()));
        viewPortRect.moveTo(mapToScene(viewport()->x(), viewport()->y()));
        m_PictureInPicture->setViewPortRect(viewPortRect);
    }

    return QGraphicsView::viewportEvent(event);
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
