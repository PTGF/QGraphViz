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

#ifndef QGRAPHVIZNODE_H
#define QGRAPHVIZNODE_H

#include <QtCore>
#include <QtGui>

#include <graphviz/types.h>

#include "QGraphVizLibrary.h"

class QGraphVizScene;
class QGraphVizView;
class QGraphVizEdge;

class QGRAPHVIZ_EXPORT QGraphVizNode : public QGraphicsItem
{
public:
    explicit QGraphVizNode(node_t *node, QGraphVizScene *graphViz, QGraphicsItem *parent = 0);
    int getGVID();
    QString getGVName();
    int type() const;

    bool isCollapsed();
    void setCollapsed(bool collapse = true);
    void toggleCollapse();

    bool isTransparent();
    void setTransparent(bool transparent);

    bool isBlurred();
    void setBlurred(bool blurred);

    bool isHighlighted();
    void setHighlighted(bool highlighted);
    qreal highlightWidth();
    void setHighlightWidth(qreal width);
    QColor highlightColor();
    void setHighlightColor(QColor color);

    QList<QGraphVizEdge*> headEdges();
    QList<QGraphVizEdge*> tailEdges();

    virtual void showToolTip(const QPoint &pos, QWidget *parent = 0, const QRect &rect = QRect());

protected:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option);

    void updateGeometry();
    void updatePath();
    void updateLabel();

    virtual QTextOption labelOptions();
    virtual QFont labelFont();
    virtual QColor labelColor();
    virtual QString labelText();

private:
    node_t *m_GraphVizNode;
    QGraphVizScene *m_GraphViz;
    bool m_Collapsed;

    QByteArray m_LastHash;

    QRectF m_BoundingRect;

    QBrush m_PathBrush;
    QPen m_PathPen;
    QPainterPath m_Path;

    QTextOption m_LabelOptions;
    QFont m_LabelFont;
    QColor m_LabelColor;
    QString m_LabelText;

    bool m_Transparent;
    bool m_Blurred;

    bool m_Highlighted;
    qreal m_HighlightWidth;
    QColor m_HighlightColor;

    bool m_HeadEdgesInitialized;
    bool m_TailEdgesInitialized;
    QList<QGraphVizEdge*> m_HeadEdges;
    QList<QGraphVizEdge*> m_TailEdges;
};

#endif // QGRAPHVIZNODE_H
