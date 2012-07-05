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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphVizView.h>
#include <QGraphVizScene.h>

#include <QtCore>
#include <QtGui>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QFile file("test/mpi_ringtopo.0001.3D.dot");
//    QFile file("test/crazy.gv");
//    QFile file("test/Trun-472procs.32228.2D.dot");
    QFile file("test/cam.6.3D.dot");
//    QFile file("test/cray_216000.dot");  // This requires pre-processing (GraphViz fails; too wide)

    if(!file.exists()) {
        qCritical() << "dot file not found: " << file.fileName();
    }

    if(file.exists()) {
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        file.close();

        QGraphVizScene *gv = new QGraphVizScene(QString(data), this);

        QGraphVizView *view = new QGraphVizView(gv);
//        view->setNodeCollapse(QGraphVizView::NodeCollapse_OnDoubleClick);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->addTab(view, "View"));

//        QByteArray content = gv->exportContent();
//        ui->textEdit->setText(QString(content));

//        content = gv->exportContent("png");
//        QPixmap pixmap;
//        pixmap.loadFromData(content);
//        ui->label->setPixmap(pixmap.scaled(QSize(512,768), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
