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

#ifndef QGRAPHVIZSCENE_H
#define QGRAPHVIZSCENE_H

#include <QtCore>
#include <QtGui>

class QGraphVizScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit QGraphVizScene(QPointF translate, QPointF scale, QWidget *parent = 0);

    QPointF transformPoint(const QPointF &point);
    QPointF transformPoint(qreal x, qreal y);

private:
    QPointF m_Translate;
    QPointF m_Scale;

};

#endif // QGRAPHVIZSCENE_H
