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

#include "QGraphVizScene.h"

#include <graphviz/cdt.h>
#include <graphviz/gvc.h>
#include <graphviz/graph.h>

#include "QGraphVizNode.h"
#include "QGraphVizEdge.h"



GVC_t *QGraphVizScene::m_Context = gvContext();



QGraphVizScene::QGraphVizScene(QString content, QObject *parent) :
    QGraphicsScene(parent),
    m_Graph(NULL),
    m_LayoutEngine("dot"),
    m_LayoutDone(false)
{
    if(content.isEmpty()) {
        return;
    }

    m_Content = content;

#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::agmemread() starting";
#endif
    m_Graph = agmemread(m_Content.toLocal8Bit().data());
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::agmemread() finished";
#endif

    doRender();
}

QGraphVizScene::~QGraphVizScene()
{
    if(m_LayoutDone) {
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::gvFreeLayout() starting";
#endif
        gvFreeLayout(m_Context, m_Graph);
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::gvFreeLayout() finished";
#endif
        m_LayoutDone = false;
    }

    if(m_Graph) {
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::agclose() starting";
#endif
        agclose(m_Graph);
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::agclose() finished";
#endif
        m_Graph = NULL;
    }
}



void QGraphVizScene::doLayout()
{
    if(m_LayoutDone) {
        return;
    }

#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::gvLayout() starting";
#endif
    if(gvLayout(m_Context, m_Graph, m_LayoutEngine.toLocal8Bit().data())) {
        throw tr("Layout failed");
    }
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::gvLayout() finished";
#endif

    m_Translate = QPointF(-m_Graph->u.bb.LL.x, -m_Graph->u.bb.UR.y);
    m_Scale = QPointF(1.0, -1.0);

    m_LayoutDone = true;
}

void QGraphVizScene::doRender()
{
    doLayout();

    node_t *node = agfstnode(graph());
    while(node) {
        if(!containsNode(node->id)) {
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " Creating node: " << node->id;
#endif
            QGraphVizNode *graphVizNode = new QGraphVizNode(node, this);
            m_Nodes.insert(node->id, graphVizNode);
            addItem(graphVizNode);
        }

        Agedge_t *edge = agfstedge(graph(), node);
        while(edge) {

            if(!containsEdge(edge->id)) {
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " Creating edge " << edge->id;
#endif
                QGraphVizEdge *graphVizEdge = new QGraphVizEdge(edge, this);
                m_Edges.insert(edge->id, graphVizEdge);
                addItem(graphVizEdge);
            }

            edge = agnxtedge(graph(), edge, node);
        }

        node = agnxtnode(graph(), node);
    }

    // Calculate the visible area
    QRectF rect = sceneRect();
    rect.setTopLeft(QPointF(0,0));
    setSceneRect(rect);

    QList<QRectF> rects;
    rects << rect;
    emit QGraphicsScene::changed(rects);
}

void QGraphVizScene::onChanged()
{
    if(m_LayoutDone) {
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::gvFreeLayout() starting";
#endif
        gvFreeLayout(m_Context, m_Graph);
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::gvFreeLayout() finished";
#endif
        m_LayoutDone = false;
    }

    doRender();
}

/*! dot; neato; circo; fdp; osage; sfdp; twopi
 */
QString QGraphVizScene::layoutEngine()
{
    return m_LayoutEngine;
}

/*! dot; neato; circo; fdp; osage; sfdp; twopi
 */
