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

#ifndef QGRAPHVIZVIEW_H
#define QGRAPHVIZVIEW_H

#include <QtCore>
#include <QtGui>

#include "QGraphVizLibrary.h"

#define SCALE_MIN 0.01
#define SCALE_MAX 50.0

class QGraphVizPIP;
class QGraphVizNode;

class QGRAPHVIZ_EXPORT QGraphVizView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit QGraphVizView(QGraphicsScene * scene, QWidget * parent = 0);

    enum NodeCollapse { NodeCollapse_None, NodeCollapse_OnClick, NodeCollapse_OnDoubleClick };
    void setNodeCollapse(NodeCollapse nodeCollapse);
    NodeCollapse nodeCollapse();

    virtual void resizeEvent(QResizeEvent *event);

    bool handlesKeyboardEvents();
    void setHandlesKeyboardEvents(bool handlesKeyboardEvents = true);

signals:
    void nodeSelected(QGraphVizNode *node);
    void nodeClicked(QGraphVizNode *node);
    void nodeDoubleClicked(QGraphVizNode *node);

public slots:
    void zoomIn();
    void zoomOut();
    void zoomFit();

protected:
    void init();

    void zoom(qreal delta);
    void setZoom(qreal zoom);

    virtual void drawForeground(QPainter *painter, const QRectF &rect);

    virtual void wheelEvent(QWheelEvent *event);

    virtual void scrollContentsBy(int dx, int dy);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseClickEvent(QMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);

    virtual bool event(QEvent *event);
    virtual bool helpEvent(QHelpEvent *event);

protected slots:
    virtual void selectionChanged();

private:
    qreal m_Scale;
    QGraphVizPIP *m_PictureInPicture;

    bool m_MouseLeftPressed;
    bool m_MouseMidPressed;
    bool m_MouseRightPressed;

    QPoint m_MousePressPosition;
    QPoint m_LastMousePressPosition;

    NodeCollapse m_NodeCollapse;

    bool m_HandleKeyboardEvents;

};

#endif // QGRAPHVIZVIEW_H
