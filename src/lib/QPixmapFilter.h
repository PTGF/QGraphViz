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

#ifndef QPIXMAPFILTER_H
#define QPIXMAPFILTER_H

#include <QtCore/qnamespace.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qgraphicseffect.h>

#ifndef QT_NO_GRAPHICSEFFECT
QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE
QT_MODULE(Gui)

class QPainter;
class QPixmapData;

class QPixmapFilterPrivate;

class Q_GUI_EXPORT QPixmapFilter : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPixmapFilter)
public:
    virtual ~QPixmapFilter() = 0;

    enum FilterType {
        ConvolutionFilter,
        ColorizeFilter,
        DropShadowFilter,
        BlurFilter,

        UserFilter = 1024
    };

    FilterType type() const;

    virtual QRectF boundingRectFor(const QRectF &rect) const;

    virtual void draw(QPainter *painter, const QPointF &p, const QPixmap &src, const QRectF &srcRect = QRectF()) const = 0;

protected:
    QPixmapFilter(QPixmapFilterPrivate &d, FilterType type, QObject *parent);
    QPixmapFilter(FilterType type, QObject *parent);
};

class QPixmapBlurFilterPrivate;

class Q_GUI_EXPORT QPixmapBlurFilter : public QPixmapFilter
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPixmapBlurFilter)

public:
    QPixmapBlurFilter(QObject *parent = 0);
    ~QPixmapBlurFilter();

    void setRadius(qreal radius);
    void setBlurHints(QGraphicsBlurEffect::BlurHints hints);

    qreal radius() const;
    QGraphicsBlurEffect::BlurHints blurHints() const;

    QRectF boundingRectFor(const QRectF &rect) const;
    void draw(QPainter *painter, const QPointF &dest, const QPixmap &src, const QRectF &srcRect = QRectF()) const;

private:
    friend class QGLPixmapBlurFilter;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif //QT_NO_GRAPHICSEFFECT
#endif // QPIXMAPFILTER_H
