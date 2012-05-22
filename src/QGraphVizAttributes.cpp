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

#include "QGraphVizAttributes.h"

#include <graphviz/types.h>
#include <graphviz/cdt.h>
#include <graphviz/gvc.h>
#include <graphviz/graph.h>

#include "QGraphViz.h"

QGraphVizAttributes::QGraphVizAttributes(void *proto, QGraphViz *parent) :
    QObject(parent),
    m_Proto(NULL)
{
    setProto(proto);
}

void QGraphVizAttributes::setProto(void *proto)
{
    if(proto == m_Proto) {
        return;
    }

    m_AttributeCache.clear();
    m_Proto = proto;

    if(m_Proto) {
        Agsym_t *nextAttr = agfstattr(m_Proto);
        while(nextAttr) {
            QString key = QString(nextAttr->name);
            QString value = QString(nextAttr->value);
            if(!key.isEmpty() && !value.isEmpty()) {
                m_AttributeCache[key] = value;
            }
            nextAttr = agnxtattr(m_Proto, nextAttr);
        }
    }

    emit changed();
}

void QGraphVizAttributes::setValue(QString key, QString value)
{
    if(key.isEmpty()) {
        return;
    }

    if(agattr(m_Proto, key.toLocal8Bit().data(), value.toLocal8Bit().data())) {
        if(!value.isEmpty()) {
            m_AttributeCache[key] = value;
        } else {
            m_AttributeCache.remove(key);
        }
    }

    emit changed();
}

bool QGraphVizAttributes::contains(const QString &key) const
{
    return m_AttributeCache.contains(key);
}

QString QGraphVizAttributes::value(const QString &key) const
{
    return m_AttributeCache.value(key);
}

void QGraphVizAttributes::remove(QString key)
{
    if(!contains(key)) {
        return;
    }
    setValue(key, QString());   // already emits changed() for us
}

QStringList QGraphVizAttributes::keys()
{
    return pairs().keys();
}

QStringList QGraphVizAttributes::values()
{
    return pairs().values();
}

QHash<QString, QString> QGraphVizAttributes::pairs()
{
    return m_AttributeCache;
}


/*** BEGIN Graph Attributes ***/
QGraphVizGraphAttributes::QGraphVizGraphAttributes(Agraph_t *graph, QGraphViz *parent) :
    QGraphVizAttributes(graph, parent)
{
}
void QGraphVizGraphAttributes::setGraph(Agraph_t *graph)
{
    setProto(graph);
}
/*** END Graph Attributes ***/

/*** BEGIN Node Attributes ***/
QGraphVizNodeAttributes::QGraphVizNodeAttributes(Agnode_t *node, QGraphViz *parent) :
    QGraphVizAttributes(node, parent)
{
}
void QGraphVizNodeAttributes::setNode(Agnode_t *node)
{
    setProto(node);
}
/*** END Node Attributes ***/

/*** BEGIN Edge Attributes ***/
QGraphVizEdgeAttributes::QGraphVizEdgeAttributes(Agedge_t *edge, QGraphViz *parent) :
    QGraphVizAttributes(edge, parent)
{
}
void QGraphVizEdgeAttributes::setEdge(Agedge_t *edge)
{
    setProto(edge);
}
/*** END Edge Attributes ***/
