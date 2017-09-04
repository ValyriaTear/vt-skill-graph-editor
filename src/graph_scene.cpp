///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "graph_scene.h"

#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

// Maybe later use resizable scene and grid size (in pixels).
const uint32_t SCENE_SIZE = 2000;
const uint32_t GRID_SIZE = 50;
const uint32_t NODE_SIZE = 10;

GraphScene::GraphScene(QGraphicsView* view):
    _view(view)
{
    // Register the scene
    _view->setScene(this);

    _view->setBackgroundBrush(QBrush(Qt::black));

    // Helps with rendering when not using OpenGL
    _view->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);

    _view->setMouseTracking(true);
    // Helps when resizing
    _view->viewport()->setAttribute(Qt::WA_StaticContents);
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* evt)
{
    // Takes in account the current scrolling
    int32_t x = evt->scenePos().x();
    int32_t y = evt->scenePos().y();

    addEllipse(x - NODE_SIZE / 2, y - NODE_SIZE / 2,
               NODE_SIZE, NODE_SIZE, QPen(Qt::white));
}

void GraphScene::Repaint()
{
    clear();

    // Grid rows headers
    const QColor text_color = QColor(255, 255, 255, 255);
    QGraphicsTextItem* text = addText(QString("X ->"));
    text->moveBy(20, 20);
    text->setDefaultTextColor(text_color);
    text = addText(QString("Y\n|\nv"));
    text->moveBy(20, 40);
    text->setDefaultTextColor(text_color);

    // Add the vertical lines first, paint them red
    for (size_t x = 0; x <= SCENE_SIZE; x += GRID_SIZE) {
        addLine(x, 0, x, SCENE_SIZE, QPen(Qt::white));
        QGraphicsTextItem* text = addText(QString("%1").arg(x));
        text->moveBy(x , 0);
        text->setDefaultTextColor(text_color);
    }

    // Now add the horizontal lines, paint them green
    for (size_t y = 0; y <= SCENE_SIZE; y += GRID_SIZE) {
        addLine(0, y, SCENE_SIZE, y, QPen(Qt::white));
        if (y > 0) {
            QGraphicsTextItem* text = addText(QString("%1").arg(y));
            text->moveBy(2 , y);
            text->setDefaultTextColor(text_color);
        }
    }
}
