///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#include "main_window.h"

#include "skill_nodes_table.h"
#include "graph_scene.h"
#include "lua_reader.h"
#include "lua_writer.h"
#include "script/script.h"

#include <QApplication>
#include <QResizeEvent>
#include <QSplitter>
#include <QPushButton>
#include <QMenuBar>
#include <QGraphicsView>
#include <QToolBar>
#include <QFileDialog>
#include <QSettings>
#include <QStatusBar>
#include <QMessageBox>

const int32_t MainWinWidth = 800;
const int32_t MainWinHeight = 800;
const int32_t NodeTableWidth = 425;

//! \brief Simple extern used to pass the main window around
MainWindow* main_win = nullptr;

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _view_splitter(nullptr),
    _nodes_table(nullptr),
    _graph_scene(nullptr),
    _settings(nullptr),
    _save_action(nullptr),
    _toggle_grid_action(nullptr)
{
    // Init the extern
    main_win = this;

    setupMainView();

    // File menu
    // TODO: Add support for open action
    QMenu* file_menu = menuBar()->addMenu(tr("&File"));
    QAction* new_action = file_menu->addAction(QString(tr("&New")));
    new_action->setShortcut(tr("Ctrl+N"));
    QAction* open_action = file_menu->addAction(QString(tr("&Open ...")));
    open_action->setShortcut(tr("Ctrl+O"));
    _save_action = file_menu->addAction(QString(tr("&Save")));
    _save_action->setEnabled(false);
    _save_action->setShortcut(tr("Ctrl+S"));
    QAction* save_as_action = file_menu->addAction(QString(tr("Save &as ...")));
    file_menu->addSeparator();
    QAction* set_game_folder = file_menu->addAction(QString(tr("Set &Game folder ...")));
    file_menu->addSeparator();
    QAction* quit_action = file_menu->addAction(QString(tr("&Quit")));

    connect(new_action, SIGNAL(triggered()), this, SLOT(fileNew()));
    connect(open_action, SIGNAL(triggered()), this, SLOT(fileOpen()));
    connect(_save_action, SIGNAL(triggered()), this, SLOT(fileSave()));
    connect(save_as_action, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    connect(set_game_folder, SIGNAL(triggered()), this, SLOT(fileSetGameFolder()));
    connect(quit_action, SIGNAL(triggered()), this, SLOT(fileQuit()));

    // One cannot save until a valid file is set
    _save_action->setEnabled(false);

    // View Menu
    QMenu* view_menu = menuBar()->addMenu(tr("&View"));
    _toggle_grid_action = new QAction("&Grid", this);
    _toggle_grid_action->setStatusTip("Toggles the grid");
    _toggle_grid_action->setShortcut(tr("G"));
    _toggle_grid_action->setCheckable(true);
    _toggle_grid_action->setChecked(true);
    view_menu->addAction(_toggle_grid_action);

    connect(_toggle_grid_action, SIGNAL(triggered()), this, SLOT(viewToggleGrid()));

    // Help menu
    QMenu* help_menu = menuBar()->addMenu("&Help");
    QAction* about_action = help_menu->addAction(QString(tr("&About Editor")));
    QAction* about_qt_action = help_menu->addAction(QString(tr("About &Qt")));

    connect(about_action, SIGNAL(triggered()), this, SLOT(helpAboutEditor()));
    connect(about_qt_action, SIGNAL(triggered()), this, SLOT(helpAboutQt()));

    // Load settings
    _settings = new QSettings("ValyriaTear", "VT-SkillGraphEditor");
    _game_data_folder_path = _settings->value("GameDataPath").toString();

    // Test the current game data existence and empty it if not valid anymore.
    QDir dataDir(_game_data_folder_path);
    if (!dataDir.exists())
        _game_data_folder_path.clear();

    // Initialize the script manager
    vt_script::ScriptManager = vt_script::ScriptEngine::SingletonCreate();
    vt_script::ScriptManager->SingletonInitialize();
}

MainWindow::~MainWindow()
{
    // delete owned widgets
    delete _view_splitter;
    delete _graph_scene;
    delete _settings;

    // Deinit the script manager
    vt_script::ScriptEngine::SingletonDestroy();
}

void MainWindow::setupMainView()
{
    setWindowTitle(tr("Skill Graph Editor"));
    resize(MainWinWidth, MainWinHeight);

    _view_splitter = new QSplitter(this);
    _view_splitter->setOrientation(Qt::Horizontal);
    setCentralWidget(_view_splitter);

    QSplitter* table_splitter = new QSplitter(_view_splitter);
    table_splitter->setOrientation(Qt::Vertical);

    QGraphicsView* graphics_view = new QGraphicsView(this);

    // Obtain skill table reference and make it handled by the skill node handler
    _nodes_table = new SkillNodesTable(this);
    _nodes_table->setMinimumWidth(NodeTableWidth);
    _nodes_table->setMaximumWidth(NodeTableWidth);
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

    button = new QPushButton(QString("Deselect Node"), node_btn_toolbar);
    button->setFixedSize(120, 30);
    button->setToolTip(tr("Deselect currently selected Node"));
    connect(button, SIGNAL(clicked(bool)), this, SLOT(deselectNode()));
    node_btn_toolbar->addWidget(button);

    table_splitter->addWidget(node_btn_toolbar);

    // Set main widgets sizes
    _view_splitter->setSizes(QList<int>() << 400 << NodeTableWidth);

    // Init scene
    _graph_scene->repaint();

    statusBar()->showMessage(tr("Ready!"), 5000);
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

void MainWindow::deselectNode()
{
    _nodes_table->clearSelectedNodeId();
    _graph_scene->repaint();
}

void MainWindow::fileNew()
{
    if (!_nodes_table->isDataModified()) {
        _nodes_table->clearData();
        statusBar()->showMessage(tr("Data cleared (No unsaved data)"), 5000);
        return;
    }

    int32_t ret = QMessageBox::warning(this, tr("New file - Unsaved changes"),
                                       tr("Some modifications have not been saved.\nWould you like to save them?"),
                                       QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);

    switch (ret) {
    case QMessageBox::Ignore:
        _nodes_table->clearData();
        statusBar()->showMessage(tr("Data cleared (Unsaved data ignored)"), 5000);
        break;
    case QMessageBox::Save:
        if (fileSave()) {
            _nodes_table->clearData();
            statusBar()->showMessage(tr("Data cleared (Data saved)"), 5000);
        }
        break;
    case QMessageBox::Cancel:
    default:
        break;
    }
}

bool MainWindow::fileOpen()
{
    // Check for unsaved data first
    if (_nodes_table->isDataModified()) {
        int32_t ret = QMessageBox::warning(this, tr("Open file - Unsaved changes"),
                                           tr("Some modifications have not been saved.\nWould you like to save them?"),
                                           QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);

        switch (ret) {
        case QMessageBox::Ignore:
            _nodes_table->clearData();
            statusBar()->showMessage(tr("Data cleared (Unsaved data ignored)"), 5000);
            break;
        case QMessageBox::Save:
            if (fileSave()) {
                _nodes_table->clearData();
                statusBar()->showMessage(tr("Data cleared (Data saved)"), 5000);
            }
            break;
        case QMessageBox::Cancel:
        default:
            break;
        }
    }

    // Select the file to open
    QString file_path = QFileDialog::getOpenFileName(this,
                                                     tr("Open File ..."),
                                                     _game_data_folder_path,
                                                     tr("Lua file (*.lua)"));
    // Wasn't selected
    if (file_path.isEmpty())
        return false;

    // Get the data model
    NodeModel* node_model = _nodes_table->getDataForOverride();
    if (!node_model)
        return false;

    // Attempt to open and read the file
    LuaReader lua_reader;
    bool ret = lua_reader.readFile(file_path, *node_model);

    // Open was successful
    if (ret) {
        _saved_graph_filepath = file_path;
        _save_action->setEnabled(true);
        _nodes_table->dataSaved();
        _nodes_table->clearSelectedNodeId();
        setWindowTitle(tr("Skill Graph Editor - %1").arg(_saved_graph_filepath));
        statusBar()->showMessage(tr("Graph open with: %1").arg(_saved_graph_filepath), 5000);
    }
    else {
        statusBar()->showMessage(tr("Graph file couldn't be opened: %1").arg(file_path), 5000);
        // Empty data
        _nodes_table->clearData();
        _graph_scene->repaint();
        return false;
    }

    _graph_scene->repaint();
    return true;
}

bool MainWindow::fileSave()
{
    QFile file(_saved_graph_filepath);
    if (_saved_graph_filepath.isEmpty() || !file.exists()) {
        return fileSaveAs();
    }

    return save(_saved_graph_filepath);
}

bool MainWindow::fileSaveAs()
{
    QString file_path = QFileDialog::getSaveFileName(this,
                                                     tr("Save Data as ..."),
                                                     _game_data_folder_path,
                                                     tr("Lua file (*.lua)"));

    // Wasn't saved
    if (file_path.isEmpty())
        return false;

    return save(file_path);
}

bool MainWindow::save(const QString& file_path)
{
    // Actually save the file now we have its location.
    const NodeModel* node_model = _nodes_table->getData();
    if (!node_model)
        return false;

    LuaWriter writer;
    bool ret = writer.save(file_path, *node_model);

    // Save was successful
    if (ret) {
        _saved_graph_filepath = file_path;
        _save_action->setEnabled(true);
        _nodes_table->dataSaved();
        setWindowTitle(tr("Skill Graph Editor - %1").arg(_saved_graph_filepath));
        statusBar()->showMessage(tr("Graph saved to: %1").arg(_saved_graph_filepath), 5000);
    }

    return ret;
}

void MainWindow::fileSetGameFolder()
{
    QString file_path = QFileDialog::getExistingDirectory(this,
                                                         tr("Skill Graph Editor -- Set Game data/ folder"),
                                                         _game_data_folder_path);

    if(file_path.isEmpty()) {
        statusBar()->showMessage(tr("Game data/ path not set"), 5000);
        return;
    }
    statusBar()->showMessage(tr("Game data/ path set to: %1").arg(file_path), 5000);
    _game_data_folder_path = file_path;
    // Set this in the settings.
    _settings->setValue("GameDataPath", _game_data_folder_path);
}

void MainWindow::fileQuit()
{
    if (!_nodes_table->isDataModified()) {
        qApp->exit(0); // Used to override close events.
        return; // Should be useless, but still ...
    }

    int32_t ret = QMessageBox::warning(this, tr("Quit Editor - Unsaved changes"),
                                       tr("Some modifications have not been saved.\nWould you like to save them?"),
                                       QMessageBox::Save | QMessageBox::Close | QMessageBox::Cancel);

    switch (ret) {
    case QMessageBox::Close:
        qApp->exit(0); // Used to override close events.
        break;
    case QMessageBox::Save:
        if (fileSave()) {
            qApp->exit(0); // Used to override close events.
        }
        break;
    case QMessageBox::Cancel:
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();
    fileQuit();
}

void MainWindow::viewToggleGrid()
{
    bool grid_enabled = _toggle_grid_action->isChecked();
    _toggle_grid_action->setChecked(grid_enabled);
    _graph_scene->setGridEnabled(grid_enabled);
}

void MainWindow::helpAboutEditor()
{
    QMessageBox::about(this, tr("Skill Graph Editor -- About"),
                       tr("<center><h2>Skill Graph Editor"
                          "</h2></center>"
                          "<center><h3>Copyright &copy; 2017<font></h3></center>"
                          "<p>A skill graph editor created for Valyria Tear. "));
}

void MainWindow::helpAboutQt()
{
    QMessageBox::aboutQt(this, tr("Skill Graph Editor -- About Qt"));
}
