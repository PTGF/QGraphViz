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

#include "QGraphVizLibrary.h"

class QGraphVizNode;
class QGraphVizEdge;

class QGRAPHVIZ_EXPORT QGraphVizScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit QGraphVizScene(QObject *parent = 0);
    explicit QGraphVizScene(QString content, QObject *parent = 0);
    ~QGraphVizScene();

    void setContent(QString content);

    QMap<QString, QString> arguments();

    QByteArray exportContent(QString renderEngine = QString("xdot"));

    QString layoutEngine();
    void setLayoutEngine(QString layoutEngine);

signals:
    void changed();

public slots:
    virtual void doRender();

protected slots:
    void onChanged();
    void doLayout();

protected:
    graph_t *graph();
    QPointF transformPoint(const pointf_s &point);
    QPointF transformPoint(const QPointF &point);
    QPointF transformPoint(qreal x, qreal y);

    QHash<QString, QString> getAttributes();
    QHash<QString, QString> getAttributes(Agnode_t *node);
    QHash<QString, QString> getAttributes(Agedge_t *edge);

    QByteArray getHash();
    QByteArray getHash(Agedge_t *edge);
    QByteArray getHash(Agnode_t *node);
    QByteArray getHash(textlabel_t *label);

    virtual QGraphVizNode *createNode(node_t *node);
    QList<QGraphVizNode*> getNodes();
    QGraphVizNode *getNode(int GVID);
    bool containsNode(int GVID);

    virtual QGraphVizEdge *createEdge(edge_t *edge);
    QList<QGraphVizEdge*> getEdges();
    QGraphVizEdge *getEdge(int GVID);
    bool containsEdge(int GVID);

private:
    static GVC_t *m_Context;

    QString m_Content;
    graph_t *m_Graph;

    QPointF m_Translate;
    QPointF m_Scale;

    QString m_LayoutEngine;
    bool m_LayoutDone;

    QHash<int, QGraphVizNode*> m_Nodes;
    QHash<int, QGraphVizEdge*> m_Edges;

    friend class QGraphVizNode;
    friend class QGraphVizEdge;
};

#endif // QGRAPHVIZ_H
