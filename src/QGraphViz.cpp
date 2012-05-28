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

#include "QGraphViz.h"

#include <graphviz/cdt.h>
#include <graphviz/gvc.h>
#include <graphviz/graph.h>

#ifdef QT_DEBUG
  #include <QtDebug>
#endif

#include "QGraphVizNode.h"
#include "QGraphVizEdge.h"

GVC_t *QGraphViz::m_Context = gvContext();

QGraphViz::QGraphViz(QString content, QObject *parent) :
    QGraphicsScene(parent),
    m_Graph(NULL),
    m_GraphAttributes(),
    m_DefaultNodeAttributes(),
    m_DefaultEdgeAttributes(),
    m_LayoutEngine("dot"),
    m_LayoutDone(false)
{
    if(content.isEmpty()) {
        return;
    }

    m_Content = content;
    m_Graph = agmemread(m_Content.toLocal8Bit().data());

    if(m_Graph) {
        m_GraphAttributes.setGraph(m_Graph);
        connect(&m_GraphAttributes, SIGNAL(changed()), this, SLOT(onChanged()));

        m_DefaultNodeAttributes.setNode(agprotonode(m_Graph));
        connect(&m_DefaultNodeAttributes, SIGNAL(changed()), this, SLOT(onChanged()));

        m_DefaultEdgeAttributes.setEdge(agprotoedge(m_Graph));
        connect(&m_DefaultEdgeAttributes, SIGNAL(changed()), this, SLOT(onChanged()));

        doRender();
    }
}

QGraphViz::~QGraphViz()
{
    if(m_LayoutDone) {
        gvFreeLayout(m_Context, m_Graph);
        m_LayoutDone = false;
    }

    if(m_Graph) {
        agclose(m_Graph);
        m_Graph = NULL;
    }
}

QGraphVizGraphAttributes &QGraphViz::graphAttributes()
{
    return m_GraphAttributes;
}

QGraphVizNodeAttributes &QGraphViz::defaultNodeAttributes()
{
    return m_DefaultNodeAttributes;
}

QGraphVizEdgeAttributes &QGraphViz::defaultEdgeAttributes()
{
    return m_DefaultEdgeAttributes;
}

void QGraphViz::doLayout()
{
    if(m_LayoutDone) {
        return;
    }

    if(gvLayout(m_Context, m_Graph, m_LayoutEngine.toLocal8Bit().data())) {
        throw tr("Layout failed");
    }

    m_Translate = QPointF(-m_Graph->u.bb.LL.x, -m_Graph->u.bb.UR.y);
    m_Scale = QPointF(1.0, -1.0);

    m_LayoutDone = true;
}

void QGraphViz::doRender()
{
    doLayout();

    node_t *node = agfstnode(graph());
    while(node) {

        if(!containsNode(node->id)) {
            addItem(new QGraphVizNode(node, this));
        }

        Agedge_t *edge = agfstedge(graph(), node);
        while(edge) {

            if(!containsEdge(edge->id)) {
                addItem(new QGraphVizEdge(edge, this));
            }

            edge = agnxtedge(graph(), edge, node);
        }

        node = agnxtnode(graph(), node);
    }
}

void QGraphViz::onChanged()
{
    if(m_LayoutDone) {
        gvFreeLayout(m_Context, m_Graph);
        m_LayoutDone = false;
    }

    doRender();
}

/*! dot; neato; circo; fdp; osage; sfdp; twopi
 */
QString QGraphViz::layoutEngine()
{
    return m_LayoutEngine;
}

/*! dot; neato; circo; fdp; osage; sfdp; twopi
 */
void QGraphViz::setLayoutEngine(QString layoutEngine)
{
    if(layoutEngine == m_LayoutEngine) {
        return;
    }

    m_LayoutEngine = layoutEngine;

    onChanged();
    emit changed();
}

/*! dot; xdot; png; svg; plain; etc.
 */
