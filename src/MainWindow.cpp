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

#include "QGraphViz.h"

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file("/home/dane/STAT_results/mpi_ringtopo.0001/mpi_ringtopo.0001.3D.dot");
    if(file.exists()) {
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        file.close();

        QGraphViz gv(QString(data), this);

        QGraphicsScene *scene = gv.renderScene(this);
        QGraphicsView *view = new QGraphicsView(scene);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->addTab(view, "View"));

        QByteArray content = gv.renderedContent();
        ui->textEdit->setText(QString(content));

//        gv.setRenderEngine("png");
//        content = gv.renderedContent();
//        QPixmap pixmap;
//        pixmap.loadFromData(content);
//        ui->label->setPixmap(pixmap);
    }


//    // Initialize GraphViz engine
//    aginit();

//    // Create GraphViz Context object
//    GVC_t *context = gvContext();

//    // Load graph from string
//    Agraph_t *graph = agmemread(data.data());

//    char *result;
//    unsigned int length;
//    gvRenderData(context, graph, "dot", &result, &length);

//    gvFreeLayout(context, graph);
//    agclose(graph);
}

MainWindow::~MainWindow()
{
    delete ui;
}
