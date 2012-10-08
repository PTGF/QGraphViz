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

#include "QGraphVizScene.h"
#include "QGraphVizPIP.h"
#include "QGraphVizNode.h"



QGraphVizView::QGraphVizView(QGraphicsScene * scene, QWidget * parent) :
    QGraphicsView(scene, parent),
    m_Scale(1.0),
    m_PictureInPicture(NULL),
    m_NodeCollapse(NodeCollapse_None),
    m_HandleKeyboardEvents(true)
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

    setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    QMatrix matrix;
    matrix.scale(m_Scale, m_Scale);
    setMatrix(matrix);

    QRectF sceneRect = scene()->sceneRect();
    sceneRect.adjust(-50, -50, 50, 50);
    setSceneRect(sceneRect);

    m_PictureInPicture = new QGraphVizPIP(scene(), this);
    m_PictureInPicture->move(2, 2);
    m_PictureInPicture->updateViewPortRect();

    connect(scene(), SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
}



void QGraphVizView::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(painter)
    Q_UNUSED(rect)

    //TODO: Draw zoom scroller
}



void QGraphVizView::setNodeCollapse(NodeCollapse nodeCollapse)
{
    m_NodeCollapse = nodeCollapse;
}

QGraphVizView::NodeCollapse QGraphVizView::nodeCollapse()
{
    return m_NodeCollapse;
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

void QGraphVizView::zoomFit()
{
    setZoom(1.0);
}


void QGraphVizView::resizeEvent(QResizeEvent *event)
{
    m_PictureInPicture->updateViewPortRect();
    QGraphicsView::resizeEvent(event);
}



bool QGraphVizView::handlesKeyboardEvents()
{
    return m_HandleKeyboardEvents;
}

void QGraphVizView::setHandlesKeyboardEvents(bool handlesKeyboardEvents)
{
    m_HandleKeyboardEvents = handlesKeyboardEvents;
}

void QGraphVizView::keyPressEvent(QKeyEvent *event)
{
    if(m_HandleKeyboardEvents &&
       ( (event->matches(QKeySequence::ZoomIn)) ||
         (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) && event->key() == Qt::Key_Plus) ||
         (QLocale::system().name() == "en_US" && event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Equal) ) ) {
        zoomIn();
        event->accept();
        return;
    } else if(m_HandleKeyboardEvents && event->matches(QKeySequence::ZoomOut)) {
        zoomOut();
        event->accept();
        return;
    } else if(m_HandleKeyboardEvents &&
              ( (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_0) ||
               (event->modifiers() == (Qt::KeypadModifier | Qt::ControlModifier) && event->key() == Qt::Key_0) ) ) {
        zoomFit();
        event->accept();
        return;
    } else if(event->matches(QKeySequence::MoveToPreviousPage)) {
        verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMinimum);
        event->accept();
        return;
    } else if(event->matches(QKeySequence::MoveToNextPage)) {
        verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
        event->accept();
        return;
    } else if(event->matches(QKeySequence::MoveToStartOfLine)) {
        horizontalScrollBar()->triggerAction(QAbstractSlider::SliderToMinimum);
        event->accept();
        return;
    } else if(event->matches(QKeySequence::MoveToEndOfLine)) {
        horizontalScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
        event->accept();
        return;
    }

    // Regular scrolling (up,down,left,right) is handled by the parent just fine.

    QGraphicsView::keyPressEvent(event);
}



void QGraphVizView::mousePressEvent(QMouseEvent *event)
{
    m_LastMousePressPosition = m_MousePressPosition = event->pos();

    if(event->buttons() == Qt::LeftButton) {
        m_MouseLeftPressed = true;
    }

    if(event->buttons() == Qt::MidButton ||
       (event->buttons() & Qt::LeftButton && event->buttons() & Qt::RightButton) ) {
        m_MouseMidPressed = true;
    }

    if(event->buttons() == Qt::RightButton) {
        m_MouseRightPressed = true;
    }

    QGraphicsView::mousePressEvent(event);
}

