///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "graph_view_handler.h"

#include <QGraphicsView>
#include <QGraphicsTextItem>

// Maybe later use resizable scene and grid size.
const uint32_t SCENE_SIZE = 2000;
const uint32_t GRID_SIZE = 50;

GraphViewHandler::GraphViewHandler(QGraphicsView* view):
    _view(view)
{
    // create a scene and add it your view
    QGraphicsScene* scene = new QGraphicsScene;
    view->setScene(scene);

    view->setBackgroundBrush(QBrush(Qt::black));

    // Helps with rendering when not using OpenGL
    view->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);

    view->setMouseTracking(true);
    // Helps when resizing
    view->viewport()->setAttribute(Qt::WA_StaticContents);
}

GraphViewHandler::~GraphViewHandler()
{
    delete _view;
}

void GraphViewHandler::Repaint()
{
    QGraphicsScene* scene = _view->scene();
    scene->clear();

    // Grid rows headers
    const QColor text_color = QColor(255, 255, 255, 255);
    QGraphicsTextItem* text = scene->addText(QString("X ->"));
    text->moveBy(20, 20);
    text->setDefaultTextColor(text_color);
    text = scene->addText(QString("Y\n|\nv"));
    text->moveBy(20, 40);
    text->setDefaultTextColor(text_color);

    // Add the vertical lines first, paint them red
    for (size_t x = 0; x <= SCENE_SIZE; x += GRID_SIZE) {
        scene->addLine(x, 0, x, SCENE_SIZE, QPen(Qt::white));
        QGraphicsTextItem* text = scene->addText(QString("%1").arg(x));
        text->moveBy(x , 0);
        text->setDefaultTextColor(text_color);
    }

    // Now add the horizontal lines, paint them green
    for (size_t y = 0; y <= SCENE_SIZE; y += GRID_SIZE) {
        scene->addLine(0, y, SCENE_SIZE, y, QPen(Qt::white));
        if (y > 0) {
            QGraphicsTextItem* text = scene->addText(QString("%1").arg(y));
            text->moveBy(2 , y);
            text->setDefaultTextColor(text_color);
        }
    }
}
