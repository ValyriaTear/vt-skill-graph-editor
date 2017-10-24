///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "main_window.h"
#include "ui_main_window.h"

#include "skill_nodes_table.h"
#include "graph_scene.h"

#include <QResizeEvent>
#include <QSplitter>
#include <QPushButton>

const int32_t MAIN_WIN_WIDTH = 800;
const int32_t MAIN_WIN_HEIGHT = 800;
const int32_t NODE_TABLE_WIDTH = 425;

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _view_splitter(nullptr),
    _nodes_table(nullptr),
    _graph_scene(nullptr)
{
    setupMainView();

    // TODO: Add real menus on need (open, save, ...)
    QMenu* file_menu = menuBar()->addMenu("&File");
    QAction* quit_action = file_menu->addAction(QString("&Quit"));
    connect(quit_action, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    // delete owned widgets
    delete _view_splitter;
    delete _graph_scene;
}

void MainWindow::setupMainView()
{
    setWindowTitle(tr("Skill Graph Editor"));
    resize(MAIN_WIN_WIDTH, MAIN_WIN_HEIGHT);

    _view_splitter = new QSplitter(this);
    _view_splitter->setOrientation(Qt::Horizontal);
    setCentralWidget(_view_splitter);

    QSplitter* table_splitter = new QSplitter(_view_splitter);
    table_splitter->setOrientation(Qt::Vertical);

    QGraphicsView* graphics_view = new QGraphicsView(this);

    // Obtain skill table reference and make it handled by the skill node handler
    _nodes_table = new SkillNodesTable(this);
    _nodes_table->setMinimumWidth(NODE_TABLE_WIDTH);
    _nodes_table->setMaximumWidth(NODE_TABLE_WIDTH);
    _graph_scene = new GraphScene(graphics_view, _nodes_table);

    _view_splitter->addWidget(graphics_view);
    _view_splitter->addWidget(table_splitter);
    table_splitter->addWidget(_nodes_table);

    QToolBar* node_btn_toolbar = new QToolBar("Nodes", table_splitter);

    // Add buttons
    QPushButton* button = new QPushButton(QString("+"), node_btn_toolbar);
    button->setFixedSize(30, 30);
    button->setToolTip(tr("Add node"));
    connect(button, SIGNAL(clicked(bool)), this, SLOT(appendNodeRow()));
    node_btn_toolbar->addWidget(button);

    button = new QPushButton(QString("-"), node_btn_toolbar);
    button->setFixedSize(30, 30);
    button->setToolTip(tr("Remove selected nodes"));
    connect(button, SIGNAL(clicked(bool)), this, SLOT(removeNodeRow()));
    node_btn_toolbar->addWidget(button);

    table_splitter->addWidget(node_btn_toolbar);

    // Set main widgets sizes
    _view_splitter->setSizes(QList<int>() << 400 << NODE_TABLE_WIDTH);

    // Init scene
    _graph_scene->repaint();
}

void MainWindow::appendNodeRow()
{
    _nodes_table->appendNodeRow();
}

void MainWindow::removeNodeRow()
{
    QModelIndexList index_list = _nodes_table->selectionModel()->selectedIndexes();
    // Multiple rows can be selected
    for(int32_t i = 0; i < index_list.count(); ++i)
    {
        QModelIndex index = index_list.at(i);
        _nodes_table->removeNodeRow(index.row());
    }
}
