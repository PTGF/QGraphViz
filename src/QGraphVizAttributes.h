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

#ifndef QGRAPHVIZATTRIBUTES_H
#define QGRAPHVIZATTRIBUTES_H

#include <QtCore>

struct Agraph_t;
struct Agnode_t;
struct Agedge_t;
class QGraphViz;

class QGraphVizAttributes : public QObject
{
    Q_OBJECT
public:
    explicit QGraphVizAttributes(void *proto = 0, QGraphViz *parent = 0);

    void setProto(void *proto);

    void setValue(QString key, QString value);
    bool contains(const QString &key) const;
    QString value(const QString &key) const;
    void remove(QString key);

    QStringList keys();
    QStringList values();
    QHash<QString, QString> pairs();

signals:
    void changed();

private:
    void *m_Proto;
    QHash<QString, QString> m_AttributeCache;
};

class QGraphVizGraphAttributes : public QGraphVizAttributes
{
    Q_OBJECT
public:
    explicit QGraphVizGraphAttributes(Agraph_t *graph = 0, QGraphViz *parent = 0);
    void setGraph(Agraph_t *graph);
};
class QGraphVizNodeAttributes : public QGraphVizAttributes
{
    Q_OBJECT
public:
    explicit QGraphVizNodeAttributes(Agnode_t *node = 0, QGraphViz *parent = 0);
    void setNode(Agnode_t *node);
};
class QGraphVizEdgeAttributes : public QGraphVizAttributes
{
    Q_OBJECT
public:
    explicit QGraphVizEdgeAttributes(Agedge_t *edge = 0, QGraphViz *parent = 0);
    void setEdge(Agedge_t *edge);
};

#endif // QGRAPHVIZATTRIBUTES_H
