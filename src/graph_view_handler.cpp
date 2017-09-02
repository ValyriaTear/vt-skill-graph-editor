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

GraphViewHandler::GraphViewHandler(QGraphicsView* view):
    _view(view)
{
    // create a scene and add it your view
    QGraphicsScene* scene = new QGraphicsScene;
    view->setScene(scene);
}

void GraphViewHandler::Repaint()
{
    // FIXME: Fit the graph size or the future background image
    QGraphicsScene* scene = _view->scene();
    scene->clear();

    const uint32_t GRID_SIZE = 2000;
    // Add the vertical lines first, paint them red
    for (size_t x = 0; x <= GRID_SIZE; x += 50)
        scene->addLine(x, 0, x, GRID_SIZE, QPen(Qt::black));

    // Now add the horizontal lines, paint them green
    for (size_t y = 0; y <= GRID_SIZE; y += 50)
        scene->addLine(0, y, GRID_SIZE, y, QPen(Qt::black));
}
