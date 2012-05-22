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

#include "QGraphVizScene.h"
#include "QGraphVizNode.h"
#include "QGraphVizEdge.h"

GVC_t *QGraphViz::m_Context = gvContext();

QGraphViz::QGraphViz(QString content, QObject *parent) :
    QObject(parent),
    m_Graph(NULL),
    m_GraphAttributes(),
    m_DefaultNodeAttributes(),
    m_DefaultEdgeAttributes(),
    m_LayoutEngine("dot"),
    m_LayoutDone(false),
    m_RenderEngine("xdot")
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
    }
}

QGraphViz::~QGraphViz()
{
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

    m_LayoutDone = true;
}

void QGraphViz::doRender()
{
    if(!m_RenderedContent.isEmpty()) {
        return;
    }

    if(!m_LayoutDone) {
        doLayout();
    }

    char *content;
    unsigned int length;
    if(gvRenderData(m_Context, m_Graph, m_RenderEngine.toLocal8Bit().data(), &content, &length)) {
        throw tr("Failed to render.");
    }

    m_RenderedContent = QByteArray(content, length);

    free(content);

    Agnode_t *node = agfstnode(m_Graph);
    while(node) {
        Agedge_t *edge = agfstedge(m_Graph, node);
        while(edge) {
            edge = agnxtedge(m_Graph, edge, node);
        }
        node = agnxtnode(m_Graph, node);
    }

}

void QGraphViz::onChanged()
{
    if(m_LayoutDone) {
        gvFreeLayout(m_Context, m_Graph);
        m_LayoutDone = false;
    }

    if(!m_RenderedContent.isEmpty()) {
        m_RenderedContent = QByteArray();
    }
}

/*! dot; xdot; png; svg; plain; etc.
 */
QString QGraphViz::renderEngine()
{
    return m_RenderEngine;
}

/*! dot; xdot; png; svg; plain; etc.
 */
void QGraphViz::setRenderEngine(QString renderEngine)
{
    if(renderEngine == m_RenderEngine) {
        return;
    }

    m_RenderEngine = renderEngine;

    onChanged();
    emit changed();
}

/*! dot; neato; circo; fdp; osage; sfdp; twopi
 */
QString QGraphViz::layoutEngine()
{
    return m_RenderEngine;
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

QByteArray QGraphViz::renderedContent()
{
    if(m_RenderedContent.isEmpty()) {
        doRender();
    }

    return m_RenderedContent;
}


QGraphicsScene *QGraphViz::renderScene(QWidget *parent)
{
    if(!m_LayoutDone) {
        doLayout();
    }

    QPointF translate(-m_Graph->u.bb.LL.x, -m_Graph->u.bb.UR.y);
    QPointF scale(1.0, -1.0);
    QGraphVizScene *scene = new QGraphVizScene(translate, scale, parent);

    node_t *node = agfstnode(m_Graph);
    while(node) {
        scene->addItem(new QGraphVizNode(node, scene));

        Agedge_t *edge = agfstedge(m_Graph, node);
        while(edge) {
            scene->addItem(new QGraphVizEdge(edge, scene));
            edge = agnxtedge(m_Graph, edge, node);
        }

        node = agnxtnode(m_Graph, node);
    }

    return (QGraphicsScene *)scene;
}