void QGraphVizView::mouseReleaseEvent(QMouseEvent *event)
{

    if(m_MouseLeftPressed) {
        QLineF delta(m_MousePressPosition, event->pos());
        if(delta.length() < 1.0) {
            mouseClickEvent(event);
        }
    }

    m_MouseLeftPressed  = false;
    m_MouseMidPressed   = false;
    m_MouseRightPressed = false;

    // The event::buttons() is always 0; so the mouseMoveEvent simply processes as a no button situation
    mouseMoveEvent(event);

    QGraphicsView::mouseReleaseEvent(event);
}

void QGraphVizView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::NoButton) {
        QGraphicsItem *item = itemAt(event->pos());
        if(item && (item->type() == (QGraphicsItem::UserType + 1))) {
            QGraphVizNode *node = dynamic_cast<QGraphVizNode *>(item);
            if(node && node->isVisible() && !node->isTransparent()) {
                viewport()->setCursor(Qt::PointingHandCursor);
            }
        } else {
            viewport()->setCursor(Qt::ArrowCursor);
        }

    } else if(event->buttons() == Qt::LeftButton || event->buttons() == Qt::MidButton) {
        QPointF delta = m_LastMousePressPosition - event->pos();
        if(event->modifiers() == Qt::ControlModifier) {
            viewport()->setCursor(Qt::SizeAllCursor);

            zoom(delta.y() / 120.0);

        } else if(event->modifiers() == Qt::NoModifier) {
            viewport()->setCursor(Qt::ClosedHandCursor);
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() + delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() + delta.y());
        }
        m_LastMousePressPosition = event->pos();
    }

    QGraphicsView::mouseMoveEvent(event);
}

void QGraphVizView::mouseClickEvent(QMouseEvent *event)
{
    QGraphicsItem *item = itemAt(event->pos());
    if(item && (item->type() == (QGraphicsItem::UserType + 1))) {
        QGraphVizNode *node = dynamic_cast<QGraphVizNode *>(item);
        if(node && node->isVisible() && !node->isTransparent()) {

            if(m_NodeCollapse == NodeCollapse_OnClick) {
                node->setCollapsed(!node->isCollapsed());
            }

            emit nodeClicked(node);
        }
    }
}

void QGraphVizView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsItem *item = itemAt(event->pos());
    if(item && (item->type() == (QGraphicsItem::UserType + 1))) {
        QGraphVizNode *node = dynamic_cast<QGraphVizNode *>(item);
        if(node && node->isVisible() && !node->isTransparent()) {

            if(m_NodeCollapse == NodeCollapse_OnDoubleClick) {
                node->setCollapsed(!node->isCollapsed());
            }

            emit nodeDoubleClicked(node);
        }
    }

    QGraphicsView::mouseDoubleClickEvent(event);
}


bool QGraphVizView::event(QEvent *event)
{
    if(event->type() == QEvent::ToolTip) {
        if(QHelpEvent *helpEvent = dynamic_cast<QHelpEvent*>(event)) {
            if(bool ret = this->helpEvent(helpEvent)) {
                event->accept();
                return ret;
            }
        }
    }

    return QGraphicsView::event(event);
}

bool QGraphVizView::helpEvent(QHelpEvent *event)
{
    QGraphicsItem *item = itemAt(event->pos());
    if(item && (item->type() == (QGraphicsItem::UserType + 1))) {
        QGraphVizNode *node = dynamic_cast<QGraphVizNode *>(item);
        if(node && node->isVisible()) {
            node->showToolTip(event->globalPos(), this);
            event->accept();
            return true;
        }
    }

    QToolTip::hideText();
    return false;
}


void QGraphVizView::selectionChanged()
{
    if(scene()->selectedItems().count() == 1) {
        QGraphicsItem *item = scene()->selectedItems().at(0);
        if(item && (item->type() == (QGraphicsItem::UserType + 1))) {
            QGraphVizNode *node = dynamic_cast<QGraphVizNode *>(item);
            if(node && node->isVisible() && !node->isTransparent()) {
                emit nodeSelected(node);
            }
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

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    zoom(delta);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

