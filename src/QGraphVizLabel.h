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

#ifndef QGRAPHVIZLABEL_H
#define QGRAPHVIZLABEL_H

#include <QtCore>
#include <QtGui>

#include <graphviz/types.h>

class QGraphVizNode;

class QGraphVizLabel : public QGraphicsSimpleTextItem
{
public:
    explicit QGraphVizLabel(textlabel_t *label, QGraphicsItem * parent = NULL);

    // Label properties
    void setText(const QString &text);
    void setFont(const QFont &font);
    Qt::AlignmentFlag alignment() const;
    void setAlignment(const Qt::AlignmentFlag &alignment);

protected:
    void updateDimensions();

private:
    Qt::AlignmentFlag m_Alignment;

    friend class QGraphVizNode;
    friend class QGraphVizEdge;
};

#endif // QGRAPHVIZLABEL_H
