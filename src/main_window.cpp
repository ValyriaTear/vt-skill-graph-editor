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

#include "skill_nodes_table.h"
#include "graph_scene.h"

#include <QResizeEvent>
#include <QSplitter>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _nodes_table(nullptr),
    _graph_scene(nullptr)
{
    setupMainView();
}

MainWindow::~MainWindow()
{
    // delete owned widgets
    delete _view_splitter;

    delete _nodes_table;
    delete _graph_scene;
}

void MainWindow::setupMainView()
{
    setWindowTitle("Skill Graph Editor");
    resize(800, 600);

    _view_splitter = new QSplitter(this);
    _view_splitter->setOrientation(Qt::Horizontal);
    setCentralWidget(_view_splitter);

    _table_splitter = new QSplitter(_view_splitter);
    _table_splitter->setOrientation(Qt::Vertical);

    _graphics_view = new QGraphicsView(this);

    // Obtain skill table reference and make it handled by the skill node handler
    _nodes_table = new SkillNodesTable(this);
    _graph_scene = new GraphScene(_graphics_view, _nodes_table);

    _view_splitter->addWidget(_graphics_view);
    _view_splitter->addWidget(_table_splitter);
    _table_splitter->addWidget(_nodes_table);

    QToolBar* node_btn_toolbar = new QToolBar("Nodes", _table_splitter);

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

    _table_splitter->addWidget(node_btn_toolbar);

    // Init scene
    _graph_scene->repaint();
}

void MainWindow::appendNodeRow()
{
    _nodes_table->appendNodeRow();
}

void MainWindow::removeNodeRow()
{
    QModelIndexList indexList = _nodes_table->selectionModel()->selectedIndexes();
    // Multiple rows can be selected
    for(int32_t i = 0; i < indexList.count(); ++i)
    {
        QModelIndex index = indexList.at(i);
        _nodes_table->removeNodeRow(index.row());
    }
}
