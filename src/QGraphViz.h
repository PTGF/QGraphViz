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

#ifndef QGRAPHVIZ_H
#define QGRAPHVIZ_H

#include <QtCore>
#include <QtGui>

#include <graphviz/types.h>

#include "QGraphVizAttributes.h"

class QGraphVizNode;
class QGraphVizEdge;

class QGraphViz : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit QGraphViz(QString content, QObject *parent = 0);
    ~QGraphViz();

    QMap<QString, QString> arguments();

    QGraphVizGraphAttributes &graphAttributes();
    QGraphVizNodeAttributes &defaultNodeAttributes();
    QGraphVizEdgeAttributes &defaultEdgeAttributes();

    QByteArray exportContent(QString renderEngine = QString("xdot"));

    QString layoutEngine();
    void setLayoutEngine(QString layoutEngine);


signals:
    void changed();

public slots:

protected slots:
    void onChanged();
    void doLayout();
    void doRender();

protected:
    graph_t *graph() { return m_Graph; }
    QPointF transformPoint(const pointf_s &point);
    QPointF transformPoint(const QPointF &point);
    QPointF transformPoint(qreal x, qreal y);

    static QByteArray getHash(Agraph_t *graph);
    static QByteArray getHash(Agedge_t *edge);
    static QByteArray getHash(Agnode_t *node);
    static QByteArray getHash(textlabel_t *label);

    QGraphVizNode *getNode(int GVID);
    bool containsNode(int GVID);
    QGraphVizEdge *getEdge(int GVID);
    bool containsEdge(int GVID);

private:
    static GVC_t *m_Context;

    QString m_Content;
    graph_t *m_Graph;

    QGraphVizGraphAttributes m_GraphAttributes;
    QGraphVizNodeAttributes m_DefaultNodeAttributes;
    QGraphVizEdgeAttributes m_DefaultEdgeAttributes;

    QPointF m_Translate;
    QPointF m_Scale;

    QString m_LayoutEngine;
    bool m_LayoutDone;

    friend class QGraphVizNode;
    friend class QGraphVizEdge;
    friend class QGraphVizLabel;
};

#endif // QGRAPHVIZ_H
