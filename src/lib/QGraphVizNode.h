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

class QGraphVizLabel;
class QGraphVizScene;

class QGRAPHVIZ_EXPORT QGraphVizNode : public QGraphicsItem
{
public:
    explicit QGraphVizNode(node_t *node, QGraphVizScene *graphViz, QGraphicsItem *parent = 0);
    int getGVID();
    QString getGVName();
    int type() const;

    bool collapsed();
    void setCollapsed(bool collapse = true);
    void toggleCollapse();

    void setTransparent(bool transparent);
    bool transparent();

protected:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updateGeometry();
    void updatePath();
    void updateLabel();

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

};

#endif // QGRAPHVIZNODE_H