QByteArray QGraphViz::exportContent(QString renderEngine)
{
    doLayout();

    char *content;
    unsigned int length;
    if(gvRenderData(m_Context, m_Graph, renderEngine.toLocal8Bit().data(), &content, &length)) {
        throw tr("Failed to render.");
    }

    QByteArray renderedContent = QByteArray(content, length);

    free(content);

    Agnode_t *node = agfstnode(m_Graph);
    while(node) {
        Agedge_t *edge = agfstedge(m_Graph, node);
        while(edge) {
            edge = agnxtedge(m_Graph, edge, node);
        }
        node = agnxtnode(m_Graph, node);
    }

    return renderedContent;
}



QPointF QGraphViz::transformPoint(const QPointF &point)
{
    return transformPoint(point.x(), point.y());
}

QPointF QGraphViz::transformPoint(const pointf_s &point)
{
    return transformPoint(point.x, point.y);
}

QPointF QGraphViz::transformPoint(qreal x, qreal y)
{
    return QPointF((x + m_Translate.x()) * m_Scale.x(), (y + m_Translate.y()) * m_Scale.y());
}



QGraphVizNode *QGraphViz::getNode(int GVID)
{
    foreach(QGraphicsItem *item, items()) {
        if(item->type() == QGraphicsItem::UserType + 1) {  // Little more optimized
            QGraphVizNode *node = dynamic_cast<QGraphVizNode*>(item);
            if(node) {
                if(node->getGVID() == GVID) {
                    return node;
                }
            }
        }
    }
    return NULL;
}

bool QGraphViz::containsNode(int GVID)
{
    return (getNode(GVID) != NULL);
}

QGraphVizEdge *QGraphViz::getEdge(int GVID)
{
    foreach(QGraphicsItem *item, items()) {
        if(item->type() == QGraphicsItem::UserType + 2) {  // Little more optimized
            QGraphVizEdge *edge = dynamic_cast<QGraphVizEdge*>(item);
            if(edge) {
                if(edge->getGVID() == GVID) {
                    return edge;
                }
            }
        }
    }
    return NULL;
}

bool QGraphViz::containsEdge(int GVID)
{
    return (getEdge(GVID) != NULL);
}



QByteArray QGraphViz::getHash(Agraph_t *graph)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData((char*)graph, sizeof(Agraph_t));

    //TODO: Add attributes
    //TODO: Add default node attributes
    //TODO: Add default edge attributes

    node_t *node = agfstnode(graph);
    while(node) {
        md5.addData(getHash(node));
        Agedge_t *edge = agfstedge(graph, node);
        while(edge) {
            md5.addData(getHash(edge));
            edge = agnxtedge(graph, edge, node);
        }
        node = agnxtnode(graph, node);
    }

    return md5.result();
}

QByteArray QGraphViz::getHash(Agedge_t *edge)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData((char*)edge, sizeof(Agedge_t));

    //TODO: Add attributes

    // Add splines
    if(edge->u.spl) {
        md5.addData((char*)edge->u.spl, sizeof(splines));
        md5.addData((char*)edge->u.spl->list, edge->u.spl->size * sizeof(bezier));
    }

    // Add labels
    if(edge->u.label) {
        md5.addData(getHash(edge->u.label));
    }
    if(edge->u.head_label) {
        md5.addData(getHash(edge->u.head_label));
    }
    if(edge->u.tail_label) {
        md5.addData(getHash(edge->u.tail_label));
    }
    if(edge->u.xlabel) {
        md5.addData(getHash(edge->u.xlabel));
    }

    return md5.result();
}

QByteArray QGraphViz::getHash(Agnode_t *node)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData((char*)node, sizeof(Agnode_t));

    //TODO: Add attributes

    if(node->u.shape) {
        md5.addData((char*)node->u.shape, sizeof(shape_desc));
        if(node->u.shape->polygon) {
            md5.addData((char*)node->u.shape->polygon, sizeof(polygon_t));
        }
    }

    // Add labels
    if(node->u.label) {
        md5.addData(getHash(node->u.label));
    }
    if(node->u.xlabel) {
        md5.addData(getHash(node->u.xlabel));
    }

    return md5.result();
}

QByteArray QGraphViz::getHash(textlabel_t *label)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData((char*)label, sizeof(textlabel_t));
    return md5.result();
}


