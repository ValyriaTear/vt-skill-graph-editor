///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "main_window.h"
#include "ui_main_window.h"

#include "skill_nodes_handler.h"
#include "graph_scene.h"

#include <QResizeEvent>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _nodes_handler(nullptr),
    _graph_scene(nullptr)
{
    // Build ui from ui file
    _ui->setupUi(this);

    // Obtain skill table reference and make it handled by the skill node handler
    _nodes_handler = new SkillNodesHandler(_ui->skill_nodes_table_view);
    _graph_scene = new GraphScene(_ui->skill_graph_graphics_view, _nodes_handler);

    // Link widgets actions
    connect(_ui->node_append_button, SIGNAL(clicked()), this, SLOT(appendNodeRow()));
    connect(_ui->node_remove_button, SIGNAL(clicked()), this, SLOT(removeNodeRow()));

    // Init scene
    _graph_scene->repaint();
}

MainWindow::~MainWindow()
{
    // delete the ui instance
    delete _ui;
    delete _nodes_handler;
    delete _graph_scene;
}

void MainWindow::appendNodeRow()
{
    _nodes_handler->appendNodeRow();
}

void MainWindow::removeNodeRow()
{
    QModelIndexList indexList = _ui->skill_nodes_table_view->selectionModel()->selectedIndexes();
    // Multiple rows can be selected
    for(int32_t i = 0; i < indexList.count(); ++i)
    {
        QModelIndex index = indexList.at(i);
        _nodes_handler->removeNodeRow(index.row());
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   QSize size = event->size();
   int32_t central_height = size.height() - _ui->status_bar->height() - _ui->main_tool_bar->height() - 20;
   // Set graphic view size
   _ui->skill_graph_graphics_view->move(0, _ui->main_tool_bar->height());
   _ui->skill_graph_graphics_view->resize(size.width() - 500,
                                         central_height);
   // Set the table position and size
   _ui->skill_nodes_table_view->move(_ui->skill_graph_graphics_view->width() + 10,
                                  _ui->main_tool_bar->height());
   _ui->skill_nodes_table_view->resize(size.width() - _ui->skill_graph_graphics_view->width() - 10,
                                    central_height - _ui->node_append_button->height() - 10);
   // Set the button location
   _ui->node_append_button->move(_ui->skill_graph_graphics_view->width() + 10,
                                 _ui->main_tool_bar->height() + _ui->skill_nodes_table_view->height() + 10);
   _ui->node_remove_button->move(_ui->node_append_button->pos().x() + _ui->node_append_button->width() + 10,
                                 _ui->node_append_button->pos().y());
}
