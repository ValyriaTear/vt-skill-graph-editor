///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "graph_scene.h"

#include "skill_nodes_table.h"

#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

// Maybe later use resizable scene and grid size (in pixels).
const uint32_t SCENE_SIZE = 2000;
const uint32_t GRID_SIZE = 50;
const uint32_t NODE_SIZE = 10;

//! \brief The data id of the Graphics item that stores the node id.
const int ITEM_ID_KEY = 0;

//! \brief Default text color
const QColor text_color = QColor(255, 255, 255, 255);

GraphScene::GraphScene(QGraphicsView* view, SkillNodesTable* node_handler):
    _view(view),
    _node_handler(node_handler)
{
    // Register the scene to other components
    _view->setScene(this);
    _node_handler->setScene(this);

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

    if (evt->button() == Qt::LeftButton) {
        int32_t node_id = _node_handler->appendNodeRow(x, y);
        if (node_id == 0) {
            qWarning("Error: The row wasn't added.");
            return;
        }

        addNode(node_id, x, y);
    }
    else if (evt->button() == Qt::RightButton) {
        // Find the node and remove it if it exists.
        int32_t node_row = _node_handler->findNode(x, y, NODE_SIZE);
        if (node_row != UNFOUND_NODE)
            _node_handler->removeNodeRow(node_row);
    }
}

void GraphScene::addNode(int32_t id, uint32_t x, uint32_t y)
{
    QGraphicsItem* item = addEllipse(x - NODE_SIZE / 2,
                                     y - NODE_SIZE / 2,
                                     NODE_SIZE,
                                     NODE_SIZE,
                                     QPen(Qt::white));
    item->setData(ITEM_ID_KEY, QVariant::fromValue(id));
    QGraphicsTextItem* text = addText(QString::number(id));
    text->moveBy(x, y);
    text->setDefaultTextColor(text_color);
    item->setData(ITEM_ID_KEY, QVariant::fromValue(id));
}

void GraphScene::repaint()
{
    clear();

    // Grid rows headers
    QGraphicsTextItem* text = addText(QString("X ->"));
    text->moveBy(20, 20);
    text->setDefaultTextColor(text_color);
    text = addText(QString("Y\n|\nv"));
    text->moveBy(20, 40);
    text->setDefaultTextColor(text_color);

    // Add the vertical lines first, paint them red
    for (size_t x = 0; x <= SCENE_SIZE; x += GRID_SIZE) {
        addLine(x, 0, x, SCENE_SIZE, QPen(Qt::white));
        QGraphicsTextItem* text = addText(QString::number(x));
        text->moveBy(x, 0);
        text->setDefaultTextColor(text_color);
    }

    // Now add the horizontal lines, paint them green
    for (size_t y = 0; y <= SCENE_SIZE; y += GRID_SIZE) {
        addLine(0, y, SCENE_SIZE, y, QPen(Qt::white));
        if (y > 0) {
            QGraphicsTextItem* text = addText(QString::number(y));
            text->moveBy(2, y);
            text->setDefaultTextColor(text_color);
        }
    }

    // Repaint ellipses and ids
    const QStandardItemModel* model = _node_handler->getData();
    if (model == nullptr)
        return;
    for (int32_t i = 0; i < model->rowCount(); ++i) {
        QModelIndex index = model->index(i, PositionX, QModelIndex());
        uint32_t node_x = model->data(index).toUInt();
        index = model->index(i, PositionY, QModelIndex());
        uint32_t node_y = model->data(index).toUInt();

        if (node_x == 0 || node_y == 0)
            continue;

        addNode(i + 1, node_x, node_y);
    }
}
