///////////////////////////////////////////////////////////////////////////////
//            Copyright (C) 2017 by Bertram (Valyria Tear)
//                         All Rights Reserved
//
// This code is licensed under the GNU GPL version 3. It is free software
// and you may modify it and/or redistribute it under the terms of this license.
// See http://www.gnu.org/copyleft/gpl.html for details.
///////////////////////////////////////////////////////////////////////////////

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class SkillNodesHandler;
class GraphScene;

//! \brief Main app window
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    //! \brief When Append button is clicked
    void AppendNodeRow();

    //! \brief When Remove button is clicked
    void RemoveNodeRow();

    //! \brief When main window is resized
    void resizeEvent(QResizeEvent* event) override;

private:
    //! \brief Main Window UI pointer.
    Ui::MainWindow* _ui;

    //! \brief Skill node handler
    SkillNodesHandler* _skill_nodes_handler;

    //! \brief Graph view handler
    GraphScene* _graph_scene;
};

#endif // MAINWINDOW_H
