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

#ifndef QGRAPHVIZEDGE_H
#define QGRAPHVIZEDGE_H

#include <QtCore>
#include <QtGui>

#include <graphviz/types.h>

#include "QGraphVizLibrary.h"

class QGraphVizNode;
class QGraphVizScene;

class QGRAPHVIZ_EXPORT QGraphVizEdge : public QGraphicsItem
{
public:
    explicit QGraphVizEdge(edge_t *edge, QGraphVizScene *graphViz, QGraphicsItem *parent = 0);
    int getGVID();
    int type() const;

    QGraphVizNode *head();
    QGraphVizNode *tail();

protected:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option);

    void updateGeometry();
    void updatePath();
    void updateLabel();

    virtual QPointF labelPosition();
    virtual QFont labelFont();
    virtual QColor labelColor();
    virtual QString labelText();

private:
    edge_t *m_GraphVizEdge;
    QGraphVizScene *m_GraphViz;

    QByteArray m_LastHash;

    QRectF m_BoundingRect;

    QPen m_PathPen;
    QBrush m_PathBrush;
    QPainterPath m_Path;
    QPainterPath m_PathArrow;
    QPainterPath m_PathSimple;
    QPainterPath m_PathArrowSimple;

    QPointF m_LabelPosition;
    QFont m_LabelFont;
    QColor m_LabelColor;
    QString m_LabelText;


    QGraphVizNode *m_Head;
    QGraphVizNode *m_Tail;

    friend class QGraphVizScene;
    friend class QGraphVizNode;
};

#endif // QGRAPHVIZEDGE_H
