///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See https://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class SkillNodesTable;
class GraphScene;

class QSplitter;
class QGraphicsView;
class QSettings;

//! \brief Main app window
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    //! \brief Handles close and/or quit events.
    void closeEvent(QCloseEvent* event);

private slots:
    //! \brief When Append button is clicked
    void appendNodeRow();

    //! \brief When Remove button is clicked
    void removeNodeRow();

    //! \brief Save graph to the current file, if any.
    //! Triggers save as, otherwise.
    bool fileSave();

    //! \brief Save the nodes data in a new file
    //! \returns whether the save was successful
    bool fileSaveAs();

    //! \brief Set the main game data/ absolute path.
    void fileSetGameFolder();

    //! \brief Attempts to quit the editor.
    //! Checks whether a file is in need of being saved first.
    void fileQuit();

    //! \brief Toggles whether the grid should be drawn
    void viewToggleGrid();

    //! \brief Provides some info about the editor
    void helpAboutEditor();

    //! \brief Provides some info about Qt
    void helpAboutQt();

private:
    //! \brief Setup main view widgets. Called once at init.
    void setupMainView();

    //! \brief Save the graph to the given file path.
    bool save(const QString& file_path);

    //! \brief Main Window UI pointer
    QSplitter* _view_splitter;

    //! \brief Skill node handler
    SkillNodesTable* _nodes_table;

    //! \brief Graph view handler
    GraphScene* _graph_scene;

    //! \brief The application settings handler
    QSettings* _settings;

    //! \brief Valyria data/ absolute path
    //! Used to compute proper relative paths
    QString _game_data_folder_path;

    //! \brief Stores the latest file used to save.
    QString _saved_graph_filepath;

    //! \brief Main window actions references, don't delete them
    QAction* _toggle_grid_action;
    QAction* _save_action;
};

#endif // MAIN_WINDOW_H
