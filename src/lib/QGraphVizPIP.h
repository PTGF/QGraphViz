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

#ifndef QGRAPHVIZPIP_H
#define QGRAPHVIZPIP_H

#include <QGraphicsView>

class QGraphVizView;

class QGraphVizPIP : public QGraphicsView
{
    Q_OBJECT
public:
    explicit QGraphVizPIP(QGraphicsScene *scene = 0, QGraphVizView *parent = 0);

    void setViewPortRect(QRectF rect);
    void setViewPortRect(qreal x, qreal y, qreal width, qreal height);

    void render(QPainter *painter, const QRectF &target, const QRect &source, Qt::AspectRatioMode aspectRatioMode);

    void updateViewPortRect();

signals:

public slots:

protected:
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
    virtual void drawBackground(QPainter *painter, const QRectF &rect);

    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:
    QGraphVizView *m_GraphVizView;
    QRectF m_ViewPortRect;

    bool m_StartedInViewport;

};

#endif // QGRAPHVIZPIP_H
