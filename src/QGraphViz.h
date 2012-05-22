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


class QGraphViz : public QObject
{
    Q_OBJECT
public:
    explicit QGraphViz(QString content, QObject *parent = 0);
    ~QGraphViz();

    QMap<QString, QString> arguments();

    QGraphVizGraphAttributes &graphAttributes();
    QGraphVizNodeAttributes &defaultNodeAttributes();
    QGraphVizEdgeAttributes &defaultEdgeAttributes();

    QByteArray renderedContent();

    QString renderEngine();
    void setRenderEngine(QString renderEngine);

    QGraphicsScene *renderScene(QWidget *parent);

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

private:
    static GVC_t *m_Context;

    QString m_Content;
    graph_t *m_Graph;

    QGraphVizGraphAttributes m_GraphAttributes;
    QGraphVizNodeAttributes m_DefaultNodeAttributes;
    QGraphVizEdgeAttributes m_DefaultEdgeAttributes;

    QString m_LayoutEngine;
    bool m_LayoutDone;
    QString m_RenderEngine;
    QByteArray m_RenderedContent;

};

#endif // QGRAPHVIZ_H