void QGraphVizScene::setLayoutEngine(QString layoutEngine)
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
QByteArray QGraphVizScene::exportContent(QString renderEngine)
{
    doLayout();

    char *content;
    unsigned int length;
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::gvRenderData() starting";
#endif
    if(gvRenderData(m_Context, m_Graph, renderEngine.toLocal8Bit().data(), &content, &length)) {
        throw tr("Failed to render.");
    }
#ifdef QGRAPHVIZSCENE_DEBUG
        qDebug() << __FILE__ << __LINE__ << " GraphViz::gvRenderData() finished";
#endif

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



QPointF QGraphVizScene::transformPoint(const QPointF &point)
{
    return transformPoint(point.x(), point.y());
}

QPointF QGraphVizScene::transformPoint(const pointf_s &point)
{
    return transformPoint(point.x, point.y);
}

QPointF QGraphVizScene::transformPoint(qreal x, qreal y)
{
    return QPointF((x + m_Translate.x()) * m_Scale.x(), (y + m_Translate.y()) * m_Scale.y());
}



QList<QGraphVizNode*> QGraphVizScene::getNodes()
{
    return m_Nodes.values();
}

QGraphVizNode *QGraphVizScene::getNode(int GVID)
{
    return m_Nodes.value(GVID, NULL);
}

bool QGraphVizScene::containsNode(int GVID)
{
    return m_Nodes.contains(GVID);
}



QList<QGraphVizEdge*> QGraphVizScene::getEdges()
{
    return m_Edges.values();
}

QGraphVizEdge *QGraphVizScene::getEdge(int GVID)
{
    return m_Edges.value(GVID, NULL);
}

bool QGraphVizScene::containsEdge(int GVID)
{
    return m_Edges.contains(GVID);
}



QHash<QString, QString> QGraphVizScene::getAttributes()
{
    QHash<QString,QString> attr;
    Agsym_t *nextAttr = agfstattr(m_Graph);
    while(nextAttr) {
        QString key = QString(nextAttr->name);
        QString value = QString(nextAttr->value);
        if(!key.isEmpty() && !value.isEmpty()) {
            attr[key] = value;
        }
        nextAttr = agnxtattr(m_Graph, nextAttr);
    }
    return attr;
}

QHash<QString, QString> QGraphVizScene::getAttributes(Agnode_t *node)
{
    QHash<QString,QString> attr;

    Agnode_t *defaultNode = agprotonode(m_Graph);
    Agsym_t *nextAttr = agfstattr(defaultNode);
    while(nextAttr) {
        QString key = QString(nextAttr->name);
        QString value = QString(nextAttr->value);
        if(!key.isEmpty() && !value.isEmpty()) {
            attr[key] = value;
        }
        nextAttr = agnxtattr(defaultNode, nextAttr);
    }

    if(node) {
        Agsym_t *nextAttr = agfstattr(node);
        while(nextAttr) {
            QString key = QString(nextAttr->name);
            QString value = QString(nextAttr->value);
            if(!key.isEmpty()) {
                attr[key] = value;
            }
            nextAttr = agnxtattr(node, nextAttr);
        }
    }

    return attr;
}

QHash<QString, QString> QGraphVizScene::getAttributes(Agedge_t *edge)
{
    QHash<QString,QString> attr;

    Agedge_t *defaultEdge = agprotoedge(m_Graph);
    Agsym_t *nextAttr = agfstattr(defaultEdge);
    while(nextAttr) {
        QString key = QString(nextAttr->name);
        QString value = QString(nextAttr->value);
        if(!key.isEmpty() && !value.isEmpty()) {
            attr[key] = value;
        }
        nextAttr = agnxtattr(defaultEdge, nextAttr);
    }

    if(edge) {
        Agsym_t *nextAttr = agfstattr(edge);
        while(nextAttr) {
            QString key = QString(nextAttr->name);
            QString value = QString(nextAttr->value);
            if(!key.isEmpty() && !value.isEmpty()) {
                attr[key] = value;
            }
            nextAttr = agnxtattr(edge, nextAttr);
        }
    }

    return attr;
}



QByteArray QGraphVizScene::getHash()
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData((char*)m_Graph, sizeof(Agraph_t));

    // Serialize and add attributes
    QByteArray attributes;
    QDataStream ds(&attributes, QIODevice::WriteOnly);
    ds << getAttributes();
    md5.addData(attributes);

    node_t *node = agfstnode(m_Graph);
    while(node) {
        md5.addData(getHash(node));
        Agedge_t *edge = agfstedge(m_Graph, node);
        while(edge) {
            md5.addData(getHash(edge));
            edge = agnxtedge(m_Graph, edge, node);
        }
        node = agnxtnode(m_Graph, node);
    }

    return md5.result();
}

QByteArray QGraphVizScene::getHash(Agedge_t *edge)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData((char*)edge, sizeof(Agedge_t));

    // Serialize and add attributes
    QByteArray attributes;
    QDataStream ds(&attributes, QIODevice::WriteOnly);
    ds << getAttributes(edge);
    md5.addData(attributes);

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

QByteArray QGraphVizScene::getHash(Agnode_t *node)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData((char*)node, sizeof(Agnode_t));

    // Serialize and add attributes
    QByteArray attributes;
    QDataStream ds(&attributes, QIODevice::WriteOnly);
    ds << getAttributes(node);
    md5.addData(attributes);

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

QByteArray QGraphVizScene::getHash(textlabel_t *label)
{
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData((char*)label, sizeof(textlabel_t));
    return md5.result();
}
